#include <iostream>
#include <vector>

#include "jet.h"

#include "TChain.h"
#include "TTreeReaderArray.h"
#include "TRandom.h"


bool JET::PrepareJet(
  int nJet,
  TTreeReaderArray<float>* Jet_pt,
  TTreeReaderArray<float>* Jet_eta,
  TTreeReaderArray<float>* Jet_phi,
  TTreeReaderArray<float>* Jet_mass,
  TTreeReaderArray<bool>* Jet_jetId,
  TTreeReaderArray<float>* Jet_btagCSVV2
) {

  return JET::PrepareJet(nJet, Jet_pt, Jet_eta, Jet_phi, Jet_mass, Jet_jetId, Jet_btagCSVV2, std::vector<StdMuon>{}, std::vector<StdElec>{});
}

bool JET::PrepareJet(
  int nJet,
  TTreeReaderArray<float>* Jet_pt,
  TTreeReaderArray<float>* Jet_eta,
  TTreeReaderArray<float>* Jet_phi,
  TTreeReaderArray<float>* Jet_mass,
  TTreeReaderArray<int>* Jet_jetId,
  TTreeReaderArray<float>* Jet_btagCSVV2,
  std::vector<StdMuon> tMuons,
  std::vector<StdElec> tElecs
) {

  fFVecJets.clear();
  fFVecBJets.clear();

  for (int i = 0; i < nJet; i++) {

    std::cout << i << " " << Jet_jetId << std::endl;

    if (!(Jet_pt->At(i) > fJetPt))
      continue;

    if (!(std::fabs(Jet_eta->At(i)) < fEta))
      continue;

    TLorentzVector jets;
    jets.SetPtEtaPhiM(Jet_pt->At(i), Jet_eta->At(i), Jet_phi->At(i), Jet_mass->At(i));

    bool isCleanJet = true;
    if (fCleaning) {
      for (int i = 0; i < tMuons.size(); i++) {
        if (tMuons.at(i).fVec.deltaR(jets))
      }
    }


    // struct StdJet {
    //   TLorentzVector fVec;
    //   bool fPassingBJetTagger;

    //   StdJet(TLorentzVector fVec_, bool fPassingBJetTagger_)
    //   : fVec(fVec_), fPassingBJetTagger(fPassingBJetTagger_)
    //   { };
    // };
  }

}
