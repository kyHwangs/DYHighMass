#include "NT.h"

void NT::init_LHE() {
  
  nLHEPart = new TTreeReaderValue<unsigned int>(*fTreeReader, "nLHEPart");
  LHEPart_pt = new TTreeReaderArray<float>(*fTreeReader, "LHEPart_pt");
  LHEPart_eta = new TTreeReaderArray<float>(*fTreeReader, "LHEPart_eta");
  LHEPart_phi = new TTreeReaderArray<float>(*fTreeReader, "LHEPart_phi");
  LHEPart_mass = new TTreeReaderArray<float>(*fTreeReader, "LHEPart_mass");
  LHEPart_pdgId = new TTreeReaderArray<int>(*fTreeReader, "LHEPart_pdgId");
}

void NT::init_MC() {
  
  genWeight = new TTreeReaderValue<float>(*fTreeReader, "genWeight");

  Jet_genJetIdx = new TTreeReaderArray<int>(*fTreeReader, "Jet_genJetIdx");
  Jet_hadronFlavour = new TTreeReaderArray<int>(*fTreeReader, "Jet_hadronFlavour");
  
  nGenJet = new TTreeReaderValue<unsigned int>(*fTreeReader, "nGenJet");
  GenJet_eta = new TTreeReaderArray<float>(*fTreeReader, "GenJet_eta");
  GenJet_mass = new TTreeReaderArray<float>(*fTreeReader, "GenJet_mass");
  GenJet_phi = new TTreeReaderArray<float>(*fTreeReader, "GenJet_phi");
  GenJet_pt = new TTreeReaderArray<float>(*fTreeReader, "GenJet_pt");
  
  nGenPart = new TTreeReaderValue<unsigned int>(*fTreeReader, "nGenPart");
  GenPart_eta = new TTreeReaderArray<float>(*fTreeReader, "GenPart_eta");
  GenPart_mass = new TTreeReaderArray<float>(*fTreeReader, "GenPart_mass");
  GenPart_phi = new TTreeReaderArray<float>(*fTreeReader, "GenPart_phi");
  GenPart_pt = new TTreeReaderArray<float>(*fTreeReader, "GenPart_pt");
  GenPart_genPartIdxMother = new TTreeReaderArray<int>(*fTreeReader, "GenPart_genPartIdxMother");
  GenPart_pdgId = new TTreeReaderArray<int>(*fTreeReader, "GenPart_pdgId");
  GenPart_status = new TTreeReaderArray<int>(*fTreeReader, "GenPart_status");
  GenPart_statusFlags = new TTreeReaderArray<int>(*fTreeReader, "GenPart_statusFlags");
  
  nGenDressedLepton = new TTreeReaderValue<unsigned int>(*fTreeReader, "nGenDressedLepton");
  GenDressedLepton_eta = new TTreeReaderArray<float>(*fTreeReader, "GenDressedLepton_eta");
  GenDressedLepton_mass = new TTreeReaderArray<float>(*fTreeReader, "GenDressedLepton_mass");
  GenDressedLepton_phi = new TTreeReaderArray<float>(*fTreeReader, "GenDressedLepton_phi");
  GenDressedLepton_pt = new TTreeReaderArray<float>(*fTreeReader, "GenDressedLepton_pt");
  GenDressedLepton_pdgId = new TTreeReaderArray<int>(*fTreeReader, "GenDressedLepton_pdgId");
  
  Pileup_nTrueInt = new TTreeReaderValue<float>(*fTreeReader, "Pileup_nTrueInt");
}


