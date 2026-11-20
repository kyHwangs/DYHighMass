#ifndef EFFICIENCYFACTORY_H
#define EFFICIENCYFACTORY_H

#include <memory>
#include <string>
#include <unordered_map>

class EventContentPool;

class EfficiencyFactory {
public:
  virtual ~EfficiencyFactory() = default;

  virtual const std::string& Name() const = 0;
  virtual void Init(EventContentPool& pool) = 0;

  virtual const std::string& InName() const = 0;

  struct WeightInput {
    std::string fMuonSrc;
    std::string fJetSrc;
  };
  
  virtual float GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst = "Nominal", const int fSystIdx = 0) = 0;
  virtual void ResetWeight() = 0;
};

using EfficiencyFactoryList = std::unordered_map<std::string, std::unique_ptr<EfficiencyFactory>>;

#endif
