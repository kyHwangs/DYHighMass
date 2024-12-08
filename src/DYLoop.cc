#include <iostream>
#include <vector>

#include "DYLoop.h"
#include "muon.h"
#include "jet.h"
#include "electron.h"
#include "NT.h"

#include "TH1.h"

void DYLoop::SetNT() {

}

void DYLoop::Loop() {


  double tMaxEntries = fNtuples->GetEntries();
  fNtuples->init();

  double tMaxLoop = 0;
  double tTotalGenWeight = 0;
  while(fNtuples->GetNext()) { // Event loop starts here
    tMaxLoop++;

    if ( (int)tMaxLoop % 10000 == 0 )
      std::cout << "Loop: " << tMaxLoop << " / " << tMaxEntries << " | " << (100. * tMaxLoop / tMaxEntries) << " %" << std::endl;

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

    if ( !(fNtuples->PrepareMuon()) )
      continue;

    if ( !(fNtuples->PrepareElec()) )
      continue;

    auto tMuon = fNtuples->GetMuons();
    auto tElec = fNtuples->GetElecs();

    if ( !(fNtuples->PrepareJet(tMuon, tElec)) )
      continue;

    auto vJets = fNtuples->GetJets();
    auto vBJets = fNtuples->GetBJets();

    int nJets = vJets.size();
    int nBJets = vBJets.size();

    auto tLeadingMuon = fNtuples->GetLeadingMuon();
    auto tFVecLedingMuon = tLeadingMuon.fVec;
    auto tFVecRawLeadingMuon = tLeadingMuon.fVecRaw;

    auto tSubLeadingMuon = fNtuples->GetSubLeadingMuon();
    auto tFVecSubLedingMuon = tSubLeadingMuon.fVec;
    auto tFVecRawSubLeadingMuon = tSubLeadingMuon.fVecRaw;

    auto tDiMuon = tFVecLedingMuon + tFVecSubLedingMuon;

    if (fIsMC && fDoID) {
      tEventGenWeight *= fID_SF.getEfficiency(tFVecRawLeadingMuon.Pt(), std::abs(tFVecRawLeadingMuon.Eta()));
      tEventGenWeight *= fID_SF.getEfficiency(tFVecRawSubLeadingMuon.Pt(), std::abs(tFVecRawSubLeadingMuon.Eta()));
    }

    if (fIsMC && fDoISO) {
      tEventGenWeight *= fISO_SF.getEfficiency(tFVecRawLeadingMuon.Pt(), std::abs(tFVecRawLeadingMuon.Eta()));
      tEventGenWeight *= fISO_SF.getEfficiency(tFVecRawSubLeadingMuon.Pt(), std::abs(tFVecRawSubLeadingMuon.Eta()));
    }

    if (fIsMC && fDoTRIGG) {
      double mu_1_data = fTRIG_DataEff.getEfficiency(tFVecRawLeadingMuon.Pt(), std::abs(tFVecRawLeadingMuon.Eta()));
      double mu_2_data = fTRIG_DataEff.getEfficiency(tFVecRawSubLeadingMuon.Pt(), std::abs(tFVecRawSubLeadingMuon.Eta()));

      double mu_1_mc = fTRIG_MCEff.getEfficiency(tFVecRawLeadingMuon.Pt(), std::abs(tFVecRawLeadingMuon.Eta()));
      double mu_2_mc = fTRIG_MCEff.getEfficiency(tFVecRawSubLeadingMuon.Pt(), std::abs(tFVecRawSubLeadingMuon.Eta()));

      double data_tot = 1. - (1. - mu_1_data) * (1. - mu_2_data);
      double mc_tot = 1. - (1. - mu_1_mc) * (1. - mu_2_mc);

      double eventTriggerEffSF = 0.;
      if ( mc_tot != 0 )
        eventTriggerEffSF = (1. * data_tot) / mc_tot;

      tEventGenWeight *= eventTriggerEffSF;
    }

    for (int i = 0; i < nJets; i++) {
      h_jetID->Fill(vJets.at(i).fID, tEventGenWeight);
    }

    h_nJet->Fill(nJets, tEventGenWeight);
    h_nBJet->Fill(nBJets, tEventGenWeight);

    tTotalGenWeight += tEventGenWeight;

    h_nPV_Count->Fill(**(fNtuples->PV_npvs), tEventGenWeight);
    h_nPVGood_Count->Fill(**(fNtuples->PV_npvsGood), tEventGenWeight);

    h_LeadingMuonPt->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
    h_LeadingMuonEta->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
    h_LeadingMuonPhi->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

    h_SubleadingMuonPt->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
    h_SubleadingMuonEta->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
    h_SubleadingMuonPhi->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

    h_MuonPt->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
    h_MuonEta->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
    h_MuonPhi->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

    h_MuonPt->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
    h_MuonEta->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
    h_MuonPhi->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

    h_dimuonMass->Fill(tDiMuon.M(), tEventGenWeight);
    h_dimuonMass_wide->Fill(tDiMuon.M(), tEventGenWeight);
    h_dimuonPt->Fill(tDiMuon.Pt(), tEventGenWeight);
    h_dimuonRap->Fill(tDiMuon.Rapidity(), tEventGenWeight);

    if (nJets == 0) {

      for (int i = 0; i < nJets; i++) {
        h_JetPt_0J->Fill(vJets.at(i).fVec.Pt(), tEventGenWeight);
        h_JetEta_0J->Fill(vJets.at(i).fVec.Eta(), tEventGenWeight);
        h_JetPhi_0J->Fill(vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        h_BJetPt_0J->Fill(vBJets.at(i).fVec.Pt(), tEventGenWeight);
        h_BJetEta_0J->Fill(vBJets.at(i).fVec.Eta(), tEventGenWeight);
        h_BJetPhi_0J->Fill(vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      h_LeadingMuonPt_0J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_LeadingMuonEta_0J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_LeadingMuonPhi_0J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_SubleadingMuonPt_0J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_SubleadingMuonEta_0J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_SubleadingMuonPhi_0J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_0J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_0J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_0J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_0J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_0J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_0J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_dimuonMass_0J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonMass_wide_0J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonPt_0J->Fill(tDiMuon.Pt(), tEventGenWeight);
      h_dimuonRap_0J->Fill(tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nJets == 1) {

      for (int i = 0; i < nJets; i++) {
        h_JetPt_1J->Fill(vJets.at(i).fVec.Pt(), tEventGenWeight);
        h_JetEta_1J->Fill(vJets.at(i).fVec.Eta(), tEventGenWeight);
        h_JetPhi_1J->Fill(vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        h_BJetPt_1J->Fill(vBJets.at(i).fVec.Pt(), tEventGenWeight);
        h_BJetEta_1J->Fill(vBJets.at(i).fVec.Eta(), tEventGenWeight);
        h_BJetPhi_1J->Fill(vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      h_LeadingMuonPt_1J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_LeadingMuonEta_1J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_LeadingMuonPhi_1J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_SubleadingMuonPt_1J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_SubleadingMuonEta_1J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_SubleadingMuonPhi_1J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_1J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_1J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_1J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_1J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_1J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_1J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_dimuonMass_1J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonMass_wide_1J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonPt_1J->Fill(tDiMuon.Pt(), tEventGenWeight);
      h_dimuonRap_1J->Fill(tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nJets > 1) {

      for (int i = 0; i < nJets; i++) {
        h_JetPt_mt1J->Fill(vJets.at(i).fVec.Pt(), tEventGenWeight);
        h_JetEta_mt1J->Fill(vJets.at(i).fVec.Eta(), tEventGenWeight);
        h_JetPhi_mt1J->Fill(vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        h_BJetPt_mt1J->Fill(vBJets.at(i).fVec.Pt(), tEventGenWeight);
        h_BJetEta_mt1J->Fill(vBJets.at(i).fVec.Eta(), tEventGenWeight);
        h_BJetPhi_mt1J->Fill(vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      h_LeadingMuonPt_mt1J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_LeadingMuonEta_mt1J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_LeadingMuonPhi_mt1J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_SubleadingMuonPt_mt1J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_SubleadingMuonEta_mt1J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_SubleadingMuonPhi_mt1J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_mt1J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_mt1J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_mt1J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_mt1J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_mt1J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_mt1J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_dimuonMass_mt1J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonMass_wide_mt1J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonPt_mt1J->Fill(tDiMuon.Pt(), tEventGenWeight);
      h_dimuonRap_mt1J->Fill(tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 0) {

      for (int i = 0; i < nJets; i++) {
        h_JetPt_0BJ->Fill(vJets.at(i).fVec.Pt(), tEventGenWeight);
        h_JetEta_0BJ->Fill(vJets.at(i).fVec.Eta(), tEventGenWeight);
        h_JetPhi_0BJ->Fill(vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        h_BJetPt_0BJ->Fill(vBJets.at(i).fVec.Pt(), tEventGenWeight);
        h_BJetEta_0BJ->Fill(vBJets.at(i).fVec.Eta(), tEventGenWeight);
        h_BJetPhi_0BJ->Fill(vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      h_LeadingMuonPt_0BJ->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_LeadingMuonEta_0BJ->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_LeadingMuonPhi_0BJ->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_SubleadingMuonPt_0BJ->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_SubleadingMuonEta_0BJ->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_SubleadingMuonPhi_0BJ->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_0BJ->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_0BJ->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_0BJ->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_0BJ->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_0BJ->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_0BJ->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_dimuonMass_0BJ->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonMass_wide_0BJ->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonPt_0BJ->Fill(tDiMuon.Pt(), tEventGenWeight);
      h_dimuonRap_0BJ->Fill(tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 1) {

      for (int i = 0; i < nJets; i++) {
        h_JetPt_1BJ->Fill(vJets.at(i).fVec.Pt(), tEventGenWeight);
        h_JetEta_1BJ->Fill(vJets.at(i).fVec.Eta(), tEventGenWeight);
        h_JetPhi_1BJ->Fill(vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        h_BJetPt_1BJ->Fill(vBJets.at(i).fVec.Pt(), tEventGenWeight);
        h_BJetEta_1BJ->Fill(vBJets.at(i).fVec.Eta(), tEventGenWeight);
        h_BJetPhi_1BJ->Fill(vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      h_LeadingMuonPt_1BJ->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_LeadingMuonEta_1BJ->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_LeadingMuonPhi_1BJ->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_SubleadingMuonPt_1BJ->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_SubleadingMuonEta_1BJ->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_SubleadingMuonPhi_1BJ->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_1BJ->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_1BJ->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_1BJ->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_1BJ->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_1BJ->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_1BJ->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_dimuonMass_1BJ->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonMass_wide_1BJ->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonPt_1BJ->Fill(tDiMuon.Pt(), tEventGenWeight);
      h_dimuonRap_1BJ->Fill(tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets > 1) {

      for (int i = 0; i < nJets; i++) {
        h_JetPt_mt1BJ->Fill(vJets.at(i).fVec.Pt(), tEventGenWeight);
        h_JetEta_mt1BJ->Fill(vJets.at(i).fVec.Eta(), tEventGenWeight);
        h_JetPhi_mt1BJ->Fill(vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        h_BJetPt_mt1BJ->Fill(vBJets.at(i).fVec.Pt(), tEventGenWeight);
        h_BJetEta_mt1BJ->Fill(vBJets.at(i).fVec.Eta(), tEventGenWeight);
        h_BJetPhi_mt1BJ->Fill(vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      h_LeadingMuonPt_mt1BJ->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_LeadingMuonEta_mt1BJ->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_LeadingMuonPhi_mt1BJ->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_SubleadingMuonPt_mt1BJ->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_SubleadingMuonEta_mt1BJ->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_SubleadingMuonPhi_mt1BJ->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_mt1BJ->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_mt1BJ->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_mt1BJ->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_mt1BJ->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_mt1BJ->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_mt1BJ->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_dimuonMass_mt1BJ->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonMass_wide_mt1BJ->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonPt_mt1BJ->Fill(tDiMuon.Pt(), tEventGenWeight);
      h_dimuonRap_mt1BJ->Fill(tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 0 && nJets == 0) {

      for (int i = 0; i < nJets; i++) {
        h_JetPt_bVeto_0J->Fill(vJets.at(i).fVec.Pt(), tEventGenWeight);
        h_JetEta_bVeto_0J->Fill(vJets.at(i).fVec.Eta(), tEventGenWeight);
        h_JetPhi_bVeto_0J->Fill(vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        h_BJetPt_bVeto_0J->Fill(vBJets.at(i).fVec.Pt(), tEventGenWeight);
        h_BJetEta_bVeto_0J->Fill(vBJets.at(i).fVec.Eta(), tEventGenWeight);
        h_BJetPhi_bVeto_0J->Fill(vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      h_LeadingMuonPt_bVeto_0J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_LeadingMuonEta_bVeto_0J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_LeadingMuonPhi_bVeto_0J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_SubleadingMuonPt_bVeto_0J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_SubleadingMuonEta_bVeto_0J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_SubleadingMuonPhi_bVeto_0J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_bVeto_0J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_bVeto_0J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_bVeto_0J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_bVeto_0J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_bVeto_0J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_bVeto_0J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_dimuonMass_bVeto_0J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonMass_wide_bVeto_0J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonPt_bVeto_0J->Fill(tDiMuon.Pt(), tEventGenWeight);
      h_dimuonRap_bVeto_0J->Fill(tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 0 && nJets == 1) {

      for (int i = 0; i < nJets; i++) {
        h_JetPt_bVeto_1J->Fill(vJets.at(i).fVec.Pt(), tEventGenWeight);
        h_JetEta_bVeto_1J->Fill(vJets.at(i).fVec.Eta(), tEventGenWeight);
        h_JetPhi_bVeto_1J->Fill(vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        h_BJetPt_bVeto_1J->Fill(vBJets.at(i).fVec.Pt(), tEventGenWeight);
        h_BJetEta_bVeto_1J->Fill(vBJets.at(i).fVec.Eta(), tEventGenWeight);
        h_BJetPhi_bVeto_1J->Fill(vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      h_LeadingMuonPt_bVeto_1J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_LeadingMuonEta_bVeto_1J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_LeadingMuonPhi_bVeto_1J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_SubleadingMuonPt_bVeto_1J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_SubleadingMuonEta_bVeto_1J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_SubleadingMuonPhi_bVeto_1J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_bVeto_1J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_bVeto_1J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_bVeto_1J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_bVeto_1J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_bVeto_1J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_bVeto_1J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_dimuonMass_bVeto_1J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonMass_wide_bVeto_1J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonPt_bVeto_1J->Fill(tDiMuon.Pt(), tEventGenWeight);
      h_dimuonRap_bVeto_1J->Fill(tDiMuon.Rapidity(), tEventGenWeight);
    }

    if (nBJets == 0 && nJets > 1) {

      for (int i = 0; i < nJets; i++) {
        h_JetPt_bVeto_mt1J->Fill(vJets.at(i).fVec.Pt(), tEventGenWeight);
        h_JetEta_bVeto_mt1J->Fill(vJets.at(i).fVec.Eta(), tEventGenWeight);
        h_JetPhi_bVeto_mt1J->Fill(vJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      for (int i = 0; i < nBJets; i++) {
        h_BJetPt_bVeto_mt1J->Fill(vBJets.at(i).fVec.Pt(), tEventGenWeight);
        h_BJetEta_bVeto_mt1J->Fill(vBJets.at(i).fVec.Eta(), tEventGenWeight);
        h_BJetPhi_bVeto_mt1J->Fill(vBJets.at(i).fVec.Phi(), tEventGenWeight);
      }

      h_LeadingMuonPt_bVeto_mt1J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_LeadingMuonEta_bVeto_mt1J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_LeadingMuonPhi_bVeto_mt1J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_SubleadingMuonPt_bVeto_mt1J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_SubleadingMuonEta_bVeto_mt1J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_SubleadingMuonPhi_bVeto_mt1J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_bVeto_mt1J->Fill(tFVecLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_bVeto_mt1J->Fill(tFVecLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_bVeto_mt1J->Fill(tFVecLedingMuon.Phi(), tEventGenWeight);

      h_MuonPt_bVeto_mt1J->Fill(tFVecSubLedingMuon.Pt(), tEventGenWeight);
      h_MuonEta_bVeto_mt1J->Fill(tFVecSubLedingMuon.Eta(), tEventGenWeight);
      h_MuonPhi_bVeto_mt1J->Fill(tFVecSubLedingMuon.Phi(), tEventGenWeight);

      h_dimuonMass_bVeto_mt1J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonMass_wide_bVeto_mt1J->Fill(tDiMuon.M(), tEventGenWeight);
      h_dimuonPt_bVeto_mt1J->Fill(tDiMuon.Pt(), tEventGenWeight);
      h_dimuonRap_bVeto_mt1J->Fill(tDiMuon.Rapidity(), tEventGenWeight);
    }
  } // End of event loop

  h_EventInfo->Fill(5, tTotalGenWeight);
  h_EventInfo->Fill(2, tMaxLoop);
  h_EventInfo->Fill(3, tMaxEntries);

  EndOfJob();
}

void DYLoop::PrepareHist() {

  TH1::SetDefaultSumw2();

  std::vector<float> xbins = {
      0,   10,  15,  20,  25,  30,  35,  40,   45,   50,   55,  60,
      64,  68,  72,  76,  81,  86,  91,  96,   101,  106,  110, 115,
      120, 126, 133, 141, 150, 160, 171, 185,  200,  220,  243, 273,
      320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000, 3010};

  h_EventInfo = new TH1D("h_EventInfo", "h_EventInfo", 5, 0.5, 5.5);

  h_LHEDimuonMass = new TH1D(Form("h_LHEDimuonMass"), Form("h_LHEDimuonMass"), 6000, 0., 6000.);
  h_GenWeight = new TH1D(Form("h_GenWeight"), Form("h_GenWeight"), 20000, -10000., 10000.);
  h_LHEnMuon = new TH1D(Form("h_LHEnMuon"), Form("h_LHEnMuon"), 10, 0., 10.);

  h_nPV_Count = new TH1D(Form("h_nPV_Count"), Form("PV_Count"), 100, 0., 100.);
  h_nPVGood_Count = new TH1D(Form("h_nPVGood_Count"), Form("PV_Count"), 100, 0., 100.);
  h_PileUp_Count_Interaction = new TH1D(Form("h_PileUp_Count_Interaction"), Form("PileUp_Count_Interaction"), 1000, 0., 100.);
  h_PileUp_Count_Intime = new TH1D(Form("h_PileUp_Count_Intime"), Form("PileUp_Count_Intime"), 1000, 0., 100.);

  h_nJet = new TH1D(Form("h_nJet"), Form("nJet"), 20, 0., 20.);
  h_nBJet = new TH1D(Form("h_nBJet"), Form("nJet"), 20, 0., 20.);

  h_JetPt = new TH1D(Form("h_JetPt"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta = new TH1D(Form("h_JetEta"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi = new TH1D(Form("h_JetPhi"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt = new TH1D(Form("h_BJetPt"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta = new TH1D(Form("h_BJetEta"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi = new TH1D(Form("h_BJetPhi"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt = new TH1D(Form("h_LeadingMuonPt"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta = new TH1D(Form("h_LeadingMuonEta"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi = new TH1D(Form("h_LeadingMuonPhi"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt = new TH1D(Form("h_SubleadingMuonPt"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta = new TH1D(Form("h_SubleadingMuonEta"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi = new TH1D(Form("h_SubleadingMuonPhi"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt = new TH1D(Form("h_MuonPt"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta = new TH1D(Form("h_MuonEta"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi = new TH1D(Form("h_MuonPhi"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass = new TH1D(Form("h_dimuonMass"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide = new TH1D(Form("h_dimuonMass_wide"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt = new TH1D(Form("h_dimuonPt"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap = new TH1D(Form("h_dimuonRap"), Form("dimuon_rap"), 60, -3., 3.);


  // 0 jet
  h_JetPt_0J = new TH1D(Form("h_JetPt_0J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_0J = new TH1D(Form("h_JetEta_0J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_0J = new TH1D(Form("h_JetPhi_0J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_0J = new TH1D(Form("h_BJetPt_0J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_0J = new TH1D(Form("h_BJetEta_0J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_0J = new TH1D(Form("h_BJetPhi_0J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_0J = new TH1D(Form("h_LeadingMuonPt_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_0J = new TH1D(Form("h_LeadingMuonEta_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_0J = new TH1D(Form("h_LeadingMuonPhi_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_0J = new TH1D(Form("h_SubleadingMuonPt_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_0J = new TH1D(Form("h_SubleadingMuonEta_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_0J = new TH1D(Form("h_SubleadingMuonPhi_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_0J = new TH1D(Form("h_MuonPt_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_0J = new TH1D(Form("h_MuonEta_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_0J = new TH1D(Form("h_MuonPhi_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_0J = new TH1D(Form("h_dimuonMass_0J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_0J = new TH1D(Form("h_dimuonMass_wide_0J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_0J = new TH1D(Form("h_dimuonPt_0J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_0J = new TH1D(Form("h_dimuonRap_0J"), Form("dimuon_rap"), 60, -3., 3.);

  // 1 jet
  h_JetPt_1J = new TH1D(Form("h_JetPt_1J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_1J = new TH1D(Form("h_JetEta_1J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_1J = new TH1D(Form("h_JetPhi_1J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_1J = new TH1D(Form("h_BJetPt_1J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_1J = new TH1D(Form("h_BJetEta_1J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_1J = new TH1D(Form("h_BJetPhi_1J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_1J = new TH1D(Form("h_LeadingMuonPt_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_1J = new TH1D(Form("h_LeadingMuonEta_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_1J = new TH1D(Form("h_LeadingMuonPhi_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_1J = new TH1D(Form("h_SubleadingMuonPt_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_1J = new TH1D(Form("h_SubleadingMuonEta_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_1J = new TH1D(Form("h_SubleadingMuonPhi_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_1J = new TH1D(Form("h_MuonPt_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_1J = new TH1D(Form("h_MuonEta_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_1J = new TH1D(Form("h_MuonPhi_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_1J = new TH1D(Form("h_dimuonMass_1J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_1J = new TH1D(Form("h_dimuonMass_wide_1J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_1J = new TH1D(Form("h_dimuonPt_1J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_1J = new TH1D(Form("h_dimuonRap_1J"), Form("dimuon_rap"), 60, -3., 3.);

  // more than 1 jet
  h_JetPt_mt1J = new TH1D(Form("h_JetPt_mt1J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_mt1J = new TH1D(Form("h_JetEta_mt1J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_mt1J = new TH1D(Form("h_JetPhi_mt1J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_mt1J = new TH1D(Form("h_BJetPt_mt1J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_mt1J = new TH1D(Form("h_BJetEta_mt1J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_mt1J = new TH1D(Form("h_BJetPhi_mt1J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_mt1J = new TH1D(Form("h_LeadingMuonPt_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_mt1J = new TH1D(Form("h_LeadingMuonEta_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_mt1J = new TH1D(Form("h_LeadingMuonPhi_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_mt1J = new TH1D(Form("h_SubleadingMuonPt_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_mt1J = new TH1D(Form("h_SubleadingMuonEta_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_mt1J = new TH1D(Form("h_SubleadingMuonPhi_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_mt1J = new TH1D(Form("h_MuonPt_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_mt1J = new TH1D(Form("h_MuonEta_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_mt1J = new TH1D(Form("h_MuonPhi_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_mt1J = new TH1D(Form("h_dimuonMass_mt1J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_mt1J = new TH1D(Form("h_dimuonMass_wide_mt1J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_mt1J = new TH1D(Form("h_dimuonPt_mt1J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_mt1J = new TH1D(Form("h_dimuonRap_mt1J"), Form("dimuon_rap"), 60, -3., 3.);


  // 0 Bjet
  h_JetPt_0BJ = new TH1D(Form("h_JetPt_0BJ"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_0BJ = new TH1D(Form("h_JetEta_0BJ"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_0BJ = new TH1D(Form("h_JetPhi_0BJ"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_0BJ = new TH1D(Form("h_BJetPt_0BJ"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_0BJ = new TH1D(Form("h_BJetEta_0BJ"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_0BJ = new TH1D(Form("h_BJetPhi_0BJ"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_0BJ = new TH1D(Form("h_LeadingMuonPt_0BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_0BJ = new TH1D(Form("h_LeadingMuonEta_0BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_0BJ = new TH1D(Form("h_LeadingMuonPhi_0BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_0BJ = new TH1D(Form("h_SubleadingMuonPt_0BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_0BJ = new TH1D(Form("h_SubleadingMuonEta_0BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_0BJ = new TH1D(Form("h_SubleadingMuonPhi_0BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_0BJ = new TH1D(Form("h_MuonPt_0BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_0BJ = new TH1D(Form("h_MuonEta_0BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_0BJ = new TH1D(Form("h_MuonPhi_0BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_0BJ = new TH1D(Form("h_dimuonMass_0BJ"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_0BJ = new TH1D(Form("h_dimuonMass_wide_0BJ"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_0BJ = new TH1D(Form("h_dimuonPt_0BJ"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_0BJ = new TH1D(Form("h_dimuonRap_0BJ"), Form("dimuon_rap"), 60, -3., 3.);

  // 1 bjet
  h_JetPt_1BJ = new TH1D(Form("h_JetPt_1BJ"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_1BJ = new TH1D(Form("h_JetEta_1BJ"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_1BJ = new TH1D(Form("h_JetPhi_1BJ"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_1BJ = new TH1D(Form("h_BJetPt_1BJ"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_1BJ = new TH1D(Form("h_BJetEta_1BJ"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_1BJ = new TH1D(Form("h_BJetPhi_1BJ"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_1BJ = new TH1D(Form("h_LeadingMuonPt_1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_1BJ = new TH1D(Form("h_LeadingMuonEta_1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_1BJ = new TH1D(Form("h_LeadingMuonPhi_1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_1BJ = new TH1D(Form("h_SubleadingMuonPt_1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_1BJ = new TH1D(Form("h_SubleadingMuonEta_1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_1BJ = new TH1D(Form("h_SubleadingMuonPhi_1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_1BJ = new TH1D(Form("h_MuonPt_1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_1BJ = new TH1D(Form("h_MuonEta_1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_1BJ = new TH1D(Form("h_MuonPhi_1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_1BJ = new TH1D(Form("h_dimuonMass_1BJ"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_1BJ = new TH1D(Form("h_dimuonMass_wide_1BJ"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_1BJ = new TH1D(Form("h_dimuonPt_1BJ"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_1BJ = new TH1D(Form("h_dimuonRap_1BJ"), Form("dimuon_rap"), 60, -3., 3.);

  // more than 1 bjet
  h_JetPt_mt1BJ = new TH1D(Form("h_JetPt_mt1BJ"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_mt1BJ = new TH1D(Form("h_JetEta_mt1BJ"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_mt1BJ = new TH1D(Form("h_JetPhi_mt1BJ"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_mt1BJ = new TH1D(Form("h_BJetPt_mt1BJ"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_mt1BJ = new TH1D(Form("h_BJetEta_mt1BJ"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_mt1BJ = new TH1D(Form("h_BJetPhi_mt1BJ"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_mt1BJ = new TH1D(Form("h_LeadingMuonPt_mt1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_mt1BJ = new TH1D(Form("h_LeadingMuonEta_mt1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_mt1BJ = new TH1D(Form("h_LeadingMuonPhi_mt1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_mt1BJ = new TH1D(Form("h_SubleadingMuonPt_mt1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_mt1BJ = new TH1D(Form("h_SubleadingMuonEta_mt1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_mt1BJ = new TH1D(Form("h_SubleadingMuonPhi_mt1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_mt1BJ = new TH1D(Form("h_MuonPt_mt1BJ"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_mt1BJ = new TH1D(Form("h_MuonEta_mt1BJ"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_mt1BJ = new TH1D(Form("h_MuonPhi_mt1BJ"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_mt1BJ = new TH1D(Form("h_dimuonMass_mt1BJ"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_mt1BJ = new TH1D(Form("h_dimuonMass_wide_mt1BJ"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_mt1BJ = new TH1D(Form("h_dimuonPt_mt1BJ"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_mt1BJ = new TH1D(Form("h_dimuonRap_mt1BJ"), Form("dimuon_rap"), 60, -3., 3.);


  // b-veto 0 jet
  h_JetPt_bVeto_0J = new TH1D(Form("h_JetPt_bVeto_0J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_bVeto_0J = new TH1D(Form("h_JetEta_bVeto_0J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_bVeto_0J = new TH1D(Form("h_JetPhi_bVeto_0J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_bVeto_0J = new TH1D(Form("h_BJetPt_bVeto_0J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_bVeto_0J = new TH1D(Form("h_BJetEta_bVeto_0J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_bVeto_0J = new TH1D(Form("h_BJetPhi_bVeto_0J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_bVeto_0J = new TH1D(Form("h_LeadingMuonPt_bVeto_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_bVeto_0J = new TH1D(Form("h_LeadingMuonEta_bVeto_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_bVeto_0J = new TH1D(Form("h_LeadingMuonPhi_bVeto_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_bVeto_0J = new TH1D(Form("h_SubleadingMuonPt_bVeto_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_bVeto_0J = new TH1D(Form("h_SubleadingMuonEta_bVeto_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_bVeto_0J = new TH1D(Form("h_SubleadingMuonPhi_bVeto_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_bVeto_0J = new TH1D(Form("h_MuonPt_bVeto_0J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_bVeto_0J = new TH1D(Form("h_MuonEta_bVeto_0J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_bVeto_0J = new TH1D(Form("h_MuonPhi_bVeto_0J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_bVeto_0J = new TH1D(Form("h_dimuonMass_bVeto_0J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_bVeto_0J = new TH1D(Form("h_dimuonMass_wide_bVeto_0J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_bVeto_0J = new TH1D(Form("h_dimuonPt_bVeto_0J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_bVeto_0J = new TH1D(Form("h_dimuonRap_bVeto_0J"), Form("dimuon_rap"), 60, -3., 3.);

  // b-veto 1 jet
  h_JetPt_bVeto_1J = new TH1D(Form("h_JetPt_bVeto_1J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_bVeto_1J = new TH1D(Form("h_JetEta_bVeto_1J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_bVeto_1J = new TH1D(Form("h_JetPhi_bVeto_1J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_bVeto_1J = new TH1D(Form("h_BJetPt_bVeto_1J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_bVeto_1J = new TH1D(Form("h_BJetEta_bVeto_1J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_bVeto_1J = new TH1D(Form("h_BJetPhi_bVeto_1J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_bVeto_1J = new TH1D(Form("h_LeadingMuonPt_bVeto_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_bVeto_1J = new TH1D(Form("h_LeadingMuonEta_bVeto_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_bVeto_1J = new TH1D(Form("h_LeadingMuonPhi_bVeto_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_bVeto_1J = new TH1D(Form("h_SubleadingMuonPt_bVeto_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_bVeto_1J = new TH1D(Form("h_SubleadingMuonEta_bVeto_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_bVeto_1J = new TH1D(Form("h_SubleadingMuonPhi_bVeto_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_bVeto_1J = new TH1D(Form("h_MuonPt_bVeto_1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_bVeto_1J = new TH1D(Form("h_MuonEta_bVeto_1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_bVeto_1J = new TH1D(Form("h_MuonPhi_bVeto_1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_bVeto_1J = new TH1D(Form("h_dimuonMass_bVeto_1J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_bVeto_1J = new TH1D(Form("h_dimuonMass_wide_bVeto_1J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_bVeto_1J = new TH1D(Form("h_dimuonPt_bVeto_1J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_bVeto_1J = new TH1D(Form("h_dimuonRap_bVeto_1J"), Form("dimuon_rap"), 60, -3., 3.);

  // b-veto more than 1 jet
  h_JetPt_bVeto_mt1J = new TH1D(Form("h_JetPt_bVeto_mt1J"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetEta_bVeto_mt1J = new TH1D(Form("h_JetEta_bVeto_mt1J"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetPhi_bVeto_mt1J = new TH1D(Form("h_JetPhi_bVeto_mt1J"), Form("Jet_Phi"), 60, -3.141594, 3.141594);

  h_BJetPt_bVeto_mt1J = new TH1D(Form("h_BJetPt_bVeto_mt1J"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetEta_bVeto_mt1J = new TH1D(Form("h_BJetEta_bVeto_mt1J"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetPhi_bVeto_mt1J = new TH1D(Form("h_BJetPhi_bVeto_mt1J"), Form("BJet_Phi"), 60, -3.141594, 3.141594);

  h_LeadingMuonPt_bVeto_mt1J = new TH1D(Form("h_LeadingMuonPt_bVeto_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonEta_bVeto_mt1J = new TH1D(Form("h_LeadingMuonEta_bVeto_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonPhi_bVeto_mt1J = new TH1D(Form("h_LeadingMuonPhi_bVeto_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_SubleadingMuonPt_bVeto_mt1J = new TH1D(Form("h_SubleadingMuonPt_bVeto_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonEta_bVeto_mt1J = new TH1D(Form("h_SubleadingMuonEta_bVeto_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonPhi_bVeto_mt1J = new TH1D(Form("h_SubleadingMuonPhi_bVeto_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_MuonPt_bVeto_mt1J = new TH1D(Form("h_MuonPt_bVeto_mt1J"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonEta_bVeto_mt1J = new TH1D(Form("h_MuonEta_bVeto_mt1J"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonPhi_bVeto_mt1J = new TH1D(Form("h_MuonPhi_bVeto_mt1J"), Form("Muon_Phi"), 60, -3.141594, 3.141594);

  h_dimuonMass_bVeto_mt1J = new TH1D(Form("h_dimuonMass_bVeto_mt1J"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass_wide_bVeto_mt1J = new TH1D(Form("h_dimuonMass_wide_bVeto_mt1J"), Form("inv_Mass"), xbins.size() - 1, &(xbins[0]));

  h_dimuonPt_bVeto_mt1J = new TH1D(Form("h_dimuonPt_bVeto_mt1J"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonRap_bVeto_mt1J = new TH1D(Form("h_dimuonRap_bVeto_mt1J"), Form("dimuon_rap"), 60, -3., 3.);

  h_jetID = new TH1D(Form("h_jetID"), Form("jetID"), 20, -0.5, 19.5);

}


void DYLoop::EndOfJob() {


  TFile* fOutputFile = new TFile(fOutputDir + fEra + "/" + fSampleName + "/output_" + std::to_string(fJobID) + ".root", "RECREATE");
  h_EventInfo->Write();

  h_nPV_Count->Write();
  h_nPVGood_Count->Write();
  h_PileUp_Count_Interaction->Write();
  h_PileUp_Count_Intime->Write();

  h_LHEDimuonMass->Write();
  h_GenWeight->Write();
  h_LHEnMuon->Write();
  h_nJet->Write();
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
