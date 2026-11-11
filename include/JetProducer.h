#ifndef JETPRODUCER_H
#define JETPRODUCER_H

#include "Producer.h"
#include "EventProducts.h"
#include "JetEnergyCorrectionFactory.h"

#include "yaml-cpp/yaml.h"

#include <memory>
#include <string>
#include <vector>

class JetProducer : public Producer {
public:
  JetProducer(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "JetProducer";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  void Produce(EventContentPool& pool) override;

  const std::string& InName() const override { return fName; }
  const std::string& OutputName() const override { return fOutputName; }
  
private:
  
  std::string fName;
  std::string fOutputName;

  std::string fSrc;
  std::string fMCSrc;
  float fPt;
  int fPUId;

  std::vector<JetSystConfigs> fCorrectionRegister;
  std::unique_ptr<JetEnergyCorrectionFactory> fJetEnergyCorrectionFactory;

  bool fIsMC;
};
  
  #endif
  