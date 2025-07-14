#include <iostream>
#include <vector>
#include <chrono>

#include "DYLoop.h"
#include "muon.h"
#include "jet.h"
#include "electron.h"
#include "muon.h"
#include "NT.h"

#include "TH1.h"

void DYLoop::SetNT() {

}

void DYLoop::Loop() {

  std::chrono::time_point tTimeBegin = std::chrono::system_clock::now();

  double tMaxLoop = 0;
  double tTotalGenWeight = 0;
  while(fNtuples->GetNext()) { // Event loop starts here
    tMaxLoop++;

    if (tMaxLoop == 10001) break;

    if ( static_cast<int>(tMaxLoop) % 10000 == 0 )
      std::cout << "Loop: " << tMaxLoop << " / " << fMaxEntries << " | " << (100. * tMaxLoop / fMaxEntries) << " %" << std::endl;

    if (fIsMC) {
      fHistoSet->FillHisto((std::string)"h_PileUp_Count_Interaction_before", **(fNtuples->Pileup_nTrueInt), 1.);
      fHistoSet->FillHisto((std::string)"h_PileUp_Count_Interaction_after", **(fNtuples->Pileup_nTrueInt), fPuReweighting->weight(**(fNtuples->Pileup_nTrueInt)));
    }

    double tEventGenWeight = 1.;
    if (fIsMC) {
      tEventGenWeight = **(fNtuples->genWeight);

      if (fSampleName.Contains("NNLO")) {
        if (tEventGenWeight > 0) tEventGenWeight = 1;
        else                     tEventGenWeight = -1;
      }

      fHistoSet->FillHisto((std::string)"h_GenWeight", tEventGenWeight, 1.);
    }

    if (fIsMC && fSampleName.Contains("NNLO")) {
      auto tLHEMuons = fNtuples->GetLHE(13);

      fHistoSet->FillHisto((std::string)"h_LHEnMuon", static_cast<int>(tLHEMuons.size()));


      double tDiMuonMassLHE = 0;

      if (tLHEMuons.size() == 2) {
        auto tDiMuonLHE = tLHEMuons.at(0) + tLHEMuons.at(1);
        tDiMuonMassLHE = tDiMuonLHE.M();
      }

      if (fSampleName == "NNLO_inc" && tDiMuonMassLHE > 100 )
        continue;

      fHistoSet->FillHisto((std::string)"h_LHEDimuonMass", tDiMuonMassLHE, tEventGenWeight);
    }

    float tPUReweightingFactor = 1;
    if (fIsMC && fDoPU) {

      tPUReweightingFactor = fPuReweighting->weight(**(fNtuples->Pileup_nTrueInt));
      tEventGenWeight *= tPUReweightingFactor;
    }

    if (fIsMC && fDoL1Pre) {
      tEventGenWeight *= **(fNtuples->L1PreFiringWeight_Nom);
    }

    fHistoSet->FillHisto((std::string)"h_EventInfo", 1, 1);
    fHistoSet->FillHisto((std::string)"h_EventInfo", 4, tEventGenWeight);

    if ( !(fNtuples->PassinNoiseFilter()) )
      continue;

    if ( !(fNtuples->PassingTrigger()) )
      continue;

    if ( !(fMuons->PrepareMuon()) )
      continue;

    // if ( !(fElecs->PrepareElec()) )
    //   continue;

    if ( !(fJets->PrepareJet()) )
      continue;

    auto vJets = fJets->GetJets();
    auto vBJets = fJets->GetBJets();

    int nJets = vJets.size();
    int nBJets = vBJets.size();

    auto tLeadingMuon = fMuons->GetLeadingMuon();
    auto tFVecLeadingMuon = tLeadingMuon.fVec;
    auto tFVecRawLeadingMuon = tLeadingMuon.fVecRaw;

    auto tSubLeadingMuon = fMuons->GetSubLeadingMuon();
    auto tFVecSubLeadingMuon = tSubLeadingMuon.fVec;
    auto tFVecRawSubLeadingMuon = tSubLeadingMuon.fVecRaw;

    auto tDiMuon = tFVecLeadingMuon + tFVecSubLeadingMuon;

    if (fIsMC && fDoID) {

      double tIDEffSFLeading = 0;

      if (tFVecRawLeadingMuon.Pt() < 15.) tIDEffSFLeading = 0;
      else                                tIDEffSFLeading = fID_SF->evaluate({std::abs(tFVecRawLeadingMuon.Eta()), tFVecRawLeadingMuon.Pt(), "nominal"});

      tEventGenWeight *= tIDEffSFLeading;


      double tIDEffSFSubleading = 0;

      if (tFVecRawSubLeadingMuon.Pt() < 15.) tIDEffSFSubleading = 0;
      else                                   tIDEffSFSubleading = fID_SF->evaluate({std::abs(tFVecRawSubLeadingMuon.Eta()), tFVecRawSubLeadingMuon.Pt(), "nominal"});

      tEventGenWeight *= tIDEffSFSubleading;

      // std::cout << "######################################################################" << std::endl;
      // std::cout << "                        ID efficiency debugging                       " << std::endl;
      // std::cout << "----------------------------------------------------------------------" << std::endl;
      // std::cout << " LEADING: " << tFVecRawLeadingMuon.Pt() << " " << tFVecRawLeadingMuon.Eta() << " " << tIDEffSFLeading << std::endl;
      // std::cout << " SUB-LLEADING: " << tFVecRawSubLeadingMuon.Pt() << " " << tFVecRawSubLeadingMuon.Eta() << " " << tIDEffSFSubleading << std::endl;
      // std::cout << "######################################################################" << std::endl;
      // std::cout << " " << std::endl;

    }

    if (fIsMC && fDoISO) {

      double tISOEffSFLeading = 0;

      if (tFVecRawLeadingMuon.Pt() < 15.) tISOEffSFLeading = 0;
      else                                tISOEffSFLeading = fISO_SF->evaluate({std::abs(tFVecRawLeadingMuon.Eta()), tFVecRawLeadingMuon.Pt(), "nominal"});

      tEventGenWeight *= tISOEffSFLeading;


      double tISOEffSFSubleading = 0;

      if (tFVecRawSubLeadingMuon.Pt() < 15.) tISOEffSFSubleading = 0;
      else                                   tISOEffSFSubleading = fISO_SF->evaluate({std::abs(tFVecRawSubLeadingMuon.Eta()), tFVecRawSubLeadingMuon.Pt(), "nominal"});

      tEventGenWeight *= tISOEffSFSubleading;

      // std::cout << "######################################################################" << std::endl;
      // std::cout << "                        ISO efficiency debugging                      " << std::endl;
      // std::cout << "----------------------------------------------------------------------" << std::endl;
      // std::cout << " LEADING: " << tFVecRawLeadingMuon.Pt() << " " << tFVecRawLeadingMuon.Eta() << " " << tISOEffSFLeading << std::endl;
      // std::cout << " SUB-LEADING: " << tFVecRawSubLeadingMuon.Pt() << " " << tFVecRawSubLeadingMuon.Eta() << " " << tISOEffSFSubleading << std::endl;
      // std::cout << "######################################################################" << std::endl;
      // std::cout << " " << std::endl;


    }

    if (fIsMC && fDoTRIGG) {

      double mu_1_data = 0;
      double mu_2_data = 0;

      double mu_1_mc = 0;
      double mu_2_mc = 0;

      if (tFVecRawLeadingMuon.Pt() < 52.) {
        mu_1_data = 0.;
        mu_1_mc = 0.;
      } else {
        mu_1_data = fTRIG_SF->evaluate({std::abs(tFVecRawLeadingMuon.Eta()), tFVecRawLeadingMuon.Pt(), "dataEff"});
        mu_1_mc = fTRIG_SF->evaluate({std::abs(tFVecRawLeadingMuon.Eta()), tFVecRawLeadingMuon.Pt(), "mcEff"});
      }

      if (tFVecRawSubLeadingMuon.Pt() < 52.) {
        mu_2_data = 0.;
        mu_2_mc = 0.;
      } else {
        mu_2_data = fTRIG_SF->evaluate({std::abs(tFVecRawSubLeadingMuon.Eta()), tFVecRawSubLeadingMuon.Pt(), "dataEff"});
        mu_2_mc = fTRIG_SF->evaluate({std::abs(tFVecRawSubLeadingMuon.Eta()), tFVecRawSubLeadingMuon.Pt(), "mcEff"});
      }

      double data_tot = 1. - (1. - mu_1_data) * (1. - mu_2_data);
      double mc_tot = 1. - (1. - mu_1_mc) * (1. - mu_2_mc);

      double eventTriggerEffSF = 0.;
      if ( mc_tot != 0 )
        eventTriggerEffSF = data_tot / mc_tot;

      tEventGenWeight *= eventTriggerEffSF;

      // std::cout << "######################################################################" << std::endl;
      // std::cout << "                       TRIGG efficiency debugging                     " << std::endl;
      // std::cout << "----------------------------------------------------------------------" << std::endl;
      // std::cout << " LEADING: " << tFVecRawLeadingMuon.Pt() << " " << tFVecRawLeadingMuon.Eta() << " " << mu_1_data << " " << mu_1_mc << std::endl;
      // std::cout << " SUB-LEADING: " << tFVecRawSubLeadingMuon.Pt() << " " << tFVecRawSubLeadingMuon.Eta() << " " << mu_2_data << " " << mu_2_mc << std::endl;
      // std::cout << eventTriggerEffSF << std::endl;
      // std::cout << "######################################################################" << std::endl;
      // std::cout << " " << std::endl;
    }

    if (fIsMC && fDoJetPUID) {
      tEventGenWeight *= fJets->GetPUIDSF();
    }

    if (fIsMC && fDoBTag) {
      double bTagWeight = fJets->GetBTagSF();
      tEventGenWeight *= bTagWeight;
    }

    tTotalGenWeight += tEventGenWeight;

    fHistoSet->FillHisto((std::string)"h_nPVGood_Count", **(fNtuples->PV_npvsGood), tEventGenWeight);


    // auto vJets = fJets->GetJets();
    // auto vBJets = fJets->GetBJets();

    // int nJets = vJets.size();
    // int nBJets = vBJets.size();

    // auto tLeadingMuon = fMuons->GetLeadingMuon();
    // auto tFVecLeadingMuon = tLeadingMuon.fVec;
    // auto tFVecRawLeadingMuon = tLeadingMuon.fVecRaw;

    // auto tSubLeadingMuon = fMuons->GetSubLeadingMuon();
    // auto tFVecSubLeadingMuon = tSubLeadingMuon.fVec;
    // auto tFVecRawSubLeadingMuon = tSubLeadingMuon.fVecRaw;

    // auto tDiMuon = tFVecLeadingMuon + tFVecSubLeadingMuon;


    fHistoSet->FillMuon(tFVecLeadingMuon, tFVecSubLeadingMuon, nJets, nBJets, tEventGenWeight);
    fHistoSet->FillJet(&vJets, &vBJets, tDiMuon.M(), tEventGenWeight);

  } // End of event loop

  fHistoSet->FillHisto((std::string)"h_EventInfo", 5, tTotalGenWeight);
  fHistoSet->FillHisto((std::string)"h_EventInfo", 2, tMaxLoop);
  fHistoSet->FillHisto((std::string)"h_EventInfo", 3, fMaxEntries);

  std::chrono::duration tTimeTaken = std::chrono::system_clock::now() - tTimeBegin;
  std::chrono::minutes tTimeMin = std::chrono::duration_cast<std::chrono::minutes>(tTimeTaken);
  std::chrono::seconds tTimeSec = std::chrono::duration_cast<std::chrono::seconds>(tTimeTaken - tTimeMin);

  std::cout << " " << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << "                             Loop summary                             " << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " Entries: " << fMaxEntries << std::endl;
  std::cout << " nLoop: " << tMaxLoop << std::endl;
  std::cout << " GenWeight: " << tTotalGenWeight << std::endl;
  std::cout << " Time taken: " << tTimeMin.count() << "min " << int(tTimeSec.count()) << "sec" << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;

  EndOfJob();
}

void DYLoop::EndOfJob() {

  fHistoSet->WriteHisto(fOutputDir + fEra + "/" + fSampleName + "/output_" + std::to_string(fJobID) + ".root");
}
