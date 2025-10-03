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
  Electron_charge = new TTreeReaderArray<int>(*fTreeReader, "Electron_charge");
}

bool ELEC::PrepareElec() {

  fFVecElecs.clear();

  for (int i = 0; i < **nElectron; i++) {

    if (!(Electron_pt->At(i) > fSubLeadingPt))
      continue;

    float eSCEta = Electron_eta->At(i) + Electron_deltaEtaSC->At(i);
    if (!(std::abs(eSCEta) < fEta))
      continue;

    if (std::abs(eSCEta) > 1.4442 && std::abs(eSCEta) < 1.5660)
      continue;

    if (Electron_cutBased->At(i) < fID)
      continue;

    TLorentzVector elecs;
    elecs.SetPtEtaPhiM(Electron_pt->At(i), eSCEta, Electron_phi->At(i), Electron_mass->At(i));

    fFVecElecs.push_back(StdElec(elecs, Electron_charge->At(i)));
  }

  std::sort(fFVecElecs.begin(), fFVecElecs.end(), [](const StdElec &lhs, const StdElec &rhs) {
    return lhs.fVec.Pt() > rhs.fVec.Pt();
  });

  fLeadingIdx = 0;

  if (fFVecElecs.size() < 2)
    return false;

  if (fFVecElecs.at(0).fVec.Pt() < fLeadingPt)
    return false;

  int tSubLeadingIdx = -1;
  int tChargeSelection = 1;
  if (!fOppositeCharge) tChargeSelection = -1;

  for (int i = 1; i < fFVecElecs.size(); i++) {
    if (tChargeSelection * (fFVecElecs.at(0).fCharge * fFVecElecs.at(i).fCharge) > 0)
      continue;

    tSubLeadingIdx = i;
    if (tSubLeadingIdx != -1)
      break;
  }

  if (tSubLeadingIdx == -1)
    return false;

  fSubLeadingIdx = tSubLeadingIdx;

  auto tLeadingElec = fFVecElecs.at(0).fVec;
  auto tSubLeadingElec = fFVecElecs.at(fSubLeadingIdx).fVec;
  double tDiMuonMass = (tLeadingElec + tSubLeadingElec).M();

  if (tDiMuonMass < fZMassCut - 10)
    return false;

  return true;
}
