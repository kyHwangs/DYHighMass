#include <iostream>
#include <vector>

#include "muon.h"

#include "TChain.h"
#include "TTreeReaderArray.h"
#include "TRandom.h"

void MUON::PrepareGenMuon(
  int nGenPart,
  TTreeReaderArray<float>* GenPart_pt,
  TTreeReaderArray<float>* GenPart_eta,
  TTreeReaderArray<float>* GenPart_phi,
  TTreeReaderArray<float>* GenPart_mass,
  TTreeReaderArray<int>* GenPart_pdgId
) {

  fFVecGenMuons.clear();

  for (int i  = 0; i < nGenPart; i++) {

    if ( !(std::abs(GenPart_pdgId->At(i)) == 13) )
      continue;

    TLorentzVector mu;
    mu.SetPtEtaPhiM(GenPart_pt->At(i), GenPart_eta->At(i), GenPart_phi->At(i), GenPart_mass->At(i));

    StdMuon mu_std = StdMuon(mu, mu, (-1) * (GenPart_pdgId->At(i) / std::abs(GenPart_pdgId->At(i))), 0, 0);
    fFVecGenMuons.push_back(mu_std);
  }

  std::sort(fFVecGenMuons.begin(), fFVecGenMuons.end(), [](const StdMuon &lhs, const StdMuon &rhs) {
    return lhs.fVec.Pt() > rhs.fVec.Pt();
  });
}

bool MUON::PrepareMuon(
  int nMuon,
  TTreeReaderArray<float>* Muon_pt,
  TTreeReaderArray<float>* Muon_eta,
  TTreeReaderArray<float>* Muon_phi,
  TTreeReaderArray<int>* Muon_charge,
  TTreeReaderArray<float>* Muon_mass,
  TTreeReaderArray<bool>* Muon_tightId,
  TTreeReaderArray<float>* Muon_pfRelIso04_all,
  TTreeReaderArray<int>* Muon_nTrackerLayers
) {

  fFVecMuons.clear();

  for (int i = 0; i < nMuon; i++) {
    if ( !(Muon_tightId->At(i) && Muon_pfRelIso04_all->At(i) < 0.15) )
      continue;

    TLorentzVector mu;
    mu.SetPtEtaPhiM(Muon_pt->At(i), Muon_eta->At(i), Muon_phi->At(i), Muon_mass->At(i));

    TLorentzVector mu_corr = mu;

    if (fDoRoccoR) {
      if (!fIsMC) {
        mu_corr *= fRoccoR->kScaleDT(
          Muon_charge->At(i),
          mu_corr.Pt(),
          mu_corr.Eta(),
          mu_corr.Phi(),
          5,
          0
        );
      } else {

        double drmin = 999.;
        bool match = false;
        int genMuonIdx = 0;

        for (int j = 0; j < fFVecGenMuons.size(); j++) {
          if (mu_corr.DeltaR(fFVecGenMuons.at(j).fVec) < 0.1 &&
              mu_corr.DeltaR(fFVecGenMuons.at(j).fVec) < drmin) {
            match = true;
            genMuonIdx = j;
            drmin = mu_corr.DeltaR(fFVecGenMuons.at(j).fVec);
          }
        }

        if (match) {
          mu_corr *= fRoccoR->kSpreadMC(
            Muon_charge->At(i),
            mu_corr.Pt(),
            mu_corr.Eta(),
            mu_corr.Phi(),
            fFVecGenMuons.at(genMuonIdx).fVec.Pt(),
            5,
            0
          );
        } else {
          double rndm = gRandom->Rndm();
          mu_corr *= fRoccoR->kSmearMC(
            Muon_charge->At(i),
            mu_corr.Pt(),
            mu_corr.Eta(),
            mu_corr.Phi(),
            Muon_nTrackerLayers->At(i),
            rndm,
            5,
            0
          );
        }
      }
    }

    if ( !(mu_corr.Pt() > fSubLeadingMuonPt) || !(std::abs(mu_corr.Eta()) < 2.4) )
      continue;

    StdMuon mu_std = StdMuon(mu_corr, mu, Muon_charge->At(i), Muon_tightId->At(i), Muon_pfRelIso04_all->At(i));
    fFVecMuons.push_back(mu_std);

    std::sort(fFVecMuons.begin(), fFVecMuons.end(), [](const StdMuon &lhs, const StdMuon &rhs) {
      return lhs.fVec.Pt() > rhs.fVec.Pt();
    });
  }

  fLeadingIdx = 0;

  if (fFVecMuons.size() < 2)
    return false;

  if (fFVecMuons.at(0).fVec.Pt() < fLeadingMuonPt)
    return false;

  int tSubLeadingIdx = -1;
  int tChargeSelection = 1;
  if (!fOppositeCharge) tChargeSelection = -1;
  for (int i = 1; i < fFVecMuons.size(); i++) {
    if (tChargeSelection * (fFVecMuons.at(0).fCharge * fFVecMuons.at(i).fCharge) > 0)
      continue;

    tSubLeadingIdx = i;
    if (tSubLeadingIdx != -1)
      break;
  }

  if (tSubLeadingIdx == -1)
    return false;

  fSubLeadingIdx = tSubLeadingIdx;

  auto tLeadingMuon = fFVecMuons.at(0).fVec;
  auto tSubLeadingMuon = fFVecMuons.at(fSubLeadingIdx).fVec;
  double tDiMuonMass = (tLeadingMuon + tSubLeadingMuon).M();

  if (tDiMuonMass < fZMassCut)
    return false;

  return true;
}
