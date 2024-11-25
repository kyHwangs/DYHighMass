#include "DYLoop.h"

#include <iostream>
#include <vector>

#include "DYLoop.h"
#include "muon.h"
#include "NT.h"

void DYLoop::SetNT() {

}

void DYLoop::Loop() {


  double tMaxEntries = fNtuples->GetEntries();
  fNtuples->init();

  double tMaxLoop = 0;
  double tTotalGenWeight = 0;
  while(fNtuples->GetNext()) { // Event loop starts here
    tMaxLoop++;

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

    if (fIsMC && fDoPU) {

      float tPUReweightingFactor = fPuReweighting->weight(**(fNtuples->Pileup_nTrueInt));

      h_PileUp_Count_Interaction_before_corr->Fill(**(fNtuples->Pileup_nTrueInt), tEventGenWeight);
      tEventGenWeight *= tPUReweightingFactor;
      h_PileUp_Count_Interaction_after_corr->Fill(**(fNtuples->Pileup_nTrueInt), tEventGenWeight);
    }

    if (fIsMC && fDoL1Pre) {
      tEventGenWeight *= **(fNtuples->L1PreFiringWeight_Nom);
    }

    h_EventInfo->Fill(1, 1);
    h_EventInfo->Fill(4, tEventGenWeight);

    if ( (int)tMaxLoop % 10000 == 0 )
      std::cout << "Loop: " << tMaxLoop << " / " << tMaxEntries << " | " << (100. * tMaxLoop / tMaxEntries) << " %" << std::endl;

    // if ( tMaxLoop == 100000 )
    //   break;

    if ( !(fNtuples->PassinNoiseFilter()) )
      continue;

    if ( !(fNtuples->PassingTrigger()) )
      continue;

    if ( !(fNtuples->PrepareMuon()) )
      continue;


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

    tTotalGenWeight += tEventGenWeight;

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

  } // End of event loop

  h_EventInfo->Fill(5, tTotalGenWeight);
  h_EventInfo->Fill(2, tMaxLoop);
  h_EventInfo->Fill(3, tMaxEntries);

  EndOfJob();
}

