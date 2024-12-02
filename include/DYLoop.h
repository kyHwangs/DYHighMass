#ifndef DYLoop_h
#define DYLoop_h

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "muon.h"
#include "NT.h"
#include "options.h"
#include "LumiReWeighting.h"
#include "EfficiencyTable.h"

#include "TFile.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TH1.h"

#include "yaml-cpp/yaml.h"

class DYLoop
{
public:

  DYLoop(options* fOpt) {
    fOpt->GetVariable("config", &fConfigPath);
    fConfig = YAML::LoadFile(std::string(fConfigPath));

    fOpt->GetVariable("id", &fJobID);
    fOpt->GetVariable("era", &fEra);
    fOpt->GetVariable("output", &fOutputDir);
    fOpt->GetVariable("sample", &fSampleName);
    fOpt->GetVariable("base", &fBaseDir);

    fIsMC = false;
    fIsMC = fConfig["Sample"][std::string(fSampleName)]["IsMC"].as<bool>();

    fDoL1Pre = false;
    fDoL1Pre = fConfig["Correction"]["L1PreFiring"].as<bool>();

    fDoID = false;
    fDoID = fConfig["Correction"]["ID"].as<bool>();
    fID_SF = EffTable("../../EfficiencyTable/" + fConfig["Efficiency"]["ID"].as<std::string>());

    fDoISO = false;
    fDoISO = fConfig["Correction"]["ISO"].as<bool>();
    fISO_SF = EffTable("../../EfficiencyTable/" + fConfig["Efficiency"]["ISO"].as<std::string>());

    fDoTRIGG = false;
    fDoTRIGG = fConfig["Correction"]["Trigger"].as<bool>();
    fTRIG_DataEff = EffTable("../../EfficiencyTable/" + fConfig["Efficiency"]["Trigger_data"].as<std::string>());
    fTRIG_MCEff = EffTable("../../EfficiencyTable/" + fConfig["Efficiency"]["Trigger_MC"].as<std::string>());

    fDoPU = false;
    fDoPU = fConfig["Correction"]["PileUp"].as<bool>();
    fPuReweighting = new LumiReWeighting(
      "../../PileupInfo/" + fConfig["Pileup"]["MC"].as<std::string>(),
      "../../PileupInfo/" + fConfig["Pileup"]["Data"].as<std::string>(),
      "pileup",
      "pileup"
    );

    std::cout << fDoID << " " << fConfig["Efficiency"]["ID"].as<std::string>() << std::endl;
    std::cout << fDoISO << " " << fConfig["Efficiency"]["ISO"].as<std::string>() << std::endl;
    std::cout << fDoTRIGG << " " << fConfig["Efficiency"]["Trigger_data"].as<std::string>() << std::endl;
    std::cout << fDoTRIGG << " " << fConfig["Efficiency"]["Trigger_MC"].as<std::string>() << std::endl;
    std::cout << fDoPU << " " << fConfig["Pileup"]["MC"].as<std::string>() << std::endl;
    std::cout << fDoPU << " " << fConfig["Pileup"]["Data"].as<std::string>() << std::endl;
    std::cout << fDoL1Pre << std::endl;

    PrepareHist();

    fNtuples = new NT(fConfig, fIsMC);
    fNtuples->SetSampleName(fSampleName);
    fNtuples->SetEra(fEra);
    fNtuples->AddChain(fSampleName, fJobID);

  }

  void Print() {
    std::cout << "config: " << fConfigPath << std::endl;
    std::cout << "era: " << fEra << std::endl;
    std::cout << "id: " << fJobID << std::endl;
    std::cout << "sample: " << fSampleName << std::endl;
    std::cout << "output: " << fOutputDir << std::endl;
    std::cout << "base: " << fBaseDir << std::endl;
    std::cout << "fIsMC: " << fIsMC << std::endl;
  }

