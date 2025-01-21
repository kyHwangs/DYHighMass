#include "NT.h"
#include "options.h"
#include "DYLoop.h"

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <exception>

#include "TObject.h"
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


  // // EffTable* fID_SF_Medium = new EffTable("/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/EfficiencyTable/Run2018_UL_ID.txt");
  // EffTable* fTrigData = new EffTable("/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/EfficiencyTable/Run2018_UL_TRIG_DATAeff.txt");
  // EffTable* fTrigMC   = new EffTable("/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/EfficiencyTable/Run2018_UL_TRIG_MCeff.txt");

  // std::string filename = "/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/correctionlib/2018_muon_Z.json";
  // std::string filename2 = "/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241215_HighPt/DYHighMass/correctionlib/medium/2018.json";

  // auto fID_SF_High_origin = CorrectionSet::from_file(filename);
  // auto fID_SF_High = fID_SF_High_origin->at("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");

  // auto fID_SF_High_origin2 = CorrectionSet::from_file(filename2);
  // auto fID_SF_High2 = fID_SF_High_origin2->at("NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight");


  // auto inputs = fID_SF_High2->inputs();

  // for (int i = 0; i < inputs.size(); i++) {
  //   std::cout << i << " " << inputs.at(i).name() << " " << inputs.at(i).description() << " " << inputs.at(i).typeStr() << std::endl;
  // }

  // std::cout << "pT: 27, eta: 2.3 : table - " << fTrigData->getEfficiency(27, 2.3) / fTrigMC->getEfficiency(27, 2.3) << " correctinlib official - " << fID_SF_High->evaluate({2.3, 27., "nominal"}) << " correctinlib ny - " << fID_SF_High2->evaluate({2.3, 27., "nominal"}) << std::endl;
  // std::cout << "pT: 27, eta: 2.3 : table - " << "MC: " << fTrigMC->getEfficiency(27, 2.3) << " DATA: " << fTrigData->getEfficiency(27, 2.3)
  //                           << " correctrionlib mine MC: " << fID_SF_High2->evaluate({2.3, 27., "mcEff"}) << " DATA: " << fID_SF_High2->evaluate({2.3, 27., "dataEff"}) << std::endl;


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

  // 46  | -        | -        | -        | X        | X        | 273725 ~ 275376

  // for (int i = 1; i <= 94; i++) {

  //   // if (i != 46) continue;


  //   TChain* fChain = new TChain("Events");
  //   // fChain->Add("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/DY_Run2_UL_NanoAOD/2017/SingleMuon_Run2017B/SingleMuon/SingleMuon_Run2017B/241017_135315/0000/tree_" + (TString)std::to_string(i) + ".root"); // 1 - 52
  //   fChain->Add("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/DY_Run2_UL_NanoAOD/2017/SingleMuon_Run2017C/SingleMuon/SingleMuon_Run2017C/241017_140047/0000/tree_" + (TString)std::to_string(i) + ".root"); // 1 - 94

  //   // fChain->Add("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/DY_Run2_UL_NanoAOD/2016_preVFP/SingleMuon_Run2016B_APV_ver2/SingleMuon/SingleMuon_Run2016B_APV_ver2/241017_140702/0000/tree_" + (TString)std::to_string(i) + ".root"); // 1 - 70

  //   auto nBranches = fChain->GetListOfBranches();
  //   TTreeReader* fTreeReader = new TTreeReader(fChain);
  //   TTreeReaderValue<unsigned int>* run = new TTreeReaderValue<unsigned int>(*fTreeReader, "run");
  //   // TTreeReaderValue<bool>* HLT_TkMu50 = new TTreeReaderValue<bool>(*fTreeReader, "HLT_TkMu50");;


  //   if (i == 1)
  //     std::cout << "    |  IsoMu24 |     Mu50 |   TkMu50 |  TkMu100 | OldMu100 | run" << std::endl;;

  //   std::cout << i << " ";
  //   if (i < 10)
  //     std::cout << " ";

  //   std::cout << " | ";

  //   TObject* tHLT_IsoMu24 = nullptr;
  //   TObject* tHLT_Mu50 = nullptr;
  //   TObject* tHLT_TkMu50 = nullptr;
  //   TObject* tHLT_TkMu100 = nullptr;
  //   TObject* tHLT_OldMu100 = nullptr;

  //   try{
  //     tHLT_IsoMu24 = nBranches->FindObject("HLT_IsoMu24");
  //   }
  //   catch(std::exception& e){
  //     tHLT_IsoMu24 = nullptr;
  //   }

  //   if (tHLT_IsoMu24 == nullptr) {
  //     std::cout << "X        | ";
  //   } else {
  //     std::cout << "-        | ";
  //   }

  //   try{
  //     tHLT_Mu50 = nBranches->FindObject("HLT_Mu50");
  //   }
  //   catch(std::exception& e){
  //     tHLT_Mu50 = nullptr;
  //   }

  //   if (tHLT_Mu50 == nullptr) {
  //     std::cout << "X        | ";
  //   } else {
  //     std::cout << "-        | ";
  //   }

  //   try{
  //     tHLT_TkMu50 = nBranches->FindObject("HLT_TkMu50");
  //   }
  //   catch(std::exception& e){
  //     tHLT_TkMu50 = nullptr;
  //   }

  //   if (tHLT_TkMu50 == nullptr) {
  //     std::cout << "X        | ";
  //   } else {
  //     std::cout << "-        | ";
  //   }

  //   try{
  //     tHLT_TkMu100 = nBranches->FindObject("HLT_TkMu100");
  //   }
  //   catch(std::exception& e){
  //     tHLT_TkMu100 = nullptr;
  //   }

  //   if (tHLT_TkMu100 == nullptr) {
  //     std::cout << "X        | ";
  //   } else {
  //     std::cout << "-        | ";
  //   }

  //   try{
  //     tHLT_OldMu100 = nBranches->FindObject("HLT_OldMu100");
  //   }
  //   catch(std::exception& e){
  //     tHLT_OldMu100 = nullptr;
  //   }

  //   if (tHLT_OldMu100 == nullptr) {
  //     std::cout << "X        | ";
  //   } else {
  //     std::cout << "-        | ";
  //   }

  //   int maxEvent = fChain->GetEntries();
  //   int run_start = 999999999;
  //   int run_end = 0;

  //   while(fTreeReader->Next()) {

  //     // std::cout << "run - " << **run << ": " << **HLT_TkMu50 << std::endl;

  //     if (**run < run_start)
  //       run_start = **run;

  //     if (**run > run_end)
  //       run_end = **run;
  //   }

  //   std::cout << run_start << " ~ " << run_end << std::endl;


  //   delete fChain;
  // }


  // for (auto aBra : nBranches)
    // std::cout << aBra << std::endl;

  // for branch in Chain.GetListOfBranches():
  //   if( branch.GetName() == "TheBranchIWant" ):
  //     DoSomeAnalysis()

  return 1;
}
