#ifndef L1PREFIRINGFACTORY_H
#define L1PREFIRINGFACTORY_H

#include "EfficiencyFactory.h"

#include "yaml-cpp/yaml.h"

#include "TTreeReaderValue.h"

#include <string>

class EventContentPool;

class L1PrefiringFactory : public EfficiencyFactory {
public:
  L1PrefiringFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "L1PrefiringFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  float GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst = "Nominal", const int fSystIdx = 0) override;
  void ResetWeight() override {}

private:
  std::string fName;
  bool fIsMC;

  TTreeReaderValue<float>* L1PreFiringWeight_Nom;
  TTreeReaderValue<float>* L1PreFiringWeight_Dn;
  TTreeReaderValue<float>* L1PreFiringWeight_Up; 
};

#endif
