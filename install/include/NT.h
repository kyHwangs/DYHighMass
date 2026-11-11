#ifndef NT_h
#define NT_h 1

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "TChain.h"
#include "TString.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TTreeReaderValue.h"

#include "yaml-cpp/yaml.h"

class NT
{
public:

  NT(std::string fEra_, bool fIsMC_, std::string fSampleName_, int fJobID)
  : fEra(fEra_), fIsMC(fIsMC_), fSampleName(fSampleName_) {

    fMaxEvent = 0;
    fChain = std::make_unique<TChain>("Events");

    AddChain(fJobID);

    init();
  }

  ~NT() {}

  void AddChain(int fJobID) {
    
    YAML::Node fConfigSample = YAML::LoadFile(std::string("../../Input/dataset.yml"));

    std::string fListPath =
        "../../Input/" + fEra + "/" + fConfigSample[fEra][fSampleName]["Name"].as<std::string>() + "/input_" + std::to_string(fJobID) + ".list";

    std::cout << "######################################################################" << std::endl;
    std::cout << " Merging List " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << fListPath << std::endl;


    FILE *fList;
    char fFile[1000];
    fList = fopen(fListPath.c_str(),"r");

    while (fscanf(fList, "%s", fFile) != EOF)
    {
      std::string fFileUpdated = (std::string)fFile;
      std::cout << " -  " << fFileUpdated << std::endl;
      
      fChain->Add((TString)fFileUpdated);
      fMaxEvent = fChain->GetEntries();
    }

    std::cout << "######################################################################" << std::endl;
    std::cout << " " << std::endl;
  }

  void AddChain(TString fFileName) {
    fChain->Add(fFileName);

    fMaxEvent = fChain->GetEntries();
  }

  void AddChain(std::vector<TString> fFileName) {
    for (int i = 0; i < fFileName.size(); i++) {
      fChain->Add(fFileName.at(i));
      fMaxEvent = fChain->GetEntries();
    }
  }

  void init();
  void init_MC();
  void init_LHE();

  TTreeReader* GetTreeReader() { return fTreeReader; }

  bool GetNext() { return fTreeReader->Next(); }

  double GetEntries() {

    return fMaxEvent;
  }

  TTreeReaderValue<unsigned int>* run;
  TTreeReaderValue<unsigned int>* luminosityBlock;
  TTreeReaderValue<unsigned long long>* event;

  TTreeReaderValue<bool>* Flag_HBHENoiseFilter;
  TTreeReaderValue<bool>* Flag_HBHENoiseIsoFilter;
  TTreeReaderValue<bool>* Flag_CSCTightHaloFilter;
  TTreeReaderValue<bool>* Flag_CSCTightHaloTrkMuUnvetoFilter;
  TTreeReaderValue<bool>* Flag_CSCTightHalo2015Filter;
  TTreeReaderValue<bool>* Flag_globalTightHalo2016Filter;
  TTreeReaderValue<bool>* Flag_globalSuperTightHalo2016Filter;
  TTreeReaderValue<bool>* Flag_HcalStripHaloFilter;
  TTreeReaderValue<bool>* Flag_hcalLaserEventFilter;
  TTreeReaderValue<bool>* Flag_EcalDeadCellTriggerPrimitiveFilter;
  TTreeReaderValue<bool>* Flag_EcalDeadCellBoundaryEnergyFilter;
  TTreeReaderValue<bool>* Flag_ecalBadCalibFilter;
  TTreeReaderValue<bool>* Flag_goodVertices;
  TTreeReaderValue<bool>* Flag_eeBadScFilter;
  TTreeReaderValue<bool>* Flag_ecalLaserCorrFilter;
  TTreeReaderValue<bool>* Flag_trkPOGFilters;
  TTreeReaderValue<bool>* Flag_chargedHadronTrackResolutionFilter;
  TTreeReaderValue<bool>* Flag_muonBadTrackFilter;
  TTreeReaderValue<bool>* Flag_BadChargedCandidateFilter;
  TTreeReaderValue<bool>* Flag_BadPFMuonFilter;
  TTreeReaderValue<bool>* Flag_BadPFMuonDzFilter;
  TTreeReaderValue<bool>* Flag_hfNoisyHitsFilter;
  TTreeReaderValue<bool>* Flag_BadChargedCandidateSummer16Filter;
  TTreeReaderValue<bool>* Flag_BadPFMuonSummer16Filter;
  TTreeReaderValue<bool>* Flag_trkPOG_manystripclus53X;
  TTreeReaderValue<bool>* Flag_trkPOG_toomanystripclus53X;
  TTreeReaderValue<bool>* Flag_trkPOG_logErrorTooManyClusters;
  TTreeReaderValue<bool>* Flag_METFilters;

  TTreeReaderValue<bool>* HLT_Mu50;
  TTreeReaderValue<bool>* HLT_TkMu50;
  TTreeReaderValue<bool>* HLT_TkMu100;
  TTreeReaderValue<bool>* HLT_OldMu100;
  TTreeReaderValue<float>* fixedGridRhoFastjetAll;

  // TTreeReaderValue<float>* LHEWeight_originalXWGTUP;
  // TTreeReaderValue<unsigned int>* nLHEPdfWeight;
  // TTreeReaderArray<float>* LHEPdfWeight;
  // TTreeReaderValue<unsigned int>* nLHEReweightingWeight;
  // TTreeReaderArray<float>* LHEReweightingWeight;
  // TTreeReaderValue<unsigned int>* nLHEScaleWeight;
  // TTreeReaderArray<float>* LHEScaleWeight;
  // TTreeReaderValue<unsigned int>* nPSWeight;
  // TTreeReaderArray<float>* PSWeight;
  
