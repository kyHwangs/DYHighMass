#ifndef NORMALIZATIONFACTORY_H
#define NORMALIZATIONFACTORY_H

#include "EfficiencyFactory.h"
#include "LumiReWeighting.h"

#include "yaml-cpp/yaml.h"

#include "TTreeReaderValue.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

class EventContentPool;

class NormalizationFactory : public EfficiencyFactory {
public:
  NormalizationFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "NormalizationFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  float GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst = "Nominal", const int fSystIdx = 0) override;
  void ResetWeight() override { }

  void SetNormFactor();

private:
 std::string fName;
 std::string fEra;
 bool fIsMC;
 std::string fSampleName;

 std::map<std::string, std::vector<float>> fNormFactor;

};

#endif
