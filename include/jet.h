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

    YAML::Node fJetConf = fConfig["Jet"];

    fJetPt = fJetConf["Pt"].as<float>();
    fEta = fJetConf["Eta"].as<float>();
    fBJetTaggerCut = fJetConf["BTag"].as<float>();
    fCleaning = fJetConf["Cleaning"].as<bool>();
    fJetID = fJetConf["ID"].as<int>();

    std::cout << "######################################################################" << std::endl;
    std::cout << "                            Jet selection                             " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << " jet pT: " << fJetPt << std::endl;
    std::cout << " jet eta: " << fEta << std::endl;
    std::cout << " jet ID: " << fJetID << std::endl;
    std::cout << " b-tagger WP: " << fBJetTaggerCut << std::endl;
    std::cout << " jet cleaning: " << fCleaning << std::endl;
    std::cout << "######################################################################" << std::endl;
    std::cout << " " << std::endl;
  }
  ~JET() {}

  struct StdJet {
    TLorentzVector fVec;
    bool fPassingBJetTagger;
    int fID;

    StdJet(TLorentzVector fVec_, bool fPassingBJetTagger_, int fID_)
    : fVec(fVec_), fPassingBJetTagger(fPassingBJetTagger_), fID(fID_)
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
    std::vector<MUON::StdMuon> tMuons,
    std::vector<ELEC::StdElec> tElecs
  );

  bool PrepareJet(
    int nJet,
    TTreeReaderArray<float>* Jet_pt,
    TTreeReaderArray<float>* Jet_eta,
    TTreeReaderArray<float>* Jet_phi,
    TTreeReaderArray<float>* Jet_mass,
    TTreeReaderArray<int>* Jet_jetId,
    TTreeReaderArray<float>* Jet_btagCSVV2
  );

  std::vector<StdJet> GetJets() { return fFVecJets; }
  std::vector<StdJet> GetBJets() { return fFVecBJets; }

private:

  std::vector<StdJet> fFVecJets;
  std::vector<StdJet> fFVecBJets;

  float fJetPt;
  float fEta;
  float fBJetTaggerCut;
  int fJetID;
  bool fCleaning;
  bool fIsMC;
};

#endif
