#include "WeightCalc.h"
#include "EventContentPool.h"
#include "EfficiencyFactoryMaker.h"

#include <iostream>

WeightCalc::WeightCalc(const YAML::Node& fConfig) {

  fName = "WeightCalc";

  YAML::Node tGenWeightConfig;
  tGenWeightConfig["Name"] = "GenWeight";
  fGenWeightFactory = std::make_unique<GenWeightFactory>(tGenWeightConfig);

  YAML::Node tNormalizationConfig;
  tNormalizationConfig["Name"] = "Normalization";
  fNormalizationFactory = std::make_unique<NormalizationFactory>(tNormalizationConfig);

  std::cout << "######################################################################" << std::endl;
  std::cout << " Weight Calculator:" << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " - Pre-defined: GenWeightFactory -> GenWeight" << std::endl;
  for (const auto& tEffConfig : fConfig["EfficinecyFactory"]) {
    fEfficiencyConfigs.push_back(tEffConfig);
    std::cout << " - Registered: " << tEffConfig["Type"].as<std::string>() << " " << tEffConfig["Name"].as<std::string>() << std::endl;
  }

  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;

  fWeightReco = 1.;
  fWeightGen = 1.;
}

void WeightCalc::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;
  fSample = pool.GetEventInfo().sampleName;

  if (fIsMC) 
    for (const auto& tEffConfig : fEfficiencyConfigs)
      fEfficiencyFactories.emplace(tEffConfig["Name"].as<std::string>(), MakeEfficiencyFactory(tEffConfig));

  fGenWeightFactory->Init(pool);
  fNormalizationFactory->Init(pool);
  for (auto& tFactory : fEfficiencyFactories)
    tFactory.second->Init(pool);
}

float WeightCalc::GetWeight(EventContentPool& pool, const EfficiencyFactory::WeightInput& fWeightInput, const std::string& fTarget,const std::string& fSyst, const int fSystIdx) {
  
  if (!fIsMC) return 1.;

  if (fSyst.find("Nominal") != std::string::npos && fSystIdx != 0) std::runtime_error("Nominal syst is not allowed to be used for other systs");
  if (fSystIdx == 0 && fSyst.find("Nominal") == std::string::npos) std::runtime_error("Nominal syst is required for nominal weight");

  if (fTarget == "GenWeight") {
    
    return fGenWeightFactory->GetValue(pool, fWeightInput, fSyst, fSystIdx);
  }

  if (fTarget == "Normalization") {
    
    return fNormalizationFactory->GetValue(pool, fWeightInput, fSyst, fSystIdx);
  }

  if (fSystIdx == 0 && fTarget == "Reco") {
    for (const auto& tFactory : fEfficiencyFactories)
      fWeightReco *= tFactory.second->GetValue(pool, fWeightInput, fSyst, fSystIdx);
    
    return fWeightReco;
  } else {

    float tSystWeight = fEfficiencyFactories[fTarget]->GetValue(pool, fWeightInput, fSyst, fSystIdx);
    return tSystWeight;
  }
}

void WeightCalc::ResetWeight() {

  fWeightReco = 1.;
  for (auto& tFactory : fEfficiencyFactories)
    tFactory.second->ResetWeight();

}
