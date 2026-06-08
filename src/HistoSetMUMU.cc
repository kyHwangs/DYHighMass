#include "HistoSetMUMU.h"
#include "jet.h"

#include "TFile.h"
#include "TLorentzVector.h"
#include "TH1.h"


void HistoSetMUMU::Init() {

  TH1::SetDefaultSumw2();

  SetHisto("h_EventInfo", std::vector<double>{-9999, 5, 0.5, 5.5});
  SetHisto("h_GenWeight", std::vector<double>{-9999, 20000, -10000., 10000.});
  SetHisto("h_LHEDimuonMass", std::vector<double>{-9999, 6000, 0., 6000.});
  SetHisto("h_LHEnMuon", std::vector<double>{-9999, 10, 0., 10.});

  SetHisto("h_nPVGood_Count", std::vector<double>{-9999, 100, 0., 100.});
  SetHisto("h_PileUp_Count_Interaction_before", std::vector<double>{-9999, 100, 0., 100.});
  SetHisto("h_PileUp_Count_Interaction_after", std::vector<double>{-9999, 100, 0., 100.});

  fPtBins = {-9999, 300, 0., 1500.};
  fEtaBins = {-9999, 60, -3., 3.};
  fPhiBins = {-9999, 60, -3.141593, 3.141593};
  fMassBins = {199, 200,  220,  243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 4000, 4001};
  fDeltaRBins = {-9999, 100, 0.0, 6.0};
  fNJetBins = {-9999, 20, 0, 20};
  fChargeBins = {-9999, 2, -1, 1};

  std::vector<std::string> fAddonMass = {""};
  std::vector<std::string> fAddonJet = {"", "_0J", "_1J", "_mt1J", "_0BJ", "_1BJ", "_mt1BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"};
  std::vector<std::string> fAddonType = {"OS", "SS", "OS_inverted", "SS_inverted"};
  
  for (int i = 0; i < fMassBins.size() -1; i++) {
    fAddonMass.push_back("_m" + std::to_string((int)fMassBins[i]) + "_" + std::to_string((int)fMassBins[i+1]));
  }

  for (int i = 0; i < fAddonMass.size(); i++) {
    for (int j = 0; j < fAddonJet.size(); j++) {
      std::string tHistSuffix = fAddonJet[j] + fAddonMass[i];
      fSuffix.push_back(tHistSuffix);

      for (auto fType : fAddonType) {

        SetHisto("h_" + fType + "_nJet" + tHistSuffix);    
        SetHisto("h_" + fType + "_JetPt" + tHistSuffix);
        SetHisto("h_" + fType + "_JetEta" + tHistSuffix);
        SetHisto("h_" + fType + "_JetPhi" + tHistSuffix);

        SetHisto("h_" + fType + "_nBJet" + tHistSuffix);
        SetHisto("h_" + fType + "_BJetPt" + tHistSuffix);
        SetHisto("h_" + fType + "_BJetEta" + tHistSuffix);
        SetHisto("h_" + fType + "_BJetPhi" + tHistSuffix);

        SetHisto("h_" + fType + "_LeadingMuonPt" + tHistSuffix);
        SetHisto("h_" + fType + "_LeadingMuonEta" + tHistSuffix);
        SetHisto("h_" + fType + "_LeadingMuonPhi" + tHistSuffix);

        SetHisto("h_" + fType + "_SubleadingMuonPt" + tHistSuffix);
        SetHisto("h_" + fType + "_SubleadingMuonEta" + tHistSuffix);
        SetHisto("h_" + fType + "_SubleadingMuonPhi" + tHistSuffix);

        SetHisto("h_" + fType + "_MuonPt" + tHistSuffix);
        SetHisto("h_" + fType + "_MuonEta" + tHistSuffix);
        SetHisto("h_" + fType + "_MuonPhi" + tHistSuffix);
        SetHisto("h_" + fType + "_MuonCharge" + tHistSuffix, fChargeBins);

        SetHisto("h_" + fType + "_dimuonMass" + tHistSuffix);
        SetHisto("h_" + fType + "_dimuonPt" + tHistSuffix);
        SetHisto("h_" + fType + "_dimuonRap" + tHistSuffix, "Eta");
      }
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

void HistoSetMUMU::FillHisto(std::string name, double value, double weight) {

  if (fHistSet.find(name) == fHistSet.end()) {
    std::cout << "Error: HistoSetMUMU::FillHisto: Unknown histogram name: " << name << std::endl;
    exit(1);
  }

  fHistSet[name]->Fill(value, weight);
}

void HistoSetMUMU::FillHisto(std::string name, float value, double weight) {

  if (fHistSet.find(name) == fHistSet.end()) {
    std::cout << "Error: HistoSetMUMU::FillHisto: Unknown histogram name: " << name << std::endl;
    exit(1);
  }

  fHistSet[name]->Fill(value, weight);
}

void HistoSetMUMU::FillHisto(std::string name, int value, double weight) {

  if (fHistSet.find(name) == fHistSet.end()) {
    std::cout << "Error: HistoSetMUMU::FillHisto: Unknown histogram name: " << name << std::endl;
    exit(1);
  }

  fHistSet[name]->Fill(value, weight);
}

void HistoSetMUMU::SetHisto(std::string name, std::vector<double> bins) {

  if (fHistSet.find(name) == fHistSet.end()) {
    if (bins[0] == -9999) fHistSet[name] = new TH1D(name.c_str(), name.c_str(), bins[1], bins[2], bins[3]);
    else                 fHistSet[name] = new TH1D(name.c_str(), name.c_str(), bins.size() - 1, &(bins[0]));
  } else {
    if (bins[0] == -9999) fHistSet[name]->SetBins(bins[1], bins[2], bins[3]);
    else                 fHistSet[name]->SetBins(bins.size() - 1, &(bins[0]));
  }
}

void HistoSetMUMU::SetHisto(std::string name, std::vector<double> bins1, std::vector<double> bins2) {

  if (fHistSet2D.find(name) == fHistSet2D.end()) {
    if (bins1[0] == -9999 && bins2[0] == -9999) fHistSet2D[name] = new TH2D(name.c_str(), name.c_str(), bins1[1], bins1[2], bins1[3], bins2[1], bins2[2], bins2[3]);
    else fHistSet2D[name] = new TH2D(name.c_str(), name.c_str(), bins1.size() - 1, &(bins1[0]), bins2.size() - 1, &(bins2[0]));
  } else {
    if (bins1[0] == -9999 && bins2[0] == -9999) fHistSet2D[name]->SetBins(bins1[1], bins1[2], bins1[3], bins2[1], bins2[2], bins2[3]);
    else fHistSet2D[name]->SetBins(bins1.size() - 1, &(bins1[0]), bins2.size() - 1, &(bins2[0]));
  }
}

void HistoSetMUMU::SetHisto(std::string name) {
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
    std::cout << "Error: HistoSetMUMU::SetHisto: Unknown histogram name: " << name << std::endl;
    exit(1);
  }
}

void HistoSetMUMU::SetHisto(std::string name, std::string binning) {
  
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
    std::cout << "Error: HistoSetMUMU::SetHisto: Unknown histogram name: " << name << " " << binning << std::endl;
    exit(1);
  }
}

void HistoSetMUMU::SetHisto(std::string name, std::string binning1, std::string binning2) {

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
    std::cout << "Error: HistoSetMUMU::SetHisto: Unknown histogram name: " << name << " " << binning1 << std::endl;
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
    std::cout << "Error: HistoSetMUMU::SetHisto: Unknown histogram name: " << name << " " << binning2 << std::endl;
    exit(1);
  }

  SetHisto(name, binEdge1, binEdge2);

}

