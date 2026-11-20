#include "SingleMuonEfficiencyFactory.h"
#include "EventContentPool.h"

#include <cmath>
#include <stdexcept>

SingleMuonEfficiencyFactory::SingleMuonEfficiencyFactory(const YAML::Node& fConfig_) {

  fName = fConfig_["Name"].as<std::string>();
  fPath = fConfig_["Path"].as<std::string>();
  fSystPath = fConfig_["SystPath"].as<std::string>();
  fGetPath = fConfig_["GetPath"].as<std::string>();
}

void SingleMuonEfficiencyFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;

  fNominal = 1.;

  fCorrection = correction::CorrectionSet::from_file(fPath)->at(fGetPath);
  fVariation = correction::CorrectionSet::from_file(fSystPath)->at(fGetPath);
}

float SingleMuonEfficiencyFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) {
  
  if (!fIsMC) return 1.;

  if (fSystIdx == 0 && fSyst != "Nominal") throw std::runtime_error("SingleMuonEfficiencyFactory: SystIdx is 0 but Syst is not nominal");
  if (fSystIdx != 0 && fSyst == "Nominal") throw std::runtime_error("SingleMuonEfficiencyFactory: SystIdx is not 0 but Syst is nominal");

  const auto& tMuonPair = pool.Get<MUON::MuonPair>(fWeightInput.fMuonSrc);
  if (tMuonPair.fMass <= 0) return 1.;

  const TLorentzVector& fVec1 = tMuonPair.fFirst.fVecRaw;
  const TLorentzVector& fVec2 = tMuonPair.fSecond.fVecRaw;

  float fPt1 = fVec1.Pt() > 50 ? fVec1.Pt() : 50.1;
  float fPt2 = fVec2.Pt() > 50 ? fVec2.Pt() : 50.1;

  if (fSystIdx == 0) {
    fNominal = fCorrection->evaluate({std::abs(fVec1.Eta()), fPt1, "nominal"});
    fNominal *= fCorrection->evaluate({std::abs(fVec2.Eta()), fPt2, "nominal"});
    return fNominal;
  }
  else {
    return  (
      (fCorrection->evaluate({std::abs(fVec1.Eta()), fPt1, "nominal"}) + fVariation->evaluate({"variation_" + std::to_string(fSystIdx), std::abs(fVec1.Eta()), fPt1}) * fCorrection->evaluate({std::abs(fVec1.Eta()), fPt1, fSyst.c_str()})) *
      (fCorrection->evaluate({std::abs(fVec2.Eta()), fPt2, "nominal"}) + fVariation->evaluate({"variation_" + std::to_string(fSystIdx), std::abs(fVec2.Eta()), fPt2}) * fCorrection->evaluate({std::abs(fVec2.Eta()), fPt2, fSyst.c_str()}))
    ) / fNominal;
  }

  return 1.;
}