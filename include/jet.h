#ifndef JET_h
#define JET_h

#include <iostream>
#include <vector>
#include <string>

#include "RoccoR.h"
#include "muon.h"
#include "electron.h"

#include "yaml-cpp/yaml.h"

#include "TTreeReaderArray.h"
#include "TTreeReaderValue.h"
#include "TLorentzVector.h"


class JET
{
public:
  JET(YAML::Node fConfig) {

    YAML::Node fMuonConf = fConfig["Jet"];

    fJetPt = fMuonConf["Pt"].as<float>();
    fEta = fMuonConf["Eta"].as<float>();
    fBJetTaggerCut = fMuonConf["BTag"].as<float>();
    fCleaning = fZConf["Cleaning"].as<bool>();
  }
  ~JET() {}

  struct StdJet {
    TLorentzVector fVec;
    bool fPassingBJetTagger;

    StdJet(TLorentzVector fVec_, bool fPassingBJetTagger_)
    : fVec(fVec_), fPassingBJetTagger(fPassingBJetTagger_)
    { };
  };

  void IsMC(bool fIsMC_) { fIsMC = fIsMC_; }

  bool PrepareJet(
    int nJet,
    TTreeReaderArray<float>* Jet_pt,
    TTreeReaderArray<float>* Jet_eta,
    TTreeReaderArray<float>* Jet_phi,
    TTreeReaderArray<float>* Jet_mass,
    TTreeReaderArray<int>* Jet_jetId,
    TTreeReaderArray<float>* Jet_btagCSVV2,
    std::vector<StdMuon> tMuons,
    std::vector<StdElec> tElecs
  );

  bool PrepareJet(
    int nJet,
    TTreeReaderArray<float>* Jet_pt,
    TTreeReaderArray<float>* Jet_eta,
    TTreeReaderArray<float>* Jet_phi,
    TTreeReaderArray<float>* Jet_mass,
    TTreeReaderArray<bool>* Jet_jetId,
    TTreeReaderArray<float>* Jet_btagCSVV2
  );

  std::vector<StdJet> GetJets() { return fFVecJets; }
  std::vector<StdJet> GetBJets() { return fFVecBJets; }

private:

  std::vector<StdMuon> fFVecJets;
  std::vector<StdMuon> fFVecBJets;

  float fJetPt;
  float fEta;
  float fBJetTaggerCut;
  bool fCleaning;
  bool fIsMC;
};

#endif
