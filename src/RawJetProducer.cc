#include "RawJetProducer.h"
#include "EventContentPool.h"
#include "EventProducts.h"
#include "NT.h"

#include "correction.h"

#include <cmath>
#include <iostream>

RawJetProducer::RawJetProducer(const YAML::Node& fConfig) {

  fName = fConfig["Name"].as<std::string>();
  fOutputName = fConfig["OutputName"].as<std::string>();

  fEta = fConfig["Eta"].as<float>();
  fId = fConfig["Id"].as<int>();
  fBTagWP = fConfig["BTagWP"].as<std::string>();
  fBTagPath = fConfig["Path"].as<std::string>();

  std::cout << "######################################################################" << std::endl;
  std::cout << " " << Name() << ": " << InName() << " -> " << OutputName() << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " fEta: " << fEta << std::endl;
  std::cout << " fId: " << fId << std::endl;
  std::cout << " fBTagWP: " << fBTagWP << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl; 
}

void RawJetProducer::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;
  fBDiscWP = correction::CorrectionSet::from_file(fBTagPath)->at("deepJet_wp_values")->evaluate({fBTagWP});
  fRandom.SetSeed(pool.GetEventInfo().fSeedForJet);
}

void RawJetProducer::Produce(EventContentPool& pool) {

  const auto& nt = pool.Source(); 

  JET::RawJets tJetCollection = {};

  for (unsigned int iJet = 0; iJet < **nt.nJet; iJet++) {

    if (std::abs(nt.Jet_eta->At(iJet)) > fEta) continue;
    if ((nt.Jet_jetId->At(iJet) & (1 << 2)) == 0) continue;

    TLorentzVector tJetVec;
    tJetVec.SetPtEtaPhiM(
      (1. - nt.Jet_rawFactor->At(iJet)) * nt.Jet_pt->At(iJet), 
      nt.Jet_eta->At(iJet), 
      nt.Jet_phi->At(iJet), 
      (1. - nt.Jet_rawFactor->At(iJet)) * nt.Jet_mass->At(iJet)
    );

    bool isBJet = nt.Jet_btagDeepB->At(iJet) > fBDiscWP;

    int genJetIdx = -1;
    if (fIsMC) genJetIdx = nt.Jet_genJetIdx->At(iJet);
    int hadronFlavour = -1;
    if (fIsMC) hadronFlavour = nt.Jet_hadronFlavour->At(iJet);

    tJetCollection.push_back(
      JET::RawJet{
        tJetVec,
        nt.Jet_area->At(iJet), 
        nt.Jet_puId->At(iJet), 
        isBJet, 
        genJetIdx,
        hadronFlavour,
        **nt.fixedGridRhoFastjetAll,
        static_cast<double>(**nt.run),
        fRandom.Gaus(0, 1)
      }
    );
  }

  pool.Put(fOutputName, tJetCollection);
}

