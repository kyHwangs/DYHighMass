#ifndef ZBOSONPRODUCER_H
#define ZBOSONPRODUCER_H

#include "Producer.h"
#include "EventProducts.h"
#include "MuonMomentumCorrectionFactory.h"

#include "yaml-cpp/yaml.h"

#include <memory>
#include <string>
#include <vector>

class ZbosonProducer : public Producer {
public:
  ZbosonProducer(const YAML::Node& fConfig_);

  const std::string& Name() const override {
    static const std::string kName = "ZbosonProducer";
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

  float fLeadingPt;
  float fSubLeadingPt;
  int fChargeConj;
  int fIsolationConj;

  std::vector<MuonSystConfigs> fCorrectionRegister;
  std::unique_ptr<MuonMomentumCorrectionFactory> fMomentumCorrectionFactory;
  
  bool fIsMC;
};
  
#endif
  
