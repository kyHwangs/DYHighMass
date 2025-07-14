#include "HistoSet.h"
#include "jet.h"

#include "TFile.h"
#include "TLorentzVector.h"
#include "TH1.h"


void HistoSet::Init() {

  TH1::SetDefaultSumw2();

  SetHisto("h_EventInfo", std::vector<double>{-9999, 5, 0.5, 5.5});
  SetHisto("h_GenWeight", std::vector<double>{-9999, 20000, -10000., 10000.});
  SetHisto("h_LHEDimuonMass", std::vector<double>{-9999, 6000, 0., 6000.});
  SetHisto("h_LHEnMuon", std::vector<double>{-9999, 10, 0., 10.});

  SetHisto("h_nPVGood_Count", std::vector<double>{-9999, 100, 0., 100.});
  SetHisto("h_PileUp_Count_Interaction_before", std::vector<double>{-9999, 100, 0., 100.});
  SetHisto("h_PileUp_Count_Interaction_after", std::vector<double>{-9999, 100, 0., 100.});

  fPtBins = {0,   15,  21,  27,  32,  37,  42,  47,  52,  57, 62,  67,  72,  77,  82,  87,  92,  97,  
            102, 107, 112, 117, 122, 127, 132, 137, 142, 147, 152, 157, 162, 167, 172, 177, 182, 187, 
            192, 197, 202, 207, 212, 217, 222, 227, 232, 237, 242, 247, 252, 257, 262, 267, 272, 277, 
            282, 287, 292, 297, 302, 307, 312, 317, 322, 327, 332, 337, 342, 347, 352, 357, 362, 367, 
            372, 377, 382, 387, 392, 397, 402, 407, 412, 417, 422, 427, 432, 437, 442, 447, 452, 457, 
            462, 467, 472, 477, 482, 487, 492, 497, 500, 501};

  fEtaBins = {-9999, 60, -3., 3.};
  fPhiBins = {-9999, 60, -3.141593, 3.141593};
  fMassBins = {199, 200,  220,  243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 4000, 4001};
  fDeltaRBins = {-9999, 100, 0.0, 3.0};
  fNJetBins = {-9999, 20, 0, 20};

  std::vector<std::string> fAddonMass = {""};
  std::vector<std::string> fAddonJet = {"", "_0J", "_1J", "_mtJ", "_0BJ", "_1BJ", "_mt1BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"};
  
  for (int i = 0; i < fMassBins.size() -1; i++) {
    fAddonMass.push_back("_m" + std::to_string((int)fMassBins[i]) + "_" + std::to_string((int)fMassBins[i+1]));
  }

  for (int i = 0; i < fAddonMass.size(); i++) {
    for (int j = 0; j < fAddonJet.size(); j++) {
      std::string tHistSuffix = fAddonJet[j] + fAddonMass[i];

      SetHisto("h_nJet" + tHistSuffix);    
      SetHisto("h_JetPt" + tHistSuffix);
      SetHisto("h_JetEta" + tHistSuffix);
      SetHisto("h_JetPhi" + tHistSuffix);

      SetHisto("h_nBJet" + tHistSuffix);
      SetHisto("h_BJetPt" + tHistSuffix);
      SetHisto("h_BJetEta" + tHistSuffix);
      SetHisto("h_BJetPhi" + tHistSuffix);

      SetHisto("h_LeadingMuonPt" + tHistSuffix);
      SetHisto("h_LeadingMuonEta" + tHistSuffix);
      SetHisto("h_LeadingMuonPhi" + tHistSuffix);

      SetHisto("h_SubleadingMuonPt" + tHistSuffix);
      SetHisto("h_SubleadingMuonEta" + tHistSuffix);
      SetHisto("h_SubleadingMuonPhi" + tHistSuffix);

      SetHisto("h_MuonPt" + tHistSuffix);
      SetHisto("h_MuonEta" + tHistSuffix);
      SetHisto("h_MuonPhi" + tHistSuffix);
      SetHisto("h_MuonDeltaR" + tHistSuffix, fDeltaRBins);

      SetHisto("h_dimuonMass" + tHistSuffix);
      SetHisto("h_dimuonPt" + tHistSuffix);
      SetHisto("h_dimuonRap" + tHistSuffix, "Eta");
    }
  }

  std::cout << "######################################################################" << std::endl;
  std::cout << "                             Hist setting                             " << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " Total size: " << fHistSet.size() << std::endl;
  std::cout << " Mass binning: " << std::endl;

  for (int i = 0; i < fMassBins.size() - 1; i++)
    std::cout << "    m" << (int)fMassBins[i] << "_" << (int)fMassBins[i+1] << std::endl;

  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;
}