  // TTreeReaderValue<float>* LHE_HT;
  // TTreeReaderValue<float>* LHE_HTIncoming;
  // TTreeReaderValue<float>* LHE_Vpt;
  // TTreeReaderValue<float>* LHE_AlphaS;
  // TTreeReaderValue<unsigned char>* LHE_Njets;
  // TTreeReaderValue<unsigned char>* LHE_Nb;
  // TTreeReaderValue<unsigned char>* LHE_Nc;
  // TTreeReaderValue<unsigned char>* LHE_Nuds;
  // TTreeReaderValue<unsigned char>* LHE_Nglu;
  // TTreeReaderValue<unsigned char>* LHE_NpNLO;
  // TTreeReaderValue<unsigned char>* LHE_NpLO;
  TTreeReaderValue<unsigned int>* nLHEPart;
  TTreeReaderArray<float>* LHEPart_pt;
  TTreeReaderArray<float>* LHEPart_eta;
  TTreeReaderArray<float>* LHEPart_phi;
  TTreeReaderArray<float>* LHEPart_mass;
  TTreeReaderArray<int>* LHEPart_pdgId;
  // TTreeReaderArray<float>* LHEPart_incomingpz;
  // TTreeReaderArray<int>* LHEPart_status;
  // TTreeReaderArray<int>* LHEPart_spin;
  
  TTreeReaderValue<float>* Pileup_nTrueInt;
  // TTreeReaderValue<float>* Pileup_pudensity;
  // TTreeReaderValue<float>* Pileup_gpudensity;
  // TTreeReaderValue<int>* Pileup_nPU;
  // TTreeReaderValue<int>* Pileup_sumEOOT;
  // TTreeReaderValue<int>* Pileup_sumLOOT;

  TTreeReaderValue<float>* genWeight;

  TTreeReaderValue<float>* L1PreFiringWeight_Nom;
  TTreeReaderValue<float>* L1PreFiringWeight_Dn;
  TTreeReaderValue<float>* L1PreFiringWeight_Up;

  TTreeReaderValue<unsigned int>* nGenPart;
  TTreeReaderArray<float>* GenPart_eta;
  TTreeReaderArray<float>* GenPart_mass;
  TTreeReaderArray<float>* GenPart_phi;
  TTreeReaderArray<float>* GenPart_pt;
  TTreeReaderArray<int>* GenPart_genPartIdxMother;
  TTreeReaderArray<int>* GenPart_pdgId;
  TTreeReaderArray<int>* GenPart_status;
  TTreeReaderArray<int>* GenPart_statusFlags;

  TTreeReaderValue<unsigned int>* nGenJet;
  TTreeReaderArray<float>* GenJet_pt;
  TTreeReaderArray<float>* GenJet_eta;
  TTreeReaderArray<float>* GenJet_phi;
  TTreeReaderArray<float>* GenJet_mass;

  TTreeReaderValue<unsigned int>* nGenDressedLepton;
  TTreeReaderArray<float>* GenDressedLepton_eta;
  TTreeReaderArray<float>* GenDressedLepton_mass;
  TTreeReaderArray<float>* GenDressedLepton_phi;
  TTreeReaderArray<float>* GenDressedLepton_pt;
  TTreeReaderArray<int>* GenDressedLepton_pdgId;
  TTreeReaderArray<bool>* GenDressedLepton_hasTauAnc;

  TTreeReaderValue<unsigned int>* nMuon;
  TTreeReaderArray<float>* Muon_pt;
  TTreeReaderArray<float>* Muon_tunepRelPt;
  TTreeReaderArray<float>* Muon_eta;
  TTreeReaderArray<float>* Muon_phi;
  TTreeReaderArray<int>* Muon_charge;
  TTreeReaderArray<float>* Muon_mass;
  TTreeReaderArray<unsigned char>* Muon_highPtId;
  TTreeReaderArray<float>* Muon_tkRelIso;
  TTreeReaderArray<bool>* Muon_highPurity;
  // TTreeReaderArray<int>* Muon_nTrackerLayers;
  // TTreeReaderArray<bool>* Muon_mediumId;

  TTreeReaderValue<unsigned int>* nJet;
  TTreeReaderArray<float>* Jet_btagDeepB;
  TTreeReaderArray<float>* Jet_btagDeepFlavB;
  TTreeReaderArray<float>* Jet_eta;
  TTreeReaderArray<float>* Jet_mass;
  TTreeReaderArray<float>* Jet_phi;
  TTreeReaderArray<float>* Jet_pt;
  TTreeReaderArray<float>* Jet_rawFactor;
  TTreeReaderArray<int>* Jet_jetId;
  TTreeReaderArray<int>* Jet_puId;
  TTreeReaderArray<float>* Jet_area;
  TTreeReaderArray<int>* Jet_genJetIdx;
  TTreeReaderArray<int>* Jet_hadronFlavour;


private:

  std::unique_ptr<TChain> fChain;
  TTreeReader* fTreeReader;

  bool fIsMC;
  std::string fEra;
  std::string fSampleName;

  double fMaxEvent;
  std::vector<double> fMaxEventVec;
};

#endif
