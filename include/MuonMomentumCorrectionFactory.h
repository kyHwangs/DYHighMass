#ifndef MUONMOMENTUMCORRECTIONFACTORY_H
#define MUONMOMENTUMCORRECTIONFACTORY_H

#include "Factory.h"
#include "EventProducts.h"

#include "correction.h"
#include "yaml-cpp/yaml.h"

#include "TF1.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

#include <memory>
#include <string>
#include <vector>

class MuonMomentumCorrectionFactory : public Factory {
public:
  MuonMomentumCorrectionFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "MuonMomentumCorrectionFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  float GetScaledPt(float fPt, float fCharge, float fBias);
  TLorentzVector GetSmeared(TLorentzVector fMuon, float fRandomGaus, bool fIsSyst);
  
  const std::vector<MUON::StdMuons> Apply(
    const MUON::RawMuons& fMuons, 
    const std::vector<MuonSystConfigs>& fSystConfigs
  );

private:
  std::string fName;  
  
  bool DoBarrel() { return fDoBarrel; }
  bool DoEndcap() { return fDoEndcap; }
  double GetBarrelSigma(double fP) { return fBarrelSmearing->Eval(fP); }
  double GetEndcapSigma(double fP) { return fEndcapSmearing->Eval(fP); }
  double GetBarrelSmearingFactor() { return fBarrelSmearingFactor; }
  double GetEndcapSmearingFactor() { return fEndcapSmearingFactor; }

  TF1* fBarrelSmearing;
  TF1* fEndcapSmearing;
  
  bool fDoBarrel;
  bool fDoEndcap;

  TRandom3 fRandom;

  double fBarrelSmearingFactor;
  double fEndcapSmearingFactor;
  
  std::string fNominalPath;
  std::string fVariationPath;

  std::shared_ptr<const correction::Correction> fNominal;
  std::shared_ptr<const correction::Correction> fVariation;

  bool fIsMC;
};

#endif
