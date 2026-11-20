#include "L1PrefiringFactory.h"
#include "EventContentPool.h"

L1PrefiringFactory::L1PrefiringFactory(const YAML::Node& fConfig_) {
  
  fName = fConfig_["Name"].as<std::string>();
}

void L1PrefiringFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;

  if (fIsMC) {
    const auto& nt = pool.Source();
    auto fTreeReader = nt.GetTreeReader();

    L1PreFiringWeight_Nom = new TTreeReaderValue<float>(*fTreeReader, "L1PreFiringWeight_Nom");
    L1PreFiringWeight_Dn = new TTreeReaderValue<float>(*fTreeReader, "L1PreFiringWeight_Dn");
    L1PreFiringWeight_Up = new TTreeReaderValue<float>(*fTreeReader, "L1PreFiringWeight_Up");  
  }
}

float L1PrefiringFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) {
  if (!fIsMC) return 1.;
  
  if (fSystIdx == 0) return **L1PreFiringWeight_Nom;
  if (fSystIdx == 1) return **L1PreFiringWeight_Dn / **L1PreFiringWeight_Nom;
  if (fSystIdx == 2) return **L1PreFiringWeight_Up / **L1PreFiringWeight_Nom;

  return 1.;
}

