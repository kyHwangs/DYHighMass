#include "JetEnergyCorrectionFactory.h"
#include "EventContentPool.h"

#include <algorithm>
#include <cmath>
#include <iostream>

JetEnergyCorrectionFactory::JetEnergyCorrectionFactory(const YAML::Node& fConfig) {

  fName = fConfig["Name"].as<std::string>();
  fPath = fConfig["JERC"].as<std::string>();

  fSystNames = {
    "FlavorQCD",
    "RelativeBal",
    "HF",
    "BBEC1",
    "EC2",
    "Absolute",
    "RelativeSample_Uncor",
    "HF_Uncor",
    "BBEC1_Uncor",
    "EC2_Uncor",
    "Absolute_Uncor",
  };
  
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << Name() << ": " << InName() << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " fPath: " << fPath << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;
}

void JetEnergyCorrectionFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;

  auto tBaseCorrection = correction::CorrectionSet::from_file(fPath);

  for (const auto& fSystName : fSystNames)
    fCorrections[fSystName] = tBaseCorrection->at("MC_Regrouped_" + fSystName);

  fCorrectionNominal[true] = tBaseCorrection->compound().at("MC_L1L2L3Res");
  fCorrectionNominal[false] = tBaseCorrection->compound().at("DATA_L1L2L3Res");

  fSmearing["pTRes"] = tBaseCorrection->at("MC_PtResolution");
  fSmearing["SF"] = tBaseCorrection->at("MC_ScaleFactor");
  fSmearing["SFUnc"] = tBaseCorrection->at("MC_SFUncertainty");
}

const std::vector<JET::StdJets> JetEnergyCorrectionFactory::Apply(
  const JET::RawJets& fJets, 
  const GEN::Jets& fGenJets, 
  const std::vector<JetSystConfigs>& fSystConfigs
) {
  
  std::vector<JET::StdJets> tReturnJets = {};

  for (unsigned int i = 0; i < fSystConfigs.size(); i++)
    tReturnJets.push_back(JET::StdJets());

  for (const auto& fJet : fJets) {

    double tCorrectionFactor = 1;
    if (fIsMC) tCorrectionFactor =  fCorrectionNominal[true]->evaluate({fJet.fArea, fJet.fVec.Eta(), fJet.fVec.Pt(), fJet.fRho});
    else       tCorrectionFactor = fCorrectionNominal[false]->evaluate({fJet.fRun, fJet.fArea, fJet.fVec.Eta(), fJet.fVec.Pt(), fJet.fRho});

    float tPtCorrected = fJet.fVec.Pt() * tCorrectionFactor;
    float tMCorrected = fJet.fVec.M() * tCorrectionFactor;

    TLorentzVector tJetCorrected;
    tJetCorrected.SetPtEtaPhiM(tPtCorrected, fJet.fVec.Eta(), fJet.fVec.Phi(), tMCorrected);

    if (!fIsMC) {
      tReturnJets.at(0).push_back(
        JET::StdJet(
          tJetCorrected, 
          fJet.fJetPUId, 
          fJet.fIsBJet, 
          fJet.fGenJetIdx, 
          fJet.fHadronFlavour
        )
      );
    } else {
      
      const float tSigma = fSmearing["pTRes"]->evaluate({fJet.fVec.Eta(), tJetCorrected.Pt(), fJet.fRho});
      const float tSF = fSmearing["SF"]->evaluate({fJet.fVec.Eta(), tJetCorrected.Pt()});
      const float tSFUnc = fSmearing["SFUnc"]->evaluate({fJet.fVec.Eta(), tJetCorrected.Pt()});
      
      unsigned int tSystIndex = 0;
      for (JetSystConfigs tSystConfig : fSystConfigs) {
        
        float tScaleUnc = 0;
        if (tSystConfig.fSystType == "Scale")
          tScaleUnc = fCorrections[tSystConfig.fSystName]->evaluate({fJet.fVec.Eta(), tJetCorrected.Pt()});

        tSystConfig.SetScaling(tScaleUnc);
        tSystConfig.SetSmearing(tSF, tSFUnc);

        bool tIsMatched = false;
        float tGenJetMatchedPt = -999;
        for (const auto& fGenJet : fGenJets) {
          tIsMatched = (
            fGenJet.DeltaR(tJetCorrected) < 0.2 && 
            std::abs(fGenJet.Pt() - tJetCorrected.Pt()) < 3. * tSigma * tJetCorrected.Pt()
          );

          if (tIsMatched) {
            tGenJetMatchedPt = fGenJet.Pt();
            break;
          }
        } // Gen jet matching loop
              
        float tSmearingFactor = 1;
        if (tIsMatched) tSmearingFactor =  1. + (tSystConfig.fSmearingSF - 1.) * (tJetCorrected.Pt() - tGenJetMatchedPt) / tJetCorrected.Pt();
        else  tSmearingFactor = std::max(0., 1. + fJet.fRandom * tSigma * std::sqrt(std::max(tSystConfig.fSmearingSF * tSystConfig.fSmearingSF - 1., 0.)));

        TLorentzVector tJetSmeared;
        tJetSmeared.SetPtEtaPhiM(
          tJetCorrected.Pt() * tSmearingFactor * tSystConfig.fScalingUnc,
          tJetCorrected.Eta(),
          tJetCorrected.Phi(),
          tJetCorrected.M() * tSmearingFactor * tSystConfig.fScalingUnc
        );

        tReturnJets.at(tSystIndex).push_back(
          JET::StdJet(
            tJetSmeared, 
            fJet.fJetPUId, 
            fJet.fIsBJet, 
            fJet.fGenJetIdx, 
            fJet.fHadronFlavour
          )
        );
        tSystIndex++;
      }
    }
  } // Jet loop

  return tReturnJets;
}

