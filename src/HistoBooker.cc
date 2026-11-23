#include "EventContentPool.h"
#include "HistoBooker.h"
#include "EfficiencyFactory.h"
#include "WeightCalc.h"

#include "TFile.h"
#include "TDirectory.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

#include <string>
#include <vector>
#include <sstream>

#include "yaml-cpp/yaml.h"

HistoBooker::HistoBooker(const YAML::Node& fConfig) {
  
  fMassBinEdges = {
    {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 4000},
    {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 4000},
    {200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 4000}
  };

  fJetNodeNames = {"NJet0", "NJet1", "NJet2"};

  fMassBinning = std::make_unique<TUnfoldBinning>("MassBinning");
  for (int iJet = 0; iJet < fJetNodeNames.size(); iJet++) {
    TUnfoldBinning* tBinningJet = fMassBinning->AddBinning(fJetNodeNames[iJet].c_str());
    const auto& tMassBinEdges = fMassBinEdges[iJet];
    tBinningJet->AddAxis("mass", tMassBinEdges.size() - 1, tMassBinEdges.data(), kTRUE, kFALSE);
  }

  auto* tSampleMigrationMatrix = fMassBinning->CreateHistogramOfMigrations(fMassBinning.get(), fMassBinning.get(), "SampleMigrationMatrix");
  fNBins = tSampleMigrationMatrix->GetNbinsX();
  
  fHistPresets["Pt"] = std::make_unique<TH1F>("SmaplePt", "", 300, 0., 1500.);
  fHistPresets["Eta"] = std::make_unique<TH1F>("SmapleEta", "", 60, -3., 3.);
  fHistPresets["Phi"] = std::make_unique<TH1F>("SmaplePhi", "", 60, -3.141593, 3.141593);
  fHistPresets["NJet"] = std::make_unique<TH1F>("SmapleNJet", "", 20, 0., 20.);

  fDetailSets = {
    "h_LeadingMuonPt",
    "h_LeadingMuonEta",
    "h_LeadingMuonPhi",

    "h_SubLeadingMuonPt",
    "h_SubLeadingMuonEta",
    "h_SubLeadingMuonPhi",

    "h_dimuonPt",
    "h_dimuonRap",

    "h_NJets",
    "h_BJets",

    "h_JetPt",
    "h_JetEta",
    "h_JetPhi",
  };

  fNJetSets = {
    "inc",
    "bVeto",
    "bVeto_0J",
    "bVeto_1J",
    "bVeto_2J"
  };

  fWeightCalc = std::make_unique<WeightCalc>(fConfig);
}

std::unique_ptr<TH1> HistoBooker::MakeDetailHisto(const std::string& tDetailSet) {

  TH1* tClone = nullptr;

  if (tDetailSet.find("Pt") != std::string::npos)
    tClone = static_cast<TH1*>(fHistPresets["Pt"]->Clone(tDetailSet.c_str()));
  else if (tDetailSet.find("Eta") != std::string::npos || tDetailSet.find("Rap") != std::string::npos)
    tClone = static_cast<TH1*>(fHistPresets["Eta"]->Clone(tDetailSet.c_str()));
  else if (tDetailSet.find("Phi") != std::string::npos)
    tClone = static_cast<TH1*>(fHistPresets["Phi"]->Clone(tDetailSet.c_str()));
  else if (tDetailSet.find("NJet") != std::string::npos || tDetailSet.find("BJets") != std::string::npos)
    tClone = static_cast<TH1*>(fHistPresets["NJet"]->Clone(tDetailSet.c_str()));

  if (!tClone) return nullptr;
  tClone->SetDirectory(nullptr);
  return std::unique_ptr<TH1>(tClone);
}

void HistoBooker::SetDetailHistos(const std::string& tPath) {
  
  for (const auto& tDetailSet : fDetailSets) {
    for (const auto& tNJets : fNJetSets) {
      fHistoCollection[tPath + ":All:" + tDetailSet + ":" + tNJets] = MakeDetailHisto(tDetailSet);
    }
  }
}

