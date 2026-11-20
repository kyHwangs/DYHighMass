#include "EfficiencyFactoryMaker.h"

#include "BTagEffcieincyFactory.h"
#include "EfficiencyFactory.h"
#include "EfficiencyFactoryMaker.h"
#include "JetPUIdEfficiencyFactory.h"
#include "L1PrefiringFactory.h"
#include "MuonTriggerEfficiencyFactory.h"
#include "PileUpReweightingFactory.h"
#include "RecoMuonEfficiencyFactory.h"
#include "SingleMuonEfficiencyFactory.h"
#include "TopPtReweightingFactory.h"
#include "GenWeightFactory.h"
#include "NormalizationFactory.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace {

using EfficiencyFactoryMaker = std::function<std::unique_ptr<EfficiencyFactory>(const YAML::Node&)>;

template <typename T> 
EfficiencyFactoryMaker Maker() {
  return [](const YAML::Node& fConfig) { return std::make_unique<T>(fConfig); };
}

const std::unordered_map<std::string, EfficiencyFactoryMaker> kFactoryRegistry = {
  {"BTagEffcieincyFactory", Maker<BTagEffcieincyFactory>()},
  {"JetPUIdEfficiencyFactory", Maker<JetPUIdEfficiencyFactory>()},
  {"L1PrefiringFactory", Maker<L1PrefiringFactory>()},
  {"MuonTriggerEfficiencyFactory", Maker<MuonTriggerEfficiencyFactory>()},
  {"PileUpReweightingFactory", Maker<PileUpReweightingFactory>()},
  {"RecoMuonEfficiencyFactory", Maker<RecoMuonEfficiencyFactory>()},
  {"SingleMuonEfficiencyFactory", Maker<SingleMuonEfficiencyFactory>()},
  {"TopPtReweightingFactory", Maker<TopPtReweightingFactory>()},
  {"GenWeightFactory", Maker<GenWeightFactory>()},
  {"NormalizationFactory", Maker<NormalizationFactory>()}
};

}

std::unique_ptr<EfficiencyFactory> MakeEfficiencyFactory(const YAML::Node& fConfig) {
  const std::string tType = fConfig["Type"].as<std::string>();

  const auto tFound = kFactoryRegistry.find(tType);
  if (tFound == kFactoryRegistry.end())
    throw std::runtime_error("Unknown efficiency factory type: " + tType);

  return tFound->second(fConfig);
}