  void SetEra(TString fEra_) { fEra = fEra_; }
  void SetSample(TString fSampleName_) { fSampleName = fSampleName_; }
  void SetJobID(int fJobID_) { fJobID = fJobID_; }
  void SetBase(TString fBaseDir_) { fBaseDir = fBaseDir_; }
  void SetOutputDir(TString fOutputDir_) { fOutputDir = fOutputDir_; }
  void SetMC(bool fIsMC_) { fIsMC = fIsMC_; }
  void SetNT();

  void PrepareHist();
  void EndOfJob();

  void Loop();


private:
  TString fConfigPath;
  TString fEra;
  TString fSampleName;
  int fJobID;
  TString fBaseDir;
  TString fOutputDir;
  bool fIsMC;

  LumiReWeighting* fPuReweighting;
  EffTable fID_SF;
  EffTable fISO_SF;
  EffTable fTRIG_DataEff;
  EffTable fTRIG_MCEff;

  bool fDoID;
  bool fDoISO;
  bool fDoTRIGG;
  bool fDoPU;
  bool fDoL1Pre;

  NT* fNtuples;
  YAML::Node fConfig;

  TH1D* h_EventInfo;

  TH1D* h_nPV_Count;
  TH1D* h_nPVGood_Count;
  TH1D* h_PileUp_Count_Interaction;
  TH1D* h_PileUp_Count_Intime;

  TH1D* h_LHEDimuonMass;
  TH1D* h_GenWeight;
  TH1D* h_LHEnMuon;
  TH1D* h_nJet;
  TH1D* h_nBJet;
  TH1D* h_JetPt;
  TH1D* h_JetEta;
  TH1D* h_JetPhi;
  TH1D* h_BJetPt;
  TH1D* h_BJetEta;
  TH1D* h_BJetPhi;
  TH1D* h_LeadingMuonPt;
  TH1D* h_LeadingMuonEta;
  TH1D* h_LeadingMuonPhi;
  TH1D* h_SubleadingMuonPt;
  TH1D* h_SubleadingMuonEta;
  TH1D* h_SubleadingMuonPhi;
  TH1D* h_MuonPt;
  TH1D* h_MuonEta;
  TH1D* h_MuonPhi;
  TH1D* h_dimuonMass;
  TH1D* h_dimuonMass_wide;
  TH1D* h_dimuonPt;
  TH1D* h_dimuonRap;

