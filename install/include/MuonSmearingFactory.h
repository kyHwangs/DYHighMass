#ifndef MUONSMEARINGFACTORY_H
#define MUONSMEARINGFACTORY_H

#include "EventContentPool.h"
#include "EventProducts.h"
#include "Factory.h"

#include "TLorentzVector.h"
#include "TRandom3.h"

#include <memory>
#include <string>

class MuonSmearingFactory : public Factory {
public:
  MuonSmearingFactory(const YAML::Node& fConfig_);
    
  const std::string& Name() const override {
    static const std::string kName = "MuonSmearingFactory";
    return kName;
  }
  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  const MUON::StdMuons GetSmeared(const MUON::StdMuons& fMuons, int fSyst);

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

  TRandom3 fRandomNominal;
  TRandom3 fRandomSyst;

  double fBarrelSmearingFactor;
  double fEndcapSmearingFactor;

  bool fIsMC;
};

#endif