void HistoSet::FillHisto(std::string name, double value, double weight) {

  if (fHistSet.find(name) == fHistSet.end()) {
    std::cout << "Error: HistoSet::FillHisto: Unknown histogram name: " << name << std::endl;
    exit(1);
  }

  fHistSet[name]->Fill(value, weight);
}

void HistoSet::FillHisto(std::string name, float value, double weight) {

  if (fHistSet.find(name) == fHistSet.end()) {
    std::cout << "Error: HistoSet::FillHisto: Unknown histogram name: " << name << std::endl;
    exit(1);
  }

  fHistSet[name]->Fill(value, weight);
}

void HistoSet::FillHisto(std::string name, int value, double weight) {

  if (fHistSet.find(name) == fHistSet.end()) {
    std::cout << "Error: HistoSet::FillHisto: Unknown histogram name: " << name << std::endl;
    exit(1);
  }

  fHistSet[name]->Fill(value, weight);
}

void HistoSet::SetHisto(std::string name, std::vector<double> bins) {

  if (fHistSet.find(name) == fHistSet.end()) {
    if (bins[0] == -9999) fHistSet[name] = new TH1D(name.c_str(), name.c_str(), bins[1], bins[2], bins[3]);
    else                 fHistSet[name] = new TH1D(name.c_str(), name.c_str(), bins.size() - 1, &(bins[0]));
  } else {
    if (bins[0] == -9999) fHistSet[name]->SetBins(bins[1], bins[2], bins[3]);
    else                 fHistSet[name]->SetBins(bins.size() - 1, &(bins[0]));
  }
}

void HistoSet::SetHisto(std::string name, std::vector<double> bins1, std::vector<double> bins2) {

  if (fHistSet2D.find(name) == fHistSet2D.end()) {
    if (bins1[0] == -9999 && bins2[0] == -9999) fHistSet2D[name] = new TH2D(name.c_str(), name.c_str(), bins1[1], bins1[2], bins1[3], bins2[1], bins2[2], bins2[3]);
    else fHistSet2D[name] = new TH2D(name.c_str(), name.c_str(), bins1.size() - 1, &(bins1[0]), bins2.size() - 1, &(bins2[0]));
  } else {
    if (bins1[0] == -9999 && bins2[0] == -9999) fHistSet2D[name]->SetBins(bins1[1], bins1[2], bins1[3], bins2[1], bins2[2], bins2[3]);
    else fHistSet2D[name]->SetBins(bins1.size() - 1, &(bins1[0]), bins2.size() - 1, &(bins2[0]));
  }
}

void HistoSet::SetHisto(std::string name) {
  if (name.find("Pt") != std::string::npos) {
    SetHisto(name, fPtBins);
  }
  else if (name.find("Eta") != std::string::npos) {
    SetHisto(name, fEtaBins);
  }
  else if (name.find("Phi") != std::string::npos) {
    SetHisto(name, fPhiBins);
  }
  else if (name.find("Mass") != std::string::npos) {
    SetHisto(name, fMassBins);
  }
  else if (name.find("DeltaR") != std::string::npos) {
    SetHisto(name, fDeltaRBins);
  }
  else if (name.find("nJet") != std::string::npos || name.find("nBJet") != std::string::npos) {
    SetHisto(name, fNJetBins);
  }
  else {
    std::cout << "Error: HistoSet::SetHisto: Unknown histogram name: " << name << std::endl;
    exit(1);
  }
}

void HistoSet::SetHisto(std::string name, std::string binning) {
  
  if (binning.find("Pt") != std::string::npos) {
    SetHisto(name, fPtBins);
  }
  else if (binning.find("Eta") != std::string::npos) {
    SetHisto(name, fEtaBins);
  }
  else if (binning.find("Phi") != std::string::npos) {
    SetHisto(name, fPhiBins);
  }
  else if (binning.find("Mass") != std::string::npos) {
    SetHisto(name, fMassBins);
  }
  else if (binning.find("DeltaR") != std::string::npos) {
    SetHisto(name, fDeltaRBins);
  }
  else if (binning.find("nJet") != std::string::npos || binning.find("nBJet") != std::string::npos) {
    SetHisto(name, fNJetBins);
  }
  else {
    std::cout << "Error: HistoSet::SetHisto: Unknown histogram name: " << name << " " << binning << std::endl;
    exit(1);
  }
}

