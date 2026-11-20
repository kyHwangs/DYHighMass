#ifndef TOPPTREWEIGHTINGFACTORY_H
#define TOPPTREWEIGHTINGFACTORY_H

#include "EfficiencyFactory.h"
#include "EventContentPool.h"

#include "yaml-cpp/yaml.h"

#include "TF1.h"
#include "TLorentzVector.h"

#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

class TopPtReweightingFactory : public EfficiencyFactory {
public:
  TopPtReweightingFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "TopPtReweightingFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  float GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst = "Nominal", const int fSystIdx = 0) override;
  void ResetWeight() override { fNominal = 1.; }

private:
 std::string fName;
 bool fIsMC;
 std::string fSampleName;

 float fNominal;

 TF1* fReweightingFunction;
};

#endif
