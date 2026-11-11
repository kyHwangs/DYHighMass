#ifndef DRESSEDLEPTONPRODUCER_H
#define DRESSEDLEPTONPRODUCER_H

#include "Producer.h"

#include "yaml-cpp/yaml.h"

#include <string>

#include "TLorentzVector.h"

class DressedLeptonProducer : public Producer {
public:
  DressedLeptonProducer(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "GenJetProducer";
    return kName;
  }

  void Init(EventContentPool& pool) override;
  void Produce(EventContentPool& pool) override;

  const std::string& InName() const override { return fName; }
  const std::string& OutputName() const override { return fOutputName; }
  
private:

  struct StoredDressedLepton {
    TLorentzVector fVec;
    int fCharge;
  };
  
  std::string fName;
  std::string fOutputName;

  float fLeadingPt;
  float fSubleadingPt;
  float fEta;
  int fPid;

  bool fIsMC;
  std::string fSampleName;
};
  
  #endif
  