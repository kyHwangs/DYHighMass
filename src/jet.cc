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
  Jet_puId = new TTreeReaderArray<int>(*fTreeReader, "Jet_puId");
  Jet_btagDeepFlavB = new TTreeReaderArray<float>(*fTreeReader, "Jet_btagDeepFlavB");

  if (fIsMC) {

    Jet_genJetIdx = new TTreeReaderArray<int>(*fTreeReader, "Jet_genJetIdx");
    Jet_hadronFlavour = new TTreeReaderArray<int>(*fTreeReader, "Jet_hadronFlavour");
  }
}

bool JET::PrepareJet() {

  fFVecJets.clear();
  fFVecBJets.clear();

  for (int i = 0; i < **nJet; i++) {

    if (!(Jet_pt->At(i) > fJetPt))
      continue;

    if (!(std::fabs(Jet_eta->At(i)) < fEta))
      continue;

    if (!(Jet_jetId->At(i) >= fJetID))
      continue;

    if (Jet_pt->At(i) < 50. && !(Jet_puId->At(i) >= fJetPUID))
      continue;

    TLorentzVector jets;
    jets.SetPtEtaPhiM(Jet_pt->At(i), Jet_eta->At(i), Jet_phi->At(i), Jet_mass->At(i));

    int hadFlav = -1;
    if (fIsMC)
      hadFlav = Jet_hadronFlavour->At(i);

    int genJetIdx = -1;
    if (fIsMC)
      genJetIdx = Jet_genJetIdx->At(i);

    bool isBJet = false;
    if (Jet_btagDeepFlavB->At(i) > fBJetTaggerCut)
      isBJet = true;

    fFVecJets.push_back(StdJet(jets, jets, isBJet, Jet_jetId->At(i), hadFlav, genJetIdx));
    if (isBJet)
      fFVecBJets.push_back(StdJet(jets, jets, isBJet, Jet_jetId->At(i), hadFlav, genJetIdx));

  }

  return true;
}

double JET::GetPUIDSF() {

  double weight = 1.;

  for (int i = 0; i < fFVecJets.size(); i++) {

    if (!(fFVecJets.at(i).fVec.Pt() < 50. && fFVecJets.at(i).fVec.Pt() >= 30.))
      continue;

    if (fFVecJets.at(i).fGenJetIdx == -1)
      continue;

    weight *= fJetPUIDTable.getEfficiency(fFVecJets.at(i).fVec.Pt(), fFVecJets.at(i).fVec.Eta());
  }

  return weight;
}

double JET::GetBTagSF() {

  double pMC = 1.;
  double pData = 1.;

  for (int i = 0; i < fFVecJets.size(); i++) {

    double tJetEta = std::abs(fFVecJets.at(i).fVec.Eta());
    double tJetPt = fFVecJets.at(i).fVec.Pt();
    int tHadFlav = fFVecJets.at(i).fHadFlav;

    if (tJetPt >= 1000.)
      tJetPt = 800.;

    double tSFcentral = 1.;
    if (tHadFlav == 5 || tHadFlav == 4) tSFcentral = fBTagMuJets->evaluate({"central", fBTagWP, tHadFlav, std::abs(tJetEta), tJetPt});
    else tSFcentral = fBTagIncl->evaluate({"central", fBTagWP, 0, std::abs(tJetEta), tJetPt});

    double tJetEff = 1.;
    if (tHadFlav == 5)        tJetEff = fJetBTagEffB.getEfficiency(tJetPt, tJetEta);
    else if (tHadFlav == 4)   tJetEff = fJetBTagEffC.getEfficiency(tJetPt, tJetEta);
    else                      tJetEff = fJetBTagEffL.getEfficiency(tJetPt, tJetEta);

    if (fFVecJets.at(i).fPassingBJetTagger) {
      pMC *= tJetEff;
      pData *= tJetEff * tSFcentral;
    } else {
      pMC *= (1 - tJetEff);
      pData *= (1 - tJetEff * tSFcentral);
    }

    // std::cout << tJetEff << " " << pMC << " " << tSFcentral << " " << tSFcentral * tJetEff << " " << pData << std::endl;
  }

  // std::cout << pData / pMC << std::endl;
  return pData / pMC;
}
