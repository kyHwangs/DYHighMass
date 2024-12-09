#ifndef ELECTRON_h
#define ELECTRON_h

#include <iostream>
#include <vector>
#include <string>

#include "RoccoR.h"

#include "yaml-cpp/yaml.h"

#include "TTreeReaderArray.h"
#include "TTreeReaderValue.h"
#include "TLorentzVector.h"


class ELEC
{
public:
  ELEC(YAML::Node fConfig) {

    YAML::Node fElecConf = fConfig["Electron"];

    fPt = fElecConf["Pt"].as<float>();
    fEta = fElecConf["Eta"].as<float>();
    fSCEtaVeto = fElecConf["SCEtaVeto"].as<bool>();

    std::cout << "######################################################################" << std::endl;
    std::cout << "                          Electron selection                          " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << " Electron pT: " << fPt << std::endl;
    std::cout << " Electron eta: " << fEta << std::endl;
    std::cout << " Electron SC eta veto: " << fSCEtaVeto << std::endl;
    std::cout << "######################################################################" << std::endl;
    std::cout << " " << std::endl;
  }
  ~ELEC() {}

  struct StdElec {
    TLorentzVector fVec;

    StdElec(TLorentzVector fVec_)
    : fVec(fVec_)
    { };
  };

  void IsMC(bool fIsMC_) { fIsMC = fIsMC_; }

  bool PrepareElec(
    int nElectron,
    TTreeReaderArray<float>* Electron_pt,
    TTreeReaderArray<float>* Electron_eta,
    TTreeReaderArray<float>* Electron_deltaEtaSC,
    TTreeReaderArray<float>* Electron_phi,
    TTreeReaderArray<float>* Electron_mass,
    TTreeReaderArray<int>* Electron_cutBased
  );

  std::vector<StdElec> GetElecs() { return fFVecElecs; }

private:

  std::vector<StdElec> fFVecElecs;

  float fPt;
  float fEta;
  bool fSCEtaVeto;

  bool fIsMC;

};

#endif
