#include "ProducerMaker.h"

#include "LHEProducer.h"
#include "RawMuonProducer.h"
#include "ZbosonProducer.h"
#include "DressedLeptonProducer.h"
#include "GenJetProducer.h"
#include "RawJetProducer.h"
#include "JetProducer.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace {

using ProducerMaker = std::function<std::unique_ptr<Producer>(const YAML::Node&)>;

template <typename T>
ProducerMaker Maker() {
  return [](const YAML::Node& fConfig) { return std::make_unique<T>(fConfig); };
}

const std::unordered_map<std::string, ProducerMaker> kProducerRegistry = {
  {"RawMuonProducer", Maker<RawMuonProducer>()},
  {"DressedLeptonProducer", Maker<DressedLeptonProducer>()},
  {"RawJetProducer",  Maker<RawJetProducer>()},
  {"GenJetProducer",  Maker<GenJetProducer>()},
  {"LHEProducer",     Maker<LHEProducer>()},
  {"ZbosonProducer",  Maker<ZbosonProducer>()},
  {"JetProducer", Maker<JetProducer>()},
};

}

std::unique_ptr<Producer> MakeProducer(const YAML::Node& fConfig) {
  const std::string tType = fConfig["Type"].as<std::string>();

  const auto tFound = kProducerRegistry.find(tType);
  if (tFound == kProducerRegistry.end())
    throw std::runtime_error("Unknown producer type: " + tType);

  return tFound->second(fConfig);
}
