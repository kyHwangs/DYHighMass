#include "BTagEffcieincyFactory.h"
#include "EventContentPool.h"

#include <cmath>

BTagEffcieincyFactory::BTagEffcieincyFactory(const YAML::Node& fConfig_) {
  fName = fConfig_["Name"].as<std::string>();
  fEfficiencyPath = fConfig_["EfficiencyPath"].as<std::string>();
  fSFPath = fConfig_["SFPath"].as<std::string>();
}

void BTagEffcieincyFactory::Init(EventContentPool& pool) {
  fIsMC = pool.GetEventInfo().isMC;
  fEra = pool.GetEventInfo().era;
  fSampleName = pool.GetEventInfo().sampleName;


  if (fSampleName.find("NNLO") != std::string::npos && fSampleName.find("tau") == std::string::npos)
    fSampleName = "DY";
  else if (fSampleName == "TTTo2L2Nu")
    fSampleName = fSampleName;
  else if (fSampleName.find("GG") != std::string::npos)
    fSampleName = "GG";
  else
    fSampleName = "Merged";


  fNominal = 1.;

  fJetBTagEff = correction::CorrectionSet::from_file(fEfficiencyPath)->at("BTagEff");
  fBTagMuJets = correction::CorrectionSet::from_file(fSFPath)->at("deepJet_mujets");
  fBTagIncl = correction::CorrectionSet::from_file(fSFPath)->at("deepJet_incl");    
}

float BTagEffcieincyFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) {
  
  if (!fIsMC) return 1.;

  const auto& tJets = pool.Get<JET::JetsCollection>(fWeightInput.fJetSrc).fJets;

  if (fSystIdx == 0) {
    for (const auto& tJet : tJets) {

      const int& fHadronFlavour = tJet.fHadronFlavour;

      float tSF = 1.;
      if (fHadronFlavour == 5 || fHadronFlavour == 4) tSF = fBTagMuJets->evaluate({"central", "M", fHadronFlavour, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt()});
      else tSF = fBTagIncl->evaluate({"central", "M", 0, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt()});

      float tEff = 1.;
      if (fHadronFlavour == 5)        tEff = fJetBTagEff->evaluate({fEra, "B", fSampleName, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt(), "nominal"});
      else if (fHadronFlavour == 4)   tEff = fJetBTagEff->evaluate({fEra, "C", fSampleName, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt(), "nominal"});
      else                            tEff = fJetBTagEff->evaluate({fEra, "L", fSampleName, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt(), "nominal"});

      if (tJet.fIsBJet) {
        fNominal *= tSF;
      } else {
        fNominal *= (1. - tEff * tSF) / (1. - tEff);
      }
    }
    return fNominal;
  } else {

    float tSystWeight = 1.;
    
    bool tIsBCsyst = true;
    if (fSyst.find("bc") == std::string::npos) tIsBCsyst = false;

    bool tIsCorrelated = true;
    if (fSyst.find("uncorr") != std::string::npos) tIsCorrelated = false;

    std::string tSystNameBC = "";
    std::string tSystNameLight = "";

    if (tIsCorrelated) {
      if (tIsBCsyst) {
        if (fSystIdx == 1) {
          tSystNameBC = "down_correlated";
          tSystNameLight = "central";
        } else if (fSystIdx == 2) {
          tSystNameBC = "up_correlated";
          tSystNameLight = "central";
        }
      } else {
        if (fSystIdx == 1) {
          tSystNameBC = "central";
          tSystNameLight = "down_correlated";
        } else if (fSystIdx == 2) {
          tSystNameBC = "central";
          tSystNameLight = "up_correlated";
        }
      }
    } else {
      if (tIsBCsyst) {
        if (fSystIdx == 1) {
          tSystNameBC = "down_uncorrelated";
          tSystNameLight = "central";
        } else if (fSystIdx == 2) {
          tSystNameBC = "up_uncorrelated";
          tSystNameLight = "central";
        }
      } else {
        if (fSystIdx == 1) {
          tSystNameBC = "central";
          tSystNameLight = "down_uncorrelated";
        } else if (fSystIdx == 2) {
          tSystNameBC = "central";
          tSystNameLight = "up_uncorrelated";
        }
      }
    }


    for (const auto& tJet : tJets) {

      const int& fHadronFlavour = tJet.fHadronFlavour;

      float tSF = 1.;
      if (fHadronFlavour == 5 || fHadronFlavour == 4) tSF = fBTagMuJets->evaluate({tSystNameBC, "M", fHadronFlavour, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt()});
      else tSF = fBTagIncl->evaluate({tSystNameLight, "M", 0, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt()});

      float tEff = 1.;
      if (fHadronFlavour == 5)        tEff = fJetBTagEff->evaluate({fEra, "B", fSampleName, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt(), "nominal"});
      else if (fHadronFlavour == 4)   tEff = fJetBTagEff->evaluate({fEra, "C", fSampleName, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt(), "nominal"});
      else                            tEff = fJetBTagEff->evaluate({fEra, "L", fSampleName, std::abs(tJet.fVec.Eta()), tJet.fVec.Pt(), "nominal"});

      if (tJet.fIsBJet) {
        tSystWeight *= tSF;
      } else {
        tSystWeight *= (1. - tEff * tSF) / (1. - tEff);
      }
    }
    return tSystWeight / fNominal;
  }
  
  return 1.;
}
