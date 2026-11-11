#include "MuonMomentumCorrectionFactory.h"
#include "EventContentPool.h"

#include "TString.h"

#include <cmath>
#include <iostream>

MuonMomentumCorrectionFactory::MuonMomentumCorrectionFactory(const YAML::Node& fConfig) {

  fName = fConfig["Name"].as<std::string>();

  const auto& tSmearingConfig = fConfig["MuonSmearing"];

  fBarrelSmearingFactor = tSmearingConfig["barrel"]["smearing"].as<double>();
  fEndcapSmearingFactor = tSmearingConfig["endcap"]["smearing"].as<double>();

  fDoBarrel = true;
  if (fBarrelSmearingFactor == -1)
    fDoBarrel = false;

  fDoEndcap = true;
  if (fEndcapSmearingFactor == -1)
    fDoEndcap = false;

  std::string tBarrelSigma = tSmearingConfig["barrel"]["sigma"].as<std::string>();
  fBarrelSmearing = new TF1("fBarrelSmearing", (TString)tBarrelSigma, 0, 5000);

  std::string tEndcapSigma = tSmearingConfig["endcap"]["sigma"].as<std::string>();
  fEndcapSmearing = new TF1("fEndcapSmearing", (TString)tEndcapSigma, 0, 5000);

  const auto& tScalingConfig = fConfig["MuonScaling"];

  fNominalPath = tScalingConfig["Default"].as<std::string>();
  fVariationPath = tScalingConfig["Variation"].as<std::string>();
  
  fNominal = correction::CorrectionSet::from_file(fNominalPath)->at("kappa_b");
  fVariation = correction::CorrectionSet::from_file(fVariationPath)->at("kappa_b_replica_sign");

  std::cout << "######################################################################" << std::endl;
  std::cout << " " << Name() << ": " << InName() << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " Smearing: " << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " fBarrelSmearingFactor: " << fBarrelSmearingFactor << std::endl;
  std::cout << " tBarrelSigma: " << tBarrelSigma << std::endl;
  std::cout << " fDoBarrel: " << fDoBarrel << std::endl;
  std::cout << " fEndcapSmearingFactor: " << fEndcapSmearingFactor << std::endl;
  std::cout << " tEndcapSigma: " << tEndcapSigma << std::endl;
  std::cout << " fDoEndcap: " << fDoEndcap << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " Scaling: " << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " fNominalPath: " << fNominalPath << std::endl;
  std::cout << " fVariationPath: " << fVariationPath << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;
}

void MuonMomentumCorrectionFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;
  fRandom.SetSeed(pool.GetEventInfo().fSeedForMuonSmearing);
}

float MuonMomentumCorrectionFactory::GetScaledPt(float fPt, float fCharge, float fBias) {

  return 1000. * fCharge / ( (fCharge / (fPt / 1000.)) + fBias );
}

TLorentzVector MuonMomentumCorrectionFactory::GetSmeared(TLorentzVector fMuon, float fRandomGaus, bool fIsSyst) {

  if (!fIsMC) return fMuon;

  float tSmearingFactor = 1;

  if (std::abs(fMuon.Eta()) <= 1.2 && DoBarrel()) {
    tSmearingFactor = 1 + fRandomGaus * GetBarrelSmearingFactor() * GetBarrelSigma(fMuon.P());
  } else if (std::abs(fMuon.Eta()) > 1.2 && DoEndcap()) { 
    tSmearingFactor = 1 + fRandomGaus * GetEndcapSmearingFactor() * GetEndcapSigma(fMuon.P());
  }

  TLorentzVector tNominalSmearedMuon;
  tNominalSmearedMuon.SetPtEtaPhiM(fMuon.Pt() * tSmearingFactor, fMuon.Eta(), fMuon.Phi(), fMuon.M());

  if (!fIsSyst) return tNominalSmearedMuon;

  if (std::abs(tNominalSmearedMuon.Eta()) <= 1.2) {
    tSmearingFactor = 1 + fRandom.Gaus(0, 1) * 0.46 * GetBarrelSigma(tNominalSmearedMuon.P());
  } else if (std::abs(tNominalSmearedMuon.Eta()) > 1.2) {
    tSmearingFactor = 1 + fRandom.Gaus(0, 1) * 0.46 * GetEndcapSigma(tNominalSmearedMuon.P());
  }

  TLorentzVector tSystSmearedMuon;
  tSystSmearedMuon.SetPtEtaPhiM(tNominalSmearedMuon.Pt() * tSmearingFactor, tNominalSmearedMuon.Eta(), tNominalSmearedMuon.Phi(), tNominalSmearedMuon.M());

  return tSystSmearedMuon;
}


const std::vector<MUON::StdMuons> MuonMomentumCorrectionFactory::Apply(const MUON::RawMuons& fMuons, const std::vector<MuonSystConfigs>& fSystConfigs) {

  std::vector<MUON::StdMuons> tReturnMuons = {};
  for (unsigned int i = 0; i < fSystConfigs.size(); i++)
    tReturnMuons.push_back(MUON::StdMuons());

  int tSystIndex = 0;
  for (const auto& fSystConfig : fSystConfigs) {

    for (const auto& fMu : fMuons) {

      TLorentzVector tScaledMuon = fMu.fVec;
      
      if (fSystConfig.fSystType == "Scaling") {
        float tBias = 
          fNominal->evaluate({tScaledMuon.Eta(), tScaledMuon.Phi(), std::string("nominal")}) + 
          fVariation->evaluate({std::string("variation_") + std::to_string(fSystConfig.iIter), tScaledMuon.Eta(), tScaledMuon.Phi()}) *
          fNominal->evaluate({tScaledMuon.Eta(), tScaledMuon.Phi(), std::string("syst")});

        tScaledMuon.SetPtEtaPhiM(GetScaledPt(tScaledMuon.Pt(), (float)fMu.fCharge, tBias), tScaledMuon.Eta(), tScaledMuon.Phi(), tScaledMuon.M());
      }

      TLorentzVector tMuonSmeared = tScaledMuon;

      if (fSystConfig.fSystType == "Smearing") tMuonSmeared = GetSmeared(tMuonSmeared, fMu.fRandom, true);
      else tMuonSmeared = GetSmeared(tMuonSmeared, fMu.fRandom, false);
    
      tReturnMuons.at(tSystIndex).push_back(MUON::StdMuon(tMuonSmeared, fMu.fVec, fMu.fCharge, fMu.fIso));
    }

    tSystIndex++;
  }

  return tReturnMuons;
}