void HistoBooker::Init(EventContentPool& pool) {

  fEra = pool.GetEventInfo().era;
  fSampleOrigin = pool.GetEventInfo().sampleName;
  fId = std::to_string(pool.GetEventInfo().jobID);
  fOutputDir = pool.GetEventInfo().outputDir;
  fIsMC = pool.GetEventInfo().isMC;

  fWeightCalc->Init(pool);
  
  fSample = fSampleOrigin;

  if (fSample.find("NNLO_MUMU") != std::string::npos) fSample = "NNLO_MUMU";
  if (fSample.find("GGToMuMu") != std::string::npos) fSample = "GG";
  if (fSample.find("Run") != std::string::npos) fSample = "Data";

  fOutputName = fOutputDir + "/" + fEra + "_" + fSampleOrigin + "_" + fId + ".root";
  if (fSampleOrigin.find("NNLO_MUMU_10to50") != std::string::npos) fSampleOrigin = "NNLO_MUMU_10to50";

  fHistoCollection[fEra + ":" + fSampleOrigin + ":EventInfo:dummy"] = std::make_unique<TH1D>("h_EventInfo", "", 5, 0.5, 5.5);

  fNominalConfig = PlottingSystConfigs{
    "None",
    "Nominal",
    fEra + ":" + fSample + ":OS",
    "None",
    "None",
    1,
    "MuonPair_Nominal",
    "NominalJets",
    false
  };

  if (fIsMC) {
    const auto& tSystematics = pool.GetSystematics();
    for (const auto& tSystematic : tSystematics) {

      if (tSystematic.second["Name"].as<std::string>() == "Nominal")
        continue;

      const auto& tPath = tSystematic.second["Path"].as<std::vector<std::string>>();
      std::string tPathStr;
      for (size_t i = 0; i < tPath.size(); i++) {
        std::string tToken = tPath.at(i);
        if (tToken == "era") tToken = fEra;
        if (tToken == "sample") tToken = fSample;

        tPathStr += tToken;
        if (i != tPath.size() - 1) tPathStr += ":";
      }
    
      if (tSystematic.second["SystType"].as<std::string>().find("Selection") != std::string::npos) {
        fPlottingSystConfigsSelection.push_back(
          PlottingSystConfigs{
            tSystematic.second["SystType"].as<std::string>(),
            tSystematic.second["Name"].as<std::string>(),
            tPathStr,
            tSystematic.second["target"] ? tSystematic.second["target"].as<std::string>() : "None",
            tSystematic.second["input"] ? tSystematic.second["input"].as<std::string>() : "None",
            tSystematic.second["Iter"].as<int>(),
            tSystematic.second["MuonSrc"].as<std::string>(),
            tSystematic.second["JetSrc"].as<std::string>(),
            tSystematic.second["Detail"].as<bool>()
          }
        );
      } else {
        fPlottingSystConfigsVariation.push_back(
          PlottingSystConfigs{
            tSystematic.second["SystType"].as<std::string>(),
            tSystematic.second["Name"].as<std::string>(),
            tPathStr,
            tSystematic.second["target"] ? tSystematic.second["target"].as<std::string>() : "None",
            tSystematic.second["input"] ? tSystematic.second["input"].as<std::string>() : "None",
            tSystematic.second["Iter"].as<int>(),
            tSystematic.second["MuonSrc"].as<std::string>(),
            tSystematic.second["JetSrc"].as<std::string>(),
            tSystematic.second["Detail"].as<bool>()
          }
        );
      }
    }
  }

  fHistoCollection[fNominalConfig.fPath + ":dimuonMass"] = std::make_unique<TH1F>("h_dimuonMass", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5);
  if (fSample.find("NNLO_MUMU") != std::string::npos) {
    fHistoCollection[fNominalConfig.fPath + ":h_ResponseMatrix"] = std::make_unique<TH2F>("h_ResponseMatrix", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5, fNBins, 0.5, static_cast<float>(fNBins) + 0.5);
    fHistoCollection[fNominalConfig.fPath + ":dimuonMassGen"] = std::make_unique<TH1F>("h_dimuonMuonGen", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5);
    fHistoCollection[fNominalConfig.fPath + ":dimuonMassFake"] = std::make_unique<TH1F>("h_dimuonMuonFake", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5);
  }

  for (const auto& tPlottingSystConfig : fPlottingSystConfigsSelection) {
    
    fHistoCollection[tPlottingSystConfig.fPath + ":dimuonMass"] = std::make_unique<TH2F>("h_dimuonMass", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5, tPlottingSystConfig.fIter, 0.5, static_cast<float>(tPlottingSystConfig.fIter) + 0.5);
    if (fSample.find("NNLO_MUMU") != std::string::npos) {
      fHistoCollection[tPlottingSystConfig.fPath + ":dimuonMassGen"] = std::make_unique<TH2F>("h_dimuonMuonGen", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5, tPlottingSystConfig.fIter, 0.5, static_cast<float>(tPlottingSystConfig.fIter) + 0.5);
      fHistoCollection[tPlottingSystConfig.fPath + ":dimuonMassFake"] = std::make_unique<TH2F>("h_dimuonMuonFake", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5, tPlottingSystConfig.fIter, 0.5, static_cast<float>(tPlottingSystConfig.fIter) + 0.5);
      fHistoCollection[tPlottingSystConfig.fPath + ":h_ResponseMatrix"] = std::make_unique<TH3F>("h_ResponseMatrix", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5, fNBins, 0.5, static_cast<float>(fNBins) + 0.5, tPlottingSystConfig.fIter, 0.5, static_cast<float>(tPlottingSystConfig.fIter) + 0.5);
    }
  }

  for (const auto& tPlottingSystConfig : fPlottingSystConfigsVariation) {
    
    fHistoCollection[tPlottingSystConfig.fPath + ":dimuonMass"] = std::make_unique<TH2F>("h_dimuonMass", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5, tPlottingSystConfig.fIter, 0.5, static_cast<float>(tPlottingSystConfig.fIter) + 0.5);
    if (fSample.find("NNLO_MUMU") != std::string::npos) {
      fHistoCollection[tPlottingSystConfig.fPath + ":dimuonMassGen"] = std::make_unique<TH2F>("h_dimuonMuonGen", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5, tPlottingSystConfig.fIter, 0.5, static_cast<float>(tPlottingSystConfig.fIter) + 0.5);
      fHistoCollection[tPlottingSystConfig.fPath + ":dimuonMassFake"] = std::make_unique<TH2F>("h_dimuonMuonFake", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5, tPlottingSystConfig.fIter, 0.5, static_cast<float>(tPlottingSystConfig.fIter) + 0.5);
      fHistoCollection[tPlottingSystConfig.fPath + ":h_ResponseMatrix"] = std::make_unique<TH3F>("h_ResponseMatrix", "", fNBins, 0.5, static_cast<float>(fNBins) + 0.5, fNBins, 0.5, static_cast<float>(fNBins) + 0.5, tPlottingSystConfig.fIter, 0.5, static_cast<float>(tPlottingSystConfig.fIter) + 0.5);
    }
  }
  
  std::cout << "######################################################################" << std::endl;
  std::cout << " Plotting configs: " << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;

  std::cout << " - Nominal: " << fNominalConfig.fName << " (" << fNominalConfig.fType << ") + " << fNominalConfig.fInput << " : [" << fNominalConfig.fMunoSrc << ", " << fNominalConfig.fJetSrc << ", " << fNominalConfig.fIter << "]  to " << fNominalConfig.fPath << " -> " << fNominalConfig.fTarget << std::endl;
  
  for (const auto& tPlottingSystConfig : fPlottingSystConfigsVariation)
    std::cout << " - " << tPlottingSystConfig.fName << " (" << tPlottingSystConfig.fType << ") + " << fNominalConfig.fInput << " : [" << tPlottingSystConfig.fMunoSrc << ", " << tPlottingSystConfig.fJetSrc << ", " << tPlottingSystConfig.fIter << "]  to " << tPlottingSystConfig.fPath << " -> " << tPlottingSystConfig.fTarget << std::endl;

  for (const auto& tPlottingSystConfig : fPlottingSystConfigsSelection)
    std::cout << " - " << tPlottingSystConfig.fName << " (" << tPlottingSystConfig.fType << ") + " << fNominalConfig.fInput << " : [" << tPlottingSystConfig.fMunoSrc << ", " << tPlottingSystConfig.fJetSrc << ", " << tPlottingSystConfig.fIter << "]  to " << tPlottingSystConfig.fPath << " -> " << tPlottingSystConfig.fTarget << std::endl;


  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;
}

