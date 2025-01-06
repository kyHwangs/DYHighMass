#include <iostream>
#include <vector>

#include "jet.h"
#include "muon.h"
#include "electron.h"

#include "TChain.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TRandom.h"

void JET::init(TTreeReader* fTreeReader) {

  nJet = new TTreeReaderValue<unsigned int>(*fTreeReader, "nJet");
  Jet_pt = new TTreeReaderArray<float>(*fTreeReader, "Jet_pt");
  Jet_eta = new TTreeReaderArray<float>(*fTreeReader, "Jet_eta");
  Jet_phi = new TTreeReaderArray<float>(*fTreeReader, "Jet_phi");
  Jet_mass = new TTreeReaderArray<float>(*fTreeReader, "Jet_mass");
  Jet_jetId = new TTreeReaderArray<int>(*fTreeReader, "Jet_jetId");
  Jet_btagCSVV2 = new TTreeReaderArray<float>(*fTreeReader, "Jet_btagCSVV2");
}

// bool JET::PrepareJet(
//   int nJet,
//   TTreeReaderArray<float>* Jet_pt,
//   TTreeReaderArray<float>* Jet_eta,
//   TTreeReaderArray<float>* Jet_phi,
//   TTreeReaderArray<float>* Jet_mass,
//   TTreeReaderArray<int>* Jet_jetId,
//   TTreeReaderArray<float>* Jet_btagCSVV2
// ) {

//   return PrepareJet(nJet, Jet_pt, Jet_eta, Jet_phi, Jet_mass, Jet_jetId, Jet_btagCSVV2, std::vector<MUON::StdMuon>{}, std::vector<ELEC::StdElec>{});
// }

bool JET::PrepareJet(
  std::vector<MUON::StdMuon> tMuons, std::vector<ELEC::StdElec> tElecs) {

  fFVecJets.clear();
  fFVecBJets.clear();

  for (int i = 0; i < **nJet; i++) {

    if (!(Jet_pt->At(i) > fJetPt))
      continue;

    if (!(std::fabs(Jet_eta->At(i)) < fEta))
      continue;

    if (!(Jet_jetId->At(i) >= fJetID))
      continue;

    TLorentzVector jets;
    jets.SetPtEtaPhiM(Jet_pt->At(i), Jet_eta->At(i), Jet_phi->At(i), Jet_mass->At(i));

    bool isCleanJet = true;
    if (fCleaning) {

      for (int i = 0; i < tMuons.size(); i++) {
        if (tMuons.at(i).fVec.DeltaR(jets) < 0.4) {
          isCleanJet = false;
          break;
        }
      }

      if (isCleanJet) {
        for (int i = 0; i < tElecs.size(); i++) {
          if (tElecs.at(i).fVec.DeltaR(jets) < 0.4) {
            isCleanJet = false;
            break;
          }
        }
      }
    }

    if (!isCleanJet)
      continue;

    bool isBJet = false;
    if (Jet_btagCSVV2->At(i) > fBJetTaggerCut)
      isBJet = true;

    fFVecJets.push_back(StdJet(jets, isBJet, Jet_jetId->At(i)));
    if (isBJet)
      fFVecBJets.push_back(StdJet(jets, isBJet, Jet_jetId->At(i)));

  }

  return true;
}
