#include <iostream>
#include <vector>

#include "electron.h"

#include "TChain.h"
#include "TTreeReaderArray.h"
#include "TRandom.h"

void ELEC::init(TTreeReader* fTreeReader) {

  nElectron = new TTreeReaderValue<unsigned int>(*fTreeReader, "nElectron");
  Electron_pt = new TTreeReaderArray<float>(*fTreeReader, "Electron_pt");
  Electron_eta = new TTreeReaderArray<float>(*fTreeReader, "Electron_eta");
  Electron_deltaEtaSC = new TTreeReaderArray<float>(*fTreeReader, "Electron_deltaEtaSC");
  Electron_phi = new TTreeReaderArray<float>(*fTreeReader, "Electron_phi");
  Electron_mass = new TTreeReaderArray<float>(*fTreeReader, "Electron_mass");
  Electron_cutBased = new TTreeReaderArray<int>(*fTreeReader, "Electron_cutBased");
}

bool ELEC::PrepareElec() {

  fFVecElecs.clear();

  for (int i = 0; i < **nElectron; i++) {

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
