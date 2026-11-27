#include "JetPUIdEfficiencyFactory.h"
#include "EventContentPool.h"

JetPUIdEfficiencyFactory::JetPUIdEfficiencyFactory(const YAML::Node& fConfig_) {
  fName = fConfig_["Name"].as<std::string>();
  fPath = fConfig_["Path"].as<std::string>();
}

void JetPUIdEfficiencyFactory::Init(EventContentPool& pool) {
  fIsMC = pool.GetEventInfo().isMC;

  fNominal = 1.;

  fCorrection = correction::CorrectionSet::from_file(fPath)->at("PUJetID_eff");
}

float JetPUIdEfficiencyFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) {
  
  if (!fIsMC) return 1.;

  const auto& tHSJets = pool.Get<JET::JetsCollection>(fWeightInput.fJetSrc).fHSJets;

  if (fSystIdx == 0) {
    for (const auto& tJet : tHSJets) {
      
      float tSF = fCorrection->evaluate({tJet.fVec.Eta(), tJet.fVec.Pt(), "nom", "L"});
      float tEff = fCorrection->evaluate({tJet.fVec.Eta(), tJet.fVec.Pt(), "MCEff", "L"});

      if (tEff > 1.) {
        fNominal *= 1;
        continue;
      }
      
      if (tJet.fJetPUId == 1) {
        fNominal *= tSF;
      } else {
        fNominal *= (1. - tSF * tEff) / (1. - tEff);
      }
    }

    return fNominal;
  } else {
    std::string tSystName = "";
    if (fSystIdx == 1) tSystName = "down";
    else if (fSystIdx == 2) tSystName = "up";

    float tSystWeight = 1.;
    for (const auto& tJet : tHSJets) {
      
      float tSF = fCorrection->evaluate({tJet.fVec.Eta(), tJet.fVec.Pt(), tSystName, "L"});
      float tEff = fCorrection->evaluate({tJet.fVec.Eta(), tJet.fVec.Pt(), "MCEff", "L"});

      if (tEff > 1.) {
        fNominal *= 1;
        continue;
      }
      
      if (tJet.fJetPUId == 1) {
        tSystWeight *= tSF;
      } else {
        tSystWeight *= (1. - tSF * tEff) / (1. - tEff);
      }
    }

    return tSystWeight / fNominal;
  }

  return 1.;
}