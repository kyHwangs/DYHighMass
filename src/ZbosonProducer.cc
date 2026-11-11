#include "ZbosonProducer.h"
#include "EventContentPool.h"

#include <iostream>

ZbosonProducer::ZbosonProducer(const YAML::Node& fConfig) {

  fName = fConfig["Name"].as<std::string>();
  fOutputName = fConfig["OutputName"].as<std::string>();
  fSrc = fConfig["src"].as<std::string>();

  fLeadingPt = fConfig["LeadingMuonPt"].as<float>();
  fSubLeadingPt = fConfig["SubLeadingMuonPt"].as<float>();
  fChargeConj = fConfig["ChargeConj"].as<int>();
  fIsolationConj = fConfig["IsolationConj"].as<int>();

  fMomentumCorrectionFactory = std::make_unique<MuonMomentumCorrectionFactory>(fConfig["Correction"]["MuonMomentumCorrection"]);

  std::cout << "######################################################################" << std::endl;
  std::cout << " " << Name() << ": " << InName() << "(" << fSrc << ") -> " << OutputName() << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " fLeadingPt: " << fLeadingPt << std::endl;
  std::cout << " fSubLeadingPt: " << fSubLeadingPt << std::endl;
  std::cout << " fChargeConj: " << fChargeConj << std::endl;
  std::cout << " fIsolationConj: " << fIsolationConj << std::endl;

  std::cout << " fCorrectionFactories: " << std::endl;
  std::cout << "  - " << fMomentumCorrectionFactory->Name() << " " << fMomentumCorrectionFactory->InName() << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl; 
}

void ZbosonProducer::Init(EventContentPool& pool) {

  fMomentumCorrectionFactory->Init(pool);
  fIsMC = pool.GetEventInfo().isMC;

  fCorrectionRegister.push_back(
    MuonSystConfigs{
      "Nominal",
      fOutputName,
      "None",
      0
    }
  );

  if (fIsMC) {
    const auto& tSystematics = pool.GetSystematics();

    for (const auto& tSystematic : tSystematics) {
      if (tSystematic.first == "MuonSmearingSyst") {
        fCorrectionRegister.push_back(
          MuonSystConfigs{
            tSystematic.first,
            tSystematic.second["MuonSrc"].as<std::string>() + "_1",
            "Smearing",
            0
          }
        );
      }

      if (tSystematic.first == "MuonScalingSyst") {
        for(int i = 1; i <= tSystematic.second["Iter"].as<int>(); i++) {
          fCorrectionRegister.push_back(
            MuonSystConfigs{
              tSystematic.first,
              tSystematic.second["MuonSrc"].as<std::string>() + "_" + std::to_string(i),
              "Scaling",
              i
            }
          );
        }
      }
    }
  }

  std::cout << "######################################################################" << std::endl;
  std::cout << " Muon associated systematics:" << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  for (const auto& tCorrection : fCorrectionRegister) {

    std::string tSystCase = "";
    if (tCorrection.fSystType == "Smearing") tSystCase = " Smearing";
    if (tCorrection.fSystType == "Scaling") tSystCase = " Scaling (Iter " + std::to_string(tCorrection.iIter) + ")";

    std::cout << " - " << tCorrection.fSystName << " : " << tCorrection.fOutputName << " " << tSystCase << std::endl;
  }
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;
}

void ZbosonProducer::Produce(EventContentPool& pool) {

  const auto& tMuons = pool.Get<MUON::RawMuons>(fSrc);

  const auto& tCorrectedMuons = fMomentumCorrectionFactory->Apply(tMuons, fCorrectionRegister);

  for (unsigned int i = 0; i < tCorrectedMuons.size(); i++) {

    const std::string& tOutputName = fCorrectionRegister.at(i).fOutputName;
    const auto& tCorrectedMuon = tCorrectedMuons.at(i);
    
    bool fFind = false;
    for (unsigned int i = 0; i < tCorrectedMuon.size(); i++) {
      for (unsigned int j = i + 1; j < tCorrectedMuon.size(); j++) {
        
        const auto& tMuon_1 = tCorrectedMuon[i];
        const auto& tMuon_2 = tCorrectedMuon[j];
  
        if (
            !((tMuon_1.fVec.Pt() > fLeadingPt && tMuon_2.fVec.Pt() > fSubLeadingPt) ||
            (tMuon_2.fVec.Pt() > fLeadingPt && tMuon_1.fVec.Pt() > fSubLeadingPt))
        ) continue;
  
        if (tMuon_1.fCharge * tMuon_2.fCharge != fChargeConj) continue;
  
        int tIso = tMuon_1.fIso + tMuon_2.fIso;
        if (tIso < 0) tIso = 0;
  
        if (tIso != fIsolationConj) continue;
  
        fFind = true;
        if (fFind) {
          pool.Put(tOutputName, MUON::MuonPair{tMuon_1, tMuon_2, 2, (tMuon_1.fVec + tMuon_2.fVec).M()});
          break;
        }
      }
      if (fFind) break;
    }
      
    if (!fFind) { 
      pool.Put(
        tOutputName, 
        MUON::MuonPair{
          MUON::StdMuon{TLorentzVector(0, 0, 0, 0), TLorentzVector(0, 0, 0, 0), -999, -999}, 
          MUON::StdMuon{TLorentzVector(0, 0, 0, 0), TLorentzVector(0, 0, 0, 0), -999, -999}, 
          0, 
          -999
        }
      );
    }
  }
}

