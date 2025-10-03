#ifndef ELECTRON_h
#define ELECTRON_h 1

#include <iostream>
#include <vector>
#include <string>

#include "RoccoR.h"

#include "yaml-cpp/yaml.h"

#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TTreeReaderValue.h"
#include "TLorentzVector.h"


class ELEC
{
public:
  ELEC(YAML::Node fConfig) {

    YAML::Node fElecConf = fConfig["Electron"];
    YAML::Node fZConf = fConfig["Z"];

    fZMassCut = fZConf["MassCut"].as<float>();

    fLeadingPt = fElecConf["LeadingPt"].as<float>();
    fSubLeadingPt = fElecConf["SubLeadingPt"].as<float>();
    fEta = fElecConf["Eta"].as<float>();
    fID = fElecConf["ID"].as<int>();
    fOppositeCharge = true;
    if (fElecConf["Charge"].as<std::string>() == "same")
      fOppositeCharge == false;


    std::cout << "######################################################################" << std::endl;
    std::cout << "                          Electron selection                          " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << " Electron LeadingPt: " << fLeadingPt << std::endl;
    std::cout << " Electron SubLeadingPt: " << fSubLeadingPt << std::endl;
    std::cout << " Electron eta: " << fEta << std::endl;
    std::cout << " Electron ID: " << fID << std::endl;
    std::cout << " OppositeCharge: " << fOppositeCharge << std::endl;
    std::cout << "######################################################################" << std::endl;
    std::cout << " " << std::endl;
  }
  ~ELEC() {}

  struct StdElec {
    TLorentzVector fVec;
    int fCharge;

    StdElec(TLorentzVector fVec_, int fCharge_)
    : fVec(fVec_), fCharge(fCharge_)
    { };
  };

  void IsMC(bool fIsMC_) { fIsMC = fIsMC_; }

  void init(TTreeReader* fTreeReader);

  bool PrepareElec();

  std::vector<StdElec> GetElecs() { return fFVecElecs; }
  StdElec GetLeadingElec() { return fFVecElecs.at(0); }
  StdElec GetSubLeadingElec() { return fFVecElecs.at(fSubLeadingIdx); }

  TTreeReaderValue<unsigned int>* nElectron;
  TTreeReaderArray<float>* Electron_pt;
  TTreeReaderArray<float>* Electron_eta;
  TTreeReaderArray<float>* Electron_deltaEtaSC;
  TTreeReaderArray<float>* Electron_phi;
  TTreeReaderArray<float>* Electron_mass;
  TTreeReaderArray<int>* Electron_cutBased;
  TTreeReaderArray<int>* Electron_charge;

private:

  std::vector<StdElec> fFVecElecs;

  float fZMassCut;
  
  float fLeadingPt;
  float fSubLeadingPt;
  float fEta;
  int fID;
  bool fOppositeCharge;

  int fSubLeadingIdx;
  int fLeadingIdx;

  bool fIsMC;

};

#endif