void DYLoop::PrepareHist() {

  h_EventInfo = new TH1D("h_EventInfo", "h_EventInfo", 5, 0.5, 5.5);
  h_EventInfo->Sumw2();

  h_LHEDimuonMass = new TH1D(Form("h_LHEDimuonMass"), Form("h_LHEDimuonMass"), 6000, 0., 6000.);
  h_LHEDimuonMass->Sumw2();
  h_GenWeight = new TH1D(Form("h_GenWeight"), Form("h_GenWeight"), 20000, -10000., 10000.);
  h_GenWeight->Sumw2();
  h_LHEnMuon = new TH1D(Form("h_LHEnMuon"), Form("h_LHEnMuon"), 10, 0., 10.);
  h_LHEnMuon->Sumw2();

  h_PV_Count_before_corr = new TH1D(Form("h_PV_Count_before_PUcorr"), Form("PV_Count"), 100, 0., 100.);
  h_PV_Count_before_corr->Sumw2();
  h_PileUp_Count_Interaction_before_corr = new TH1D(Form("h_PileUp_Count_Interaction_before_PUcorr"), Form("PileUp_Count_Interaction"), 1000, 0., 100.);
  h_PileUp_Count_Interaction_before_corr->Sumw2();
  h_PileUp_Count_Intime_before_corr = new TH1D(Form("h_PileUp_Count_Intime_before_PUcorr"), Form("PileUp_Count_Intime"), 1000, 0., 100.);
  h_PileUp_Count_Intime_before_corr->Sumw2();

  h_PV_Count_after_corr = new TH1D(Form("h_PV_Count_after_PUcorr"), Form("PV_Count"), 100, 0., 100.);
  h_PV_Count_after_corr->Sumw2();
  h_PileUp_Count_Interaction_after_corr = new TH1D(Form("h_PileUp_Count_Interaction_after_PUcorr"), Form("PileUp_Count_Interaction"), 1000, 0., 100.);
  h_PileUp_Count_Interaction_after_corr->Sumw2();
  h_PileUp_Count_Intime_after_corr = new TH1D(Form("h_PileUp_Count_Intime_after_PUcorr"), Form("PileUp_Count_Intime"), 1000, 0., 100.);
  h_PileUp_Count_Intime_after_corr->Sumw2();

  h_nJet_before = new TH1D(Form("h_nJet_before"), Form("nJet_before"), 20, 0., 20.);
  h_nJet_before->Sumw2();

  h_nJet_after = new TH1D(Form("h_nJet_after"), Form("nJet_after"), 20, 0., 20.);
  h_nJet_after->Sumw2();

  h_nBJet_before = new TH1D(Form("h_nBJet_before"), Form("nJet_before"), 20, 0., 20.);
  h_nBJet_before->Sumw2();
  h_nBJet_after = new TH1D(Form("h_nBJet_after"), Form("nJet_after"), 20, 0., 20.);
  h_nBJet_after->Sumw2();

  h_JetPt = new TH1D(Form("h_JetPt"), Form("Jet_pT"), 1000, 0, 1000);
  h_JetPt->Sumw2();
  h_JetEta = new TH1D(Form("h_JetEta"), Form("Jet_Eta"), 60, -3., 3.);
  h_JetEta->Sumw2();
  h_JetPhi = new TH1D(Form("h_JetPhi"), Form("Jet_Phi"), 60, -3.141594, 3.141594);
  h_JetPhi->Sumw2();
  h_BJetPt = new TH1D(Form("h_BJetPt"), Form("BJet_pT"), 1000, 0, 1000);
  h_BJetPt->Sumw2();
  h_BJetEta = new TH1D(Form("h_BJetEta"), Form("BJet_Eta"), 60, -3., 3.);
  h_BJetEta->Sumw2();
  h_BJetPhi = new TH1D(Form("h_BJetPhi"), Form("BJet_Phi"), 60, -3.141594, 3.141594);
  h_BJetPhi->Sumw2();

  h_LeadingMuonPt = new TH1D(Form("h_LeadingMuonPt"), Form("Muon_pT"), 1000, 0, 1000);
  h_LeadingMuonPt->Sumw2();
  h_LeadingMuonEta = new TH1D(Form("h_LeadingMuonEta"), Form("Muon_Eta"), 60, -3., 3.);
  h_LeadingMuonEta->Sumw2();
  h_LeadingMuonPhi = new TH1D(Form("h_LeadingMuonPhi"), Form("Muon_Phi"), 60, -3.141594, 3.141594);
  h_LeadingMuonPhi->Sumw2();

  h_SubleadingMuonPt = new TH1D(Form("h_SubleadingMuonPt"), Form("Muon_pT"), 1000, 0, 1000);
  h_SubleadingMuonPt->Sumw2();
  h_SubleadingMuonEta = new TH1D(Form("h_SubleadingMuonEta"), Form("Muon_Eta"), 60, -3., 3.);
  h_SubleadingMuonEta->Sumw2();
  h_SubleadingMuonPhi = new TH1D(Form("h_SubleadingMuonPhi"), Form("Muon_Phi"), 60, -3.141594, 3.141594);
  h_SubleadingMuonPhi->Sumw2();

  h_MuonPt = new TH1D(Form("h_MuonPt"), Form("Muon_pT"), 1000, 0, 1000);
  h_MuonPt->Sumw2();
  h_MuonEta = new TH1D(Form("h_MuonEta"), Form("Muon_Eta"), 60, -3., 3.);
  h_MuonEta->Sumw2();
  h_MuonPhi = new TH1D(Form("h_MuonPhi"), Form("Muon_Phi"), 60, -3.141594, 3.141594);
  h_MuonPhi->Sumw2();

  std::vector<float> xbins = {
      0,   10,  15,  20,  25,  30,  35,  40,   45,   50,   55,  60,
      64,  68,  72,  76,  81,  86,  91,  96,   101,  106,  110, 115,
      120, 126, 133, 141, 150, 160, 171, 185,  200,  220,  243, 273,
      320, 380, 440, 510, 600, 700, 830, 1000, 1500, 3000, 3010};

  h_dimuonMass = new TH1D(Form("h_dimuonMass"), Form("inv_Mass"), 100, 41., 141.);
  h_dimuonMass->Sumw2();
  h_dimuonMass_wide = new TH1D(Form("h_dimuonMass_wide"), Form("inv_Mass"),
                               xbins.size() - 1, &(xbins[0]));
  h_dimuonMass_wide->Sumw2();

  h_dimuonPt = new TH1D(Form("h_dimuonPt"), Form("dimuon_pT"), 1000, 0., 1000.);
  h_dimuonPt->Sumw2();
  h_dimuonRap = new TH1D(Form("h_dimuonRap"), Form("dimuon_rap"), 60, -3., 3.);
  h_dimuonRap->Sumw2();
}


void DYLoop::EndOfJob() {


  TFile* fOutputFile = new TFile(fOutputDir + fEra + "/" + fSampleName + "/output_" + std::to_string(fJobID) + ".root", "RECREATE");
  h_EventInfo->Write();
  h_LHEDimuonMass->Write();
  h_GenWeight->Write();
  h_LHEnMuon->Write();
  h_nJet_before->Write();
  h_nJet_after->Write();
  h_nBJet_before->Write();
  h_nBJet_after->Write();
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

  h_PV_Count_before_corr->Write();
  h_PileUp_Count_Interaction_before_corr->Write();
  h_PileUp_Count_Intime_before_corr->Write();
  h_PV_Count_after_corr->Write();
  h_PileUp_Count_Interaction_after_corr->Write();
  h_PileUp_Count_Intime_after_corr->Write();

  fOutputFile->Close();
}
