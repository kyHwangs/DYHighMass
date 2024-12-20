#ifndef NT_h
#define NT_h

#include <iostream>
#include <map>
#include <string>

#include "muon.h"
#include "electron.h"
#include "jet.h"

#include "TFile.h"
#include "TROOT.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TStopwatch.h"
#include "TChain.h"

#include "yaml-cpp/yaml.h"

class NT
{
public:

  NT(YAML::Node fConfig_, bool fIsMC_)
  : fConfig(fConfig_), fIsMC(fIsMC_) {

    fMaxEvent = 0;
    fMaxEventVec = {};

    fChain = new TChain("Events");
    fEra = fConfig["Info"]["Era"].as<std::string>();

    fMuons = new MUON(fConfig);
    fMuons->IsMC(fIsMC);

    fElecs = new ELEC(fConfig);
    fElecs->IsMC(fIsMC);

    fJets = new JET(fConfig);
    fJets->IsMC(fIsMC);
  }

  ~NT() {

    delete fChain;
  }

  void SetSampleName(TString fSampleName_) { fSampleName = fSampleName_; }
  void SetEra(TString fEra_) { fEra = fEra_; }

  void AddChain(TString fSample, int fJobID) {

    std::string fListPath =
        "../../input/" + (std::string)(fEra.Data()) + "/" + fConfig["Sample"][(std::string)(fSample.Data())]["Name"].as<std::string>() +
        "/input_" + std::to_string(fJobID) + ".list";

    std::cout <<"Merge list: " << fListPath << std::endl;

    FILE *fList;
    char fFile[1000];
    fList = fopen(fListPath.c_str(),"r");

    while (fscanf(fList, "%s", fFile) != EOF)
    {
        fChain->Add(fFile);
        double tMaxEvent = fChain->GetEntries();
        fMaxEvent = tMaxEvent;
        fMaxEventVec.push_back(tMaxEvent);
    }
  }

  void AddChain(TString fFileName) {
    fChain->Add(fFileName);

    fMaxEvent = fChain->GetEntries();
  }

  void AddChain(std::vector<TString> fFileName) {
    for (int i = 0; i < fFileName.size(); i++)
      fChain->Add(fFileName.at(i));

    fMaxEvent = fChain->GetEntries();
  }

  void init();
  void init_MC();
  void init_LHE();
  void init_trigger();
  void init_trigger_2016();
  void init_trigger_2017();
  void init_trigger_2018();

  bool GetNext();

  double GetEntries() {

    return fMaxEvent;
  }

  std::vector<double> GetEntriesVec() {

     return fMaxEventVec;
  }

  void SetMC() {
    fIsMC = true;
    fMuons->IsMC(fIsMC);
  }

  bool PassingTrigger();
  bool PassinNoiseFilter();

  bool PrepareMuon() {

    if (fIsMC) {
      fMuons->PrepareGenMuon(
        **nGenPart,
        GenPart_pt,
        GenPart_eta,
        GenPart_phi,
        GenPart_mass,
        GenPart_pdgId
      );
    }

    return fMuons->PrepareMuon(
      **nMuon,
      Muon_pt,
      Muon_eta,
      Muon_phi,
      Muon_charge,
      Muon_mass,
      Muon_tightId,
      Muon_pfRelIso04_all,
      Muon_nTrackerLayers
    );
  }

  bool PrepareElec() {

    return fElecs->PrepareElec(
      **nElectron,
      Electron_pt,
      Electron_eta,
      Electron_deltaEtaSC,
      Electron_phi,
      Electron_mass,
      Electron_cutBased
    );
  }

  bool PrepareJet(std::vector<MUON::StdMuon> tMuons, std::vector<ELEC::StdElec> tElecs) {

    return fJets->PrepareJet(
      **nJet,
      Jet_pt,
      Jet_eta,
      Jet_phi,
      Jet_mass,
      Jet_jetId,
      Jet_btagCSVV2,
      tMuons,
      tElecs
    );
  }

  bool PrepareJet() {

      return fJets->PrepareJet(
        **nJet,
        Jet_pt,
        Jet_eta,
        Jet_phi,
        Jet_mass,
        Jet_jetId,
        Jet_btagCSVV2,
        std::vector<MUON::StdMuon>{},
        std::vector<ELEC::StdElec>{}
      );
    }

  std::vector<TLorentzVector> GetLHE(int fPID);

  std::vector<MUON::StdMuon> GetMuons() { return fMuons->GetMuons(); }
  std::vector<MUON::StdMuon> GetGenMuons() { return fMuons->GetGenMuons(); }
  MUON::StdMuon GetLeadingMuon() { return fMuons->GetLeadingMuon(); }
  MUON::StdMuon GetSubLeadingMuon() { return fMuons->GetSubLeadingMuon(); }

  std::vector<ELEC::StdElec> GetElecs() { return fElecs->GetElecs(); }

  std::vector<JET::StdJet> GetJets() { return fJets->GetJets(); }
  std::vector<JET::StdJet> GetBJets() { return fJets->GetBJets(); }