void HistoSet::SetHisto(std::string name, std::string binning1, std::string binning2) {

  std::vector<double> binEdge1; 
  if (binning1.find("Pt") != std::string::npos) {
    binEdge1 = fPtBins;
  }
  else if (binning1.find("Eta") != std::string::npos) {
    binEdge1 = fEtaBins;
  }
  else if (binning1.find("Phi") != std::string::npos) {
    binEdge1 = fPhiBins;
  }
  else if (binning1.find("Mass") != std::string::npos) {
    binEdge1 = fMassBins;
  }
  else if (binning1.find("DeltaR") != std::string::npos) {
    binEdge1 = fDeltaRBins;
  }
  else if (binning1.find("nJet") != std::string::npos || binning1.find("nBJet") != std::string::npos) {
    binEdge1 = fNJetBins;
  }
  else {
    std::cout << "Error: HistoSet::SetHisto: Unknown histogram name: " << name << " " << binning1 << std::endl;
    exit(1);
  }

  std::vector<double> binEdge2; 
  if (binning2.find("Pt") != std::string::npos) {
    binEdge2 = fPtBins;
  }
  else if (binning2.find("Eta") != std::string::npos) {
    binEdge2 = fEtaBins;
  }
  else if (binning2.find("Phi") != std::string::npos) {
    binEdge2 = fPhiBins;
  }
  else if (binning2.find("Mass") != std::string::npos) {
    binEdge2 = fMassBins;
  }
  else if (binning2.find("DeltaR") != std::string::npos) {
    binEdge2 = fDeltaRBins;
  }
  else if (binning2.find("nJet") != std::string::npos || binning2.find("nBJet") != std::string::npos) {
    binEdge2 = fNJetBins;
  }
  else {
    std::cout << "Error: HistoSet::SetHisto: Unknown histogram name: " << name << " " << binning2 << std::endl;
    exit(1);
  }

  SetHisto(name, binEdge1, binEdge2);

}

std::string HistoSet::GetMassBin(double fDimuonMass) {
  for (int i = 0; i < fMassBins.size() - 1; i++) {
    if (fDimuonMass >= fMassBins[i] && fDimuonMass < fMassBins[i+1]) {
      return "_m" + std::to_string((int)fMassBins[i]) + "_" + std::to_string((int)fMassBins[i+1]);
    }
  }
  return "";
}

std::string HistoSet::GetJetBin(double fNJet) {
  
  if (fNJet == 0) return "_0J";
  else if (fNJet == 1) return "_1J";
  else if (fNJet >= 2) return "_mtJ";
  else return "";
}

std::string HistoSet::GetBJetBin(double fNBJet) {
  
  if (fNBJet == 0) return "_0BJ";
  else if (fNBJet == 1) return "_1BJ";
  else if (fNBJet >= 2) return "_mt1BJ";
  else return "";
}

std::string HistoSet::GetbVetoJetBin(double fNJet) {
  
  if (fNJet == 0) return "_bVeto_0BJ";
  else if (fNJet == 1) return "_bVeto_1BJ";
  else if (fNJet >= 2) return "_bVeto_mt1BJ";
  else return "";
}

double HistoSet::SetPtOverflow(double fPt) {
  if (fPt > 500) return 500.5;
  else return fPt;
}
double HistoSet::SetMassOverflow(double fMass) {
  if (fMass < 200) return 199.5;
  if (fMass >= 4000) return 4000.5;
  else return fMass;
}

