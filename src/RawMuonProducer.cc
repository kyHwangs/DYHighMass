#include "RawMuonProducer.h"
#include "EventContentPool.h"
#include "EventProducts.h"
#include "NT.h"

#include <cmath>
#include <iostream>

RawMuonProducer::RawMuonProducer(const YAML::Node& fConfig) {

  fName = fConfig["Name"].as<std::string>();
  fOutputName = fConfig["OutputName"].as<std::string>();

  fEta = fConfig["Eta"].as<float>();
  
  std::string tId = fConfig["Id"].as<std::string>();
  if (tId == "global") fId = (UChar_t)(2);
  else if (tId == "tracker") fId = (UChar_t)(1);
  else fId = (UChar_t)(0);
  
  fIso = fConfig["Iso"].as<float>();

  std::cout << "######################################################################" << std::endl;
  std::cout << " " << Name() << ": " << InName() << " -> " << OutputName() << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " fEta: " << fEta << std::endl;
  std::cout << " fID: " << tId << std::endl;
  std::cout << " fIso: " << fIso << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl; 
}

void RawMuonProducer::Init(EventContentPool& pool) {

  fRandom.SetSeed(pool.GetEventInfo().fSeedForMuon);
}

void RawMuonProducer::Produce(EventContentPool& pool) {

  const NT& nt = pool.Source();

  MUON::RawMuons tMuonCollection = {};

  for (unsigned int i = 0; i < **nt.nMuon; i++) {

    if (!(nt.Muon_highPtId->At(i) == fId)) continue;
    if (std::abs(nt.Muon_eta->At(i)) > fEta) continue;
    if (!nt.Muon_highPurity->At(i)) continue;

    TLorentzVector tMu;
    tMu.SetPtEtaPhiM(nt.Muon_pt->At(i) * nt.Muon_tunepRelPt->At(i), nt.Muon_eta->At(i), nt.Muon_phi->At(i), nt.Muon_mass->At(i));

    int tIso = (nt.Muon_tkRelIso->At(i) < fIso) ? 1 : -1;
    int tCharge = (nt.Muon_charge->At(i) > 0) ? 1 : -1;

    tMuonCollection.push_back(MUON::RawMuon{tMu, tCharge, tIso, fRandom.Gaus(0, 1)});
  }

  pool.Put<MUON::RawMuons>(fOutputName, tMuonCollection);
}

