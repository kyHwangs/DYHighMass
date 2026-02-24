#ifndef JET_h
#define JET_h 1

#include <iostream>
#include <vector>
#include <string>

#include "RoccoR.h"
#include "muon.h"
#include "electron.h"
#include "EfficiencyTable.h"
#include "BTagCalibrationStandalone.h"

#include "yaml-cpp/yaml.h"

#include "TTreeReader.h"
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
    fJetPUID = fJetConf["PUID"].as<int>();

    fJetPUIDTable = EffTable(fConfig["Efficiency"]["JetPU"]["Path"].as<std::string>());

    fJetBTagEffB = EffTable(fConfig["Efficiency"]["BTagEff"]["bQuark"].as<std::string>());
    fJetBTagEffC = EffTable(fConfig["Efficiency"]["BTagEff"]["cQuark"].as<std::string>());
    fJetBTagEffL = EffTable(fConfig["Efficiency"]["BTagEff"]["lQuark"].as<std::string>());

    BTagCalibration tCalibTable("DeepCSV", fConfig["Efficiency"]["BTag"]["Path"].as<std::string>());

    fBTagCalibReader = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", {"up_correlated", "down_correlated", "up_uncorrelated", "down_uncorrelated"});
    fBTagCalibReader->load(tCalibTable, BTagEntry::FLAV_UDSG, "incl");
    fBTagCalibReader->load(tCalibTable, BTagEntry::FLAV_C, "mujets");
    fBTagCalibReader->load(tCalibTable, BTagEntry::FLAV_B, "mujets");

    std::cout << "######################################################################" << std::endl;
    std::cout << "                            Jet selection                             " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << " jet pT: " << fJetPt << std::endl;
    std::cout << " jet eta: " << fEta << std::endl;
    std::cout << " jet ID: " << fJetID << std::endl;
    std::cout << " jet PU ID: " << fJetPUID << std::endl;
    std::cout << " b-tagger WP: " << fBJetTaggerCut << std::endl;
    std::cout << " jet cleaning: " << fCleaning << std::endl;
    std::cout << "######################################################################" << std::endl;
    std::cout << " " << std::endl;
  }
  ~JET() {}

  struct StdJet {
    TLorentzVector fVec;
    TLorentzVector fVecRaw;
    bool fPassingBJetTagger;
    int fID;
    int fHadFlav;
    int fGenJetIdx;

    StdJet(TLorentzVector fVec_, TLorentzVector fVecRaw_, bool fPassingBJetTagger_, int fID_, int fHadFlav_, int fGenJetIdx_)
    : fVec(fVec_), fVecRaw(fVecRaw_), fPassingBJetTagger(fPassingBJetTagger_), fID(fID_), fHadFlav(fHadFlav_), fGenJetIdx(fGenJetIdx_)
    { };
  };

  void init(TTreeReader* fTreeReader);

  void IsMC(bool fIsMC_) { fIsMC = fIsMC_; }

  bool PrepareJet();

  std::vector<StdJet> GetJets() { return fFVecJets; }
  std::vector<StdJet> GetBJets() { return fFVecBJets; }

  double GetPUIDSF();
  double GetBTagSF();

  TTreeReaderValue<unsigned int>* nJet;
  TTreeReaderArray<float>* Jet_pt;
  TTreeReaderArray<float>* Jet_eta;
  TTreeReaderArray<float>* Jet_phi;
  TTreeReaderArray<float>* Jet_mass;
  TTreeReaderArray<int>* Jet_jetId;
  TTreeReaderArray<int>* Jet_puId;
  TTreeReaderArray<float>* Jet_btagDeepFlavB;
  TTreeReaderArray<int>* Jet_hadronFlavour;
  TTreeReaderArray<int>* Jet_genJetIdx;

private:

  std::vector<StdJet> fFVecJets;
  std::vector<StdJet> fFVecBJets;

  float fJetPt;
  float fEta;
  float fBJetTaggerCut;
  int fJetID;
  int fJetPUID;
  bool fCleaning;
  bool fIsMC;

  EffTable fJetPUIDTable;
  EffTable fJetBTagEffB;
  EffTable fJetBTagEffC;
  EffTable fJetBTagEffL;
  BTagCalibrationReader* fBTagCalibReader;

};

#endif