std::string HistoSetMUMU::GetMassBin(double fDimuonMass) {
  for (int i = 0; i < fMassBins.size() - 1; i++) {
    if (fDimuonMass >= fMassBins[i] && fDimuonMass < fMassBins[i+1]) {
      return "_m" + std::to_string((int)fMassBins[i]) + "_" + std::to_string((int)fMassBins[i+1]);
    }
  }
  return "";
}

std::string HistoSetMUMU::GetJetBin(double fNJet) {
  
  if (fNJet == 0) return "_0J";
  else if (fNJet == 1) return "_1J";
  else if (fNJet >= 2) return "_mt1J";
  else return "";
}

std::string HistoSetMUMU::GetBJetBin(double fNBJet) {
  
  if (fNBJet == 0) return "_0BJ";
  else if (fNBJet == 1) return "_1BJ";
  else if (fNBJet >= 2) return "_mt1BJ";
  else return "";
}

std::string HistoSetMUMU::GetbVetoJetBin(double fNJet) {
  
  if (fNJet == 0) return "_bVeto_0J";
  else if (fNJet == 1) return "_bVeto_1J";
  else if (fNJet >= 2) return "_bVeto_mt1J";
  else return "";
}

double HistoSetMUMU::SetPtOverflow(double fPt) {
  if (fPt > 1500) return 1510;
  else return fPt;
}
double HistoSetMUMU::SetMassOverflow(double fMass) {
  if (fMass < 200) return 199.5;
  if (fMass >= 4000) return 4000.5;
  else return fMass;
}

