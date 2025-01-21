#include "NT.h"
#include "options.h"
#include "DYLoop.h"

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "TFile.h"
#include "TROOT.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TStopwatch.h"
#include "TChain.h"
#include "TLorentzVector.h"

#include "correction.h"

#include "yaml-cpp/yaml.h"

using correction::CorrectionSet;

int main(int argc, char* argv[]) {


  // EffTable* fID_SF_Medium = new EffTable("/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/EfficiencyTable/Run2018_UL_ID.txt");
  EffTable* fTrigData = new EffTable("/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/EfficiencyTable/Run2018_UL_TRIG_DATAeff.txt");
  EffTable* fTrigMC   = new EffTable("/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/EfficiencyTable/Run2018_UL_TRIG_MCeff.txt");

  std::string filename = "/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/correctionlib/2018_muon_Z.json";
  std::string filename2 = "/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/correctionlib/medium/2018.json";

  auto fID_SF_High_origin = CorrectionSet::from_file(filename);
  auto fID_SF_High = fID_SF_High_origin->at("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");

  auto fID_SF_High_origin2 = CorrectionSet::from_file(filename2);
  auto fID_SF_High2 = fID_SF_High_origin2->at("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");

  std::cout << typeid(fID_SF_High2).name() << std::endl;


  auto inputs = fID_SF_High2->inputs();

  for (int i = 0; i < inputs.size(); i++) {
    std::cout << i << " " << inputs.at(i).name() << " " << inputs.at(i).description() << " " << inputs.at(i).typeStr() << std::endl;
  }

  std::cout << "pT: 27, eta: 2.3 : table - " << fTrigData->getEfficiency(27, 2.3) / fTrigMC->getEfficiency(27, 2.3) << " correctinlib official - " << fID_SF_High->evaluate({2.3, 27., "nominal"}) << " correctinlib ny - " << fID_SF_High2->evaluate({2.3, 27., "nominal"}) << std::endl;
  std::cout << "pT: 27, eta: 2.3 : table - " << "MC: " << fTrigMC->getEfficiency(27, 2.3) << " DATA: " << fTrigData->getEfficiency(27, 2.3)
                            << " correctrionlib mine MC: " << fID_SF_High2->evaluate({2.3, 27., "mcEff"}) << " DATA: " << fID_SF_High2->evaluate({2.3, 27., "dataEff"}) << std::endl;


  // for (auto aTest : fID_SF_High_origin->values()) {
  //   std::cout << aTest->name << std::endl;
  // }

  // cset_2016preID->evaluate({"2016preVFP", "sf", "wp90iso", std::abs(eta[0]), pt[0]})

  // auto cset_2016preID = cset->at();


  // TChain* fChain = new TChain("Events");
  // fChain->Add("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/DY_Run2_UL_NanoAOD/2018/DYJetsToMuMu_M-50/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/DYJetsToMuMu_M-50/241017_133720/0000/tree_1.root");
  // TTreeReader* fTreeReader = new TTreeReader(fChain);

  // class test {

  // public:
  //   test(TTreeReader* fTreeReader) {
  //     nMuon = new TTreeReaderValue<unsigned int>(*fTreeReader, "nMuon");
  //     Muon_pt = new TTreeReaderArray<float>(*fTreeReader, "Muon_pt");
  //   }
  //   TTreeReaderValue<unsigned int>* nMuon;
  //   TTreeReaderArray<float>* Muon_pt;

  //   ~test() {}
  // };

  // test classtest(fTreeReader);


  // // TTreeReaderValue<unsigned int>* nMuon = new TTreeReaderValue<unsigned int>(*fTreeReader, "nMuon");
  // // TTreeReaderArray<float>* Muon_pt = new TTreeReaderArray<float>(*fTreeReader, "Muon_pt");


  // for (int i = 0; i < 10; i++) {
  //   fTreeReader->Next();
  //   std::cout << i << " nMuon: " << **(classtest.nMuon) << " ";
  //   for (int j = 0; j < **(classtest.nMuon); j++)
  //     std::cout << classtest.Muon_pt->At(j) << " ";

  //   std::cout << " " << std::endl;

  // }




  return 1;
}