void NT::init() {

  fTreeReader = new TTreeReader(fChain.get(), nullptr, true, {"HLT_Mu50", "HLT_TkMu50", "HLT_TkMu100", "HLT_OldMu100"});

  run = new TTreeReaderValue<unsigned int>(*fTreeReader, "run");
  luminosityBlock = new TTreeReaderValue<unsigned int>(*fTreeReader, "luminosityBlock");
  event = new TTreeReaderValue<unsigned long long>(*fTreeReader, "event");

  nJet = new TTreeReaderValue<unsigned int>(*fTreeReader, "nJet");
  Jet_btagDeepB = new TTreeReaderArray<float>(*fTreeReader, "Jet_btagDeepB");
  Jet_btagDeepFlavB = new TTreeReaderArray<float>(*fTreeReader, "Jet_btagDeepFlavB");
  Jet_eta = new TTreeReaderArray<float>(*fTreeReader, "Jet_eta");
  Jet_mass = new TTreeReaderArray<float>(*fTreeReader, "Jet_mass");
  Jet_phi = new TTreeReaderArray<float>(*fTreeReader, "Jet_phi");
  Jet_pt = new TTreeReaderArray<float>(*fTreeReader, "Jet_pt");
  Jet_rawFactor = new TTreeReaderArray<float>(*fTreeReader, "Jet_rawFactor");
  Jet_jetId = new TTreeReaderArray<int>(*fTreeReader, "Jet_jetId");
  Jet_puId = new TTreeReaderArray<int>(*fTreeReader, "Jet_puId");
  Jet_area = new TTreeReaderArray<float>(*fTreeReader, "Jet_area");

  nMuon = new TTreeReaderValue<unsigned int>(*fTreeReader, "nMuon");
  Muon_pt = new TTreeReaderArray<float>(*fTreeReader, "Muon_pt");
  Muon_tunepRelPt = new TTreeReaderArray<float>(*fTreeReader, "Muon_tunepRelPt");
  Muon_eta = new TTreeReaderArray<float>(*fTreeReader, "Muon_eta");
  Muon_phi = new TTreeReaderArray<float>(*fTreeReader, "Muon_phi");
  Muon_charge = new TTreeReaderArray<int>(*fTreeReader, "Muon_charge");
  Muon_mass = new TTreeReaderArray<float>(*fTreeReader, "Muon_mass");
  Muon_highPtId = new TTreeReaderArray<unsigned char>(*fTreeReader, "Muon_highPtId");
  Muon_tkRelIso = new TTreeReaderArray<float>(*fTreeReader, "Muon_tkRelIso");
  Muon_highPurity = new TTreeReaderArray<bool>(*fTreeReader, "Muon_highPurity");
  
  L1PreFiringWeight_Nom = new TTreeReaderValue<float>(*fTreeReader, "L1PreFiringWeight_Nom");
  L1PreFiringWeight_Dn = new TTreeReaderValue<float>(*fTreeReader, "L1PreFiringWeight_Dn");
  L1PreFiringWeight_Up = new TTreeReaderValue<float>(*fTreeReader, "L1PreFiringWeight_Up");  


  auto optionalBool = [this](const char* name) -> TTreeReaderValue<bool>* {
    if (!fChain->GetBranch(name)) return nullptr;
    return new TTreeReaderValue<bool>(*fTreeReader, name);
  };
  
  HLT_Mu50 = optionalBool("HLT_Mu50");
  HLT_TkMu50 = optionalBool("HLT_TkMu50");
  HLT_TkMu100 = optionalBool("HLT_TkMu100");
  HLT_OldMu100 = optionalBool("HLT_OldMu100");
  
  fixedGridRhoFastjetAll = new TTreeReaderValue<float>(*fTreeReader, "fixedGridRhoFastjetAll");

  Flag_HBHENoiseFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_HBHENoiseFilter");
  Flag_HBHENoiseIsoFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_HBHENoiseIsoFilter");
  Flag_CSCTightHaloFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_CSCTightHaloFilter");
  Flag_CSCTightHaloTrkMuUnvetoFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_CSCTightHaloTrkMuUnvetoFilter");
  Flag_CSCTightHalo2015Filter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_CSCTightHalo2015Filter");
  Flag_globalTightHalo2016Filter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_globalTightHalo2016Filter");
  Flag_globalSuperTightHalo2016Filter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_globalSuperTightHalo2016Filter");
  Flag_HcalStripHaloFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_HcalStripHaloFilter");
  Flag_hcalLaserEventFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_hcalLaserEventFilter");
  Flag_EcalDeadCellTriggerPrimitiveFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_EcalDeadCellTriggerPrimitiveFilter");
  Flag_EcalDeadCellBoundaryEnergyFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_EcalDeadCellBoundaryEnergyFilter");
  Flag_ecalBadCalibFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_ecalBadCalibFilter");
  Flag_goodVertices = new TTreeReaderValue<bool>(*fTreeReader, "Flag_goodVertices");
  Flag_eeBadScFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_eeBadScFilter");
  Flag_ecalLaserCorrFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_ecalLaserCorrFilter");
  Flag_trkPOGFilters = new TTreeReaderValue<bool>(*fTreeReader, "Flag_trkPOGFilters");
  Flag_chargedHadronTrackResolutionFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_chargedHadronTrackResolutionFilter");
  Flag_muonBadTrackFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_muonBadTrackFilter");
  Flag_BadChargedCandidateFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_BadChargedCandidateFilter");
  Flag_BadPFMuonFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_BadPFMuonFilter");
  Flag_BadPFMuonDzFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_BadPFMuonDzFilter");
  Flag_hfNoisyHitsFilter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_hfNoisyHitsFilter");
  Flag_BadChargedCandidateSummer16Filter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_BadChargedCandidateSummer16Filter");
  Flag_BadPFMuonSummer16Filter = new TTreeReaderValue<bool>(*fTreeReader, "Flag_BadPFMuonSummer16Filter");
  Flag_trkPOG_manystripclus53X = new TTreeReaderValue<bool>(*fTreeReader, "Flag_trkPOG_manystripclus53X");
  Flag_trkPOG_toomanystripclus53X = new TTreeReaderValue<bool>(*fTreeReader, "Flag_trkPOG_toomanystripclus53X");
  Flag_trkPOG_logErrorTooManyClusters = new TTreeReaderValue<bool>(*fTreeReader, "Flag_trkPOG_logErrorTooManyClusters");
  Flag_METFilters = new TTreeReaderValue<bool>(*fTreeReader, "Flag_METFilters");
  
  if (fIsMC) init_MC();
  if (fIsMC && ((TString)fSampleName).Contains("NNLO_MUMU")) init_LHE();
}
