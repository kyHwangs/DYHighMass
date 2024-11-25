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

  TH1D* h_PV_Count_before_corr;
  TH1D* h_PileUp_Count_Interaction_before_corr;
  TH1D* h_PileUp_Count_Intime_before_corr;
  TH1D* h_PV_Count_after_corr;
  TH1D* h_PileUp_Count_Interaction_after_corr;
  TH1D* h_PileUp_Count_Intime_after_corr;

  TH1D* h_LHEDimuonMass;
  TH1D* h_GenWeight;
  TH1D* h_LHEnMuon;
  TH1D* h_nJet_before;
  TH1D* h_nJet_after;
  TH1D* h_nBJet_before;
  TH1D* h_nBJet_after;
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

};

#endif
