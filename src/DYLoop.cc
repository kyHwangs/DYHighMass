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

    if ( (int)tMaxLoop % 10000 == 0 )
      std::cout << "Loop: " << tMaxLoop << " / " << fMaxEntries << " | " << (100. * tMaxLoop / fMaxEntries) << " %" << std::endl;

    if (fIsMC) {
      h_PileUp_Count_Interaction_before->Fill(**(fNtuples->Pileup_nTrueInt), 1.);
      h_PileUp_Count_Interaction_after->Fill(**(fNtuples->Pileup_nTrueInt), fPuReweighting->weight(**(fNtuples->Pileup_nTrueInt)));
    }

    double tEventGenWeight = 1.;
    if (fIsMC) {
      tEventGenWeight = **(fNtuples->genWeight);

      if (fSampleName.Contains("NNLO")) {
        if (tEventGenWeight > 0) tEventGenWeight = 1;
        else                     tEventGenWeight = -1;
      }

      h_GenWeight->Fill(tEventGenWeight, 1.);
    }

    if (fIsMC && fSampleName.Contains("NNLO")) {
      auto tLHEMuons = fNtuples->GetLHE(13);

      h_LHEnMuon->Fill(tLHEMuons.size());


      double tDiMuonMassLHE = 0;

      if (tLHEMuons.size() == 2) {
        auto tDiMuonLHE = tLHEMuons.at(0) + tLHEMuons.at(1);
        tDiMuonMassLHE = tDiMuonLHE.M();
      }

      if (fSampleName == "NNLO_inc" && tDiMuonMassLHE > 100 )
        continue;

      h_LHEDimuonMass->Fill(tDiMuonMassLHE, tEventGenWeight);
    }

    float tPUReweightingFactor = 1;
    if (fIsMC && fDoPU) {

      tPUReweightingFactor = fPuReweighting->weight(**(fNtuples->Pileup_nTrueInt));
      tEventGenWeight *= tPUReweightingFactor;
    }

    if (fIsMC && fDoL1Pre) {
      tEventGenWeight *= **(fNtuples->L1PreFiringWeight_Nom);
    }

    h_EventInfo->Fill(1, 1);
    h_EventInfo->Fill(4, tEventGenWeight);

    if ( !(fNtuples->PassinNoiseFilter()) )
      continue;

    if ( !(fNtuples->PassingTrigger()) )
      continue;

    if ( !(fMuons->PrepareMuon()) )
      continue;

    // if ( !(fElecs->PrepareElec()) )
    //   continue;

    // auto tMuon = fMuons->GetMuons();
    // auto tElec = fElecs->GetElecs();

    if ( !(fJets->PrepareJet()) )
      continue;

    auto vJets = fJets->GetJets();
    auto vBJets = fJets->GetBJets();

    int nJets = vJets.size();
    int nBJets = vBJets.size();

    auto tLeadingMuon = fMuons->GetLeadingMuon();
    auto tFVecLedingMuon = tLeadingMuon.fVec;
    auto tFVecRawLeadingMuon = tLeadingMuon.fVecRaw;

    auto tSubLeadingMuon = fMuons->GetSubLeadingMuon();
    auto tFVecSubLedingMuon = tSubLeadingMuon.fVec;
    auto tFVecRawSubLeadingMuon = tSubLeadingMuon.fVecRaw;

    auto tDiMuon = tFVecLedingMuon + tFVecSubLedingMuon;

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

    for (int i = 0; i < nJets; i++) {
      h_jetID->Fill(vJets.at(i).fID, tEventGenWeight);
    }

    if (nBJets == 0)
      h_nJet_bVeto->Fill(nJets, tEventGenWeight);

    tTotalGenWeight += tEventGenWeight;

    h_nPV_Count->Fill(**(fNtuples->PV_npvs), tEventGenWeight);
    h_nPVGood_Count->Fill(**(fNtuples->PV_npvsGood), tEventGenWeight);

    h_nJet->Fill(nJets, tEventGenWeight);
    h_nBJet->Fill(nBJets, tEventGenWeight);

    FillHisto(h_LeadingMuonPt, tFVecLedingMuon.Pt(), tEventGenWeight);
    FillHisto(h_LeadingMuonEta, tFVecLedingMuon.Eta(), tEventGenWeight);
    FillHisto(h_LeadingMuonPhi, tFVecLedingMuon.Phi(), tEventGenWeight);

    FillHisto(h_SubleadingMuonPt, tFVecSubLedingMuon.Pt(), tEventGenWeight);
    FillHisto(h_SubleadingMuonEta, tFVecSubLedingMuon.Eta(), tEventGenWeight);
    FillHisto(h_SubleadingMuonPhi, tFVecSubLedingMuon.Phi(), tEventGenWeight);

    FillHisto(h_MuonPt, tFVecLedingMuon.Pt(), tEventGenWeight);
    FillHisto(h_MuonEta, tFVecLedingMuon.Eta(), tEventGenWeight);
    FillHisto(h_MuonPhi, tFVecLedingMuon.Phi(), tEventGenWeight);

    FillHisto(h_MuonPt, tFVecSubLedingMuon.Pt(), tEventGenWeight);
    FillHisto(h_MuonEta, tFVecSubLedingMuon.Eta(), tEventGenWeight);
    FillHisto(h_MuonPhi, tFVecSubLedingMuon.Phi(), tEventGenWeight);

    FillHisto(h_dimuonMass, tDiMuon.M(), tEventGenWeight);
    FillHisto(h_dimuonMass_wide, tDiMuon.M(), tEventGenWeight);
    FillHisto(h_dimuonPt, tDiMuon.Pt(), tEventGenWeight);
    FillHisto(h_dimuonRap, tDiMuon.Rapidity(), tEventGenWeight);

    if (nJets == 0) {

      for (int i = 0; i < nJets; i++) {
        FillHisto(h_JetPt_0J, vJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_JetEta_0J, vJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_JetPhi_0J, vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        FillHisto(h_BJetPt_0J, vBJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_BJetEta_0J, vBJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_BJetPhi_0J, vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      FillHisto(h_LeadingMuonPt_0J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_LeadingMuonEta_0J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_LeadingMuonPhi_0J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_SubleadingMuonPt_0J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_SubleadingMuonEta_0J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_SubleadingMuonPhi_0J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_0J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_0J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_0J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_0J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_0J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_0J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_dimuonMass_0J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonMass_wide_0J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonPt_0J, tDiMuon.Pt(), tEventGenWeight);
      FillHisto(h_dimuonRap_0J, tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nJets == 1) {

      for (int i = 0; i < nJets; i++) {
        FillHisto(h_JetPt_1J, vJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_JetEta_1J, vJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_JetPhi_1J, vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        FillHisto(h_BJetPt_1J, vBJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_BJetEta_1J, vBJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_BJetPhi_1J, vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      FillHisto(h_LeadingMuonPt_1J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_LeadingMuonEta_1J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_LeadingMuonPhi_1J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_SubleadingMuonPt_1J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_SubleadingMuonEta_1J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_SubleadingMuonPhi_1J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_1J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_1J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_1J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_1J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_1J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_1J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_dimuonMass_1J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonMass_wide_1J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonPt_1J, tDiMuon.Pt(), tEventGenWeight);
      FillHisto(h_dimuonRap_1J, tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nJets > 1) {

      for (int i = 0; i < nJets; i++) {
        FillHisto(h_JetPt_mt1J, vJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_JetEta_mt1J, vJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_JetPhi_mt1J, vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        FillHisto(h_BJetPt_mt1J, vBJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_BJetEta_mt1J, vBJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_BJetPhi_mt1J, vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      FillHisto(h_LeadingMuonPt_mt1J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_LeadingMuonEta_mt1J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_LeadingMuonPhi_mt1J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_SubleadingMuonPt_mt1J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_SubleadingMuonEta_mt1J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_SubleadingMuonPhi_mt1J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_mt1J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_mt1J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_mt1J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_mt1J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_mt1J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_mt1J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_dimuonMass_mt1J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonMass_wide_mt1J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonPt_mt1J, tDiMuon.Pt(), tEventGenWeight);
      FillHisto(h_dimuonRap_mt1J, tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 0) {

      for (int i = 0; i < nJets; i++) {
        FillHisto(h_JetPt_0BJ, vJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_JetEta_0BJ, vJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_JetPhi_0BJ, vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        FillHisto(h_BJetPt_0BJ, vBJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_BJetEta_0BJ, vBJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_BJetPhi_0BJ, vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      FillHisto(h_LeadingMuonPt_0BJ, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_LeadingMuonEta_0BJ, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_LeadingMuonPhi_0BJ, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_SubleadingMuonPt_0BJ, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_SubleadingMuonEta_0BJ, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_SubleadingMuonPhi_0BJ, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_0BJ, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_0BJ, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_0BJ, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_0BJ, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_0BJ, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_0BJ, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_dimuonMass_0BJ, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonMass_wide_0BJ, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonPt_0BJ, tDiMuon.Pt(), tEventGenWeight);
      FillHisto(h_dimuonRap_0BJ, tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 1) {

      for (int i = 0; i < nJets; i++) {
        FillHisto(h_JetPt_1BJ, vJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_JetEta_1BJ, vJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_JetPhi_1BJ, vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        FillHisto(h_BJetPt_1BJ, vBJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_BJetEta_1BJ, vBJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_BJetPhi_1BJ, vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      FillHisto(h_LeadingMuonPt_1BJ, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_LeadingMuonEta_1BJ, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_LeadingMuonPhi_1BJ, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_SubleadingMuonPt_1BJ, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_SubleadingMuonEta_1BJ, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_SubleadingMuonPhi_1BJ, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_1BJ, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_1BJ, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_1BJ, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_1BJ, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_1BJ, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_1BJ, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_dimuonMass_1BJ, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonMass_wide_1BJ, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonPt_1BJ, tDiMuon.Pt(), tEventGenWeight);
      FillHisto(h_dimuonRap_1BJ, tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets > 1) {

      for (int i = 0; i < nJets; i++) {
        FillHisto(h_JetPt_mt1BJ, vJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_JetEta_mt1BJ, vJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_JetPhi_mt1BJ, vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        FillHisto(h_BJetPt_mt1BJ, vBJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_BJetEta_mt1BJ, vBJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_BJetPhi_mt1BJ, vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      FillHisto(h_LeadingMuonPt_mt1BJ, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_LeadingMuonEta_mt1BJ, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_LeadingMuonPhi_mt1BJ, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_SubleadingMuonPt_mt1BJ, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_SubleadingMuonEta_mt1BJ, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_SubleadingMuonPhi_mt1BJ, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_mt1BJ, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_mt1BJ, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_mt1BJ, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_mt1BJ, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_mt1BJ, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_mt1BJ, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_dimuonMass_mt1BJ, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonMass_wide_mt1BJ, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonPt_mt1BJ, tDiMuon.Pt(), tEventGenWeight);
      FillHisto(h_dimuonRap_mt1BJ, tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 0 && nJets == 0) {

      for (int i = 0; i < nJets; i++) {
        FillHisto(h_JetPt_bVeto_0J, vJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_JetEta_bVeto_0J, vJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_JetPhi_bVeto_0J, vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        FillHisto(h_BJetPt_bVeto_0J, vBJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_BJetEta_bVeto_0J, vBJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_BJetPhi_bVeto_0J, vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      FillHisto(h_LeadingMuonPt_bVeto_0J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_LeadingMuonEta_bVeto_0J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_LeadingMuonPhi_bVeto_0J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_SubleadingMuonPt_bVeto_0J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_SubleadingMuonEta_bVeto_0J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_SubleadingMuonPhi_bVeto_0J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_bVeto_0J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_bVeto_0J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_bVeto_0J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_bVeto_0J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_bVeto_0J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_bVeto_0J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_dimuonMass_bVeto_0J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonMass_wide_bVeto_0J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonPt_bVeto_0J, tDiMuon.Pt(), tEventGenWeight);
      FillHisto(h_dimuonRap_bVeto_0J, tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 0 && nJets == 1) {

      for (int i = 0; i < nJets; i++) {
        FillHisto(h_JetPt_bVeto_1J, vJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_JetEta_bVeto_1J, vJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_JetPhi_bVeto_1J, vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        FillHisto(h_BJetPt_bVeto_1J, vBJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_BJetEta_bVeto_1J, vBJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_BJetPhi_bVeto_1J, vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      FillHisto(h_LeadingMuonPt_bVeto_1J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_LeadingMuonEta_bVeto_1J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_LeadingMuonPhi_bVeto_1J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_SubleadingMuonPt_bVeto_1J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_SubleadingMuonEta_bVeto_1J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_SubleadingMuonPhi_bVeto_1J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_bVeto_1J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_bVeto_1J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_bVeto_1J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_bVeto_1J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_bVeto_1J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_bVeto_1J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_dimuonMass_bVeto_1J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonMass_wide_bVeto_1J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonPt_bVeto_1J, tDiMuon.Pt(), tEventGenWeight);
      FillHisto(h_dimuonRap_bVeto_1J, tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 0 && nJets > 1) {

      for (int i = 0; i < nJets; i++) {
        FillHisto(h_JetPt_bVeto_mt1J, vJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_JetEta_bVeto_mt1J, vJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_JetPhi_bVeto_mt1J, vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        FillHisto(h_BJetPt_bVeto_mt1J, vBJets.at(i).fVec.Pt(), tEventGenWeight);
        FillHisto(h_BJetEta_bVeto_mt1J, vBJets.at(i).fVec.Eta(), tEventGenWeight);
        FillHisto(h_BJetPhi_bVeto_mt1J, vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      FillHisto(h_LeadingMuonPt_bVeto_mt1J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_LeadingMuonEta_bVeto_mt1J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_LeadingMuonPhi_bVeto_mt1J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_SubleadingMuonPt_bVeto_mt1J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_SubleadingMuonEta_bVeto_mt1J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_SubleadingMuonPhi_bVeto_mt1J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_bVeto_mt1J, tFVecLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_bVeto_mt1J, tFVecLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_bVeto_mt1J, tFVecLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_MuonPt_bVeto_mt1J, tFVecSubLedingMuon.Pt(), tEventGenWeight);
      FillHisto(h_MuonEta_bVeto_mt1J, tFVecSubLedingMuon.Eta(), tEventGenWeight);
      FillHisto(h_MuonPhi_bVeto_mt1J, tFVecSubLedingMuon.Phi(), tEventGenWeight);

      FillHisto(h_dimuonMass_bVeto_mt1J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonMass_wide_bVeto_mt1J, tDiMuon.M(), tEventGenWeight);
      FillHisto(h_dimuonPt_bVeto_mt1J, tDiMuon.Pt(), tEventGenWeight);
      FillHisto(h_dimuonRap_bVeto_mt1J, tDiMuon.Rapidity(), tEventGenWeight);
    }
  } // End of event loop

  h_EventInfo->Fill(5, tTotalGenWeight);
  h_EventInfo->Fill(2, tMaxLoop);
  h_EventInfo->Fill(3, fMaxEntries);

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

void DYLoop::PrepareHist() {

  TH1::SetDefaultSumw2();

  std::vector<float> xbins = {
      0,   10,  15,  20,  25,  30,  35,  40,   45,   50,   55,  60,
      64,  68,  72,  76,  81,  86,  91,  96,   101,  106,  110, 115,
      120, 126, 133, 141, 150, 160, 171, 185,  200,  220,  243, 273,
      320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000, 4000, 4010};

  h_EventInfo = new TH1D("h_EventInfo", "h_EventInfo", 5, 0.5, 5.5);

  h_LHEDimuonMass = GetHist(Form("h_LHEDimuonMass"), Form("h_LHEDimuonMass"), 6000, 0., 6000.);
  h_GenWeight = GetHist(Form("h_GenWeight"), Form("h_GenWeight"), 20000, -10000., 10000.);
  h_LHEnMuon = GetHist(Form("h_LHEnMuon"), Form("h_LHEnMuon"), 10, 0., 10.);

  h_nPV_Count = new TH1D(Form("h_nPV_Count"), Form("PV_Count"), 100, 0., 100.);
  h_nPVGood_Count = new TH1D(Form("h_nPVGood_Count"), Form("PV_Count"), 100, 0., 100.);
  h_PileUp_Count_Interaction_before = new TH1D(Form("h_PileUp_Count_Interaction_before"), Form("h_PileUp_Count_Interaction_before"), 1000, 0., 100.);
  h_PileUp_Count_Interaction_after = new TH1D(Form("h_PileUp_Count_Interaction_after"), Form("h_PileUp_Count_Interaction_after"), 1000, 0., 100.);
  h_PileUp_Count_Intime = new TH1D(Form("h_PileUp_Count_Intime"), Form("PileUp_Count_Intime"), 1000, 0., 100.);

  h_nJet = new TH1D(Form("h_nJet"), Form("nJet"), 20, 0., 20.);
  h_nJet_bVeto = new TH1D(Form("h_nJet_bVeto"), Form("nJet"), 20, 0., 20.);
  h_nBJet = new TH1D(Form("h_nBJet"), Form("nJet"), 20, 0., 20.);

  h_JetPt = GetHist(Form("h_JetPt"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta = GetHist(Form("h_JetEta"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi = GetHist(Form("h_JetPhi"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt = GetHist(Form("h_BJetPt"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta = GetHist(Form("h_BJetEta"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi = GetHist(Form("h_BJetPhi"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt = GetHist(Form("h_LeadingMuonPt"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta = GetHist(Form("h_LeadingMuonEta"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi = GetHist(Form("h_LeadingMuonPhi"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt = GetHist(Form("h_SubleadingMuonPt"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta = GetHist(Form("h_SubleadingMuonEta"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi = GetHist(Form("h_SubleadingMuonPhi"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt = GetHist(Form("h_MuonPt"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta = GetHist(Form("h_MuonEta"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi = GetHist(Form("h_MuonPhi"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass = GetHist(Form("h_dimuonMass"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide = new TH1D(Form("h_dimuonMass_wide"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt = GetHist(Form("h_dimuonPt"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap = GetHist(Form("h_dimuonRap"), Form("dimuon_rap"), 60, -3., 3.);


  // 0 jet
  h_JetPt_0J = GetHist(Form("h_JetPt_0J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_0J = GetHist(Form("h_JetEta_0J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_0J = GetHist(Form("h_JetPhi_0J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_0J = GetHist(Form("h_BJetPt_0J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_0J = GetHist(Form("h_BJetEta_0J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_0J = GetHist(Form("h_BJetPhi_0J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_0J = GetHist(Form("h_LeadingMuonPt_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_0J = GetHist(Form("h_LeadingMuonEta_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_0J = GetHist(Form("h_LeadingMuonPhi_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_0J = GetHist(Form("h_SubleadingMuonPt_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_0J = GetHist(Form("h_SubleadingMuonEta_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_0J = GetHist(Form("h_SubleadingMuonPhi_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_0J = GetHist(Form("h_MuonPt_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_0J = GetHist(Form("h_MuonEta_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_0J = GetHist(Form("h_MuonPhi_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_0J = GetHist(Form("h_dimuonMass_0J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_0J = new TH1D(Form("h_dimuonMass_wide_0J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_0J = GetHist(Form("h_dimuonPt_0J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_0J = GetHist(Form("h_dimuonRap_0J"), Form("dimuon_rap"), 60, -3., 3.);

  // 1 jet
  h_JetPt_1J = GetHist(Form("h_JetPt_1J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_1J = GetHist(Form("h_JetEta_1J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_1J = GetHist(Form("h_JetPhi_1J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_1J = GetHist(Form("h_BJetPt_1J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_1J = GetHist(Form("h_BJetEta_1J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_1J = GetHist(Form("h_BJetPhi_1J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_1J = GetHist(Form("h_LeadingMuonPt_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_1J = GetHist(Form("h_LeadingMuonEta_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_1J = GetHist(Form("h_LeadingMuonPhi_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_1J = GetHist(Form("h_SubleadingMuonPt_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_1J = GetHist(Form("h_SubleadingMuonEta_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_1J = GetHist(Form("h_SubleadingMuonPhi_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_1J = GetHist(Form("h_MuonPt_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_1J = GetHist(Form("h_MuonEta_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_1J = GetHist(Form("h_MuonPhi_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_1J = GetHist(Form("h_dimuonMass_1J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_1J = new TH1D(Form("h_dimuonMass_wide_1J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_1J = GetHist(Form("h_dimuonPt_1J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_1J = GetHist(Form("h_dimuonRap_1J"), Form("dimuon_rap"), 60, -3., 3.);

  // more than 1 jet
  h_JetPt_mt1J = GetHist(Form("h_JetPt_mt1J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_mt1J = GetHist(Form("h_JetEta_mt1J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_mt1J = GetHist(Form("h_JetPhi_mt1J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_mt1J = GetHist(Form("h_BJetPt_mt1J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_mt1J = GetHist(Form("h_BJetEta_mt1J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_mt1J = GetHist(Form("h_BJetPhi_mt1J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_mt1J = GetHist(Form("h_LeadingMuonPt_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_mt1J = GetHist(Form("h_LeadingMuonEta_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_mt1J = GetHist(Form("h_LeadingMuonPhi_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_mt1J = GetHist(Form("h_SubleadingMuonPt_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_mt1J = GetHist(Form("h_SubleadingMuonEta_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_mt1J = GetHist(Form("h_SubleadingMuonPhi_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_mt1J = GetHist(Form("h_MuonPt_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_mt1J = GetHist(Form("h_MuonEta_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_mt1J = GetHist(Form("h_MuonPhi_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_mt1J = GetHist(Form("h_dimuonMass_mt1J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_mt1J = new TH1D(Form("h_dimuonMass_wide_mt1J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_mt1J = GetHist(Form("h_dimuonPt_mt1J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_mt1J = GetHist(Form("h_dimuonRap_mt1J"), Form("dimuon_rap"), 60, -3., 3.);


  // 0 Bjet
  h_JetPt_0BJ = GetHist(Form("h_JetPt_0BJ"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_0BJ = GetHist(Form("h_JetEta_0BJ"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_0BJ = GetHist(Form("h_JetPhi_0BJ"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_0BJ = GetHist(Form("h_BJetPt_0BJ"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_0BJ = GetHist(Form("h_BJetEta_0BJ"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_0BJ = GetHist(Form("h_BJetPhi_0BJ"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_0BJ = GetHist(Form("h_LeadingMuonPt_0BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_0BJ = GetHist(Form("h_LeadingMuonEta_0BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_0BJ = GetHist(Form("h_LeadingMuonPhi_0BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_0BJ = GetHist(Form("h_SubleadingMuonPt_0BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_0BJ = GetHist(Form("h_SubleadingMuonEta_0BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_0BJ = GetHist(Form("h_SubleadingMuonPhi_0BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_0BJ = GetHist(Form("h_MuonPt_0BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_0BJ = GetHist(Form("h_MuonEta_0BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_0BJ = GetHist(Form("h_MuonPhi_0BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_0BJ = GetHist(Form("h_dimuonMass_0BJ"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_0BJ = new TH1D(Form("h_dimuonMass_wide_0BJ"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_0BJ = GetHist(Form("h_dimuonPt_0BJ"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_0BJ = GetHist(Form("h_dimuonRap_0BJ"), Form("dimuon_rap"), 60, -3., 3.);

  // 1 bjet
  h_JetPt_1BJ = GetHist(Form("h_JetPt_1BJ"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_1BJ = GetHist(Form("h_JetEta_1BJ"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_1BJ = GetHist(Form("h_JetPhi_1BJ"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_1BJ = GetHist(Form("h_BJetPt_1BJ"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_1BJ = GetHist(Form("h_BJetEta_1BJ"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_1BJ = GetHist(Form("h_BJetPhi_1BJ"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_1BJ = GetHist(Form("h_LeadingMuonPt_1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_1BJ = GetHist(Form("h_LeadingMuonEta_1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_1BJ = GetHist(Form("h_LeadingMuonPhi_1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_1BJ = GetHist(Form("h_SubleadingMuonPt_1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_1BJ = GetHist(Form("h_SubleadingMuonEta_1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_1BJ = GetHist(Form("h_SubleadingMuonPhi_1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_1BJ = GetHist(Form("h_MuonPt_1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_1BJ = GetHist(Form("h_MuonEta_1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_1BJ = GetHist(Form("h_MuonPhi_1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_1BJ = GetHist(Form("h_dimuonMass_1BJ"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_1BJ = new TH1D(Form("h_dimuonMass_wide_1BJ"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_1BJ = GetHist(Form("h_dimuonPt_1BJ"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_1BJ = GetHist(Form("h_dimuonRap_1BJ"), Form("dimuon_rap"), 60, -3., 3.);

  // more than 1 bjet
  h_JetPt_mt1BJ = GetHist(Form("h_JetPt_mt1BJ"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_mt1BJ = GetHist(Form("h_JetEta_mt1BJ"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_mt1BJ = GetHist(Form("h_JetPhi_mt1BJ"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_mt1BJ = GetHist(Form("h_BJetPt_mt1BJ"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_mt1BJ = GetHist(Form("h_BJetEta_mt1BJ"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_mt1BJ = GetHist(Form("h_BJetPhi_mt1BJ"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_mt1BJ = GetHist(Form("h_LeadingMuonPt_mt1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_mt1BJ = GetHist(Form("h_LeadingMuonEta_mt1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_mt1BJ = GetHist(Form("h_LeadingMuonPhi_mt1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_mt1BJ = GetHist(Form("h_SubleadingMuonPt_mt1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_mt1BJ = GetHist(Form("h_SubleadingMuonEta_mt1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_mt1BJ = GetHist(Form("h_SubleadingMuonPhi_mt1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_mt1BJ = GetHist(Form("h_MuonPt_mt1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_mt1BJ = GetHist(Form("h_MuonEta_mt1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_mt1BJ = GetHist(Form("h_MuonPhi_mt1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_mt1BJ = GetHist(Form("h_dimuonMass_mt1BJ"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_mt1BJ = new TH1D(Form("h_dimuonMass_wide_mt1BJ"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_mt1BJ = GetHist(Form("h_dimuonPt_mt1BJ"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_mt1BJ = GetHist(Form("h_dimuonRap_mt1BJ"), Form("dimuon_rap"), 60, -3., 3.);


  // b-veto 0 jet
  h_JetPt_bVeto_0J = GetHist(Form("h_JetPt_bVeto_0J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_bVeto_0J = GetHist(Form("h_JetEta_bVeto_0J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_bVeto_0J = GetHist(Form("h_JetPhi_bVeto_0J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_bVeto_0J = GetHist(Form("h_BJetPt_bVeto_0J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_bVeto_0J = GetHist(Form("h_BJetEta_bVeto_0J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_bVeto_0J = GetHist(Form("h_BJetPhi_bVeto_0J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_bVeto_0J = GetHist(Form("h_LeadingMuonPt_bVeto_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_bVeto_0J = GetHist(Form("h_LeadingMuonEta_bVeto_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_bVeto_0J = GetHist(Form("h_LeadingMuonPhi_bVeto_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_bVeto_0J = GetHist(Form("h_SubleadingMuonPt_bVeto_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_bVeto_0J = GetHist(Form("h_SubleadingMuonEta_bVeto_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_bVeto_0J = GetHist(Form("h_SubleadingMuonPhi_bVeto_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_bVeto_0J = GetHist(Form("h_MuonPt_bVeto_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_bVeto_0J = GetHist(Form("h_MuonEta_bVeto_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_bVeto_0J = GetHist(Form("h_MuonPhi_bVeto_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_bVeto_0J = GetHist(Form("h_dimuonMass_bVeto_0J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_bVeto_0J = new TH1D(Form("h_dimuonMass_wide_bVeto_0J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_bVeto_0J = GetHist(Form("h_dimuonPt_bVeto_0J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_bVeto_0J = GetHist(Form("h_dimuonRap_bVeto_0J"), Form("dimuon_rap"), 60, -3., 3.);

  // b-veto 1 jet
  h_JetPt_bVeto_1J = GetHist(Form("h_JetPt_bVeto_1J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_bVeto_1J = GetHist(Form("h_JetEta_bVeto_1J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_bVeto_1J = GetHist(Form("h_JetPhi_bVeto_1J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_bVeto_1J = GetHist(Form("h_BJetPt_bVeto_1J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_bVeto_1J = GetHist(Form("h_BJetEta_bVeto_1J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_bVeto_1J = GetHist(Form("h_BJetPhi_bVeto_1J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_bVeto_1J = GetHist(Form("h_LeadingMuonPt_bVeto_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_bVeto_1J = GetHist(Form("h_LeadingMuonEta_bVeto_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_bVeto_1J = GetHist(Form("h_LeadingMuonPhi_bVeto_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_bVeto_1J = GetHist(Form("h_SubleadingMuonPt_bVeto_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_bVeto_1J = GetHist(Form("h_SubleadingMuonEta_bVeto_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_bVeto_1J = GetHist(Form("h_SubleadingMuonPhi_bVeto_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_bVeto_1J = GetHist(Form("h_MuonPt_bVeto_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_bVeto_1J = GetHist(Form("h_MuonEta_bVeto_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_bVeto_1J = GetHist(Form("h_MuonPhi_bVeto_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_bVeto_1J = GetHist(Form("h_dimuonMass_bVeto_1J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_bVeto_1J = new TH1D(Form("h_dimuonMass_wide_bVeto_1J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_bVeto_1J = GetHist(Form("h_dimuonPt_bVeto_1J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_bVeto_1J = GetHist(Form("h_dimuonRap_bVeto_1J"), Form("dimuon_rap"), 60, -3., 3.);

  // b-veto more than 1 jet
  h_JetPt_bVeto_mt1J = GetHist(Form("h_JetPt_bVeto_mt1J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_bVeto_mt1J = GetHist(Form("h_JetEta_bVeto_mt1J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_bVeto_mt1J = GetHist(Form("h_JetPhi_bVeto_mt1J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_bVeto_mt1J = GetHist(Form("h_BJetPt_bVeto_mt1J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_bVeto_mt1J = GetHist(Form("h_BJetEta_bVeto_mt1J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_bVeto_mt1J = GetHist(Form("h_BJetPhi_bVeto_mt1J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_bVeto_mt1J = GetHist(Form("h_LeadingMuonPt_bVeto_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_bVeto_mt1J = GetHist(Form("h_LeadingMuonEta_bVeto_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_bVeto_mt1J = GetHist(Form("h_LeadingMuonPhi_bVeto_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_bVeto_mt1J = GetHist(Form("h_SubleadingMuonPt_bVeto_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_bVeto_mt1J = GetHist(Form("h_SubleadingMuonEta_bVeto_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_bVeto_mt1J = GetHist(Form("h_SubleadingMuonPhi_bVeto_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_bVeto_mt1J = GetHist(Form("h_MuonPt_bVeto_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_bVeto_mt1J = GetHist(Form("h_MuonEta_bVeto_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_bVeto_mt1J = GetHist(Form("h_MuonPhi_bVeto_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_bVeto_mt1J = GetHist(Form("h_dimuonMass_bVeto_mt1J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_bVeto_mt1J = new TH1D(Form("h_dimuonMass_wide_bVeto_mt1J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_bVeto_mt1J = GetHist(Form("h_dimuonPt_bVeto_mt1J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_bVeto_mt1J = GetHist(Form("h_dimuonRap_bVeto_mt1J"), Form("dimuon_rap"), 60, -3., 3.);

  h_jetID = GetHist(Form("h_jetID"), Form("jetID"), 20, -0.5, 19.5);

}


void DYLoop::EndOfJob() {


  TFile* fOutputFile = new TFile(fOutputDir + fEra + "/" + fSampleName + "/output_" + std::to_string(fJobID) + ".root", "RECREATE");
  h_EventInfo->Write();

  h_nPV_Count->Write();
  h_nPVGood_Count->Write();
  h_PileUp_Count_Interaction_before->Write();
  h_PileUp_Count_Interaction_after->Write();
  h_PileUp_Count_Intime->Write();

  h_LHEDimuonMass->Write();
  h_GenWeight->Write();
  h_LHEnMuon->Write();
  h_nJet->Write();
  h_nJet_bVeto->Write();
  h_nBJet->Write();
  h_JetPt->Write();
  h_JetEta->Write();
  h_JetPhi->Write();
  h_BJetPt->Write();
  h_BJetEta->Write();
  h_BJetPhi->Write();
  h_LeadingMuonPt->Write();
  h_LeadingMuonEta->Write();
  h_LeadingMuonPhi->Write();
  h_SubleadingMuonPt->Write();
  h_SubleadingMuonEta->Write();
  h_SubleadingMuonPhi->Write();
  h_MuonPt->Write();
  h_MuonEta->Write();
  h_MuonPhi->Write();
  h_dimuonMass->Write();
  h_dimuonMass_wide->Write();
  h_dimuonPt->Write();
  h_dimuonRap->Write();

  h_JetPt_0J->Write();
  h_JetEta_0J->Write();
  h_JetPhi_0J->Write();
  h_BJetPt_0J->Write();
  h_BJetEta_0J->Write();
  h_BJetPhi_0J->Write();
  h_LeadingMuonPt_0J->Write();
  h_LeadingMuonEta_0J->Write();
  h_LeadingMuonPhi_0J->Write();
  h_SubleadingMuonPt_0J->Write();
  h_SubleadingMuonEta_0J->Write();
  h_SubleadingMuonPhi_0J->Write();
  h_MuonPt_0J->Write();
  h_MuonEta_0J->Write();
  h_MuonPhi_0J->Write();
  h_dimuonMass_0J->Write();
  h_dimuonMass_wide_0J->Write();
  h_dimuonPt_0J->Write();
  h_dimuonRap_0J->Write();
  h_JetPt_1J->Write();
  h_JetEta_1J->Write();
  h_JetPhi_1J->Write();
  h_BJetPt_1J->Write();
  h_BJetEta_1J->Write();
  h_BJetPhi_1J->Write();
  h_LeadingMuonPt_1J->Write();
  h_LeadingMuonEta_1J->Write();
  h_LeadingMuonPhi_1J->Write();
  h_SubleadingMuonPt_1J->Write();
  h_SubleadingMuonEta_1J->Write();
  h_SubleadingMuonPhi_1J->Write();
  h_MuonPt_1J->Write();
  h_MuonEta_1J->Write();
  h_MuonPhi_1J->Write();
  h_dimuonMass_1J->Write();
  h_dimuonMass_wide_1J->Write();
  h_dimuonPt_1J->Write();
  h_dimuonRap_1J->Write();
  h_JetPt_mt1J->Write();
  h_JetEta_mt1J->Write();
  h_JetPhi_mt1J->Write();
  h_BJetPt_mt1J->Write();
  h_BJetEta_mt1J->Write();
  h_BJetPhi_mt1J->Write();
  h_LeadingMuonPt_mt1J->Write();
  h_LeadingMuonEta_mt1J->Write();
  h_LeadingMuonPhi_mt1J->Write();
  h_SubleadingMuonPt_mt1J->Write();
  h_SubleadingMuonEta_mt1J->Write();
  h_SubleadingMuonPhi_mt1J->Write();
  h_MuonPt_mt1J->Write();
  h_MuonEta_mt1J->Write();
  h_MuonPhi_mt1J->Write();
  h_dimuonMass_mt1J->Write();
  h_dimuonMass_wide_mt1J->Write();
  h_dimuonPt_mt1J->Write();
  h_dimuonRap_mt1J->Write();
  h_JetPt_0BJ->Write();
  h_JetEta_0BJ->Write();
  h_JetPhi_0BJ->Write();
  h_BJetPt_0BJ->Write();
  h_BJetEta_0BJ->Write();
  h_BJetPhi_0BJ->Write();
  h_LeadingMuonPt_0BJ->Write();
  h_LeadingMuonEta_0BJ->Write();
  h_LeadingMuonPhi_0BJ->Write();
  h_SubleadingMuonPt_0BJ->Write();
  h_SubleadingMuonEta_0BJ->Write();
  h_SubleadingMuonPhi_0BJ->Write();
  h_MuonPt_0BJ->Write();
  h_MuonEta_0BJ->Write();
  h_MuonPhi_0BJ->Write();
  h_dimuonMass_0BJ->Write();
  h_dimuonMass_wide_0BJ->Write();
  h_dimuonPt_0BJ->Write();
  h_dimuonRap_0BJ->Write();
  h_JetPt_1BJ->Write();
  h_JetEta_1BJ->Write();
  h_JetPhi_1BJ->Write();
  h_BJetPt_1BJ->Write();
  h_BJetEta_1BJ->Write();
  h_BJetPhi_1BJ->Write();
  h_LeadingMuonPt_1BJ->Write();
  h_LeadingMuonEta_1BJ->Write();
  h_LeadingMuonPhi_1BJ->Write();
  h_SubleadingMuonPt_1BJ->Write();
  h_SubleadingMuonEta_1BJ->Write();
  h_SubleadingMuonPhi_1BJ->Write();
  h_MuonPt_1BJ->Write();
  h_MuonEta_1BJ->Write();
  h_MuonPhi_1BJ->Write();
  h_dimuonMass_1BJ->Write();
  h_dimuonMass_wide_1BJ->Write();
  h_dimuonPt_1BJ->Write();
  h_dimuonRap_1BJ->Write();
  h_JetPt_mt1BJ->Write();
  h_JetEta_mt1BJ->Write();
  h_JetPhi_mt1BJ->Write();
  h_BJetPt_mt1BJ->Write();
  h_BJetEta_mt1BJ->Write();
  h_BJetPhi_mt1BJ->Write();
  h_LeadingMuonPt_mt1BJ->Write();
  h_LeadingMuonEta_mt1BJ->Write();
  h_LeadingMuonPhi_mt1BJ->Write();
  h_SubleadingMuonPt_mt1BJ->Write();
  h_SubleadingMuonEta_mt1BJ->Write();
  h_SubleadingMuonPhi_mt1BJ->Write();
  h_MuonPt_mt1BJ->Write();
  h_MuonEta_mt1BJ->Write();
  h_MuonPhi_mt1BJ->Write();
  h_dimuonMass_mt1BJ->Write();
  h_dimuonMass_wide_mt1BJ->Write();
  h_dimuonPt_mt1BJ->Write();
  h_dimuonRap_mt1BJ->Write();
  h_JetPt_bVeto_0J->Write();
  h_JetEta_bVeto_0J->Write();
  h_JetPhi_bVeto_0J->Write();
  h_BJetPt_bVeto_0J->Write();
  h_BJetEta_bVeto_0J->Write();
  h_BJetPhi_bVeto_0J->Write();
  h_LeadingMuonPt_bVeto_0J->Write();
  h_LeadingMuonEta_bVeto_0J->Write();
  h_LeadingMuonPhi_bVeto_0J->Write();
  h_SubleadingMuonPt_bVeto_0J->Write();
  h_SubleadingMuonEta_bVeto_0J->Write();
  h_SubleadingMuonPhi_bVeto_0J->Write();
  h_MuonPt_bVeto_0J->Write();
  h_MuonEta_bVeto_0J->Write();
  h_MuonPhi_bVeto_0J->Write();
  h_dimuonMass_bVeto_0J->Write();
  h_dimuonMass_wide_bVeto_0J->Write();
  h_dimuonPt_bVeto_0J->Write();
  h_dimuonRap_bVeto_0J->Write();
  h_JetPt_bVeto_1J->Write();
  h_JetEta_bVeto_1J->Write();
  h_JetPhi_bVeto_1J->Write();
  h_BJetPt_bVeto_1J->Write();
  h_BJetEta_bVeto_1J->Write();
  h_BJetPhi_bVeto_1J->Write();
  h_LeadingMuonPt_bVeto_1J->Write();
  h_LeadingMuonEta_bVeto_1J->Write();
  h_LeadingMuonPhi_bVeto_1J->Write();
  h_SubleadingMuonPt_bVeto_1J->Write();
  h_SubleadingMuonEta_bVeto_1J->Write();
  h_SubleadingMuonPhi_bVeto_1J->Write();
  h_MuonPt_bVeto_1J->Write();
  h_MuonEta_bVeto_1J->Write();
  h_MuonPhi_bVeto_1J->Write();
  h_dimuonMass_bVeto_1J->Write();
  h_dimuonMass_wide_bVeto_1J->Write();
  h_dimuonPt_bVeto_1J->Write();
  h_dimuonRap_bVeto_1J->Write();
  h_JetPt_bVeto_mt1J->Write();
  h_JetEta_bVeto_mt1J->Write();
  h_JetPhi_bVeto_mt1J->Write();
  h_BJetPt_bVeto_mt1J->Write();
  h_BJetEta_bVeto_mt1J->Write();
  h_BJetPhi_bVeto_mt1J->Write();
  h_LeadingMuonPt_bVeto_mt1J->Write();
  h_LeadingMuonEta_bVeto_mt1J->Write();
  h_LeadingMuonPhi_bVeto_mt1J->Write();
  h_SubleadingMuonPt_bVeto_mt1J->Write();
  h_SubleadingMuonEta_bVeto_mt1J->Write();
  h_SubleadingMuonPhi_bVeto_mt1J->Write();
  h_MuonPt_bVeto_mt1J->Write();
  h_MuonEta_bVeto_mt1J->Write();
  h_MuonPhi_bVeto_mt1J->Write();
  h_dimuonMass_bVeto_mt1J->Write();
  h_dimuonMass_wide_bVeto_mt1J->Write();
  h_dimuonPt_bVeto_mt1J->Write();
  h_dimuonRap_bVeto_mt1J->Write();

  h_jetID->Write();

  fOutputFile->Close();
}
