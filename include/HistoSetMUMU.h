#ifndef HistoSetMUMU_h
#define HistoSetMUMU_h 1

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "jet.h"

#include "TFile.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"


class HistoSetMUMU
{
public:
  HistoSetMUMU() {
    Init();
  }

  void Init();

  /**
    * \brief Filling the muon related histograms 
    *
    * \param fLeadingMuon: 4-vector of the leading muon
    * \param fSubleadingMuon: 4-vector of the subleading muon
    * \param fCharge: charge of the dimuon (eg. OS > 0, SS < 0)
    * \param nJet: number of jets
    * \param nBJet: number of b-jets
    * \param weight: weight of the event
    * \param fType: type of muon pair event (eg. "OS", "SS", "OS_inverted", "SS_inverted")
    *
    * \throws No exception is thrown for this function
    */
  void FillMuon(
    const TLorentzVector& fLeadingMuon, 
    const TLorentzVector& fSubleadingMuon, 
    const float& tCharge, 
    const int& nJet, 
    const int& nBJet, 
    const double& weight,
    const std::string& fType
  );

  /**
    * \brief Filling the muon related histograms 
    *
    * \param fJet: vector of jets in form of JET::StdJet
    * \param fBJet: vector of b-jets in form of JET::StdJet
    * \param fDimuonMass: mass of the dimuon
    * \param fWeight: weight of the event
    * \param fType: type of jet event (eg. "OS", "SS", "OS_inverted", "SS_inverted")
    *
    * \throws No exception is thrown for this function
    */
  void FillJet(
    const std::vector<JET::StdJet>& fJet, 
    const std::vector<JET::StdJet>& fBJet, 
    const double& fDimuonMass, 
    const double& fWeight,
    const std::string& fType
  );

  void FillHisto(std::string name, double value, double weight = 1.);
  void FillHisto(std::string name, float value, double weight = 1.);
  void FillHisto(std::string name, int value, double weight = 1.);
  void Fill2DHisto(std::string name, double value1, double value2, double weight = 1.);

  void SetHisto(std::string name, std::vector<double> bins);
  void SetHisto(std::string name, std::vector<double> bins1, std::vector<double> bins2);

  void SetHisto(std::string name);
  void SetHisto(std::string name, std::string binning);
  void SetHisto(std::string name, std::string binning1, std::string binning2);

  std::string GetMassBin(double fDimuonMass);
  std::string GetJetBin(double fNJet);
  std::string GetBJetBin(double fNBJet);
  std::string GetbVetoJetBin(double fNJet);

  double SetPtOverflow(double fPt);
  double SetMassOverflow(double fMass);

  void WriteHisto(TString fEra, TString fSampleName, TString fOutputDir, bool fIsData);

private:
  std::map<std::string, TH1D*> fHistSet;
  std::map<std::string, TH2D*> fHistSet2D;

  std::vector<std::string> fSuffix;

  std::vector<double> fPtBins;
  std::vector<double> fEtaBins;
  std::vector<double> fPhiBins;
  std::vector<double> fMassBins;
  std::vector<double> fDeltaRBins;
  std::vector<double> fNJetBins;
  std::vector<double> fChargeBins;
};

#endif
