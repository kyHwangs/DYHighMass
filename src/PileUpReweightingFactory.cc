#include "PileUpReweightingFactory.h"
#include "EventContentPool.h"

PileUpReweightingFactory::PileUpReweightingFactory(const YAML::Node& fConfig_) {
  
  fName = fConfig_["Name"].as<std::string>();

  fDataPath = fConfig_["DataPath"].as<std::string>();
  fDataUpPath = fConfig_["DataUp"].as<std::string>();
  fDataDownPath = fConfig_["DataDown"].as<std::string>();
  fMCPath = fConfig_["MCPath"].as<std::string>();
}

void PileUpReweightingFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;

  fNominal = 1.;

  fPuReweightingNominal = std::make_unique<LumiReWeighting>(fMCPath, fDataPath, "pileup", "pileup");
  fPuReweightingUp = std::make_unique<LumiReWeighting>(fMCPath, fDataUpPath, "pileup", "pileup");
  fPuReweightingDown = std::make_unique<LumiReWeighting>(fMCPath, fDataDownPath, "pileup", "pileup");

  if (fIsMC) {
    const auto& nt = pool.Source();
    auto fTreeReader = nt.GetTreeReader();

    Pileup_nTrueInt = new TTreeReaderValue<float>(*fTreeReader, "Pileup_nTrueInt");
  }
}

float PileUpReweightingFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) {
  if (!fIsMC) return 1.;
  
  if (fSystIdx == 0) {
    fNominal = fPuReweightingNominal->weight(**Pileup_nTrueInt);
    return fNominal;
  }
  if (fSystIdx == 1) return fPuReweightingDown->weight(**Pileup_nTrueInt) / fNominal;
  if (fSystIdx == 2) return fPuReweightingUp->weight(**Pileup_nTrueInt) / fNominal;

  return 1.;
}

