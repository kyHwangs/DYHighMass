#include "FactoryMaker.h"

#include "MuonMomentumCorrectionFactory.h"
#include "JetEnergyCorrectionFactory.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace {

using FactoryMaker = std::function<std::unique_ptr<Factory>(const YAML::Node&)>;

template <typename T>
FactoryMaker Maker() {
  return [](const YAML::Node& fConfig) { return std::make_unique<T>(fConfig); };
}

const std::unordered_map<std::string, FactoryMaker> kFactoryRegistry = {
  {"JetEnergyCorrectionFactory", Maker<JetEnergyCorrectionFactory>()},
  {"MuonMomentumCorrectionFactory", Maker<MuonMomentumCorrectionFactory>()},
};

}

std::unique_ptr<Factory> MakeFactory(const YAML::Node& fConfig) {
  const std::string tType = fConfig["Type"].as<std::string>();

  const auto tFound = kFactoryRegistry.find(tType);
  if (tFound == kFactoryRegistry.end())
    throw std::runtime_error("Unknown factory type: " + tType);

  return tFound->second(fConfig);
}
