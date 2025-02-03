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
  Jet_btagCSVV2 = new TTreeReaderArray<float>(*fTreeReader, "Jet_btagCSVV2");

  if (fIsMC) {

    Jet_hadronFlavour = new TTreeReaderArray<int>(*fTreeReader, "Jet_hadronFlavour");
  }
}

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

    if (Jet_pt->At(i) < 50. && !(Jet_puId->At(i) >= fJetPUID))
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

    int hadFlav = -1;
    if (fIsMC)
      hadFlav = Jet_hadronFlavour->At(i);

    bool isBJet = false;
    if (Jet_btagCSVV2->At(i) > fBJetTaggerCut)
      isBJet = true;

    fFVecJets.push_back(StdJet(jets, isBJet, Jet_jetId->At(i), hadFlav));
    if (isBJet)
      fFVecBJets.push_back(StdJet(jets, isBJet, Jet_jetId->At(i), hadFlav));

  }

  return true;
}

double JET::GetPUIDSF() {

  double weight = 1.;

  for (int i = 0; i < fFVecJets.size(); i++) {

    if (!(fFVecJets.at(i).fVec.Pt() < 50. && fFVecJets.at(i).fVec.Pt() >= 30.))
      continue;

    weight *= fJetPUIDTable.getEfficiency(fFVecJets.at(i).fVec.Pt(), fFVecJets.at(i).fVec.Eta());
  }

  return weight;
}

double JET::GetBTagSF() {

  double pMC = 1.;
  double pData = 1.;

  for (int i = 0; i < fFVecJets.size(); i++) {

    double tJetEta = fFVecJets.at(i).fVec.Eta();
    double tJetPt = fFVecJets.at(i).fVec.Pt();
    int tHadFlav = fFVecJets.at(i).fHadFlav;

    // std::cout << fFVecJets.size() << " " << i << " " << tJetPt << " " << tJetEta << " " << tHadFlav << " " << fFVecJets.at(i).fPassingBJetTagger << " ";

    BTagEntry::JetFlavor jFLAV;
    if (tHadFlav == 5)        jFLAV = BTagEntry::FLAV_B;
    else if (tHadFlav == 4)   jFLAV = BTagEntry::FLAV_C;
    else                      jFLAV = BTagEntry::FLAV_UDSG;

    double tSFcentral   = fBTagCalibReader->eval_auto_bounds("central", jFLAV, tJetEta, tJetPt);

    double tJetEff = -1;
    if (tHadFlav == 5)        tJetEff = fJetBTagEffB.getEfficiency(tJetEta, tJetPt);
    else if (tHadFlav == 4)   tJetEff = fJetBTagEffC.getEfficiency(tJetEta, tJetPt);
    else                      tJetEff = fJetBTagEffL.getEfficiency(tJetEta, tJetPt);

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