void HistoBooker::FillEventInfo(const int nBinIdx, const float fWeight) {
  fHistoCollection[fEra + ":" + fSampleOrigin + ":EventInfo:dummy"]->Fill(nBinIdx, fWeight);
}

void HistoBooker::Fill(EventContentPool& pool) {

  fWeightCalc->ResetWeight();

  float tWeightReco = 1.;
  float tWeightGen = 1.;

  EfficiencyFactory::WeightInput tWeightInput{fNominalConfig.fMunoSrc, fNominalConfig.fJetSrc};

  if (fIsMC) {
    tWeightGen = fWeightCalc->GetWeight(pool, tWeightInput, "GenWeight");
    tWeightGen *= fWeightCalc->GetWeight(pool, tWeightInput, "Normalization");
    tWeightReco = fWeightCalc->GetWeight(pool, tWeightInput, "Reco");
  }
  
  FillHisto(pool, fNominalConfig.fPath, tWeightInput, tWeightGen, tWeightReco, false, 0);

  for (const auto& tPlottingSystConfig : fPlottingSystConfigsVariation) {
    std::string tMuonName = tPlottingSystConfig.fMunoSrc;
    std::string tJetName = tPlottingSystConfig.fJetSrc;
    std::string tTarget = tPlottingSystConfig.fTarget;
    std::string tInput = tPlottingSystConfig.fInput;
    int tIter = tPlottingSystConfig.fIter;
    
    for (int i = 1; i <= tIter; i++) {
      float tWeightGenSyst = 1.;
      float tWeightRecoSyst = 1.;
      
      if (tTarget == "GenWeight") tWeightGenSyst = fWeightCalc->GetWeight(pool, EfficiencyFactory::WeightInput{tMuonName, tJetName}, "GenWeight", tInput, i);
      else if (tTarget == "Normalization") tWeightGenSyst = fWeightCalc->GetWeight(pool, EfficiencyFactory::WeightInput{tMuonName, tJetName}, "Normalization", tInput, i);
      else tWeightRecoSyst = fWeightCalc->GetWeight(pool, EfficiencyFactory::WeightInput{tMuonName, tJetName}, tTarget, tInput, i);

      FillHisto(pool, tPlottingSystConfig.fPath, EfficiencyFactory::WeightInput{tMuonName, tJetName}, tWeightGen * tWeightGenSyst, tWeightReco * tWeightRecoSyst, true, i);
    }
  }

  for (const auto& tPlottingSystConfig : fPlottingSystConfigsSelection) {
  
    std::string tMuonName = tPlottingSystConfig.fMunoSrc;
    std::string tJetName = tPlottingSystConfig.fJetSrc;

    for (int i = 1; i <= tPlottingSystConfig.fIter; i++) {

      fWeightCalc->ResetWeight();

      std::string tMuonNameSyst = tMuonName;
      std::string tJetNameSyst = tJetName;

      if (tPlottingSystConfig.fType == "JetSelection") tJetNameSyst = tJetName + "_" + std::to_string(i);
      if (tPlottingSystConfig.fType == "MuonSelection") tMuonNameSyst = tMuonName + "_" + std::to_string(i);
      
      float tWeightRecoSelectionSyst = fWeightCalc->GetWeight(pool, EfficiencyFactory::WeightInput{tMuonNameSyst, tJetNameSyst}, "Reco");

      FillHisto(pool, tPlottingSystConfig.fPath, EfficiencyFactory::WeightInput{tMuonNameSyst, tJetNameSyst}, tWeightGen, tWeightRecoSelectionSyst, true, i);
    }
  }
}

