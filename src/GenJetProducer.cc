#include "GenJetProducer.h"
#include "EventContentPool.h"
#include "EventProducts.h"
#include "NT.h"

#include <iostream>
#include <vector>

GenJetProducer::GenJetProducer(const YAML::Node& fConfig) {

  fName = fConfig["Name"].as<std::string>();
  fOutputName = fConfig["OutputName"].as<std::string>();

  fPt = fConfig["Pt"].as<float>();
  fEta = fConfig["Eta"].as<float>();
  fVeto = fConfig["Veto"].as<std::string>();

  std::cout << "######################################################################" << std::endl;
  std::cout << " " << Name() << ": " << InName() << " -> " << OutputName() << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " fPt: " << fPt << std::endl;
  std::cout << " fEta: " << fEta << std::endl;
  std::cout << " fVeto: " << fVeto << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl; 
}

void GenJetProducer::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;
}

void GenJetProducer::Produce(EventContentPool& pool) {

  if (!fIsMC) return;

  const auto& nt = pool.Source();

  GEN::Jets tGenJetCollection = {};

  GEN::DressedMuon tVetoMuon;
  std::vector<TLorentzVector> tVetoLeptons;
  if (fVeto != "None") {
    tVetoMuon = pool.Get<GEN::DressedMuon>(fVeto);
    tVetoLeptons.push_back(tVetoMuon.first);
    tVetoLeptons.push_back(tVetoMuon.second);
  }

  for (unsigned int iGenJet = 0; iGenJet < **nt.nGenJet; iGenJet++) {

    TLorentzVector fVec;
    fVec.SetPtEtaPhiM(
      nt.GenJet_pt->At(iGenJet), 
      nt.GenJet_eta->At(iGenJet), 
      nt.GenJet_phi->At(iGenJet), 
      nt.GenJet_mass->At(iGenJet)
    );

    if (fVec.Pt() < fPt) continue;
    if (fabs(fVec.Eta()) > fEta) continue;

    if (fVeto != "None")
      for (const auto& tVeto : tVetoLeptons)
        if (tVeto.DeltaR(fVec) < 0.3) continue;

    tGenJetCollection.push_back(fVec);
  }

  pool.Put(fOutputName, tGenJetCollection);
}