  TTreeReaderValue<unsigned int>* run;
  TTreeReaderValue<unsigned int>* luminosityBlock;
  TTreeReaderValue<unsigned long long>* event;
  // TTreeReaderValue<float>* btagWeight_CSVV2;
  // TTreeReaderValue<float>* btagWeight_DeepCSVB;
  // TTreeReaderValue<float>* CaloMET_phi;
  // TTreeReaderValue<float>* CaloMET_pt;
  // TTreeReaderValue<float>* CaloMET_sumEt;
  // TTreeReaderValue<float>* ChsMET_phi;
  // TTreeReaderValue<float>* ChsMET_pt;
  // TTreeReaderValue<float>* ChsMET_sumEt;
  // TTreeReaderValue<unsigned int>* nCorrT1METJet;
  // TTreeReaderArray<float>* CorrT1METJet_area;
  // TTreeReaderArray<float>* CorrT1METJet_eta;
  // TTreeReaderArray<float>* CorrT1METJet_muonSubtrFactor;
  // TTreeReaderArray<float>* CorrT1METJet_phi;
  // TTreeReaderArray<float>* CorrT1METJet_rawPt;
  // TTreeReaderValue<float>* DeepMETResolutionTune_phi;
  // TTreeReaderValue<float>* DeepMETResolutionTune_pt;
  // TTreeReaderValue<float>* DeepMETResponseTune_phi;
  // TTreeReaderValue<float>* DeepMETResponseTune_pt;
  TTreeReaderValue<unsigned int>* nElectron;
  TTreeReaderArray<float>* Electron_dEscaleDown;
  TTreeReaderArray<float>* Electron_dEscaleUp;
  TTreeReaderArray<float>* Electron_dEsigmaDown;
  TTreeReaderArray<float>* Electron_dEsigmaUp;
  TTreeReaderArray<float>* Electron_deltaEtaSC;
  TTreeReaderArray<float>* Electron_dr03EcalRecHitSumEt;
  TTreeReaderArray<float>* Electron_dr03HcalDepth1TowerSumEt;
  TTreeReaderArray<float>* Electron_dr03TkSumPt;
  TTreeReaderArray<float>* Electron_dr03TkSumPtHEEP;
  TTreeReaderArray<float>* Electron_dxy;
  TTreeReaderArray<float>* Electron_dxyErr;
  TTreeReaderArray<float>* Electron_dz;
  TTreeReaderArray<float>* Electron_dzErr;
  TTreeReaderArray<float>* Electron_eCorr;
  TTreeReaderArray<float>* Electron_eInvMinusPInv;
  TTreeReaderArray<float>* Electron_energyErr;
  TTreeReaderArray<float>* Electron_eta;
  TTreeReaderArray<float>* Electron_hoe;
  TTreeReaderArray<float>* Electron_ip3d;
  TTreeReaderArray<float>* Electron_jetPtRelv2;
  TTreeReaderArray<float>* Electron_jetRelIso;
  TTreeReaderArray<float>* Electron_mass;
  TTreeReaderArray<float>* Electron_miniPFRelIso_all;
  TTreeReaderArray<float>* Electron_miniPFRelIso_chg;
  TTreeReaderArray<float>* Electron_mvaFall17V2Iso;
  TTreeReaderArray<float>* Electron_mvaFall17V2noIso;
  TTreeReaderArray<float>* Electron_pfRelIso03_all;
  TTreeReaderArray<float>* Electron_pfRelIso03_chg;
  TTreeReaderArray<float>* Electron_phi;
  TTreeReaderArray<float>* Electron_pt;
  TTreeReaderArray<float>* Electron_r9;
  TTreeReaderArray<float>* Electron_scEtOverPt;
  TTreeReaderArray<float>* Electron_sieie;
  TTreeReaderArray<float>* Electron_sip3d;
  TTreeReaderArray<float>* Electron_mvaTTH;
  TTreeReaderArray<int>* Electron_charge;
  TTreeReaderArray<int>* Electron_cutBased;
  TTreeReaderArray<int>* Electron_jetIdx;
  TTreeReaderArray<int>* Electron_pdgId;
  TTreeReaderArray<int>* Electron_photonIdx;
  TTreeReaderArray<int>* Electron_tightCharge;
  TTreeReaderArray<int>* Electron_vidNestedWPBitmap;
  TTreeReaderArray<int>* Electron_vidNestedWPBitmapHEEP;
  TTreeReaderArray<bool>* Electron_convVeto;
  TTreeReaderArray<bool>* Electron_cutBased_HEEP;
  TTreeReaderArray<bool>* Electron_isPFcand;
  TTreeReaderArray<unsigned char>* Electron_jetNDauCharged;
  TTreeReaderArray<unsigned char>* Electron_lostHits;
  TTreeReaderArray<bool>* Electron_mvaFall17V2Iso_WP80;
  TTreeReaderArray<bool>* Electron_mvaFall17V2Iso_WP90;
  TTreeReaderArray<bool>* Electron_mvaFall17V2Iso_WPL;
  TTreeReaderArray<bool>* Electron_mvaFall17V2noIso_WP80;
  TTreeReaderArray<bool>* Electron_mvaFall17V2noIso_WP90;
  TTreeReaderArray<bool>* Electron_mvaFall17V2noIso_WPL;
  TTreeReaderArray<unsigned char>* Electron_seedGain;
  // TTreeReaderValue<unsigned int>* nFatJet;
  // TTreeReaderArray<float>* FatJet_area;
  // TTreeReaderArray<float>* FatJet_btagCSVV2;
  // TTreeReaderArray<float>* FatJet_btagDDBvLV2;
  // TTreeReaderArray<float>* FatJet_btagDDCvBV2;
  // TTreeReaderArray<float>* FatJet_btagDDCvLV2;
  // TTreeReaderArray<float>* FatJet_btagDeepB;
  // TTreeReaderArray<float>* FatJet_btagHbb;
  // TTreeReaderArray<float>* FatJet_deepTagMD_H4qvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTagMD_HbbvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTagMD_TvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTagMD_WvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTagMD_ZHbbvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTagMD_ZHccvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTagMD_ZbbvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTagMD_ZvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTagMD_bbvsLight;
  // TTreeReaderArray<float>* FatJet_deepTagMD_ccvsLight;
  // TTreeReaderArray<float>* FatJet_deepTag_H;
  // TTreeReaderArray<float>* FatJet_deepTag_QCD;
  // TTreeReaderArray<float>* FatJet_deepTag_QCDothers;
  // TTreeReaderArray<float>* FatJet_deepTag_TvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTag_WvsQCD;
  // TTreeReaderArray<float>* FatJet_deepTag_ZvsQCD;
  // TTreeReaderArray<float>* FatJet_eta;
  // TTreeReaderArray<float>* FatJet_mass;
  // TTreeReaderArray<float>* FatJet_msoftdrop;
  // TTreeReaderArray<float>* FatJet_n2b1;
  // TTreeReaderArray<float>* FatJet_n3b1;
  // TTreeReaderArray<float>* FatJet_particleNetMD_QCD;
  // TTreeReaderArray<float>* FatJet_particleNetMD_Xbb;
  // TTreeReaderArray<float>* FatJet_particleNetMD_Xcc;
  // TTreeReaderArray<float>* FatJet_particleNetMD_Xqq;
  // TTreeReaderArray<float>* FatJet_particleNet_H4qvsQCD;
  // TTreeReaderArray<float>* FatJet_particleNet_HbbvsQCD;
  // TTreeReaderArray<float>* FatJet_particleNet_HccvsQCD;
  // TTreeReaderArray<float>* FatJet_particleNet_QCD;
  // TTreeReaderArray<float>* FatJet_particleNet_TvsQCD;
  // TTreeReaderArray<float>* FatJet_particleNet_WvsQCD;
  // TTreeReaderArray<float>* FatJet_particleNet_ZvsQCD;
  // TTreeReaderArray<float>* FatJet_particleNet_mass;
  // TTreeReaderArray<float>* FatJet_phi;
  // TTreeReaderArray<float>* FatJet_pt;
  // TTreeReaderArray<float>* FatJet_rawFactor;
  // TTreeReaderArray<float>* FatJet_tau1;
  // TTreeReaderArray<float>* FatJet_tau2;
  // TTreeReaderArray<float>* FatJet_tau3;
  // TTreeReaderArray<float>* FatJet_tau4;
  // TTreeReaderArray<float>* FatJet_lsf3;
  // TTreeReaderArray<int>* FatJet_jetId;
  // TTreeReaderArray<int>* FatJet_subJetIdx1;
  // TTreeReaderArray<int>* FatJet_subJetIdx2;
  // TTreeReaderArray<int>* FatJet_electronIdx3SJ;
  // TTreeReaderArray<int>* FatJet_muonIdx3SJ;
  // TTreeReaderArray<unsigned char>* FatJet_nConstituents;
  // TTreeReaderValue<unsigned int>* nFsrPhoton;
  // TTreeReaderArray<float>* FsrPhoton_dROverEt2;
  // TTreeReaderArray<float>* FsrPhoton_eta;
  // TTreeReaderArray<float>* FsrPhoton_phi;
  // TTreeReaderArray<float>* FsrPhoton_pt;
  // TTreeReaderArray<float>* FsrPhoton_relIso03;
  // TTreeReaderArray<int>* FsrPhoton_muonIdx;
  TTreeReaderValue<unsigned int>* nGenJetAK8;
  TTreeReaderArray<float>* GenJetAK8_eta;
  TTreeReaderArray<float>* GenJetAK8_mass;
  TTreeReaderArray<float>* GenJetAK8_phi;
  TTreeReaderArray<float>* GenJetAK8_pt;
  TTreeReaderValue<unsigned int>* nGenJet;
  TTreeReaderArray<float>* GenJet_eta;
  TTreeReaderArray<float>* GenJet_mass;
  TTreeReaderArray<float>* GenJet_phi;
  TTreeReaderArray<float>* GenJet_pt;
  TTreeReaderValue<unsigned int>* nGenPart;
  TTreeReaderArray<float>* GenPart_eta;
  TTreeReaderArray<float>* GenPart_mass;
  TTreeReaderArray<float>* GenPart_phi;
  TTreeReaderArray<float>* GenPart_pt;
  TTreeReaderArray<int>* GenPart_genPartIdxMother;
  TTreeReaderArray<int>* GenPart_pdgId;
  TTreeReaderArray<int>* GenPart_status;
  TTreeReaderArray<int>* GenPart_statusFlags;
  TTreeReaderValue<float>* Generator_binvar;
  TTreeReaderValue<float>* Generator_scalePDF;
  TTreeReaderValue<float>* Generator_weight;
  TTreeReaderValue<float>* Generator_x1;
  TTreeReaderValue<float>* Generator_x2;
  TTreeReaderValue<float>* Generator_xpdf1;
  TTreeReaderValue<float>* Generator_xpdf2;
  TTreeReaderValue<int>* Generator_id1;
  TTreeReaderValue<int>* Generator_id2;
  TTreeReaderValue<float>* GenVtx_x;
  TTreeReaderValue<float>* GenVtx_y;
  TTreeReaderValue<float>* GenVtx_z;
  TTreeReaderValue<float>* genWeight;
  TTreeReaderValue<float>* LHEWeight_originalXWGTUP;
  TTreeReaderValue<unsigned int>* nLHEPdfWeight;
  TTreeReaderArray<float>* LHEPdfWeight;
  TTreeReaderValue<unsigned int>* nLHEReweightingWeight;
  TTreeReaderArray<float>* LHEReweightingWeight;
  TTreeReaderValue<unsigned int>* nLHEScaleWeight;
  TTreeReaderArray<float>* LHEScaleWeight;
  TTreeReaderValue<unsigned int>* nPSWeight;
  TTreeReaderArray<float>* PSWeight;
  TTreeReaderValue<unsigned int>* nJet;
  TTreeReaderArray<float>* Jet_area;
  TTreeReaderArray<float>* Jet_btagCSVV2;
  TTreeReaderArray<float>* Jet_btagDeepB;
  TTreeReaderArray<float>* Jet_btagDeepCvB;
  TTreeReaderArray<float>* Jet_btagDeepCvL;
  TTreeReaderArray<float>* Jet_btagDeepFlavB;
  TTreeReaderArray<float>* Jet_btagDeepFlavCvB;
  TTreeReaderArray<float>* Jet_btagDeepFlavCvL;
  TTreeReaderArray<float>* Jet_btagDeepFlavQG;
  TTreeReaderArray<float>* Jet_chEmEF;
  TTreeReaderArray<float>* Jet_chFPV0EF;
  TTreeReaderArray<float>* Jet_chHEF;
  TTreeReaderArray<float>* Jet_eta;
  TTreeReaderArray<float>* Jet_hfsigmaEtaEta;
  TTreeReaderArray<float>* Jet_hfsigmaPhiPhi;
  TTreeReaderArray<float>* Jet_mass;
  TTreeReaderArray<float>* Jet_muEF;
  TTreeReaderArray<float>* Jet_muonSubtrFactor;
  TTreeReaderArray<float>* Jet_neEmEF;
  TTreeReaderArray<float>* Jet_neHEF;
  TTreeReaderArray<float>* Jet_phi;
  TTreeReaderArray<float>* Jet_pt;
  TTreeReaderArray<float>* Jet_puIdDisc;
  TTreeReaderArray<float>* Jet_qgl;
  TTreeReaderArray<float>* Jet_rawFactor;
  TTreeReaderArray<float>* Jet_bRegCorr;
  TTreeReaderArray<float>* Jet_bRegRes;
  TTreeReaderArray<float>* Jet_cRegCorr;
  TTreeReaderArray<float>* Jet_cRegRes;
  TTreeReaderArray<int>* Jet_electronIdx1;
  TTreeReaderArray<int>* Jet_electronIdx2;
  TTreeReaderArray<int>* Jet_hfadjacentEtaStripsSize;
  TTreeReaderArray<int>* Jet_hfcentralEtaStripSize;
  TTreeReaderArray<int>* Jet_jetId;
  TTreeReaderArray<int>* Jet_muonIdx1;
  TTreeReaderArray<int>* Jet_muonIdx2;
  TTreeReaderArray<int>* Jet_nElectrons;
  TTreeReaderArray<int>* Jet_nMuons;
  TTreeReaderArray<int>* Jet_puId;
  TTreeReaderArray<unsigned char>* Jet_nConstituents;
  TTreeReaderValue<float>* L1PreFiringWeight_Dn;
  TTreeReaderValue<float>* L1PreFiringWeight_ECAL_Dn;
  TTreeReaderValue<float>* L1PreFiringWeight_ECAL_Nom;
  TTreeReaderValue<float>* L1PreFiringWeight_ECAL_Up;
  TTreeReaderValue<float>* L1PreFiringWeight_Muon_Nom;
  TTreeReaderValue<float>* L1PreFiringWeight_Muon_StatDn;
  TTreeReaderValue<float>* L1PreFiringWeight_Muon_StatUp;
  TTreeReaderValue<float>* L1PreFiringWeight_Muon_SystDn;
  TTreeReaderValue<float>* L1PreFiringWeight_Muon_SystUp;
  TTreeReaderValue<float>* L1PreFiringWeight_Nom;
  TTreeReaderValue<float>* L1PreFiringWeight_Up;
  TTreeReaderValue<float>* LHE_HT;
  TTreeReaderValue<float>* LHE_HTIncoming;
  TTreeReaderValue<float>* LHE_Vpt;
  TTreeReaderValue<float>* LHE_AlphaS;
  TTreeReaderValue<unsigned char>* LHE_Njets;
  TTreeReaderValue<unsigned char>* LHE_Nb;
  TTreeReaderValue<unsigned char>* LHE_Nc;
  TTreeReaderValue<unsigned char>* LHE_Nuds;
  TTreeReaderValue<unsigned char>* LHE_Nglu;
  TTreeReaderValue<unsigned char>* LHE_NpNLO;
  TTreeReaderValue<unsigned char>* LHE_NpLO;
  TTreeReaderValue<unsigned int>* nLHEPart;
  TTreeReaderArray<float>* LHEPart_pt;
  TTreeReaderArray<float>* LHEPart_eta;
  TTreeReaderArray<float>* LHEPart_phi;
  TTreeReaderArray<float>* LHEPart_mass;
  TTreeReaderArray<float>* LHEPart_incomingpz;
  TTreeReaderArray<int>* LHEPart_pdgId;
  TTreeReaderArray<int>* LHEPart_status;
  TTreeReaderArray<int>* LHEPart_spin;
  TTreeReaderValue<float>* GenMET_phi;
  TTreeReaderValue<float>* GenMET_pt;
  // TTreeReaderValue<float>* MET_MetUnclustEnUpDeltaX;
  // TTreeReaderValue<float>* MET_MetUnclustEnUpDeltaY;
  // TTreeReaderValue<float>* MET_covXX;
  // TTreeReaderValue<float>* MET_covXY;
  // TTreeReaderValue<float>* MET_covYY;
  // TTreeReaderValue<float>* MET_phi;
  // TTreeReaderValue<float>* MET_pt;
  // TTreeReaderValue<float>* MET_significance;
  // TTreeReaderValue<float>* MET_sumEt;
  // TTreeReaderValue<float>* MET_sumPtUnclustered;
  TTreeReaderValue<unsigned int>* nMuon;
  TTreeReaderArray<float>* Muon_dxy;
  TTreeReaderArray<float>* Muon_dxyErr;
  TTreeReaderArray<float>* Muon_dxybs;
  TTreeReaderArray<float>* Muon_dz;
  TTreeReaderArray<float>* Muon_dzErr;
  TTreeReaderArray<float>* Muon_eta;
  TTreeReaderArray<float>* Muon_ip3d;
  TTreeReaderArray<float>* Muon_jetPtRelv2;
  TTreeReaderArray<float>* Muon_jetRelIso;
  TTreeReaderArray<float>* Muon_mass;
  TTreeReaderArray<float>* Muon_miniPFRelIso_all;
  TTreeReaderArray<float>* Muon_miniPFRelIso_chg;
  TTreeReaderArray<float>* Muon_pfRelIso03_all;
  TTreeReaderArray<float>* Muon_pfRelIso03_chg;
  TTreeReaderArray<float>* Muon_pfRelIso04_all;
  TTreeReaderArray<float>* Muon_phi;
  TTreeReaderArray<float>* Muon_pt;
  TTreeReaderArray<float>* Muon_ptErr;
  TTreeReaderArray<float>* Muon_segmentComp;
  TTreeReaderArray<float>* Muon_sip3d;
  TTreeReaderArray<float>* Muon_softMva;
  TTreeReaderArray<float>* Muon_tkRelIso;
  TTreeReaderArray<float>* Muon_tunepRelPt;
  TTreeReaderArray<float>* Muon_mvaLowPt;
  TTreeReaderArray<float>* Muon_mvaTTH;
  TTreeReaderArray<int>* Muon_charge;
  TTreeReaderArray<int>* Muon_jetIdx;
  TTreeReaderArray<int>* Muon_nStations;
  TTreeReaderArray<int>* Muon_nTrackerLayers;
  TTreeReaderArray<int>* Muon_pdgId;
  TTreeReaderArray<int>* Muon_tightCharge;
  TTreeReaderArray<int>* Muon_fsrPhotonIdx;
  TTreeReaderArray<unsigned char>* Muon_highPtId;
  TTreeReaderArray<bool>* Muon_highPurity;
  TTreeReaderArray<bool>* Muon_inTimeMuon;
  TTreeReaderArray<bool>* Muon_isGlobal;
  TTreeReaderArray<bool>* Muon_isPFcand;
  TTreeReaderArray<bool>* Muon_isStandalone;
  TTreeReaderArray<bool>* Muon_isTracker;
  TTreeReaderArray<unsigned char>* Muon_jetNDauCharged;
  TTreeReaderArray<bool>* Muon_looseId;
  TTreeReaderArray<bool>* Muon_mediumId;
  TTreeReaderArray<bool>* Muon_mediumPromptId;
  TTreeReaderArray<unsigned char>* Muon_miniIsoId;
  TTreeReaderArray<unsigned char>* Muon_multiIsoId;
  TTreeReaderArray<unsigned char>* Muon_mvaId;
  TTreeReaderArray<unsigned char>* Muon_mvaLowPtId;
  TTreeReaderArray<unsigned char>* Muon_pfIsoId;
  TTreeReaderArray<unsigned char>* Muon_puppiIsoId;
  TTreeReaderArray<bool>* Muon_softId;
  TTreeReaderArray<bool>* Muon_softMvaId;
  TTreeReaderArray<bool>* Muon_tightId;
  TTreeReaderArray<unsigned char>* Muon_tkIsoId;
  TTreeReaderArray<bool>* Muon_triggerIdLoose;
  // TTreeReaderValue<unsigned int>* nPhoton;
  // TTreeReaderArray<float>* Photon_dEscaleDown;
  // TTreeReaderArray<float>* Photon_dEscaleUp;
  // TTreeReaderArray<float>* Photon_dEsigmaDown;
  // TTreeReaderArray<float>* Photon_dEsigmaUp;
  // TTreeReaderArray<float>* Photon_eCorr;
  // TTreeReaderArray<float>* Photon_energyErr;
  // TTreeReaderArray<float>* Photon_eta;
  // TTreeReaderArray<float>* Photon_hoe;
  // TTreeReaderArray<float>* Photon_mass;
  // TTreeReaderArray<float>* Photon_mvaID;
  // TTreeReaderArray<float>* Photon_mvaID_Fall17V1p1;
  // TTreeReaderArray<float>* Photon_pfRelIso03_all;
  // TTreeReaderArray<float>* Photon_pfRelIso03_chg;
  // TTreeReaderArray<float>* Photon_phi;
  // TTreeReaderArray<float>* Photon_pt;
  // TTreeReaderArray<float>* Photon_r9;
  // TTreeReaderArray<float>* Photon_sieie;
  // TTreeReaderArray<int>* Photon_charge;
  // TTreeReaderArray<int>* Photon_cutBased;
  // TTreeReaderArray<int>* Photon_cutBased_Fall17V1Bitmap;
  // TTreeReaderArray<int>* Photon_electronIdx;
  // TTreeReaderArray<int>* Photon_jetIdx;
  // TTreeReaderArray<int>* Photon_pdgId;
  // TTreeReaderArray<int>* Photon_vidNestedWPBitmap;
  // TTreeReaderArray<bool>* Photon_electronVeto;
  // TTreeReaderArray<bool>* Photon_isScEtaEB;
  // TTreeReaderArray<bool>* Photon_isScEtaEE;
  // TTreeReaderArray<bool>* Photon_mvaID_WP80;
  // TTreeReaderArray<bool>* Photon_mvaID_WP90;
  // TTreeReaderArray<bool>* Photon_pixelSeed;
  // TTreeReaderArray<unsigned char>* Photon_seedGain;
  TTreeReaderValue<float>* Pileup_nTrueInt;
  TTreeReaderValue<float>* Pileup_pudensity;
  TTreeReaderValue<float>* Pileup_gpudensity;
  TTreeReaderValue<int>* Pileup_nPU;
  TTreeReaderValue<int>* Pileup_sumEOOT;
  TTreeReaderValue<int>* Pileup_sumLOOT;
  // TTreeReaderValue<float>* PuppiMET_phi;
  // TTreeReaderValue<float>* PuppiMET_phiJERDown;
  // TTreeReaderValue<float>* PuppiMET_phiJERUp;
  // TTreeReaderValue<float>* PuppiMET_phiJESDown;
  // TTreeReaderValue<float>* PuppiMET_phiJESUp;
  // TTreeReaderValue<float>* PuppiMET_phiUnclusteredDown;
  // TTreeReaderValue<float>* PuppiMET_phiUnclusteredUp;
  // TTreeReaderValue<float>* PuppiMET_pt;
  // TTreeReaderValue<float>* PuppiMET_ptJERDown;
  // TTreeReaderValue<float>* PuppiMET_ptJERUp;
  // TTreeReaderValue<float>* PuppiMET_ptJESDown;
  // TTreeReaderValue<float>* PuppiMET_ptJESUp;
  // TTreeReaderValue<float>* PuppiMET_ptUnclusteredDown;
  // TTreeReaderValue<float>* PuppiMET_ptUnclusteredUp;
  // TTreeReaderValue<float>* PuppiMET_sumEt;
  // TTreeReaderValue<float>* RawMET_phi;
  // TTreeReaderValue<float>* RawMET_pt;
  // TTreeReaderValue<float>* RawMET_sumEt;
  // TTreeReaderValue<float>* RawPuppiMET_phi;
  // TTreeReaderValue<float>* RawPuppiMET_pt;
  // TTreeReaderValue<float>* RawPuppiMET_sumEt;
  // TTreeReaderValue<float>* fixedGridRhoFastjetAll;
  // TTreeReaderValue<float>* fixedGridRhoFastjetCentral;
  // TTreeReaderValue<float>* fixedGridRhoFastjetCentralCalo;
  // TTreeReaderValue<float>* fixedGridRhoFastjetCentralChargedPileUp;
  // TTreeReaderValue<float>* fixedGridRhoFastjetCentralNeutral;
  TTreeReaderValue<unsigned int>* nGenDressedLepton;
  TTreeReaderArray<float>* GenDressedLepton_eta;
  TTreeReaderArray<float>* GenDressedLepton_mass;
  TTreeReaderArray<float>* GenDressedLepton_phi;
  TTreeReaderArray<float>* GenDressedLepton_pt;
  TTreeReaderArray<int>* GenDressedLepton_pdgId;
  TTreeReaderArray<bool>* GenDressedLepton_hasTauAnc;
  TTreeReaderValue<float>* TkMET_phi;
  TTreeReaderValue<float>* TkMET_pt;
  TTreeReaderValue<float>* TkMET_sumEt;
  TTreeReaderValue<unsigned int>* nTrigObj;
  TTreeReaderArray<float>* TrigObj_pt;
  TTreeReaderArray<float>* TrigObj_eta;
  TTreeReaderArray<float>* TrigObj_phi;
  TTreeReaderArray<float>* TrigObj_l1pt;
  TTreeReaderArray<float>* TrigObj_l1pt_2;
  TTreeReaderArray<float>* TrigObj_l2pt;
  TTreeReaderArray<int>* TrigObj_id;
  TTreeReaderArray<int>* TrigObj_l1iso;
  TTreeReaderArray<int>* TrigObj_l1charge;
  TTreeReaderArray<int>* TrigObj_filterBits;
  TTreeReaderValue<int>* genTtbarId;
  // TTreeReaderValue<unsigned int>* nOtherPV;
  // TTreeReaderArray<float>* OtherPV_z;
  // TTreeReaderValue<float>* PV_ndof;
  // TTreeReaderValue<float>* PV_x;
  // TTreeReaderValue<float>* PV_y;
  // TTreeReaderValue<float>* PV_z;
  // TTreeReaderValue<float>* PV_chi2;
  // TTreeReaderValue<float>* PV_score;
  TTreeReaderValue<int>* PV_npvs;
  TTreeReaderValue<int>* PV_npvsGood;
  // TTreeReaderValue<unsigned int>* nSV;
  // TTreeReaderArray<float>* SV_dlen;
  // TTreeReaderArray<float>* SV_dlenSig;
  // TTreeReaderArray<float>* SV_dxy;
  // TTreeReaderArray<float>* SV_dxySig;
  // TTreeReaderArray<float>* SV_pAngle;
  // TTreeReaderArray<int>* SV_charge;
  // TTreeReaderArray<int>* Electron_genPartIdx;
  // TTreeReaderArray<unsigned char>* Electron_genPartFlav;
  // TTreeReaderArray<int>* FatJet_genJetAK8Idx;
  // TTreeReaderArray<int>* FatJet_hadronFlavour;
  // TTreeReaderArray<unsigned char>* FatJet_nBHadrons;
  // TTreeReaderArray<unsigned char>* FatJet_nCHadrons;
  // TTreeReaderArray<int>* GenJetAK8_partonFlavour;
  // TTreeReaderArray<unsigned char>* GenJetAK8_hadronFlavour;
  // TTreeReaderArray<int>* GenJet_partonFlavour;
  // TTreeReaderArray<unsigned char>* GenJet_hadronFlavour;
  // TTreeReaderValue<float>* GenVtx_t0;
  // TTreeReaderArray<int>* Jet_genJetIdx;
  // TTreeReaderArray<int>* Jet_hadronFlavour;
  // TTreeReaderArray<int>* Jet_partonFlavour;
  // TTreeReaderArray<int>* Muon_genPartIdx;
  // TTreeReaderArray<unsigned char>* Muon_genPartFlav;
  // TTreeReaderArray<int>* Photon_genPartIdx;
  // TTreeReaderArray<unsigned char>* Photon_genPartFlav;
  // TTreeReaderValue<float>* MET_fiducialGenPhi;
  // TTreeReaderValue<float>* MET_fiducialGenPt;
  // TTreeReaderArray<unsigned char>* Electron_cleanmask;
  // TTreeReaderArray<unsigned char>* Jet_cleanmask;
  // TTreeReaderArray<unsigned char>* Muon_cleanmask;
  // TTreeReaderArray<unsigned char>* Photon_cleanmask;
  // TTreeReaderArray<float>* SV_chi2;
  // TTreeReaderArray<float>* SV_eta;
  // TTreeReaderArray<float>* SV_mass;
  // TTreeReaderArray<float>* SV_ndof;
  // TTreeReaderArray<float>* SV_phi;
  // TTreeReaderArray<float>* SV_pt;
  // TTreeReaderArray<float>* SV_x;
  // TTreeReaderArray<float>* SV_y;
  // TTreeReaderArray<float>* SV_z;
  // TTreeReaderArray<unsigned char>* SV_ntracks;
  // TTreeReaderValue<bool>* L1_AlwaysTrue;
  // TTreeReaderValue<bool>* L1_BPTX_AND_Ref1_VME;
  // TTreeReaderValue<bool>* L1_BPTX_AND_Ref3_VME;
  // TTreeReaderValue<bool>* L1_BPTX_AND_Ref4_VME;
  // TTreeReaderValue<bool>* L1_BPTX_BeamGas_B1_VME;
  // TTreeReaderValue<bool>* L1_BPTX_BeamGas_B2_VME;
  // TTreeReaderValue<bool>* L1_BPTX_BeamGas_Ref1_VME;
  // TTreeReaderValue<bool>* L1_BPTX_BeamGas_Ref2_VME;
  // TTreeReaderValue<bool>* L1_BPTX_NotOR_VME;
  // TTreeReaderValue<bool>* L1_BPTX_OR_Ref3_VME;
  // TTreeReaderValue<bool>* L1_BPTX_OR_Ref4_VME;
  // TTreeReaderValue<bool>* L1_BPTX_RefAND_VME;
  // TTreeReaderValue<bool>* L1_BptxMinus;
  // TTreeReaderValue<bool>* L1_BptxOR;
  // TTreeReaderValue<bool>* L1_BptxPlus;
  // TTreeReaderValue<bool>* L1_BptxXOR;
  // TTreeReaderValue<bool>* L1_CDC_SingleMu_3_er1p2_TOP120_DPHI2p618_3p142;
  // TTreeReaderValue<bool>* L1_DoubleEG8er2p5_HTT260er;
  // TTreeReaderValue<bool>* L1_DoubleEG8er2p5_HTT280er;
  // TTreeReaderValue<bool>* L1_DoubleEG8er2p5_HTT300er;
  // TTreeReaderValue<bool>* L1_DoubleEG8er2p5_HTT320er;
  // TTreeReaderValue<bool>* L1_DoubleEG8er2p5_HTT340er;
  // TTreeReaderValue<bool>* L1_DoubleEG_15_10_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleEG_20_10_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleEG_22_10_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleEG_25_12_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleEG_25_14_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleEG_27_14_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleEG_LooseIso20_10_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleEG_LooseIso22_10_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleEG_LooseIso22_12_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleEG_LooseIso25_12_er2p5;
  // TTreeReaderValue<bool>* L1_DoubleIsoTau32er2p1;
  // TTreeReaderValue<bool>* L1_DoubleIsoTau34er2p1;
  // TTreeReaderValue<bool>* L1_DoubleIsoTau36er2p1;
  // TTreeReaderValue<bool>* L1_DoubleJet100er2p3_dEta_Max1p6;
  // TTreeReaderValue<bool>* L1_DoubleJet100er2p5;
  // TTreeReaderValue<bool>* L1_DoubleJet112er2p3_dEta_Max1p6;
  // TTreeReaderValue<bool>* L1_DoubleJet120er2p5;
  // TTreeReaderValue<bool>* L1_DoubleJet150er2p5;
  // TTreeReaderValue<bool>* L1_DoubleJet30er2p5_Mass_Min150_dEta_Max1p5;
  // TTreeReaderValue<bool>* L1_DoubleJet30er2p5_Mass_Min200_dEta_Max1p5;
  // TTreeReaderValue<bool>* L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5;
  // TTreeReaderValue<bool>* L1_DoubleJet30er2p5_Mass_Min300_dEta_Max1p5;
  // TTreeReaderValue<bool>* L1_DoubleJet30er2p5_Mass_Min330_dEta_Max1p5;
  // TTreeReaderValue<bool>* L1_DoubleJet30er2p5_Mass_Min360_dEta_Max1p5;
  // TTreeReaderValue<bool>* L1_DoubleJet35_Mass_Min450_IsoTau45_RmOvlp;
  // TTreeReaderValue<bool>* L1_DoubleJet40er2p5;
  // TTreeReaderValue<bool>* L1_DoubleJet_100_30_DoubleJet30_Mass_Min620;
  // TTreeReaderValue<bool>* L1_DoubleJet_110_35_DoubleJet35_Mass_Min620;
  // TTreeReaderValue<bool>* L1_DoubleJet_115_40_DoubleJet40_Mass_Min620;
  // TTreeReaderValue<bool>* L1_DoubleJet_115_40_DoubleJet40_Mass_Min620_Jet60TT28;
  // TTreeReaderValue<bool>* L1_DoubleJet_120_45_DoubleJet45_Mass_Min620;
  // TTreeReaderValue<bool>* L1_DoubleJet_120_45_DoubleJet45_Mass_Min620_Jet60TT28;
  // TTreeReaderValue<bool>* L1_DoubleJet_80_30_Mass_Min420_DoubleMu0_SQ;
  // TTreeReaderValue<bool>* L1_DoubleJet_80_30_Mass_Min420_IsoTau40_RmOvlp;
  // TTreeReaderValue<bool>* L1_DoubleJet_80_30_Mass_Min420_Mu8;
  // TTreeReaderValue<bool>* L1_DoubleJet_90_30_DoubleJet30_Mass_Min620;
  // TTreeReaderValue<bool>* L1_DoubleLooseIsoEG22er2p1;
  // TTreeReaderValue<bool>* L1_DoubleLooseIsoEG24er2p1;
  // TTreeReaderValue<bool>* L1_DoubleMu0;
  // TTreeReaderValue<bool>* L1_DoubleMu0_Mass_Min1;
  // TTreeReaderValue<bool>* L1_DoubleMu0_OQ;
  // TTreeReaderValue<bool>* L1_DoubleMu0_SQ;
  // TTreeReaderValue<bool>* L1_DoubleMu0_SQ_OS;
  // TTreeReaderValue<bool>* L1_DoubleMu0_dR_Max1p6_Jet90er2p5_dR_Max0p8;
  // TTreeReaderValue<bool>* L1_DoubleMu0er1p4_SQ_OS_dR_Max1p4;
  // TTreeReaderValue<bool>* L1_DoubleMu0er1p5_SQ;
  // TTreeReaderValue<bool>* L1_DoubleMu0er1p5_SQ_OS;
  // TTreeReaderValue<bool>* L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4;
  // TTreeReaderValue<bool>* L1_DoubleMu0er1p5_SQ_dR_Max1p4;
  // TTreeReaderValue<bool>* L1_DoubleMu0er2p0_SQ_OS_dR_Max1p4;
  // TTreeReaderValue<bool>* L1_DoubleMu0er2p0_SQ_dR_Max1p4;
  // TTreeReaderValue<bool>* L1_DoubleMu10_SQ;
  // TTreeReaderValue<bool>* L1_DoubleMu18er2p1;
  // TTreeReaderValue<bool>* L1_DoubleMu3_OS_DoubleEG7p5Upsilon;
  // TTreeReaderValue<bool>* L1_DoubleMu3_SQ_ETMHF50_HTT60er;
  // TTreeReaderValue<bool>* L1_DoubleMu3_SQ_ETMHF50_Jet60er2p5;
  // TTreeReaderValue<bool>* L1_DoubleMu3_SQ_ETMHF50_Jet60er2p5_OR_DoubleJet40er2p5;
  // TTreeReaderValue<bool>* L1_DoubleMu3_SQ_ETMHF60_Jet60er2p5;
  // TTreeReaderValue<bool>* L1_DoubleMu3_SQ_HTT220er;
  // TTreeReaderValue<bool>* L1_DoubleMu3_SQ_HTT240er;
  // TTreeReaderValue<bool>* L1_DoubleMu3_SQ_HTT260er;
  // TTreeReaderValue<bool>* L1_DoubleMu3_dR_Max1p6_Jet90er2p5_dR_Max0p8;
  // TTreeReaderValue<bool>* L1_DoubleMu4_SQ_EG9er2p5;
  // TTreeReaderValue<bool>* L1_DoubleMu4_SQ_OS;
  // TTreeReaderValue<bool>* L1_DoubleMu4_SQ_OS_dR_Max1p2;
  // TTreeReaderValue<bool>* L1_DoubleMu4p5_SQ_OS;
  // TTreeReaderValue<bool>* L1_DoubleMu4p5_SQ_OS_dR_Max1p2;
  // TTreeReaderValue<bool>* L1_DoubleMu4p5er2p0_SQ_OS;
  // TTreeReaderValue<bool>* L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18;
  // TTreeReaderValue<bool>* L1_DoubleMu5Upsilon_OS_DoubleEG3;
  // TTreeReaderValue<bool>* L1_DoubleMu5_SQ_EG9er2p5;
  // TTreeReaderValue<bool>* L1_DoubleMu9_SQ;
  // TTreeReaderValue<bool>* L1_DoubleMu_12_5;
  // TTreeReaderValue<bool>* L1_DoubleMu_15_5_SQ;
  // TTreeReaderValue<bool>* L1_DoubleMu_15_7;
  // TTreeReaderValue<bool>* L1_DoubleMu_15_7_Mass_Min1;
  // TTreeReaderValue<bool>* L1_DoubleMu_15_7_SQ;
  // TTreeReaderValue<bool>* L1_DoubleTau70er2p1;
  // TTreeReaderValue<bool>* L1_ETM120;
  // TTreeReaderValue<bool>* L1_ETM150;
  // TTreeReaderValue<bool>* L1_ETMHF100;
  // TTreeReaderValue<bool>* L1_ETMHF100_HTT60er;
  // TTreeReaderValue<bool>* L1_ETMHF110;
  // TTreeReaderValue<bool>* L1_ETMHF110_HTT60er;
  // TTreeReaderValue<bool>* L1_ETMHF110_HTT60er_NotSecondBunchInTrain;
  // TTreeReaderValue<bool>* L1_ETMHF120;
  // TTreeReaderValue<bool>* L1_ETMHF120_HTT60er;
  // TTreeReaderValue<bool>* L1_ETMHF120_NotSecondBunchInTrain;
  // TTreeReaderValue<bool>* L1_ETMHF130;
  // TTreeReaderValue<bool>* L1_ETMHF130_HTT60er;
  // TTreeReaderValue<bool>* L1_ETMHF140;
  // TTreeReaderValue<bool>* L1_ETMHF150;
  // TTreeReaderValue<bool>* L1_ETMHF90_HTT60er;
  // TTreeReaderValue<bool>* L1_ETT1200;
  // TTreeReaderValue<bool>* L1_ETT1600;
  // TTreeReaderValue<bool>* L1_ETT2000;
  // TTreeReaderValue<bool>* L1_FirstBunchAfterTrain;
  // TTreeReaderValue<bool>* L1_FirstBunchBeforeTrain;
  // TTreeReaderValue<bool>* L1_FirstBunchInTrain;
  // TTreeReaderValue<bool>* L1_FirstCollisionInOrbit;
  // TTreeReaderValue<bool>* L1_FirstCollisionInTrain;
  // TTreeReaderValue<bool>* L1_HCAL_LaserMon_Trig;
  // TTreeReaderValue<bool>* L1_HCAL_LaserMon_Veto;
  // TTreeReaderValue<bool>* L1_HTT120er;
  // TTreeReaderValue<bool>* L1_HTT160er;
  // TTreeReaderValue<bool>* L1_HTT200er;
  // TTreeReaderValue<bool>* L1_HTT255er;
  // TTreeReaderValue<bool>* L1_HTT280er;
  // TTreeReaderValue<bool>* L1_HTT280er_QuadJet_70_55_40_35_er2p4;
  // TTreeReaderValue<bool>* L1_HTT320er;
  // TTreeReaderValue<bool>* L1_HTT320er_QuadJet_70_55_40_40_er2p4;
  // TTreeReaderValue<bool>* L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3;
  // TTreeReaderValue<bool>* L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3;
  // TTreeReaderValue<bool>* L1_HTT360er;
  // TTreeReaderValue<bool>* L1_HTT400er;
  // TTreeReaderValue<bool>* L1_HTT450er;
  // TTreeReaderValue<bool>* L1_IsoEG32er2p5_Mt40;
  // TTreeReaderValue<bool>* L1_IsoEG32er2p5_Mt44;
  // TTreeReaderValue<bool>* L1_IsoEG32er2p5_Mt48;
  // TTreeReaderValue<bool>* L1_IsoTau40er2p1_ETMHF100;
  // TTreeReaderValue<bool>* L1_IsoTau40er2p1_ETMHF110;
  // TTreeReaderValue<bool>* L1_IsoTau40er2p1_ETMHF120;
  // TTreeReaderValue<bool>* L1_IsoTau40er2p1_ETMHF90;
  // TTreeReaderValue<bool>* L1_IsolatedBunch;
  // TTreeReaderValue<bool>* L1_LastBunchInTrain;
  // TTreeReaderValue<bool>* L1_LastCollisionInTrain;
  // TTreeReaderValue<bool>* L1_LooseIsoEG22er2p1_IsoTau26er2p1_dR_Min0p3;
  // TTreeReaderValue<bool>* L1_LooseIsoEG22er2p1_Tau70er2p1_dR_Min0p3;
  // TTreeReaderValue<bool>* L1_LooseIsoEG24er2p1_HTT100er;
  // TTreeReaderValue<bool>* L1_LooseIsoEG24er2p1_IsoTau27er2p1_dR_Min0p3;
  // TTreeReaderValue<bool>* L1_LooseIsoEG26er2p1_HTT100er;
  // TTreeReaderValue<bool>* L1_LooseIsoEG26er2p1_Jet34er2p5_dR_Min0p3;
  // TTreeReaderValue<bool>* L1_LooseIsoEG28er2p1_HTT100er;
  // TTreeReaderValue<bool>* L1_LooseIsoEG28er2p1_Jet34er2p5_dR_Min0p3;
  // TTreeReaderValue<bool>* L1_LooseIsoEG30er2p1_HTT100er;
  // TTreeReaderValue<bool>* L1_LooseIsoEG30er2p1_Jet34er2p5_dR_Min0p3;
  // TTreeReaderValue<bool>* L1_MinimumBiasHF0_AND_BptxAND;
  // TTreeReaderValue<bool>* L1_Mu10er2p3_Jet32er2p3_dR_Max0p4_DoubleJet32er2p3_dEta_Max1p6;
  // TTreeReaderValue<bool>* L1_Mu12er2p3_Jet40er2p1_dR_Max0p4_DoubleJet40er2p1_dEta_Max1p6;
  // TTreeReaderValue<bool>* L1_Mu12er2p3_Jet40er2p3_dR_Max0p4_DoubleJet40er2p3_dEta_Max1p6;
  // TTreeReaderValue<bool>* L1_Mu18er2p1_Tau24er2p1;
  // TTreeReaderValue<bool>* L1_Mu18er2p1_Tau26er2p1;
  // TTreeReaderValue<bool>* L1_Mu20_EG10er2p5;
  // TTreeReaderValue<bool>* L1_Mu22er2p1_IsoTau32er2p1;
  // TTreeReaderValue<bool>* L1_Mu22er2p1_IsoTau34er2p1;
  // TTreeReaderValue<bool>* L1_Mu22er2p1_IsoTau36er2p1;
  // TTreeReaderValue<bool>* L1_Mu22er2p1_IsoTau40er2p1;
  // TTreeReaderValue<bool>* L1_Mu22er2p1_Tau70er2p1;
  // TTreeReaderValue<bool>* L1_Mu3_Jet120er2p5_dR_Max0p4;
  // TTreeReaderValue<bool>* L1_Mu3_Jet120er2p5_dR_Max0p8;
  // TTreeReaderValue<bool>* L1_Mu3_Jet16er2p5_dR_Max0p4;
  // TTreeReaderValue<bool>* L1_Mu3_Jet30er2p5;
  // TTreeReaderValue<bool>* L1_Mu3_Jet35er2p5_dR_Max0p4;
  // TTreeReaderValue<bool>* L1_Mu3_Jet60er2p5_dR_Max0p4;
  // TTreeReaderValue<bool>* L1_Mu3_Jet80er2p5_dR_Max0p4;
  // TTreeReaderValue<bool>* L1_Mu3er1p5_Jet100er2p5_ETMHF40;
  // TTreeReaderValue<bool>* L1_Mu3er1p5_Jet100er2p5_ETMHF50;
  // TTreeReaderValue<bool>* L1_Mu5_EG23er2p5;
  // TTreeReaderValue<bool>* L1_Mu5_LooseIsoEG20er2p5;
  // TTreeReaderValue<bool>* L1_Mu6_DoubleEG10er2p5;
  // TTreeReaderValue<bool>* L1_Mu6_DoubleEG12er2p5;
  // TTreeReaderValue<bool>* L1_Mu6_DoubleEG15er2p5;
  // TTreeReaderValue<bool>* L1_Mu6_DoubleEG17er2p5;
  // TTreeReaderValue<bool>* L1_Mu6_HTT240er;
  // TTreeReaderValue<bool>* L1_Mu6_HTT250er;
  // TTreeReaderValue<bool>* L1_Mu7_EG23er2p5;
  // TTreeReaderValue<bool>* L1_Mu7_LooseIsoEG20er2p5;
  // TTreeReaderValue<bool>* L1_Mu7_LooseIsoEG23er2p5;
  // TTreeReaderValue<bool>* L1_NotBptxOR;
  // TTreeReaderValue<bool>* L1_QuadJet36er2p5_IsoTau52er2p1;
  // TTreeReaderValue<bool>* L1_QuadJet60er2p5;
  // TTreeReaderValue<bool>* L1_QuadJet_95_75_65_20_DoubleJet_75_65_er2p5_Jet20_FWD3p0;
  // TTreeReaderValue<bool>* L1_QuadMu0;
  // TTreeReaderValue<bool>* L1_QuadMu0_OQ;
  // TTreeReaderValue<bool>* L1_QuadMu0_SQ;
  // TTreeReaderValue<bool>* L1_SecondBunchInTrain;
  // TTreeReaderValue<bool>* L1_SecondLastBunchInTrain;
  // TTreeReaderValue<bool>* L1_SingleEG10er2p5;
  // TTreeReaderValue<bool>* L1_SingleEG15er2p5;
  // TTreeReaderValue<bool>* L1_SingleEG26er2p5;
  // TTreeReaderValue<bool>* L1_SingleEG34er2p5;
  // TTreeReaderValue<bool>* L1_SingleEG36er2p5;
  // TTreeReaderValue<bool>* L1_SingleEG38er2p5;
  // TTreeReaderValue<bool>* L1_SingleEG40er2p5;
  // TTreeReaderValue<bool>* L1_SingleEG42er2p5;
  // TTreeReaderValue<bool>* L1_SingleEG45er2p5;
  // TTreeReaderValue<bool>* L1_SingleEG50;
  // TTreeReaderValue<bool>* L1_SingleEG60;
  // TTreeReaderValue<bool>* L1_SingleEG8er2p5;
  // TTreeReaderValue<bool>* L1_SingleIsoEG24er1p5;
  // TTreeReaderValue<bool>* L1_SingleIsoEG24er2p1;
  // TTreeReaderValue<bool>* L1_SingleIsoEG26er1p5;
  // TTreeReaderValue<bool>* L1_SingleIsoEG26er2p1;
  // TTreeReaderValue<bool>* L1_SingleIsoEG26er2p5;
  // TTreeReaderValue<bool>* L1_SingleIsoEG28er1p5;
  // TTreeReaderValue<bool>* L1_SingleIsoEG28er2p1;
  // TTreeReaderValue<bool>* L1_SingleIsoEG28er2p5;
  // TTreeReaderValue<bool>* L1_SingleIsoEG30er2p1;
  // TTreeReaderValue<bool>* L1_SingleIsoEG30er2p5;
  // TTreeReaderValue<bool>* L1_SingleIsoEG32er2p1;
  // TTreeReaderValue<bool>* L1_SingleIsoEG32er2p5;
  // TTreeReaderValue<bool>* L1_SingleIsoEG34er2p5;
  // TTreeReaderValue<bool>* L1_SingleJet10erHE;
  // TTreeReaderValue<bool>* L1_SingleJet120;
  // TTreeReaderValue<bool>* L1_SingleJet120_FWD3p0;
  // TTreeReaderValue<bool>* L1_SingleJet120er2p5;
  // TTreeReaderValue<bool>* L1_SingleJet12erHE;
  // TTreeReaderValue<bool>* L1_SingleJet140er2p5;
  // TTreeReaderValue<bool>* L1_SingleJet140er2p5_ETMHF80;
  // TTreeReaderValue<bool>* L1_SingleJet140er2p5_ETMHF90;
  // TTreeReaderValue<bool>* L1_SingleJet160er2p5;
  // TTreeReaderValue<bool>* L1_SingleJet180;
  // TTreeReaderValue<bool>* L1_SingleJet180er2p5;
  // TTreeReaderValue<bool>* L1_SingleJet200;
  // TTreeReaderValue<bool>* L1_SingleJet20er2p5_NotBptxOR;
  // TTreeReaderValue<bool>* L1_SingleJet20er2p5_NotBptxOR_3BX;
  // TTreeReaderValue<bool>* L1_SingleJet35;
  // TTreeReaderValue<bool>* L1_SingleJet35_FWD3p0;
  // TTreeReaderValue<bool>* L1_SingleJet35er2p5;
  // TTreeReaderValue<bool>* L1_SingleJet43er2p5_NotBptxOR_3BX;
  // TTreeReaderValue<bool>* L1_SingleJet46er2p5_NotBptxOR_3BX;
  // TTreeReaderValue<bool>* L1_SingleJet60;
  // TTreeReaderValue<bool>* L1_SingleJet60_FWD3p0;
  // TTreeReaderValue<bool>* L1_SingleJet60er2p5;
  // TTreeReaderValue<bool>* L1_SingleJet8erHE;
  // TTreeReaderValue<bool>* L1_SingleJet90;
  // TTreeReaderValue<bool>* L1_SingleJet90_FWD3p0;
  // TTreeReaderValue<bool>* L1_SingleJet90er2p5;
  // TTreeReaderValue<bool>* L1_SingleLooseIsoEG28er1p5;
  // TTreeReaderValue<bool>* L1_SingleLooseIsoEG30er1p5;
  // TTreeReaderValue<bool>* L1_SingleMu0_BMTF;
  // TTreeReaderValue<bool>* L1_SingleMu0_DQ;
  // TTreeReaderValue<bool>* L1_SingleMu0_EMTF;
  // TTreeReaderValue<bool>* L1_SingleMu0_OMTF;
  // TTreeReaderValue<bool>* L1_SingleMu10er1p5;
  // TTreeReaderValue<bool>* L1_SingleMu12_DQ_BMTF;
  // TTreeReaderValue<bool>* L1_SingleMu12_DQ_EMTF;
  // TTreeReaderValue<bool>* L1_SingleMu12_DQ_OMTF;
  // TTreeReaderValue<bool>* L1_SingleMu12er1p5;
  // TTreeReaderValue<bool>* L1_SingleMu14er1p5;
  // TTreeReaderValue<bool>* L1_SingleMu15_DQ;
  // TTreeReaderValue<bool>* L1_SingleMu16er1p5;
  // TTreeReaderValue<bool>* L1_SingleMu18;
  // TTreeReaderValue<bool>* L1_SingleMu18er1p5;
  // TTreeReaderValue<bool>* L1_SingleMu20;
  // TTreeReaderValue<bool>* L1_SingleMu22;
  // TTreeReaderValue<bool>* L1_SingleMu22_BMTF;
  // TTreeReaderValue<bool>* L1_SingleMu22_EMTF;
  // TTreeReaderValue<bool>* L1_SingleMu22_OMTF;
  // TTreeReaderValue<bool>* L1_SingleMu25;
  // TTreeReaderValue<bool>* L1_SingleMu3;
  // TTreeReaderValue<bool>* L1_SingleMu5;
  // TTreeReaderValue<bool>* L1_SingleMu6er1p5;
  // TTreeReaderValue<bool>* L1_SingleMu7;
  // TTreeReaderValue<bool>* L1_SingleMu7_DQ;
  // TTreeReaderValue<bool>* L1_SingleMu7er1p5;
  // TTreeReaderValue<bool>* L1_SingleMu8er1p5;
  // TTreeReaderValue<bool>* L1_SingleMu9er1p5;
  // TTreeReaderValue<bool>* L1_SingleMuCosmics;
  // TTreeReaderValue<bool>* L1_SingleMuCosmics_BMTF;
  // TTreeReaderValue<bool>* L1_SingleMuCosmics_EMTF;
  // TTreeReaderValue<bool>* L1_SingleMuCosmics_OMTF;
  // TTreeReaderValue<bool>* L1_SingleMuOpen;
  // TTreeReaderValue<bool>* L1_SingleMuOpen_NotBptxOR;
  // TTreeReaderValue<bool>* L1_SingleMuOpen_er1p1_NotBptxOR_3BX;
  // TTreeReaderValue<bool>* L1_SingleMuOpen_er1p4_NotBptxOR_3BX;
  // TTreeReaderValue<bool>* L1_SingleTau120er2p1;
  // TTreeReaderValue<bool>* L1_SingleTau130er2p1;
  // TTreeReaderValue<bool>* L1_TOTEM_1;
  // TTreeReaderValue<bool>* L1_TOTEM_2;
  // TTreeReaderValue<bool>* L1_TOTEM_3;
  // TTreeReaderValue<bool>* L1_TOTEM_4;
  // TTreeReaderValue<bool>* L1_TripleEG16er2p5;
  // TTreeReaderValue<bool>* L1_TripleEG_16_12_8_er2p5;
  // TTreeReaderValue<bool>* L1_TripleEG_16_15_8_er2p5;
  // TTreeReaderValue<bool>* L1_TripleEG_18_17_8_er2p5;
  // TTreeReaderValue<bool>* L1_TripleEG_18_18_12_er2p5;
  // TTreeReaderValue<bool>* L1_TripleJet_100_80_70_DoubleJet_80_70_er2p5;
  // TTreeReaderValue<bool>* L1_TripleJet_105_85_75_DoubleJet_85_75_er2p5;
  // TTreeReaderValue<bool>* L1_TripleJet_95_75_65_DoubleJet_75_65_er2p5;
  // TTreeReaderValue<bool>* L1_TripleMu0;
  // TTreeReaderValue<bool>* L1_TripleMu0_OQ;
  // TTreeReaderValue<bool>* L1_TripleMu0_SQ;
  // TTreeReaderValue<bool>* L1_TripleMu3;
  // TTreeReaderValue<bool>* L1_TripleMu3_SQ;
  // TTreeReaderValue<bool>* L1_TripleMu_5SQ_3SQ_0OQ;
  // TTreeReaderValue<bool>* L1_TripleMu_5SQ_3SQ_0OQ_DoubleMu_5_3_SQ_OS_Mass_Max9;
  // TTreeReaderValue<bool>* L1_TripleMu_5SQ_3SQ_0_DoubleMu_5_3_SQ_OS_Mass_Max9;
  // TTreeReaderValue<bool>* L1_TripleMu_5_3_3;
  // TTreeReaderValue<bool>* L1_TripleMu_5_3_3_SQ;
  // TTreeReaderValue<bool>* L1_TripleMu_5_3p5_2p5;
  // TTreeReaderValue<bool>* L1_TripleMu_5_3p5_2p5_DoubleMu_5_2p5_OS_Mass_5to17;
  // TTreeReaderValue<bool>* L1_TripleMu_5_3p5_2p5_OQ_DoubleMu_5_2p5_OQ_OS_Mass_5to17;
  // TTreeReaderValue<bool>* L1_TripleMu_5_4_2p5_DoubleMu_5_2p5_OS_Mass_5to17;
  // TTreeReaderValue<bool>* L1_TripleMu_5_5_3;
  // TTreeReaderValue<bool>* L1_UnpairedBunchBptxMinus;
  // TTreeReaderValue<bool>* L1_UnpairedBunchBptxPlus;
  // TTreeReaderValue<bool>* L1_ZeroBias;
  // TTreeReaderValue<bool>* L1_ZeroBias_copy;
  // TTreeReaderValue<bool>* L1_UnprefireableEvent;
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
  // TTreeReaderValue<bool>* L1Reco_step;
  // TTreeReaderValue<bool>* HLT_DoubleMu5_Upsilon_DoubleEle3_CaloIdL_TrackIdL;
  // TTreeReaderValue<bool>* HLT_DoubleMu3_DoubleEle7p5_CaloIdL_TrackIdL_Upsilon;
  // TTreeReaderValue<bool>* HLT_Trimuon5_3p5_2_Upsilon_Muon;
  // TTreeReaderValue<bool>* HLT_TrimuonOpen_5_3p5_2_Upsilon_Muon;
  // TTreeReaderValue<bool>* HLT_DoubleEle25_CaloIdL_MW;
  // TTreeReaderValue<bool>* HLT_DoubleEle27_CaloIdL_MW;
  // TTreeReaderValue<bool>* HLT_DoubleEle33_CaloIdL_MW;
  // TTreeReaderValue<bool>* HLT_DoubleEle24_eta2p1_WPTight_Gsf;
  // TTreeReaderValue<bool>* HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350;
  // TTreeReaderValue<bool>* HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT350;
  // TTreeReaderValue<bool>* HLT_Ele27_Ele37_CaloIdL_MW;
  // TTreeReaderValue<bool>* HLT_Mu27_Ele37_CaloIdL_MW;
  // TTreeReaderValue<bool>* HLT_Mu37_Ele27_CaloIdL_MW;
  // TTreeReaderValue<bool>* HLT_Mu37_TkMu27;
  // TTreeReaderValue<bool>* HLT_DoubleMu4_3_Bs;
  // TTreeReaderValue<bool>* HLT_DoubleMu4_3_Jpsi;
  // TTreeReaderValue<bool>* HLT_DoubleMu4_JpsiTrk_Displaced;
  // TTreeReaderValue<bool>* HLT_DoubleMu4_LowMassNonResonantTrk_Displaced;
  // TTreeReaderValue<bool>* HLT_DoubleMu3_Trk_Tau3mu;
  // TTreeReaderValue<bool>* HLT_DoubleMu3_TkMu_DsTau3Mu;
  // TTreeReaderValue<bool>* HLT_DoubleMu4_PsiPrimeTrk_Displaced;
  // TTreeReaderValue<bool>* HLT_DoubleMu4_Mass3p8_DZ_PFHT350;
  // TTreeReaderValue<bool>* HLT_Mu3_PFJet40;
  // TTreeReaderValue<bool>* HLT_Mu7p5_L2Mu2_Jpsi;
  // TTreeReaderValue<bool>* HLT_Mu7p5_L2Mu2_Upsilon;
  // TTreeReaderValue<bool>* HLT_Mu7p5_Track2_Jpsi;
  // TTreeReaderValue<bool>* HLT_Mu7p5_Track3p5_Jpsi;
  // TTreeReaderValue<bool>* HLT_Mu7p5_Track7_Jpsi;
  // TTreeReaderValue<bool>* HLT_Mu7p5_Track2_Upsilon;
  // TTreeReaderValue<bool>* HLT_Mu7p5_Track3p5_Upsilon;
  // TTreeReaderValue<bool>* HLT_Mu7p5_Track7_Upsilon;
  // TTreeReaderValue<bool>* HLT_Mu3_L1SingleMu5orSingleMu7;
  // TTreeReaderValue<bool>* HLT_Ele20_WPTight_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele15_WPLoose_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele17_WPLoose_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele20_WPLoose_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele20_eta2p1_WPLoose_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele27_WPTight_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele28_WPTight_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele30_WPTight_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele32_WPTight_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele35_WPTight_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele35_WPTight_Gsf_L1EGMT;
  // TTreeReaderValue<bool>* HLT_Ele38_WPTight_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele40_WPTight_Gsf;
  // TTreeReaderValue<bool>* HLT_Ele32_WPTight_Gsf_L1DoubleEG;
  // TTreeReaderValue<bool>* HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1;
  // TTreeReaderValue<bool>* HLT_Ele24_eta2p1_WPTight_Gsf_MediumChargedIsoPFTauHPS30_eta2p1_CrossL1;
  // TTreeReaderValue<bool>* HLT_Ele24_eta2p1_WPTight_Gsf_TightChargedIsoPFTauHPS30_eta2p1_CrossL1;
  // TTreeReaderValue<bool>* HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_TightID_CrossL1;
  // TTreeReaderValue<bool>* HLT_Ele24_eta2p1_WPTight_Gsf_MediumChargedIsoPFTauHPS30_eta2p1_TightID_CrossL1;
  // TTreeReaderValue<bool>* HLT_Ele24_eta2p1_WPTight_Gsf_TightChargedIsoPFTauHPS30_eta2p1_TightID_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu20_eta2p1_MediumChargedIsoPFTauHPS27_eta2p1_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu20_eta2p1_TightChargedIsoPFTauHPS27_eta2p1_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_TightID_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu20_eta2p1_MediumChargedIsoPFTauHPS27_eta2p1_TightID_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu20_eta2p1_TightChargedIsoPFTauHPS27_eta2p1_TightID_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu24_eta2p1_TightChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu24_eta2p1_MediumChargedIsoPFTauHPS35_Trk1_TightID_eta2p1_Reg_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu24_eta2p1_TightChargedIsoPFTauHPS35_Trk1_TightID_eta2p1_Reg_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu24_eta2p1_MediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_CrossL1;
  // TTreeReaderValue<bool>* HLT_IsoMu27_LooseChargedIsoPFTauHPS20_Trk1_eta2p1_SingleL1;
  // TTreeReaderValue<bool>* HLT_IsoMu27_MediumChargedIsoPFTauHPS20_Trk1_eta2p1_SingleL1;
  // TTreeReaderValue<bool>* HLT_IsoMu27_TightChargedIsoPFTauHPS20_Trk1_eta2p1_SingleL1;
  // TTreeReaderValue<bool>* HLT_IsoMu20;
  TTreeReaderValue<bool>* HLT_IsoMu24;
  TTreeReaderValue<bool>* HLT_IsoTkMu24;
  // TTreeReaderValue<bool>* HLT_IsoMu24_eta2p1;
  TTreeReaderValue<bool>* HLT_IsoMu27;
  // TTreeReaderValue<bool>* HLT_IsoMu30;
  // TTreeReaderValue<bool>* HLT_L1SingleMu18;
  // TTreeReaderValue<bool>* HLT_L1SingleMu25;
  // TTreeReaderValue<bool>* HLT_L2Mu10;
  // TTreeReaderValue<bool>* HLT_L2Mu10_NoVertex_NoBPTX3BX;
  // TTreeReaderValue<bool>* HLT_L2Mu10_NoVertex_NoBPTX;
  // TTreeReaderValue<bool>* HLT_L2Mu45_NoVertex_3Sta_NoBPTX3BX;
  // TTreeReaderValue<bool>* HLT_L2Mu40_NoVertex_3Sta_NoBPTX3BX;
  // TTreeReaderValue<bool>* HLT_L2Mu50;
  // TTreeReaderValue<bool>* HLT_L2Mu23NoVtx_2Cha;
  // TTreeReaderValue<bool>* HLT_L2Mu23NoVtx_2Cha_CosmicSeed;
  // TTreeReaderValue<bool>* HLT_DoubleL2Mu50;
  // TTreeReaderValue<bool>* HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL;
  // TTreeReaderValue<bool>* HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL;
  // TTreeReaderValue<bool>* HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
  // TTreeReaderValue<bool>* HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ;
  // TTreeReaderValue<bool>* HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
  // TTreeReaderValue<bool>* HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass8;
  // TTreeReaderValue<bool>* HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8;
  // TTreeReaderValue<bool>* HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass3p8;
  // TTreeReaderValue<bool>* HLT_Mu25_TkMu0_Onia;
  // TTreeReaderValue<bool>* HLT_Mu30_TkMu0_Psi;
  // TTreeReaderValue<bool>* HLT_Mu30_TkMu0_Upsilon;
  // TTreeReaderValue<bool>* HLT_Mu20_TkMu0_Phi;
  // TTreeReaderValue<bool>* HLT_Mu25_TkMu0_Phi;
  // TTreeReaderValue<bool>* HLT_Mu12;
  // TTreeReaderValue<bool>* HLT_Mu15;
  // TTreeReaderValue<bool>* HLT_Mu20;
  // TTreeReaderValue<bool>* HLT_Mu27;
  TTreeReaderValue<bool>* HLT_Mu50;
  TTreeReaderValue<bool>* HLT_TkMu50;
  // TTreeReaderValue<bool>* HLT_Mu55;
  TTreeReaderValue<bool>* HLT_OldMu100;
  TTreeReaderValue<bool>* HLT_TkMu100;
  // TTreeReaderValue<bool>* HLT_PFMET110_PFMHT110_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMET120_PFMHT120_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMET130_PFMHT130_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMET140_PFMHT140_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMET100_PFMHT100_IDTight_CaloBTagDeepCSV_3p1;
  // TTreeReaderValue<bool>* HLT_PFMET110_PFMHT110_IDTight_CaloBTagDeepCSV_3p1;
  // TTreeReaderValue<bool>* HLT_PFMET120_PFMHT120_IDTight_CaloBTagDeepCSV_3p1;
  // TTreeReaderValue<bool>* HLT_PFMET130_PFMHT130_IDTight_CaloBTagDeepCSV_3p1;
  // TTreeReaderValue<bool>* HLT_PFMET140_PFMHT140_IDTight_CaloBTagDeepCSV_3p1;
  // TTreeReaderValue<bool>* HLT_PFMET120_PFMHT120_IDTight_PFHT60;
  // TTreeReaderValue<bool>* HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60;
  // TTreeReaderValue<bool>* HLT_PFMETTypeOne120_PFMHT120_IDTight_PFHT60;
  // TTreeReaderValue<bool>* HLT_PFMETTypeOne110_PFMHT110_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMETTypeOne120_PFMHT120_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMETTypeOne130_PFMHT130_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMETTypeOne140_PFMHT140_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMETNoMu110_PFMHTNoMu110_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMETNoMu120_PFMHTNoMu120_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMETNoMu130_PFMHTNoMu130_IDTight;
  // TTreeReaderValue<bool>* HLT_PFMETNoMu140_PFMHTNoMu140_IDTight;
  // TTreeReaderValue<bool>* HLT_CaloMET80_NotCleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET90_NotCleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET100_NotCleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET110_NotCleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET250_NotCleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET70_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET80_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET90_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET100_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET250_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET300_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_CaloMET350_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_PFMET200_NotCleaned;
  // TTreeReaderValue<bool>* HLT_PFMET200_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_PFMET250_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_PFMET300_HBHECleaned;
  // TTreeReaderValue<bool>* HLT_PFMET200_HBHE_BeamHaloCleaned;
  // TTreeReaderValue<bool>* HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned;
  // TTreeReaderValue<bool>* HLT_MET105_IsoTrk50;
  // TTreeReaderValue<bool>* HLT_MET120_IsoTrk50;
  // TTreeReaderValue<bool>* HLT_Mu12_DoublePFJets40_CaloBTagDeepCSV_p71;
  // TTreeReaderValue<bool>* HLT_Mu12_DoublePFJets100_CaloBTagDeepCSV_p71;
  // TTreeReaderValue<bool>* HLT_Mu12_DoublePFJets200_CaloBTagDeepCSV_p71;
  // TTreeReaderValue<bool>* HLT_Mu12_DoublePFJets350_CaloBTagDeepCSV_p71;
  // TTreeReaderValue<bool>* HLT_Mu12_DoublePFJets40MaxDeta1p6_DoubleCaloBTagDeepCSV_p71;
  // TTreeReaderValue<bool>* HLT_Mu12_DoublePFJets54MaxDeta1p6_DoubleCaloBTagDeepCSV_p71;
  // TTreeReaderValue<bool>* HLT_Mu12_DoublePFJets62MaxDeta1p6_DoubleCaloBTagDeepCSV_p71;
  // TTreeReaderValue<bool>* HLT_Mu8_TrkIsoVVL;
  // TTreeReaderValue<bool>* HLT_Mu8_DiEle12_CaloIdL_TrackIdL_DZ;
  // TTreeReaderValue<bool>* HLT_Mu8_DiEle12_CaloIdL_TrackIdL;
  // TTreeReaderValue<bool>* HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_DZ;
  // TTreeReaderValue<bool>* HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350;
  // TTreeReaderValue<bool>* HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ;
  // TTreeReaderValue<bool>* HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_PFDiJet30;
  // TTreeReaderValue<bool>* HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_CaloDiJet30;
  // TTreeReaderValue<bool>* HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_PFDiJet30_PFBtagDeepCSV_1p5;
  // TTreeReaderValue<bool>* HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_CaloDiJet30_CaloBtagDeepCSV_1p5;
  // TTreeReaderValue<bool>* HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;
  // TTreeReaderValue<bool>* HLT_Mu17_TrkIsoVVL;
  // TTreeReaderValue<bool>* HLT_Mu19_TrkIsoVVL;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet20_Mu5;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet40_Mu5;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet70_Mu5;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet110_Mu5;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet170_Mu5;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4Jet300_Mu5;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK8DiJet170_Mu5;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK8Jet170_DoubleMu5;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK8Jet300_Mu5;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet20_Mu5_noalgo;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet40_Mu5_noalgo;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet70_Mu5_noalgo;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet110_Mu5_noalgo;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4DiJet170_Mu5_noalgo;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK4Jet300_Mu5_noalgo;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK8DiJet170_Mu5_noalgo;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK8Jet170_DoubleMu5_noalgo;
  // TTreeReaderValue<bool>* HLT_BTagMu_AK8Jet300_Mu5_noalgo;
  // TTreeReaderValue<bool>* HLT_Ele15_Ele8_CaloIdL_TrackIdL_IsoVL;
  // TTreeReaderValue<bool>* HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  // TTreeReaderValue<bool>* HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
  // TTreeReaderValue<bool>* HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  // TTreeReaderValue<bool>* HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  // TTreeReaderValue<bool>* HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;
  // TTreeReaderValue<bool>* HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ;
  // TTreeReaderValue<bool>* HLT_Mu12_DoublePhoton20;
  // TTreeReaderValue<bool>* HLT_IsoMu24_TwoProngs35;
  // TTreeReaderValue<bool>* HLT_TripleMu_5_3_3_Mass3p8_DZ;
  // TTreeReaderValue<bool>* HLT_TripleMu_10_5_5_DZ;
  // TTreeReaderValue<bool>* HLT_TripleMu_12_10_5;
  // TTreeReaderValue<bool>* HLT_DoubleMu3_DZ_PFMET50_PFMHT60;
  // TTreeReaderValue<bool>* HLT_DoubleMu3_DZ_PFMET70_PFMHT70;
  // TTreeReaderValue<bool>* HLT_DoubleMu3_DZ_PFMET90_PFMHT90;
  // TTreeReaderValue<bool>* HLT_DoubleMu3_Trk_Tau3mu_NoL1Mass;
  // TTreeReaderValue<bool>* HLT_DoubleMu4_Jpsi_Displaced;
  // TTreeReaderValue<bool>* HLT_DoubleMu4_Jpsi_NoVertexing;
  // TTreeReaderValue<bool>* HLT_DoubleMu4_JpsiTrkTrk_Displaced;
  // TTreeReaderValue<bool>* HLT_DoubleMu43NoFiltersNoVtx;
  // TTreeReaderValue<bool>* HLT_DoubleMu48NoFiltersNoVtx;
  // TTreeReaderValue<bool>* HLT_Mu43NoFiltersNoVtx_Photon43_CaloIdL;
  // TTreeReaderValue<bool>* HLT_Mu48NoFiltersNoVtx_Photon48_CaloIdL;
  // TTreeReaderValue<bool>* HLT_Mu38NoFiltersNoVtxDisplaced_Photon38_CaloIdL;
  // TTreeReaderValue<bool>* HLT_Mu43NoFiltersNoVtxDisplaced_Photon43_CaloIdL;
  // TTreeReaderValue<bool>* HLT_DoubleMu33NoFiltersNoVtxDisplaced;
  // TTreeReaderValue<bool>* HLT_DoubleMu40NoFiltersNoVtxDisplaced;
  // TTreeReaderValue<bool>* HLT_DoubleMu20_7_Mass0to30_L1_DM4;
  // TTreeReaderValue<bool>* HLT_DoubleMu20_7_Mass0to30_L1_DM4EG;
  // TTreeReaderValue<bool>* HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned;
  // TTreeReaderValue<bool>* HLT_Ele28_eta2p1_WPTight_Gsf_HT150;
  // TTreeReaderValue<bool>* HLT_Ele28_HighEta_SC20_Mass55;
  // TTreeReaderValue<bool>* HLT_DoubleMu20_7_Mass0to30_Photon23;
  // TTreeReaderValue<bool>* HLT_Ele15_IsoVVVL_PFHT450_CaloBTagDeepCSV_4p5;
  // TTreeReaderValue<bool>* HLT_Ele15_IsoVVVL_PFHT450_PFMET50;
  // TTreeReaderValue<bool>* HLT_Ele15_IsoVVVL_PFHT450;
  // TTreeReaderValue<bool>* HLT_Ele50_IsoVVVL_PFHT450;
  // TTreeReaderValue<bool>* HLT_Ele15_IsoVVVL_PFHT600;
  // TTreeReaderValue<bool>* HLT_Mu4_TrkIsoVVL_DiPFJet90_40_DEta3p5_MJJ750_HTT300_PFMETNoMu60;
  // TTreeReaderValue<bool>* HLT_Mu8_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT300_PFMETNoMu60;
  // TTreeReaderValue<bool>* HLT_Mu10_TrkIsoVVL_DiPFJet40_DEta3p5_MJJ750_HTT350_PFMETNoMu60;
  // TTreeReaderValue<bool>* HLT_Mu15_IsoVVVL_PFHT450_CaloBTagDeepCSV_4p5;
  // TTreeReaderValue<bool>* HLT_Mu15_IsoVVVL_PFHT450_PFMET50;
  // TTreeReaderValue<bool>* HLT_Mu15_IsoVVVL_PFHT450;
  // TTreeReaderValue<bool>* HLT_Mu50_IsoVVVL_PFHT450;
  // TTreeReaderValue<bool>* HLT_Mu15_IsoVVVL_PFHT600;
  // TTreeReaderValue<bool>* HLT_Mu3er1p5_PFJet100er2p5_PFMET70_PFMHT70_IDTight;
  // TTreeReaderValue<bool>* HLT_Mu3er1p5_PFJet100er2p5_PFMET80_PFMHT80_IDTight;
  // TTreeReaderValue<bool>* HLT_Mu3er1p5_PFJet100er2p5_PFMET90_PFMHT90_IDTight;
  // TTreeReaderValue<bool>* HLT_Mu3er1p5_PFJet100er2p5_PFMET100_PFMHT100_IDTight;
  // TTreeReaderValue<bool>* HLT_Mu3er1p5_PFJet100er2p5_PFMETNoMu70_PFMHTNoMu70_IDTight;
  // TTreeReaderValue<bool>* HLT_Mu3er1p5_PFJet100er2p5_PFMETNoMu80_PFMHTNoMu80_IDTight;
  // TTreeReaderValue<bool>* HLT_Mu3er1p5_PFJet100er2p5_PFMETNoMu90_PFMHTNoMu90_IDTight;
  // TTreeReaderValue<bool>* HLT_Mu3er1p5_PFJet100er2p5_PFMETNoMu100_PFMHTNoMu100_IDTight;
  // TTreeReaderValue<bool>* HLT_DoubleIsoMu20_eta2p1;
  // TTreeReaderValue<bool>* HLT_TrkMu12_DoubleTrkMu5NoFiltersNoVtx;
  // TTreeReaderValue<bool>* HLT_TrkMu16_DoubleTrkMu6NoFiltersNoVtx;
  // TTreeReaderValue<bool>* HLT_TrkMu17_DoubleTrkMu8NoFiltersNoVtx;
  // TTreeReaderValue<bool>* HLT_Mu8;
  // TTreeReaderValue<bool>* HLT_Mu17;
  // TTreeReaderValue<bool>* HLT_Mu19;
  // TTreeReaderValue<bool>* HLT_Mu17_Photon30_IsoCaloId;
  // TTreeReaderValue<bool>* HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30;
  // TTreeReaderValue<bool>* HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30;
  // TTreeReaderValue<bool>* HLT_Ele15_CaloIdL_TrackIdL_IsoVL_PFJet30;
  // TTreeReaderValue<bool>* HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30;
  // TTreeReaderValue<bool>* HLT_Ele8_CaloIdM_TrackIdM_PFJet30;
  // TTreeReaderValue<bool>* HLT_Ele17_CaloIdM_TrackIdM_PFJet30;
  // TTreeReaderValue<bool>* HLT_Ele23_CaloIdM_TrackIdM_PFJet30;
  // TTreeReaderValue<bool>* HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165;
  // TTreeReaderValue<bool>* HLT_Ele115_CaloIdVT_GsfTrkIdT;
  // TTreeReaderValue<bool>* HLT_Ele135_CaloIdVT_GsfTrkIdT;
  // TTreeReaderValue<bool>* HLT_Ele145_CaloIdVT_GsfTrkIdT;
  // TTreeReaderValue<bool>* HLT_Ele200_CaloIdVT_GsfTrkIdT;
  // TTreeReaderValue<bool>* HLT_Ele250_CaloIdVT_GsfTrkIdT;
  // TTreeReaderValue<bool>* HLT_Ele300_CaloIdVT_GsfTrkIdT;
  // TTreeReaderValue<bool>* HLT_IsoMu24_eta2p1_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr;
  // TTreeReaderValue<bool>* HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL;
  // TTreeReaderValue<bool>* HLT_IsoMu27_MET90;
  // TTreeReaderValue<bool>* HLT_PFMET100_PFMHT100_IDTight_PFHT60;
  // TTreeReaderValue<bool>* HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_PFHT60;
  // TTreeReaderValue<bool>* HLT_PFMETTypeOne100_PFMHT100_IDTight_PFHT60;
  // TTreeReaderValue<bool>* HLT_Mu18_Mu9_SameSign;
  // TTreeReaderValue<bool>* HLT_Mu18_Mu9_SameSign_DZ;
  // TTreeReaderValue<bool>* HLT_Mu18_Mu9;
  // TTreeReaderValue<bool>* HLT_Mu18_Mu9_DZ;
  // TTreeReaderValue<bool>* HLT_Mu20_Mu10_SameSign;
  // TTreeReaderValue<bool>* HLT_Mu20_Mu10_SameSign_DZ;
  // TTreeReaderValue<bool>* HLT_Mu20_Mu10;
  // TTreeReaderValue<bool>* HLT_Mu20_Mu10_DZ;
  // TTreeReaderValue<bool>* HLT_Mu23_Mu12_SameSign;
  // TTreeReaderValue<bool>* HLT_Mu23_Mu12_SameSign_DZ;
  // TTreeReaderValue<bool>* HLT_Mu23_Mu12;
  // TTreeReaderValue<bool>* HLT_Mu23_Mu12_DZ;
  // TTreeReaderValue<bool>* HLT_DoubleMu2_Jpsi_DoubleTrk1_Phi1p05;
  // TTreeReaderValue<bool>* HLT_DoubleMu2_Jpsi_DoubleTkMu0_Phi;
  // TTreeReaderValue<bool>* HLT_DoubleMu3_DCA_PFMET50_PFMHT60;
  // TTreeReaderValue<bool>* HLT_TripleMu_5_3_3_Mass3p8_DCA;
  // TTreeReaderValue<bool>* HLT_Mu12_IP6_part0;
  // TTreeReaderValue<bool>* HLT_Mu12_IP6_part1;
  // TTreeReaderValue<bool>* HLT_Mu12_IP6_part2;
  // TTreeReaderValue<bool>* HLT_Mu12_IP6_part3;
  // TTreeReaderValue<bool>* HLT_Mu12_IP6_part4;
  // TTreeReaderValue<bool>* HLT_Mu9_IP5_part0;
  // TTreeReaderValue<bool>* HLT_Mu9_IP5_part1;
  // TTreeReaderValue<bool>* HLT_Mu9_IP5_part2;
  // TTreeReaderValue<bool>* HLT_Mu9_IP5_part3;
  // TTreeReaderValue<bool>* HLT_Mu9_IP5_part4;
  // TTreeReaderValue<bool>* HLT_Mu7_IP4_part0;
  // TTreeReaderValue<bool>* HLT_Mu7_IP4_part1;
  // TTreeReaderValue<bool>* HLT_Mu7_IP4_part2;
  // TTreeReaderValue<bool>* HLT_Mu7_IP4_part3;
  // TTreeReaderValue<bool>* HLT_Mu7_IP4_part4;
  // TTreeReaderValue<bool>* HLT_Mu9_IP4_part0;
  // TTreeReaderValue<bool>* HLT_Mu9_IP4_part1;
  // TTreeReaderValue<bool>* HLT_Mu9_IP4_part2;
  // TTreeReaderValue<bool>* HLT_Mu9_IP4_part3;
  // TTreeReaderValue<bool>* HLT_Mu9_IP4_part4;
  // TTreeReaderValue<bool>* HLT_Mu8_IP5_part0;
  // TTreeReaderValue<bool>* HLT_Mu8_IP5_part1;
  // TTreeReaderValue<bool>* HLT_Mu8_IP5_part2;
  // TTreeReaderValue<bool>* HLT_Mu8_IP5_part3;
  // TTreeReaderValue<bool>* HLT_Mu8_IP5_part4;
  // TTreeReaderValue<bool>* HLT_Mu8_IP6_part0;
  // TTreeReaderValue<bool>* HLT_Mu8_IP6_part1;
  // TTreeReaderValue<bool>* HLT_Mu8_IP6_part2;
  // TTreeReaderValue<bool>* HLT_Mu8_IP6_part3;
  // TTreeReaderValue<bool>* HLT_Mu8_IP6_part4;
  // TTreeReaderValue<bool>* HLT_Mu9_IP6_part0;
  // TTreeReaderValue<bool>* HLT_Mu9_IP6_part1;
  // TTreeReaderValue<bool>* HLT_Mu9_IP6_part2;
  // TTreeReaderValue<bool>* HLT_Mu9_IP6_part3;
  // TTreeReaderValue<bool>* HLT_Mu9_IP6_part4;
  // TTreeReaderValue<bool>* HLT_Mu8_IP3_part0;
  // TTreeReaderValue<bool>* HLT_Mu8_IP3_part1;
  // TTreeReaderValue<bool>* HLT_Mu8_IP3_part2;
  // TTreeReaderValue<bool>* HLT_Mu8_IP3_part3;
  // TTreeReaderValue<bool>* HLT_Mu8_IP3_part4;
  // TTreeReaderValue<bool>* HLT_TrkMu6NoFiltersNoVtx;
  // TTreeReaderValue<bool>* HLT_TrkMu16NoFiltersNoVtx;
  // TTreeReaderValue<bool>* L1simulation_step;



private:

  YAML::Node fConfig;
  TChain* fChain;
  TTreeReader* fTreeReader;

  bool fIsMC;
  TString fEra;
  TString fSampleName;

  double fMaxEvent;
  std::vector<double> fMaxEventVec;

  MUON* fMuons;
  ELEC* fElecs;
  JET* fJets;

};

#endif
