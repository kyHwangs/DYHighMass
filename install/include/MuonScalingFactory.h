#ifndef MUONSCALINGFACTORY_H
#define MUONSCALINGFACTORY_H

#include "EventContentPool.h"
#include "EventProducts.h"
#include "Factory.h"

#include "correction.h"

#include "TLorentzVector.h"

#include <memory>
#include <string>

class MuonScalingFactory : public Factory {
public:
  MuonScalingFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "MuonScalingFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  float GetScaledPt(float fPt, float fCharge, float fBias);
  const MUON::StdMuons GetScaled(const MUON::StdMuons& fMuons, int fSyst);

private:
  std::string fName;  
  std::string fNominalPath;
  std::string fVariationPath;

  std::shared_ptr<const correction::Correction> fNominal;
  std::shared_ptr<const correction::Correction> fVariation;

  bool fIsMC;
};

#endif
