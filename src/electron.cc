#include <iostream>
#include <vector>

#include "electron.h"

#include "TChain.h"
#include "TTreeReaderArray.h"
#include "TRandom.h"

bool ELEC::PrepareElec(
  int nElectron,
  TTreeReaderArray<float>* Electron_pt,
  TTreeReaderArray<float>* Electron_eta,
  TTreeReaderArray<float>* Electron_deltaEtaSC,
  TTreeReaderArray<float>* Electron_phi,
  TTreeReaderArray<float>* Electron_mass,
  TTreeReaderArray<int>* Electron_cutBased
) {

  fFVecElecs.clear();

  for (int i = 0; i < nElectron; i++) {

    if (!(Electron_pt->At(i) > fPt))
      continue;

    float eSCEta = Electron_eta->At(i) + Electron_deltaEtaSC->At(i);
    if (!(std::abs(eSCEta) < fEta))
      continue;

    if (fSCEtaVeto && (std::abs(eSCEta) > 1.4442 && std::abs(eSCEta) < 1.5660))
      continue;

    if (Electron_cutBased->At(i) != 4)
      continue;

    TLorentzVector elecs;
    elecs.SetPtEtaPhiM(Electron_pt->At(i), eSCEta, Electron_phi->At(i), Electron_mass->At(i));

    fFVecElecs.push_back(StdElec(elecs));
  }

  return true;
}
