#include "TopPtReweightingFactory.h"

TopPtReweightingFactory::TopPtReweightingFactory(const YAML::Node& fConfig) {

  fName = fConfig["Name"].as<std::string>();
  fReweightingFunction = new TF1("fReweightingFunction", fConfig["Formula"].as<std::string>().c_str(), 0, 600);
}

void TopPtReweightingFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;
  fSampleName = pool.GetEventInfo().sampleName;
}

float TopPtReweightingFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) { 

  if (!fIsMC) return 1.;
  if (fSampleName != "TTTo2L2Nu") return 1.;

  if (fSystIdx == 0) {

    const auto& nt = pool.Source();
    
    std::vector<TLorentzVector> tGenTopVec;
    for (int i = 0; i < **nt.nGenPart; i++) {
      if (std::abs(nt.GenPart_pdgId->At(i)) == 6 && nt.GenPart_status->At(i) == 22 && nt.GenPart_genPartIdxMother->At(i) == 0) {
        TLorentzVector tTmpVec;
        tTmpVec.SetPtEtaPhiM(nt.GenPart_pt->At(i), nt.GenPart_eta->At(i), nt.GenPart_phi->At(i), nt.GenPart_mass->At(i));
        tGenTopVec.push_back(tTmpVec);
      }
    }

    if (tGenTopVec.size() != 2) throw std::runtime_error("TopPtReweightingFactory::GetValue - Number of gen tops is not 2!");
    
    float tGenTopPtReweightFactor = 1.;
    for (int i = 0; i < tGenTopVec.size(); i++) {
      if (tGenTopVec.at(i).Pt() >500) tGenTopPtReweightFactor *= fReweightingFunction->Eval(500);
      else tGenTopPtReweightFactor *= fReweightingFunction->Eval(tGenTopVec.at(i).Pt());
    }

    fNominal = std::sqrt(tGenTopPtReweightFactor);
  
    return fNominal;

  } else {
    return 1. / fNominal;
  }
}