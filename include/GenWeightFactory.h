#ifndef GENWEIGHTFACTORY_H
#define GENWEIGHTFACTORY_H

#include "EfficiencyFactory.h"
#include "EventProducts.h"
#include "EventContentPool.h"

#include "correction.h"
#include "yaml-cpp/yaml.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

class GenWeightFactory : public EfficiencyFactory {
public:
  GenWeightFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "GenWeightFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  float GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst = "Nominal", const int fSystIdx = 0) override;
  void ResetWeight() override { }

private:
  std::string fName;
  bool fIsMC;
  std::string fSampleName;

  std::vector<int> fPDFScaleIdx;
  
  TTreeReaderValue<float>* genWeight;
  TTreeReaderValue<float>* LHEWeight_originalXWGTUP;
  TTreeReaderValue<unsigned int>* nLHEPdfWeight;
  TTreeReaderArray<float>* LHEPdfWeight;
  TTreeReaderValue<unsigned int>* nLHEReweightingWeight;
  TTreeReaderArray<float>* LHEReweightingWeight;
  TTreeReaderValue<unsigned int>* nLHEScaleWeight;
  TTreeReaderArray<float>* LHEScaleWeight;
};

#endif
