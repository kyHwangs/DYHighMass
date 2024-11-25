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

#include "yaml-cpp/yaml.h"

int main(int argc, char* argv[]) {

  // --config /u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241001/DYHighMass/config/UL2018.yml
  // --base instal/..
  // --era 2018
  // --sample NNLO_M_50
  // --id '{10, 11}'
  // --output

  options* opt = new options(argc, argv);
  // opt->Print();

  DYLoop* fLoops = new DYLoop(opt);
  fLoops->Print();
  fLoops->Loop();


  // YAML::Node fConfig = YAML::LoadFile("/u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241001/DYHighMass/config/UL2018.yml");

  // YAML::Node fMuonNode = fConfig["Muon"];

  // YAML::Node fZNode = fConfig["Z"];

  // float leadingMuonPt = fMuonNode["LeadingMuonPt"].as<float>();
  // float subLeadingMuonPt = fMuonNode["SubLeadingMuonPt"].as<float>();
  // float eta = fMuonNode["Eta"].as<float>();
  // TString id = fMuonNode["ID"].as<std::string>();
  // TString iso = fMuonNode["ISO"].as<std::string>();
  // TString charge = fMuonNode["Charge"].as<std::string>();

  // float zMassCut = fZNode["MassCut"].as<float>();

  // std::cout << leadingMuonPt << std::endl;
  // std::cout << subLeadingMuonPt << std::endl;
  // std::cout << eta << std::endl;
  // std::cout << id << std::endl;
  // std::cout << iso << std::endl;
  // std::cout << charge << std::endl;
  // std::cout << zMassCut << std::endl;


  // NT* tNtuples = new NT(fConfig);
  // tNtuples->AddChain("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/DY_Run2_UL_NanoAOD/2018/DYJetsToMuMu_M-50/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/DYJetsToMuMu_M-50/241017_133720/0000/tree_90.root");
  // tNtuples->AddChain("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/DY_Run2_UL_NanoAOD/2018/DYJetsToMuMu_M-50/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/DYJetsToMuMu_M-50/241017_133720/0000/tree_91.root");
  // tNtuples->AddChain("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/DY_Run2_UL_NanoAOD/2018/DYJetsToMuMu_M-50/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/DYJetsToMuMu_M-50/241017_133720/0000/tree_92.root");
  // tNtuples->AddChain("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/DY_Run2_UL_NanoAOD/2018/DYJetsToMuMu_M-50/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/DYJetsToMuMu_M-50/241017_133720/0000/tree_93.root");
  // tNtuples->AddChain("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/DY_Run2_UL_NanoAOD/2018/DYJetsToMuMu_M-50/DYJetsToMuMu_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos/DYJetsToMuMu_M-50/241017_133720/0000/tree_94.root");
  // tNtuples->init();

  // std::cout << "Max Event: " << tNtuples->GetEntries() << std::endl;

  // int i = 0;
  // // while(tNtuples->GetNext()) {
  // while(tNtuples->GetNext()) {
  //   // if (tNtuples->GetNext()) {
  //   //   // std::cout << **(tNtuples->run) << " " << **(tNtuples->event) << std::endl;
  //   // }

  //   int nMuon = **(tNtuples->nMuon);
  //   if (nMuon < 4)
  //     continue;

  //   std::vector<MUON::StdMuon> muons_ = tNtuples->GetMuons();
  //   if (muons_.size() < 3)
  //     continue;

  //   std::cout << "nMuon: " << nMuon << std::endl;

  //   std::cout <<"Muons From Ntuples: " << std::endl;
  //   for (int j = 0; j < nMuon; j++) {

  //     std::string highPtID_test = "null";
  //     // if (tNtuples->Muon_highPtId->At(i) == (UChar_t)(1)) highPtID_test = "tracker high pT";
  //     // if (tNtuples->Muon_highPtId->At(i) == (UChar_t)(2)) highPtID_test = "global high pT";

  //     std::cout << j << " "
  //               << tNtuples->Muon_pt->At(j) << " "
  //               << tNtuples->Muon_eta->At(j) << " "
  //               << tNtuples->Muon_phi->At(j) << " "
  //               << tNtuples->Muon_charge->At(j) << " "
  //               << tNtuples->Muon_tightId->At(j) << " "
  //               << tNtuples->Muon_pfRelIso04_all->At(j) << " "
  //               << std::endl;
  //   }

  //   // if (!tNtuples->PrepareMuon()) {
  //   //   std::cout << "no selected muons!" << std::endl;
  //   // }

  //   bool tmp = tNtuples->PrepareMuon();

  //   // std::vector<MUON::StdMuon> muons_ = tNtuples->GetMuons();
  //   std::cout <<"Muons From MUON class: " << std::endl;

  //   for(int j = 0; j < muons_.size(); j++) {

  //     std::cout << j << " "
  //               << muons_.at(j).fVecRaw.Pt() << " "
  //               << muons_.at(j).fVecRaw.Eta() << " "
  //               << muons_.at(j).fVecRaw.Phi() << " "
  //               << muons_.at(j).fCharge << " "
  //               << muons_.at(j).fVec.Pt() << " "
  //               << muons_.at(j).fVec.Eta() << " "
  //               << muons_.at(j).fVec.Phi() << " "
  //               << std::endl;
  //   }

  //   if (tmp) {
  //     std::cout << "Leading Muon:" << std::endl;
  //     auto lMuon = tNtuples->GetLeadingMuon();
  //     std::cout << " Leading "
  //               << lMuon.fVecRaw.Pt() << " "
  //               << lMuon.fVecRaw.Eta() << " "
  //               << lMuon.fVecRaw.Phi() << " "
  //               << lMuon.fCharge << " "
  //               << lMuon.fVec.Pt() << " "
  //               << lMuon.fVec.Eta() << " "
  //               << lMuon.fVec.Phi() << " "
  //               << std::endl;

  //     std::cout << "SubLeading Muon:" << std::endl;
  //     auto sMuon = tNtuples->GetSubLeadingMuon();
  //     std::cout << " SubLeading "
  //               << sMuon.fVecRaw.Pt() << " "
  //               << sMuon.fVecRaw.Eta() << " "
  //               << sMuon.fVecRaw.Phi() << " "
  //               << sMuon.fCharge << " "
  //               << sMuon.fVec.Pt() << " "
  //               << sMuon.fVec.Eta() << " "
  //               << sMuon.fVec.Phi() << " "
  //               << std::endl;


  //   }

  //   std::cout << " " << std::endl;
  //   std::cout << " " << std::endl;
  //   std::cout << " " << std::endl;
  //   std::cout << " " << std::endl;
  //   std::cout << " " << std::endl;
  //   std::cout << " " << std::endl;
  //   std::cout << " " << std::endl;





    // std::cout << typeid(tNtuples->Muon_highPtId).name() << std::endl;
    // std::cout << typeid(*(tNtuples->Muon_highPtId)).name() << std::endl;
    // std::cout << typeid((*(tNtuples->Muon_highPtId))[0]).name() << std::endl;

    // std::cout << typeid(tNtuples->Muon_pt).name() << std::endl;
    // std::cout << typeid(*(tNtuples->Muon_pt)).name() << std::endl;
    // std::cout << typeid((*(tNtuples->Muon_pt))[0]).name() << std::endl;
  // }


  return 1;
}
