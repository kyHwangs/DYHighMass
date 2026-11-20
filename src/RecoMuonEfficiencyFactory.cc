#include "RecoMuonEfficiencyFactory.h"
#include "EventContentPool.h"

#include <cmath>
#include <stdexcept>

RecoMuonEfficiencyFactory::RecoMuonEfficiencyFactory(const YAML::Node& fConfig_) {

  fName = fConfig_["Name"].as<std::string>();
  fPath = fConfig_["Path"].as<std::string>();
  fSystPath = fConfig_["SystPath"].as<std::string>();
  fGetPath = fConfig_["GetPath"].as<std::string>();
}

void RecoMuonEfficiencyFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;

  fNominal = 1.;

  fCorrection = correction::CorrectionSet::from_file(fPath)->at(fGetPath);
  fVariation = correction::CorrectionSet::from_file(fSystPath)->at(fGetPath);
}

float RecoMuonEfficiencyFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) {
  
  if (!fIsMC) return 1.;

  if (fSystIdx == 0 && fSyst != "Nominal") throw std::runtime_error("RecoMuonEfficiencyFactory: SystIdx is 0 but Syst is not nominal");
  if (fSystIdx != 0 && fSyst == "Nominal") throw std::runtime_error("RecoMuonEfficiencyFactory: SystIdx is not 0 but Syst is nominal");

  const auto& tMuonPair = pool.Get<MUON::MuonPair>(fWeightInput.fMuonSrc);
  if (tMuonPair.fMass <= 0) return 1.;

  const TLorentzVector& fVec1 = tMuonPair.fFirst.fVecRaw;
  const TLorentzVector& fVec2 = tMuonPair.fSecond.fVecRaw;
  
  float fP1 = fVec1.P();
  float fP2 = fVec2.P();
  
  if (std::abs(fVec1.Eta()) < 1.6) fP1 = fP1 < 50 ? 50.1 : fP1;
  else fP1 = fP1 < 100 ? 100.1 : fP1;

  if (std::abs(fVec2.Eta()) < 1.6) fP2 = fP2 < 50 ? 50.1 : fP2;
  else fP2 = fP2 < 100 ? 100.1 : fP2;

  if (fSystIdx == 0) {
    fNominal = fCorrection->evaluate({std::abs(fVec1.Eta()), fP1, "nominal"});
    fNominal *= fCorrection->evaluate({std::abs(fVec2.Eta()), fP2, "nominal"});
    return fNominal;
  } else {
    return  (
      (fCorrection->evaluate({std::abs(fVec1.Eta()), fP1, "nominal"}) + fVariation->evaluate({"variation_" + std::to_string(fSystIdx), std::abs(fVec1.Eta()), fP1}) * fCorrection->evaluate({std::abs(fVec1.Eta()), fP1, fSyst.c_str()})) *
      (fCorrection->evaluate({std::abs(fVec2.Eta()), fP2, "nominal"}) + fVariation->evaluate({"variation_" + std::to_string(fSystIdx), std::abs(fVec2.Eta()), fP2}) * fCorrection->evaluate({std::abs(fVec2.Eta()), fP2, fSyst.c_str()}))
    ) / fNominal;
  }

  return 1.;
}