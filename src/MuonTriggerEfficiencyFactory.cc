#include "MuonTriggerEfficiencyFactory.h"
#include "EventContentPool.h"

#include <cmath>
#include <stdexcept>

MuonTriggerEfficiencyFactory::MuonTriggerEfficiencyFactory(const YAML::Node& fConfig_) {

  fName = fConfig_["Name"].as<std::string>();
  fPath = fConfig_["Path"].as<std::string>();
  fDataPath = fConfig_["Data"].as<std::string>();
  fMCPath = fConfig_["MC"].as<std::string>();
  fSystPath = fConfig_["SystPath"].as<std::string>();
}

void MuonTriggerEfficiencyFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;

  const auto tCorrectionSet = correction::CorrectionSet::from_file(fPath);
  fCorrectionData = tCorrectionSet->at(fDataPath);
  fCorrectionMC = tCorrectionSet->at(fMCPath);
  
  const auto tVariationSet = correction::CorrectionSet::from_file(fSystPath);
  fVariationData = tVariationSet->at(fDataPath);
  fVariationMC = tVariationSet->at(fMCPath);
}

float MuonTriggerEfficiencyFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) {
  
  if (!fIsMC) return 1.;

  if (fSystIdx == 0 && fSyst != "Nominal") throw std::runtime_error("MuonTriggerEfficiencyFactory: SystIdx is 0 but Syst is not nominal");
  if (fSystIdx != 0 && fSyst == "Nominal") throw std::runtime_error("MuonTriggerEfficiencyFactory: SystIdx is not 0 but Syst is nominal");

  const auto& tMuonPair = pool.Get<MUON::MuonPair>(fWeightInput.fMuonSrc);
  if (tMuonPair.fMass <= 0) return 1.;

  const TLorentzVector& fVec1 = tMuonPair.fFirst.fVecRaw;
  const TLorentzVector& fVec2 = tMuonPair.fSecond.fVecRaw;

  float fPt1 = fVec1.Pt() > 50 ? fVec1.Pt() : 50.1;
  float fPt2 = fVec2.Pt() > 50 ? fVec2.Pt() : 50.1;

  if (fSystIdx == 0) {
    const float eData1 = fCorrectionData->evaluate({std::abs(fVec1.Eta()), fPt1, "nominal"});
    const float eData2 = fCorrectionData->evaluate({std::abs(fVec2.Eta()), fPt2, "nominal"});
    const float eMC1 = fCorrectionMC->evaluate({std::abs(fVec1.Eta()), fPt1, "nominal"});
    const float eMC2 = fCorrectionMC->evaluate({std::abs(fVec2.Eta()), fPt2, "nominal"});
  
    fNominal = (1. - (1. - eData1) * (1. - eData2)) / (1. - (1. - eMC1) * (1. - eMC2));
    return fNominal;
  } else {
    
    bool tIsDataVariation = false;
    if (fSyst.find("data") != std::string::npos) tIsDataVariation = true;

    const auto p = fSyst.find(':');
    const std::string tKey  = fSyst.substr(p + 1);
    
    if (tIsDataVariation) { 
      const float eData1 = fCorrectionData->evaluate({std::abs(fVec1.Eta()), fPt1, "nominal"}) + fVariationData->evaluate({"variation_" + std::to_string(fSystIdx), std::abs(fVec1.Eta()), fPt1}) * fCorrectionData->evaluate({std::abs(fVec1.Eta()), fPt1, tKey.c_str()});
      const float eData2 = fCorrectionData->evaluate({std::abs(fVec2.Eta()), fPt2, "nominal"}) + fVariationData->evaluate({"variation_" + std::to_string(fSystIdx), std::abs(fVec2.Eta()), fPt2}) * fCorrectionData->evaluate({std::abs(fVec2.Eta()), fPt2, tKey.c_str()});
      const float eMC1 = fCorrectionMC->evaluate({std::abs(fVec1.Eta()), fPt1, "nominal"});
      const float eMC2 = fCorrectionMC->evaluate({std::abs(fVec2.Eta()), fPt2, "nominal"});

      float tEffVaiation = (1. - (1. - eData1) * (1. - eData2)) / (1. - (1. - eMC1) * (1. - eMC2));
      return tEffVaiation / fNominal;
    } else {
      const float eData1 = fCorrectionData->evaluate({std::abs(fVec1.Eta()), fPt1, "nominal"});
      const float eData2 = fCorrectionData->evaluate({std::abs(fVec2.Eta()), fPt2, "nominal"});
      const float eMC1 = fCorrectionMC->evaluate({std::abs(fVec1.Eta()), fPt1, "nominal"}) + fVariationMC->evaluate({"variation_" + std::to_string(fSystIdx), std::abs(fVec1.Eta()), fPt1}) * fCorrectionMC->evaluate({std::abs(fVec1.Eta()), fPt1, tKey.c_str()});
      const float eMC2 = fCorrectionMC->evaluate({std::abs(fVec2.Eta()), fPt2, "nominal"}) + fVariationMC->evaluate({"variation_" + std::to_string(fSystIdx), std::abs(fVec2.Eta()), fPt2}) * fCorrectionMC->evaluate({std::abs(fVec2.Eta()), fPt2, tKey.c_str()});

      float tEffVaiation = (1. - (1. - eData1) * (1. - eData2)) / (1. - (1. - eMC1) * (1. - eMC2));
      return tEffVaiation / fNominal;
    }
  }

  return 1.;
}