void HistoBooker::FillHisto(
  EventContentPool& pool, 
  const std::string& tPath, 
  const EfficiencyFactory::WeightInput& tWeightInput, 
  float tWeightGen, 
  float tWeightReco, 
  bool tIsSyst, 
  int tIter) {

  const auto& tEventPass = pool.Get<BaselineInfo>("BaselineInfo");
  const auto& tMuonPair = pool.Get<MUON::MuonPair>(tWeightInput.fMuonSrc);
  const auto& tJetCollection = pool.Get<JET::JetsCollection>(tWeightInput.fJetSrc);

  int nJet = tJetCollection.nJet;
  int nBJet = tJetCollection.nBJet;
  float tMass = tMuonPair.fMass;
  float tMassCut = SetMassOutflow(tMass);
  int nJetNode = nJet > 2 ? 2 : nJet;
  int nMassBinIdx = fMassBinning->FindNode(fJetNodeNames[nJetNode].c_str())->GetGlobalBinNumber(tMassCut);

  bool tPassingOfflineSelection = tEventPass.fPassingTrigger && tEventPass.fPassingNoiseFilter && (nBJet == 0) && (tMass > 0.) && (tMass < 4000.) && (tMuonPair.nMuon == 2);

  if (!tIsSyst) { 
    if (tPassingOfflineSelection) fHistoCollection[tPath + ":dimuonMass"]->Fill(nMassBinIdx, tWeightGen * tWeightReco);
  } else {
    if (tPassingOfflineSelection) static_cast<TH2*>(fHistoCollection[tPath + ":dimuonMass"].get())->Fill(nMassBinIdx, tIter, tWeightGen * tWeightReco);
  }

  // std::cout << tJetCollection.nJet << " " << nJet << " " << nJetNode << " " << nBJet << " " << std::endl;
  // std::cout << tMuonPair.fMass << " " << tMassCut << " " << nMassBinIdx << " " << std::endl;
  // std::cout << tPassingOfflineSelection << " " << tWeightGen << " " << tWeightReco << std::endl;
  // std::cout << "----------------------------------------------------------------------" << std::endl;

  if (fSample.find("NNLO_MUMU") != std::string::npos) {
    
    const auto& tGenMuon = pool.Get<GEN::DressedMuon>("DressedMuons");
    const auto& tGenJet = pool.Get<GEN::Jets>("GenJets");

    int nGenJet = tGenJet.size() > 2 ? 2 : tGenJet.size();
    float tGenMass = tGenMuon.fMass;
    float tGenMassCut = SetMassOutflow(tGenMass);
    int nGenMassBinIdx = fMassBinning->FindNode(fJetNodeNames[nGenJet].c_str())->GetGlobalBinNumber(tGenMassCut);

    bool tPassingGenSelection = tGenMuon.nMuon == 2 && tGenMass > 0. && tGenMass < 4000.;

    // std::cout << tPath << std::endl;
    // std::cout << tJetCollection.nJet << " " << nJet << " " << nJetNode << " " << nBJet << " " << tGenJet.size() << " " << nGenJet << " " << nGenJet << std::endl;
    // std::cout << tMuonPair.fMass << " " << tMassCut << " " << nMassBinIdx << " " << std::endl;
    // std::cout << tGenMuon.fMass << " " << tGenMassCut << " " << nGenMassBinIdx << " " << std::endl;
    // std::cout << tPassingOfflineSelection << " " << tPassingGenSelection << " " << tWeightGen << " " << tWeightReco << std::endl;
    // std::cout << "----------------------------------------------------------------------" << std::endl;


    if (!tIsSyst) {
      if (tPassingGenSelection) fHistoCollection[tPath + ":dimuonMassGen"]->Fill(nGenMassBinIdx, tWeightGen);
      if (!tPassingGenSelection && tPassingOfflineSelection) fHistoCollection[tPath + ":dimuonMassFake"]->Fill(nMassBinIdx, tWeightGen * tWeightReco);
      
      if (tPassingGenSelection && tPassingOfflineSelection) {
        static_cast<TH2*>(fHistoCollection[tPath + ":h_ResponseMatrix"].get())->Fill(nMassBinIdx, nGenMassBinIdx, tWeightGen * tWeightReco); // nominal
        if (tWeightReco != 1.) static_cast<TH2*>(fHistoCollection[tPath + ":h_ResponseMatrix"].get())->Fill(0., nGenMassBinIdx, tWeightGen * (1. - tWeightReco));
      }
      if (tPassingGenSelection && !tPassingOfflineSelection) {
        static_cast<TH2*>(fHistoCollection[tPath + ":h_ResponseMatrix"].get())->Fill(0., nGenMassBinIdx, tWeightGen);
      }
    } else {
      if (tPassingGenSelection) static_cast<TH2*>(fHistoCollection[tPath + ":dimuonMassGen"].get())->Fill(nGenMassBinIdx, tIter, tWeightGen);
      if (!tPassingGenSelection && tPassingOfflineSelection) static_cast<TH2*>(fHistoCollection[tPath + ":dimuonMassFake"].get())->Fill(nMassBinIdx, tIter, tWeightGen * tWeightReco);
      
      if (tPassingGenSelection && tPassingOfflineSelection) {
        static_cast<TH3*>(fHistoCollection[tPath + ":h_ResponseMatrix"].get())->Fill(nMassBinIdx, nGenMassBinIdx, tIter, tWeightGen * tWeightReco); // nominal
        if (tWeightReco != 1.) static_cast<TH3*>(fHistoCollection[tPath + ":h_ResponseMatrix"].get())->Fill(0., nGenMassBinIdx, tIter, tWeightGen * (1. - tWeightReco));
      }
      if (tPassingGenSelection &&!tPassingOfflineSelection) {
        static_cast<TH3*>(fHistoCollection[tPath + ":h_ResponseMatrix"].get())->Fill(0., nGenMassBinIdx, tIter, tWeightGen);
      }
    }
  }
}