void HistoSetMUMU::FillMuon(
  const TLorentzVector& fLeadingMuon, 
  const TLorentzVector& fSubleadingMuon, 
  const float& fCharge, 
  const int& nJet, 
  const int& nBJet, 
  const double& fWeight,
  const std::string& fType
) {

  TLorentzVector fDimuon = fLeadingMuon + fSubleadingMuon;

  std::string tMassSuffix = GetMassBin(SetMassOverflow(fDimuon.M()));
  std::string tJetSuffix = GetJetBin(nJet);
  std::string tBJetSuffix = GetBJetBin(nBJet);
  std::string tbVetoJetSuffix = GetbVetoJetBin(nJet);

  std::vector<std::string> tHistSuffix;
  if (nBJet == 0) tHistSuffix = {"", tMassSuffix, tJetSuffix, tJetSuffix + tMassSuffix, tBJetSuffix, tBJetSuffix + tMassSuffix, tbVetoJetSuffix, tbVetoJetSuffix + tMassSuffix};
  else tHistSuffix = {"", tMassSuffix, tJetSuffix, tJetSuffix + tMassSuffix, tBJetSuffix, tBJetSuffix + tMassSuffix};
  
  for (auto suffix : tHistSuffix) {

    fHistSet["h_" + fType + "_LeadingMuonPt" + suffix]->Fill(SetPtOverflow(fLeadingMuon.Pt()), fWeight);
    fHistSet["h_" + fType + "_LeadingMuonEta" + suffix]->Fill(fLeadingMuon.Eta(), fWeight);
    fHistSet["h_" + fType + "_LeadingMuonPhi" + suffix]->Fill(fLeadingMuon.Phi(), fWeight);

    fHistSet["h_" + fType + "_SubleadingMuonPt" + suffix]->Fill(SetPtOverflow(fSubleadingMuon.Pt()), fWeight);
    fHistSet["h_" + fType + "_SubleadingMuonEta" + suffix]->Fill(fSubleadingMuon.Eta(), fWeight);
    fHistSet["h_" + fType + "_SubleadingMuonPhi" + suffix]->Fill(fSubleadingMuon.Phi(), fWeight);

    fHistSet["h_" + fType + "_MuonPt" + suffix]->Fill(SetPtOverflow(fLeadingMuon.Pt()), fWeight);
    fHistSet["h_" + fType + "_MuonEta" + suffix]->Fill(fLeadingMuon.Eta(), fWeight);
    fHistSet["h_" + fType + "_MuonPhi" + suffix]->Fill(fLeadingMuon.Phi(), fWeight);

    fHistSet["h_" + fType + "_MuonPt" + suffix]->Fill(SetPtOverflow(fSubleadingMuon.Pt()), fWeight);
    fHistSet["h_" + fType + "_MuonEta" + suffix]->Fill(fSubleadingMuon.Eta(), fWeight);
    fHistSet["h_" + fType + "_MuonPhi" + suffix]->Fill(fSubleadingMuon.Phi(), fWeight);
    fHistSet["h_" + fType + "_MuonCharge" + suffix]->Fill(fCharge, fWeight);

    fHistSet["h_" + fType + "_dimuonMass" + suffix]->Fill(SetMassOverflow(fDimuon.M()), fWeight);
    fHistSet["h_" + fType + "_dimuonPt" + suffix]->Fill(SetPtOverflow(fDimuon.Pt()), fWeight);
    fHistSet["h_" + fType + "_dimuonRap" + suffix]->Fill(fDimuon.Rapidity(), fWeight);
  }
}

