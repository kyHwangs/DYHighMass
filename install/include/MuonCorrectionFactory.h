#ifndef MUONCORRECTIONFACTORY_H
#define MUONCORRECTIONFACTORY_H

#include "Factory.h"
#include "MuonCorrection.h"

#include "yaml-cpp/yaml.h"

#include <memory>

class MuonCorrectionFactory : public Factory {
public:
  virtual std::unique_ptr<MuonCorrection> MakeCorrection(const YAML::Node& fStepConfig) = 0;
};

#endif
