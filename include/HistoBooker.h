#ifndef HISTOBOOKER_H
#define HISTOBOOKER_H

#include "EventProducts.h"
#include "WeightCalc.h"

#include "TUnfoldBinning.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

class EventContentPool;
class TH1;

class HistoBooker {
public:
  HistoBooker(const YAML::Node& fConfig);
  ~HistoBooker() = default;

  void Init(EventContentPool& pool);
  void Write();

  std::vector<std::string> GetSplitString(const std::string& str);
  std::unique_ptr<TH1> MakeDetailHisto(const std::string& tDetailSet);
  void SetDetailHistos(const std::string& tPath);

  void Fill(EventContentPool& pool);
  void FillHisto(EventContentPool& pool, const std::string& tPath, const EfficiencyFactory::WeightInput& tWeightInput, float tWeightGen, float tWeightReco, bool tIsSyst, int tIter);
  void FillEventInfo(const int nBinIdx, const double fWeight);
  float SetMassOutflow(float tMass);

private:

  std::string fEra;
  std::string fSample;
  std::string fSampleOrigin;
  std::string fId;
  std::string fOutputDir;
  bool fIsMC;

  std::string fOutputName;

  std::vector<std::string> fJetNodeNames;
  std::vector<std::vector<double>> fMassBinEdges;
  std::unique_ptr<TUnfoldBinning> fMassBinning;
  int fNBins;

  std::vector<std::string> fDetailSets;
  std::vector<std::string> fNJetSets;

  PlottingSystConfigs fNominalConfig;
  std::vector<PlottingSystConfigs> fPlottingSystConfigsSelection;
  std::vector<PlottingSystConfigs> fPlottingSystConfigsVariation;

  std::unordered_map<std::string, std::unique_ptr<TH1>> fHistPresets;
  std::unordered_map<std::string, std::unique_ptr<TH1>> fHistoCollection;

  std::unique_ptr<WeightCalc> fWeightCalc;
};



#endif