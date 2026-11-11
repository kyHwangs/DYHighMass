#ifndef JETENERGYCORRECTIONFACTORY_H
#define JETENERGYCORRECTIONFACTORY_H

#include "Factory.h"
#include "EventProducts.h"

#include "correction.h"
#include "yaml-cpp/yaml.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class JetEnergyCorrectionFactory : public Factory {
public:
  JetEnergyCorrectionFactory(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "JetEnergyCorrectionFactory";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  const std::string& InName() const override { return fName; }

  const std::vector<JET::StdJets> Apply(const JET::RawJets& fJets, const GEN::Jets& fGenJets, const std::vector<JetSystConfigs>& fSystConfigs);

private:
 std::string fName;
 std::string fPath;
 bool fIsMC;
 
 std::vector<std::string> fSystNames;
 std::unordered_map<std::string, std::shared_ptr<const correction::Correction>> fCorrections;
 std::unordered_map<bool, std::shared_ptr<const correction::CompoundCorrection>> fCorrectionNominal;
 std::unordered_map<std::string, std::shared_ptr<const correction::Correction>> fSmearing;
};

#endif
