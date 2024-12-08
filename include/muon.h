#ifndef MUON_h
#define MUON_h

#include <iostream>
#include <vector>
#include <string>

#include "RoccoR.h"

#include "yaml-cpp/yaml.h"

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

    fOppositeCharge = true;
    if (fMuonConf["Charge"].as<std::string>() == "same")
      fOppositeCharge == false;

    fDoRoccoR = true;
    if (!fMuonConf["doRoccoR"].as<bool>())
      fDoRoccoR = false;

    fRoccoR = new RoccoR(fMuonConf["RoccoR"].as<std::string>());

    std::cout <<"LeadingMuonPt: " << fLeadingMuonPt << std::endl;
    std::cout <<"SubleadingMuonPt: " << fSubLeadingMuonPt << std::endl;
    std::cout <<"Eta: " << fEta << std::endl;
    std::cout <<"MassCut: " << fZMassCut << std::endl;
    std::cout <<"OppositeCharge: " << fOppositeCharge << std::endl;
    std::cout <<"doRoccoR: " << fDoRoccoR << std::endl;

  }
  ~MUON() {}

  struct StdMuon {
    TLorentzVector fVec;
    TLorentzVector fVecRaw;
    int fCharge;
    bool fPassingID;
    float fISO;

    StdMuon(TLorentzVector fVec_, TLorentzVector fVecRaw_, int fCharge_, bool fPassingID_, float fISO_)
    : fVec(fVec_), fVecRaw(fVecRaw_), fCharge(fCharge_), fPassingID(fPassingID_), fISO(fISO_)
    { };
  };

  void IsMC(bool fIsMC_) { fIsMC = fIsMC_; }

  bool PrepareMuon(
    int nMuon,
    TTreeReaderArray<float>* Muon_pt,
    TTreeReaderArray<float>* Muon_eta,
    TTreeReaderArray<float>* Muon_phi,
    TTreeReaderArray<int>* Muon_charge,
    TTreeReaderArray<float>* Muon_mass,
    TTreeReaderArray<bool>* Muon_tightId,
    TTreeReaderArray<float>* Muon_pfRelIso04_all,
    TTreeReaderArray<int>* Muon_nTrackerLayers
  );

  void PrepareGenMuon(
    int nGenPart,
    TTreeReaderArray<float>* GenPart_pt,
    TTreeReaderArray<float>* GenPart_eta,
    TTreeReaderArray<float>* GenPart_phi,
    TTreeReaderArray<float>* GenPart_mass,
    TTreeReaderArray<int>* GenPart_pdgId
  );

  std::vector<StdMuon> GetMuons() { return fFVecMuons; }
  std::vector<StdMuon> GetGenMuons() { return fFVecGenMuons; }
  StdMuon GetLeadingMuon() { return fFVecMuons.at(0); }
  StdMuon GetSubLeadingMuon() { return fFVecMuons.at(fSubLeadingIdx); }

private:

  std::vector<StdMuon> fFVecMuons;
  std::vector<StdMuon> fFVecGenMuons;

  float fLeadingMuonPt;
  float fSubLeadingMuonPt;
  float fEta;
  bool fOppositeCharge;
  float fZMassCut;

  RoccoR* fRoccoR;
  bool fDoRoccoR;

  int fLeadingIdx;
  int fSubLeadingIdx;

  bool fIsMC;

};

#endif