void HistoSetMUMU::FillJet(
  const std::vector<JET::StdJet>& fJet, 
  const std::vector<JET::StdJet>& fBJet, 
  const double& fDimuonMass, 
  const double& fWeight,
  const std::string& fType
) {
  
  std::string tMassSuffix = GetMassBin(SetMassOverflow(fDimuonMass));
  std::string tJetSuffix = GetJetBin(fJet.size());
  std::string tBJetSuffix = GetBJetBin(fBJet.size());
  std::string tbVetoJetSuffix = GetbVetoJetBin(fJet.size());

  std::vector<std::string> tHistSuffix;
  if (fBJet.size() == 0) tHistSuffix = {"", tMassSuffix, tJetSuffix, tJetSuffix + tMassSuffix, tBJetSuffix, tBJetSuffix + tMassSuffix, tbVetoJetSuffix, tbVetoJetSuffix + tMassSuffix};
  else tHistSuffix = {"", tMassSuffix, tJetSuffix, tJetSuffix + tMassSuffix, tBJetSuffix, tBJetSuffix + tMassSuffix};

  for (auto suffix : tHistSuffix) {

    fHistSet["h_" + fType + "_nJet" + suffix]->Fill(fJet.size(), fWeight);
    for (int i = 0; i < fJet.size(); i++) {
      fHistSet["h_" + fType + "_JetPt" + suffix]->Fill(SetPtOverflow(fJet.at(i).fVec.Pt()), fWeight);
      fHistSet["h_" + fType + "_JetEta" + suffix]->Fill(fJet.at(i).fVec.Eta(), fWeight);
      fHistSet["h_" + fType + "_JetPhi" + suffix]->Fill(fJet.at(i).fVec.Phi(), fWeight);
    }

    fHistSet["h_" + fType + "_nBJet" + suffix]->Fill(fBJet.size(), fWeight);
    for (int i = 0; i < fBJet.size(); i++) {
      fHistSet["h_" + fType + "_BJetPt" + suffix]->Fill(SetPtOverflow(fBJet.at(i).fVec.Pt()), fWeight);
      fHistSet["h_" + fType + "_BJetEta" + suffix]->Fill(fBJet.at(i).fVec.Eta(), fWeight);
      fHistSet["h_" + fType + "_BJetPhi" + suffix]->Fill(fBJet.at(i).fVec.Phi(), fWeight);
    }
  }
}