  TH1D* h_JetPt_0J;
  TH1D* h_JetEta_0J;
  TH1D* h_JetPhi_0J;
  TH1D* h_BJetPt_0J;
  TH1D* h_BJetEta_0J;
  TH1D* h_BJetPhi_0J;
  TH1D* h_LeadingMuonPt_0J;
  TH1D* h_LeadingMuonEta_0J;
  TH1D* h_LeadingMuonPhi_0J;
  TH1D* h_SubleadingMuonPt_0J;
  TH1D* h_SubleadingMuonEta_0J;
  TH1D* h_SubleadingMuonPhi_0J;
  TH1D* h_MuonPt_0J;
  TH1D* h_MuonEta_0J;
  TH1D* h_MuonPhi_0J;
  TH1D* h_dimuonMass_0J;
  TH1D* h_dimuonMass_wide_0J;
  TH1D* h_dimuonPt_0J;
  TH1D* h_dimuonRap_0J;
  TH1D* h_JetPt_1J;
  TH1D* h_JetEta_1J;
  TH1D* h_JetPhi_1J;
  TH1D* h_BJetPt_1J;
  TH1D* h_BJetEta_1J;
  TH1D* h_BJetPhi_1J;
  TH1D* h_LeadingMuonPt_1J;
  TH1D* h_LeadingMuonEta_1J;
  TH1D* h_LeadingMuonPhi_1J;
  TH1D* h_SubleadingMuonPt_1J;
  TH1D* h_SubleadingMuonEta_1J;
  TH1D* h_SubleadingMuonPhi_1J;
  TH1D* h_MuonPt_1J;
  TH1D* h_MuonEta_1J;
  TH1D* h_MuonPhi_1J;
  TH1D* h_dimuonMass_1J;
  TH1D* h_dimuonMass_wide_1J;
  TH1D* h_dimuonPt_1J;
  TH1D* h_dimuonRap_1J;
  TH1D* h_JetPt_mt1J;
  TH1D* h_JetEta_mt1J;
  TH1D* h_JetPhi_mt1J;
  TH1D* h_BJetPt_mt1J;
  TH1D* h_BJetEta_mt1J;
  TH1D* h_BJetPhi_mt1J;
  TH1D* h_LeadingMuonPt_mt1J;
  TH1D* h_LeadingMuonEta_mt1J;
  TH1D* h_LeadingMuonPhi_mt1J;
  TH1D* h_SubleadingMuonPt_mt1J;
  TH1D* h_SubleadingMuonEta_mt1J;
  TH1D* h_SubleadingMuonPhi_mt1J;
  TH1D* h_MuonPt_mt1J;
  TH1D* h_MuonEta_mt1J;
  TH1D* h_MuonPhi_mt1J;
  TH1D* h_dimuonMass_mt1J;
  TH1D* h_dimuonMass_wide_mt1J;
  TH1D* h_dimuonPt_mt1J;
  TH1D* h_dimuonRap_mt1J;
  TH1D* h_JetPt_0BJ;
  TH1D* h_JetEta_0BJ;
  TH1D* h_JetPhi_0BJ;
  TH1D* h_BJetPt_0BJ;
  TH1D* h_BJetEta_0BJ;
  TH1D* h_BJetPhi_0BJ;
  TH1D* h_LeadingMuonPt_0BJ;
  TH1D* h_LeadingMuonEta_0BJ;
  TH1D* h_LeadingMuonPhi_0BJ;
  TH1D* h_SubleadingMuonPt_0BJ;
  TH1D* h_SubleadingMuonEta_0BJ;
  TH1D* h_SubleadingMuonPhi_0BJ;
  TH1D* h_MuonPt_0BJ;
  TH1D* h_MuonEta_0BJ;
  TH1D* h_MuonPhi_0BJ;
  TH1D* h_dimuonMass_0BJ;
  TH1D* h_dimuonMass_wide_0BJ;
  TH1D* h_dimuonPt_0BJ;
  TH1D* h_dimuonRap_0BJ;
  TH1D* h_JetPt_1BJ;
  TH1D* h_JetEta_1BJ;
  TH1D* h_JetPhi_1BJ;
  TH1D* h_BJetPt_1BJ;
  TH1D* h_BJetEta_1BJ;
  TH1D* h_BJetPhi_1BJ;
  TH1D* h_LeadingMuonPt_1BJ;
  TH1D* h_LeadingMuonEta_1BJ;
  TH1D* h_LeadingMuonPhi_1BJ;
  TH1D* h_SubleadingMuonPt_1BJ;
  TH1D* h_SubleadingMuonEta_1BJ;
  TH1D* h_SubleadingMuonPhi_1BJ;
  TH1D* h_MuonPt_1BJ;
  TH1D* h_MuonEta_1BJ;
  TH1D* h_MuonPhi_1BJ;
  TH1D* h_dimuonMass_1BJ;
  TH1D* h_dimuonMass_wide_1BJ;
  TH1D* h_dimuonPt_1BJ;
  TH1D* h_dimuonRap_1BJ;
  TH1D* h_JetPt_mt1BJ;
  TH1D* h_JetEta_mt1BJ;
  TH1D* h_JetPhi_mt1BJ;
  TH1D* h_BJetPt_mt1BJ;
  TH1D* h_BJetEta_mt1BJ;
  TH1D* h_BJetPhi_mt1BJ;
  TH1D* h_LeadingMuonPt_mt1BJ;
  TH1D* h_LeadingMuonEta_mt1BJ;
  TH1D* h_LeadingMuonPhi_mt1BJ;
  TH1D* h_SubleadingMuonPt_mt1BJ;
  TH1D* h_SubleadingMuonEta_mt1BJ;
  TH1D* h_SubleadingMuonPhi_mt1BJ;
  TH1D* h_MuonPt_mt1BJ;
  TH1D* h_MuonEta_mt1BJ;
  TH1D* h_MuonPhi_mt1BJ;
  TH1D* h_dimuonMass_mt1BJ;
  TH1D* h_dimuonMass_wide_mt1BJ;
  TH1D* h_dimuonPt_mt1BJ;
  TH1D* h_dimuonRap_mt1BJ;
  TH1D* h_JetPt_bVeto_0J;
  TH1D* h_JetEta_bVeto_0J;
  TH1D* h_JetPhi_bVeto_0J;
  TH1D* h_BJetPt_bVeto_0J;
  TH1D* h_BJetEta_bVeto_0J;
  TH1D* h_BJetPhi_bVeto_0J;
  TH1D* h_LeadingMuonPt_bVeto_0J;
  TH1D* h_LeadingMuonEta_bVeto_0J;
  TH1D* h_LeadingMuonPhi_bVeto_0J;
  TH1D* h_SubleadingMuonPt_bVeto_0J;
  TH1D* h_SubleadingMuonEta_bVeto_0J;
  TH1D* h_SubleadingMuonPhi_bVeto_0J;
  TH1D* h_MuonPt_bVeto_0J;
  TH1D* h_MuonEta_bVeto_0J;
  TH1D* h_MuonPhi_bVeto_0J;
  TH1D* h_dimuonMass_bVeto_0J;
  TH1D* h_dimuonMass_wide_bVeto_0J;
  TH1D* h_dimuonPt_bVeto_0J;
  TH1D* h_dimuonRap_bVeto_0J;
  TH1D* h_JetPt_bVeto_1J;
  TH1D* h_JetEta_bVeto_1J;
  TH1D* h_JetPhi_bVeto_1J;
  TH1D* h_BJetPt_bVeto_1J;
  TH1D* h_BJetEta_bVeto_1J;
  TH1D* h_BJetPhi_bVeto_1J;
  TH1D* h_LeadingMuonPt_bVeto_1J;
  TH1D* h_LeadingMuonEta_bVeto_1J;
  TH1D* h_LeadingMuonPhi_bVeto_1J;
  TH1D* h_SubleadingMuonPt_bVeto_1J;
  TH1D* h_SubleadingMuonEta_bVeto_1J;
  TH1D* h_SubleadingMuonPhi_bVeto_1J;
  TH1D* h_MuonPt_bVeto_1J;
  TH1D* h_MuonEta_bVeto_1J;
  TH1D* h_MuonPhi_bVeto_1J;
  TH1D* h_dimuonMass_bVeto_1J;
  TH1D* h_dimuonMass_wide_bVeto_1J;
  TH1D* h_dimuonPt_bVeto_1J;
  TH1D* h_dimuonRap_bVeto_1J;
  TH1D* h_JetPt_bVeto_mt1J;
  TH1D* h_JetEta_bVeto_mt1J;
  TH1D* h_JetPhi_bVeto_mt1J;
  TH1D* h_BJetPt_bVeto_mt1J;
  TH1D* h_BJetEta_bVeto_mt1J;
  TH1D* h_BJetPhi_bVeto_mt1J;
  TH1D* h_LeadingMuonPt_bVeto_mt1J;
  TH1D* h_LeadingMuonEta_bVeto_mt1J;
  TH1D* h_LeadingMuonPhi_bVeto_mt1J;
  TH1D* h_SubleadingMuonPt_bVeto_mt1J;
  TH1D* h_SubleadingMuonEta_bVeto_mt1J;
  TH1D* h_SubleadingMuonPhi_bVeto_mt1J;
  TH1D* h_MuonPt_bVeto_mt1J;
  TH1D* h_MuonEta_bVeto_mt1J;
  TH1D* h_MuonPhi_bVeto_mt1J;
  TH1D* h_dimuonMass_bVeto_mt1J;
  TH1D* h_dimuonMass_wide_bVeto_mt1J;
  TH1D* h_dimuonPt_bVeto_mt1J;
  TH1D* h_dimuonRap_bVeto_mt1J;

};

#endif
