#ifndef RAWMUONPRODUCER_H
#define RAWMUONPRODUCER_H

#include "Producer.h"

#include "yaml-cpp/yaml.h"

#include <string>

#include "TRandom3.h"

class RawMuonProducer : public Producer {
public:
  RawMuonProducer(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "RawMuonProducer";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  void Produce(EventContentPool& pool) override;

  const std::string& InName() const override { return fName; }
  const std::string& OutputName() const override { return fOutputName; }
  
private:
  
  std::string fName;
  std::string fOutputName;

  TRandom3 fRandom;

  float fEta;
  unsigned char fId;
  float fIso;
};
  
  #endif
  