void HistoSetMUMU::WriteHisto(TString fEra, TString fSampleName, TString fOutputDir, bool fIsData) {

  if (fSampleName.Contains("NNLO_MUMU_10to50"))
    fSampleName = "NNLO_MUMU_10to50";
  
  TFile* fOutputFile = new TFile(fOutputDir, "RECREATE");
  
  fOutputFile->mkdir(fEra + '/' + fSampleName);

  for (auto tSuffix : fSuffix)
    if (tSuffix != "")
      fOutputFile->mkdir(fEra + '/' + fSampleName + '/' + tSuffix);

  fOutputFile->cd(fEra + '/' + fSampleName);
  fHistSet["h_EventInfo"]->Write();
  fHistSet["h_GenWeight"]->Write();
  fHistSet["h_LHEDimuonMass"]->Write();
  fHistSet["h_LHEnMuon"]->Write();
  fHistSet["h_nPVGood_Count"]->Write();
  fHistSet["h_PileUp_Count_Interaction_before"]->Write();
  fHistSet["h_PileUp_Count_Interaction_after"]->Write();

  std::vector<std::string> fPairType = {"OS", "SS", "OS_inverted", "SS_inverted"};
  
  for (const auto& fType : fPairType) {
    for (auto tSuffix : fSuffix) {
      if (tSuffix != "") fOutputFile->cd(fEra + '/' + fSampleName + '/' + tSuffix);
      else fOutputFile->cd(fEra + '/' + fSampleName);
        

      fHistSet["h_" + fType + "_LeadingMuonPt" + tSuffix]->Write();
      fHistSet["h_" + fType + "_LeadingMuonEta" + tSuffix]->Write();
      fHistSet["h_" + fType + "_LeadingMuonPhi" + tSuffix]->Write();

      fHistSet["h_" + fType + "_SubleadingMuonPt" + tSuffix]->Write();
      fHistSet["h_" + fType + "_SubleadingMuonEta" + tSuffix]->Write();
      fHistSet["h_" + fType + "_SubleadingMuonPhi" + tSuffix]->Write();

      fHistSet["h_" + fType + "_MuonPt" + tSuffix]->Write();  
      fHistSet["h_" + fType + "_MuonEta" + tSuffix]->Write();
      fHistSet["h_" + fType + "_MuonPhi" + tSuffix]->Write();
      fHistSet["h_" + fType + "_MuonCharge" + tSuffix]->Write();

      fHistSet["h_" + fType + "_dimuonMass" + tSuffix]->Write();
      fHistSet["h_" + fType + "_dimuonPt" + tSuffix]->Write();
      fHistSet["h_" + fType + "_dimuonRap" + tSuffix]->Write();

      fHistSet["h_" + fType + "_nJet" + tSuffix]->Write();
      fHistSet["h_" + fType + "_JetPt" + tSuffix]->Write();
      fHistSet["h_" + fType + "_JetEta" + tSuffix]->Write();
      fHistSet["h_" + fType + "_JetPhi" + tSuffix]->Write();
      
      fHistSet["h_" + fType + "_nBJet" + tSuffix]->Write();
      fHistSet["h_" + fType + "_BJetPt" + tSuffix]->Write();
      fHistSet["h_" + fType + "_BJetEta" + tSuffix]->Write();
      fHistSet["h_" + fType + "_BJetPhi" + tSuffix]->Write();
    }
  }

  if (fIsData) {
    
    fOutputFile->mkdir(fEra + "/Data");
    fOutputFile->cd(fEra + "/Data");
    
    fHistSet["h_EventInfo"]->Write();
    fHistSet["h_GenWeight"]->Write();
    fHistSet["h_LHEDimuonMass"]->Write();
    fHistSet["h_LHEnMuon"]->Write();
    fHistSet["h_nPVGood_Count"]->Write();
    fHistSet["h_PileUp_Count_Interaction_before"]->Write();
    fHistSet["h_PileUp_Count_Interaction_after"]->Write();

    for (auto tSuffix : fSuffix)
      if (tSuffix != "")
        fOutputFile->mkdir(fEra + "/Data/" + tSuffix);

    for (const auto& fType : fPairType) {
      for (auto tSuffix : fSuffix) {
        if (tSuffix != "") fOutputFile->cd(fEra + "/Data/" + tSuffix);
        else               fOutputFile->cd(fEra + "/Data");

        fHistSet["h_" + fType + "_LeadingMuonPt" + tSuffix]->Write();
        fHistSet["h_" + fType + "_LeadingMuonEta" + tSuffix]->Write();
        fHistSet["h_" + fType + "_LeadingMuonPhi" + tSuffix]->Write();

        fHistSet["h_" + fType + "_SubleadingMuonPt" + tSuffix]->Write();
        fHistSet["h_" + fType + "_SubleadingMuonEta" + tSuffix]->Write();
        fHistSet["h_" + fType + "_SubleadingMuonPhi" + tSuffix]->Write();

        fHistSet["h_" + fType + "_MuonPt" + tSuffix]->Write();  
        fHistSet["h_" + fType + "_MuonEta" + tSuffix]->Write();
        fHistSet["h_" + fType + "_MuonPhi" + tSuffix]->Write();
        fHistSet["h_" + fType + "_MuonCharge" + tSuffix]->Write();

        fHistSet["h_" + fType + "_dimuonMass" + tSuffix]->Write();
        fHistSet["h_" + fType + "_dimuonPt" + tSuffix]->Write();
        fHistSet["h_" + fType + "_dimuonRap" + tSuffix]->Write();

        fHistSet["h_" + fType + "_nJet" + tSuffix]->Write();
        fHistSet["h_" + fType + "_JetPt" + tSuffix]->Write();
        fHistSet["h_" + fType + "_JetEta" + tSuffix]->Write();
        fHistSet["h_" + fType + "_JetPhi" + tSuffix]->Write();
        
        fHistSet["h_" + fType + "_nBJet" + tSuffix]->Write();
        fHistSet["h_" + fType + "_BJetPt" + tSuffix]->Write();
        fHistSet["h_" + fType + "_BJetEta" + tSuffix]->Write();
        fHistSet["h_" + fType + "_BJetPhi" + tSuffix]->Write();
      }
    }
  }

  if (fHistSet2D.size() > 0) {
    fOutputFile->mkdir("Hist2D/" + fEra + '/' + fSampleName);

    for (auto [name, hist] : fHistSet2D) {
      hist->SetDirectory(fOutputFile);
      fOutputFile->cd("Hist2D/" + fEra + '/' + fSampleName);
      hist->Write();
    }
  }

  fOutputFile->Close();
}