float HistoBooker::SetMassOutflow(float tMass) {
  if (tMass < 200.) return 190.;
  if (tMass > 4000.) return 4010.;

  return tMass;
}

void HistoBooker::Write() {
  
  std::cout << " " << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " Writing histograms...  ";
  

  auto GetDir = [](TDirectory* fParent, const TString& fName) {
    
    TDirectory* fDir = fParent->GetDirectory(fName.Data());
    if (!fDir) fDir = fParent->mkdir(fName.Data());
    if (!fDir) throw std::runtime_error("Cannot create output directory");
    
    return fDir;
  };

  TFile* fOutputFile = new TFile(fOutputName.c_str(), "RECREATE");
  fOutputFile->cd();

  TDirectory* fDir = nullptr;
  for (const auto& tHisto : fHistoCollection) {
    
    fDir = fOutputFile;

    const auto& tChain = GetSplitString(tHisto.first);
    for (unsigned int i = 0; i < tChain.size() - 1; i++)
      fDir = GetDir(fDir, tChain.at(i));

    fDir->cd();
    tHisto.second->Write();
  }

  // if (!fIsMC) {
  //   for (const auto& tHisto : fHistoCollection) {
    
  //     fDir = fOutputFile;
  
  //     std::vector<std::string> tChain = GetSplitString(tHisto.first);    
  //     tChain.at(1) = "Data";
  //     for (unsigned int i = 0; i < tChain.size() - 1; i++)
  //       fDir = GetDir(fDir, tChain.at(i));
  
  //     fDir->cd();
  //     tHisto.second->Write();
  //   }
  // }

  fOutputFile->Close();

  std::cout << " DONE! " << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl; 
}

std::vector<std::string> HistoBooker::GetSplitString(const std::string& str) {

  std::vector<std::string> result;
  
  std::stringstream ss(str);
  std::string item;
  while (std::getline(ss, item, ':'))
    result.push_back(item);

  return result;
}