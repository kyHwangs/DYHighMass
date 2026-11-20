#ifndef PILEUPREWEIGHTINGFACTORY_H
#define PILEUPREWEIGHTINGFACTORY_H

#include "EfficiencyFactory.h"
#include "LumiReWeighting.h"

#include "yaml-cpp/yaml.h"

#include "TTreeReaderValue.h"

#include <memory>
#include <string>

class EventContentPool;

class PileUpReweightingFactory : public EfficiencyFactory {
public:
  PileUpReweightingFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "PileUpReweightingFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  float GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst = "Nominal", const int fSystIdx = 0) override;
  void ResetWeight() override { fNominal = 1.; }

private:
 std::string fName;
 bool fIsMC;

 std::string fDataPath;
 std::string fDataUpPath;
 std::string fDataDownPath;
 std::string fMCPath;

 TTreeReaderValue<float>* Pileup_nTrueInt;

 float fNominal;

 std::unique_ptr<LumiReWeighting> fPuReweightingNominal;
 std::unique_ptr<LumiReWeighting> fPuReweightingUp;
 std::unique_ptr<LumiReWeighting> fPuReweightingDown;
};

#endif
