#ifndef RECOMUONEFFICIENCYFACTORY_H
#define RECOMUONEFFICIENCYFACTORY_H

#include "EfficiencyFactory.h"

#include "correction.h"
#include "yaml-cpp/yaml.h"

#include "TLorentzVector.h"

#include <memory>
#include <string>

class EventContentPool;

class RecoMuonEfficiencyFactory : public EfficiencyFactory {
public:
  RecoMuonEfficiencyFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "RecoMuonEfficiencyFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  float GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst = "Nominal", const int fSystIdx = 0) override;
  void ResetWeight() override { fNominal = 1.; }
  
private:
  std::string fName;
  bool fIsMC;

  std::string fPath;
  std::string fSystPath;
  std::string fGetPath;

  float fNominal;

  std::shared_ptr<const correction::Correction> fCorrection;
  std::shared_ptr<const correction::Correction> fVariation;
 
};

#endif
