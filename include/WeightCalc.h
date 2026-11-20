#ifndef WEIGHTCALC_H
#define WEIGHTCALC_H

#include "EfficiencyFactory.h"
#include "GenWeightFactory.h"
#include "NormalizationFactory.h"

#include "yaml-cpp/yaml.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class EventContentPool;

class WeightCalc {
public:
  WeightCalc(const YAML::Node& fConfig_);

  const std::string& Name() const {
    static const std::string kName = "WeightCalc";
    return kName;
  }

  void Init(EventContentPool& pool);
  const std::string& InName() const { return fName; }

  float GetWeight(EventContentPool& pool, const EfficiencyFactory::WeightInput& fWeightInput, const std::string& fTarget, const std::string& fSyst = "Nominal", const int fSystIdx = 0);
  void ResetWeight();

private:
 std::string fName;
 std::string fSample;
 bool fIsMC;

 float fWeightReco;
 float fWeightGen;

 std::unique_ptr<GenWeightFactory> fGenWeightFactory;
 std::unique_ptr<NormalizationFactory> fNormalizationFactory;
 std::vector<YAML::Node> fEfficiencyConfigs;
 EfficiencyFactoryList fEfficiencyFactories;
};

#endif
