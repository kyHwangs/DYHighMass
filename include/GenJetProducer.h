#ifndef GENJETPRODUCER_H
#define GENJETPRODUCER_H

#include "Producer.h"

#include "yaml-cpp/yaml.h"

#include <string>

class GenJetProducer : public Producer {
public:
  GenJetProducer(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "GenJetProducer";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  void Produce(EventContentPool& pool) override;

  const std::string& InName() const override { return fName; }
  const std::string& OutputName() const override { return fOutputName; }
  
private:
  
  std::string fName;
  std::string fOutputName;

  float fPt;
  float fEta;
  std::string fVeto;

  bool fIsMC;
};
  
  #endif
  