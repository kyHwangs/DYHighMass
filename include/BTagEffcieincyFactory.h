#ifndef BTAFFICIENCYFACTORY_H
#define BTAFFICIENCYFACTORY_H

#include "EfficiencyFactory.h"

#include "correction.h"
#include "yaml-cpp/yaml.h"

#include <memory>
#include <string>

class EventContentPool;

class BTagEffcieincyFactory : public EfficiencyFactory {
public:
  BTagEffcieincyFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "BTagEffcieincyFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  float GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst = "Nominal", const int fSystIdx = 0) override;
  void ResetWeight() override { fNominal = 1.; }

private:
  std::string fName;
  std::string fEfficiencyPath;
  std::string fSFPath;

  bool fIsMC;
  std::string fEra;
  std::string fSampleName;

  float fNominal;

  std::shared_ptr<const correction::Correction> fJetBTagEff;
  std::shared_ptr<const correction::Correction> fBTagMuJets;
  std::shared_ptr<const correction::Correction> fBTagIncl;
};

#endif
