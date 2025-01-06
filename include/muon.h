#ifndef MUON_h
#define MUON_h 1

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "RoccoR.h"
#include "NT.h"

#include "yaml-cpp/yaml.h"

#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TTreeReaderValue.h"
#include "TLorentzVector.h"


class MUON
{
public:
  MUON(YAML::Node fConfig) {

    YAML::Node fMuonConf = fConfig["Muon"];
    YAML::Node fZConf = fConfig["Z"];

    fLeadingMuonPt = fMuonConf["LeadingMuonPt"].as<float>();
    fSubLeadingMuonPt = fMuonConf["SubLeadingMuonPt"].as<float>();
    fEta = fMuonConf["Eta"].as<float>();
    fZMassCut = fZConf["MassCut"].as<float>();

    std::string id_string = fMuonConf["ID"].as<std::string>();
    if (id_string == "global") fID = (UChar_t)(2);
    else if (id_string == "tracker") fID = (UChar_t)(1);
    else throw std::runtime_error("Wrong definitions for HighPtID, allowed optsions: global, tracker");

    fISO = fMuonConf["ISO"].as<float>();

    fOppositeCharge = true;
    if (fMuonConf["Charge"].as<std::string>() == "same")
      fOppositeCharge == false;

    fDoRoccoR = true;
    if (!fMuonConf["doRoccoR"].as<bool>())
      fDoRoccoR = false;

    fRoccoR = new RoccoR(fMuonConf["RoccoR"].as<std::string>());

    std::cout << "######################################################################" << std::endl;
    std::cout << "                            Muon selection                            " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << " LeadingMuonPt: " << fLeadingMuonPt << std::endl;
    std::cout << " SubleadingMuonPt: " << fSubLeadingMuonPt << std::endl;
    std::cout << " Eta: " << fEta << std::endl;
    std::cout << " ID: " << fMuonConf["ID"].as<std::string>() << " " << fID << std::endl;
    std::cout << " ISO: " << fISO << std::endl;
    std::cout << " MassCut: " << fZMassCut << std::endl;
    std::cout << " OppositeCharge: " << fOppositeCharge << std::endl;
    std::cout << " doRoccoR: " << fDoRoccoR << std::endl;
    std::cout << "######################################################################" << std::endl;
    std::cout << " " << std::endl;
  }
  ~MUON() {}

  struct StdMuon {
    TLorentzVector fVec;
    TLorentzVector fVecRaw;
    int fCharge;
    bool fPassingID;
    float fISO;

    StdMuon(TLorentzVector fVec_, TLorentzVector fVecRaw_, int fCharge_)
    : fVec(fVec_), fVecRaw(fVecRaw_), fCharge(fCharge_)
    { };
  };

  void init(TTreeReader* fTreeReader);

  void IsMC(bool fIsMC_) { fIsMC = fIsMC_; }

  bool PrepareMuon();
  void PrepareGenMuon();

  std::vector<StdMuon> GetMuons() { return fFVecMuons; }
  std::vector<StdMuon> GetGenMuons() { return fFVecGenMuons; }
  StdMuon GetLeadingMuon() { return fFVecMuons.at(0); }
  StdMuon GetSubLeadingMuon() { return fFVecMuons.at(fSubLeadingIdx); }

  TTreeReaderValue<unsigned int>* nMuon;
  TTreeReaderArray<float>* Muon_pt;
  TTreeReaderArray<float>* Muon_tunepRelPt;
  TTreeReaderArray<float>* Muon_eta;
  TTreeReaderArray<float>* Muon_phi;
  TTreeReaderArray<int>* Muon_charge;
  TTreeReaderArray<float>* Muon_mass;
  TTreeReaderArray<unsigned char>* Muon_highPtId;
  TTreeReaderArray<float>* Muon_tkRelIso;
  TTreeReaderArray<int>* Muon_nTrackerLayers;

  TTreeReaderValue<unsigned int>* nGenPart;
  TTreeReaderArray<float>* GenPart_pt;
  TTreeReaderArray<float>* GenPart_eta;
  TTreeReaderArray<float>* GenPart_phi;
  TTreeReaderArray<float>* GenPart_mass;
  TTreeReaderArray<int>* GenPart_pdgId;

private:

  std::vector<StdMuon> fFVecMuons;
  std::vector<StdMuon> fFVecGenMuons;

  float fLeadingMuonPt;
  float fSubLeadingMuonPt;
  float fEta;
  bool fOppositeCharge;
  float fZMassCut;
  UChar_t fID;
  float fISO;

  RoccoR* fRoccoR;
  bool fDoRoccoR;

  int fLeadingIdx;
  int fSubLeadingIdx;

  bool fIsMC;

};

#endif