void HistoSet::FillMuon(TLorentzVector& fLeadingMuon, TLorentzVector& fSubleadingMuon, int nJet, int nBJet, double weight) {

  TLorentzVector fDimuon = fLeadingMuon + fSubleadingMuon;

  std::string tMassSuffix = GetMassBin(SetMassOverflow(fDimuon.M()));
  std::string tJetSuffix = GetJetBin(nJet);
  std::string tBJetSuffix = GetBJetBin(nBJet);
  std::string tbVetoJetSuffix = GetbVetoJetBin(nJet);

  std::vector<std::string> tHistSuffix;
  if (nBJet == 0) tHistSuffix = {"", tMassSuffix, tJetSuffix, tJetSuffix + tMassSuffix, tBJetSuffix, tBJetSuffix + tMassSuffix, tbVetoJetSuffix, tbVetoJetSuffix + tMassSuffix};
  else tHistSuffix = {"", tMassSuffix, tJetSuffix, tJetSuffix + tMassSuffix, tBJetSuffix, tBJetSuffix + tMassSuffix};
  
  for (auto suffix : tHistSuffix) {
    fHistSet.at("h_LeadingMuonPt" + suffix)->Fill(SetPtOverflow(fLeadingMuon.Pt()), weight);
    fHistSet.at("h_LeadingMuonEta" + suffix)->Fill(fLeadingMuon.Eta(), weight);
    fHistSet.at("h_LeadingMuonPhi" + suffix)->Fill(fLeadingMuon.Phi(), weight);

    fHistSet.at("h_SubleadingMuonPt" + suffix)->Fill(SetPtOverflow(fSubleadingMuon.Pt()), weight);
    fHistSet.at("h_SubleadingMuonEta" + suffix)->Fill(fSubleadingMuon.Eta(), weight);
    fHistSet.at("h_SubleadingMuonPhi" + suffix)->Fill(fSubleadingMuon.Phi(), weight);

    fHistSet.at("h_MuonPt" + suffix)->Fill(SetPtOverflow(fLeadingMuon.Pt()), weight);
    fHistSet.at("h_MuonEta" + suffix)->Fill(fLeadingMuon.Eta(), weight);
    fHistSet.at("h_MuonPhi" + suffix)->Fill(fLeadingMuon.Phi(), weight);

    fHistSet.at("h_MuonPt" + suffix)->Fill(SetPtOverflow(fSubleadingMuon.Pt()), weight);
    fHistSet.at("h_MuonEta" + suffix)->Fill(fSubleadingMuon.Eta(), weight);
    fHistSet.at("h_MuonPhi" + suffix)->Fill(fSubleadingMuon.Phi(), weight);

    fHistSet.at("h_dimuonMass" + suffix)->Fill(SetMassOverflow(fDimuon.M()), weight);
    fHistSet.at("h_dimuonPt" + suffix)->Fill(SetPtOverflow(fDimuon.Pt()), weight);
    fHistSet.at("h_dimuonRap" + suffix)->Fill(fDimuon.Rapidity(), weight);
  }
}

void HistoSet::FillJet(std::vector<JET::StdJet>* fJet, std::vector<JET::StdJet>* fBJet, double fDimuonMass, double weight) {
  
  std::string tMassSuffix = GetMassBin(SetMassOverflow(fDimuonMass));
  std::string tJetSuffix = GetJetBin(fJet->size());
  std::string tBJetSuffix = GetBJetBin(fBJet->size());
  std::string tbVetoJetSuffix = GetbVetoJetBin(fJet->size());

  std::vector<std::string> tHistSuffix;
  if (fBJet->size() == 0) tHistSuffix = {"", tMassSuffix, tJetSuffix, tJetSuffix + tMassSuffix, tBJetSuffix, tBJetSuffix + tMassSuffix, tbVetoJetSuffix, tbVetoJetSuffix + tMassSuffix};
  else tHistSuffix = {"", tMassSuffix, tJetSuffix, tJetSuffix + tMassSuffix, tBJetSuffix, tBJetSuffix + tMassSuffix};

  for (auto suffix : tHistSuffix) {

    fHistSet.at("h_nJet" + suffix)->Fill(fJet->size(), weight);
    for (int i = 0; i < fJet->size(); i++) {
      fHistSet.at("h_JetPt" + suffix)->Fill(SetPtOverflow(fJet->at(i).fVec.Pt()), weight);
      fHistSet.at("h_JetEta" + suffix)->Fill(fJet->at(i).fVec.Eta(), weight);
      fHistSet.at("h_JetPhi" + suffix)->Fill(fJet->at(i).fVec.Phi(), weight);
    }

    fHistSet.at("h_nBJet" + suffix)->Fill(fBJet->size(), weight);
    for (int i = 0; i < fBJet->size(); i++) {
      fHistSet.at("h_BJetPt" + suffix)->Fill(SetPtOverflow(fBJet->at(i).fVec.Pt()), weight);
      fHistSet.at("h_BJetEta" + suffix)->Fill(fBJet->at(i).fVec.Eta(), weight);
      fHistSet.at("h_BJetPhi" + suffix)->Fill(fBJet->at(i).fVec.Phi(), weight);
    }
  }
}

void HistoSet::WriteHisto(TString fOutputDir) {

  TFile* fOutputFile = new TFile(fOutputDir, "RECREATE");
  
  fOutputFile->cd();
  for (auto [name, hist] : fHistSet) {
    hist->SetDirectory(fOutputFile);
    hist->Write();
  }
  for (auto [name, hist] : fHistSet2D) {
    hist->SetDirectory(fOutputFile);
    hist->Write();
  }

  fOutputFile->Close();
}

