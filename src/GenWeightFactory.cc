#include "GenWeightFactory.h"
#include "EventContentPool.h"

GenWeightFactory::GenWeightFactory(const YAML::Node& fConfig_) {
  
  fName = fConfig_["Name"].as<std::string>();
}

void GenWeightFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;
  fSampleName = pool.GetEventInfo().sampleName;

  fPDFScaleIdx = {0, 1, 3, 5, 7, 8};

  if (fIsMC) {
    const auto& nt = pool.Source();
    auto fTreeReader = nt.GetTreeReader();

    genWeight = new TTreeReaderValue<float>(*fTreeReader, "genWeight");
    if (
      !(fSampleName.find("GGToMuMu") != std::string::npos || fSampleName.find("WZ") != std::string::npos || fSampleName.find("ZZ") != std::string::npos)
    ) {
       
      LHEWeight_originalXWGTUP = new TTreeReaderValue<float>(*fTreeReader, "LHEWeight_originalXWGTUP");
      nLHEPdfWeight = new TTreeReaderValue<unsigned int>(*fTreeReader, "nLHEPdfWeight");
      LHEPdfWeight = new TTreeReaderArray<float>(*fTreeReader, "LHEPdfWeight");
      nLHEReweightingWeight = new TTreeReaderValue<unsigned int>(*fTreeReader, "nLHEReweightingWeight");
      LHEReweightingWeight = new TTreeReaderArray<float>(*fTreeReader, "LHEReweightingWeight");
      nLHEScaleWeight = new TTreeReaderValue<unsigned int>(*fTreeReader, "nLHEScaleWeight");
      LHEScaleWeight = new TTreeReaderArray<float>(*fTreeReader, "LHEScaleWeight");  
    }
  }
}

float GenWeightFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) {
  if (!fIsMC) return 1.;
  
  if (fSystIdx == 0) {
    float tNominal = **genWeight;
    if (fSampleName.find("NNLO") != std::string::npos) tNominal = tNominal > 0 ? 1. : -1.;
    return tNominal;
  } else {
    if (fSampleName.find("GGToMuMu") != std::string::npos || fSampleName.find("WZ") != std::string::npos || fSampleName.find("ZZ") != std::string::npos) 
      return 1.;

    if (fSyst == "Hessian") {
      return LHEPdfWeight->At(fSystIdx);
    }
    if (fSyst == "alphaS") {
      if (**nLHEPdfWeight != 103)
        return 1.;

      return LHEPdfWeight->At(fSystIdx + 100);
    }
    if (fSyst == "scale") {
      if (fSampleName.find("NNLO") != std::string::npos) return LHEScaleWeight->At(2 * fPDFScaleIdx[fSystIdx - 1]);
      return LHEScaleWeight->At(fPDFScaleIdx[fSystIdx - 1]);
    }
  }

  return 1.;
}

