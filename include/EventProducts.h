#ifndef EVENTPRODUCTS_H
#define EVENTPRODUCTS_H

#include "TLorentzVector.h"

#include <cstdint>
#include <string>
#include <vector>

struct EventInfo {
  std::string era;
  std::string sampleName;
  int jobID;
  std::string outputDir;
  bool isMC;
  std::uint32_t fSeedForMuon;
  std::uint32_t fSeedForJet;
  std::uint32_t fSeedForMuonSmearing;
};

struct BaselineInfo {
  bool fPassingTrigger;
  bool fPassingNoiseFilter;
};

struct MuonSystConfigs {
  std::string fSystName;
  std::string fOutputName;
  std::string fSystType;
  int iIter;
};

struct JetSystConfigs {
  std::string fSystName;
  std::string fOutputName;
  std::string fSystType;
  float fScalingUnc;
  float fSmearingSF;

  void SetSmearing(float fSF, float fSFUnc) {
    if (fSmearingSF == 0) {
      fSmearingSF = fSF;
      return;
    }
    fSmearingSF = fSF + fSmearingSF * fSFUnc;
  };

  void SetScaling(float fScaleUnc) {
    if (fScalingUnc == 0) {
      fScalingUnc = 1.;
      return;
    }
    fScalingUnc = 1 + fScalingUnc * fScaleUnc;
  };
};

struct PlottingSystConfigs {
  std::string fType;
  std::string fName;
  std::string fPath;
  std::string fTarget;
  std::string fInput;
  int fIter;
  std::string fMunoSrc;
  std::string fJetSrc;
  bool fDetail;
};

namespace GEN {
  struct LHEContent {
    TLorentzVector first;
    TLorentzVector second;
    int nLepton = 0;
    double fMass = 0;
  };

  struct DressedMuon {
    TLorentzVector first;
    TLorentzVector second;
    int nMuon = 0;
    double fMass = 0;
  };

  using Jets = std::vector<TLorentzVector>;
};

namespace MUON {
  struct RawMuon {
    TLorentzVector fVec;
    int fCharge;
    int fIso;
    double fRandom;
  };

  struct StdMuon {
    TLorentzVector fVec;
    TLorentzVector fVecRaw;
    int fCharge;
    int fIso;
  };

  struct MuonPair {
    MUON::StdMuon fFirst;
    MUON::StdMuon fSecond;
    int nMuon;
    double fMass;
  };
  
  using RawMuons = std::vector<RawMuon>;
  using StdMuons = std::vector<StdMuon>;
};

namespace JET {

  struct RawJet {
    TLorentzVector fVec;

    float fArea;
    int fJetPUId;
    bool fIsBJet;
    int fGenJetIdx;
    int fHadronFlavour;
    float fRho;
    double fRun;

    double fRandom;
  };

  struct StdJet {
    TLorentzVector fVec;

    int fJetPUId;
    bool fIsBJet;
    int fGenJetIdx;
    int fHadronFlavour;
  };

  using RawJets = std::vector<RawJet>;
  using StdJets = std::vector<StdJet>;

  struct JetsCollection {
    StdJets fJets;
    StdJets fBJets;
    StdJets fHSJets;
    int nJet;
    int nBJet;
  };
}

#endif
