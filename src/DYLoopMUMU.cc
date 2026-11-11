#include "DYLoopMUMU.h"
#include "EventProducts.h"
#include "NT.h"
#include "ProducerMaker.h"
#include "options.h"

#include "TString.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

std::string TimeProgress::getCurrentTimeString() {
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
  
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
  ss << "." << std::setfill('0') << std::setw(3) << ms.count();
  return ss.str();
}

std::string TimeProgress::formatDuration(const std::chrono::high_resolution_clock::duration& tDuration) {
  auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tDuration).count();
  auto minutes = total_ms / 60000;
  auto seconds = (total_ms % 60000) / 1000;
  auto milliseconds = total_ms % 1000;
  
  std::stringstream ss;
  if (minutes > 0)
    ss << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
  else
    ss << "0:" << std::setfill('0') << std::setw(2) << seconds;

  return ss.str();
}

void TimeProgress::Print(int tCurrent) {

  auto tCurrentTime = std::chrono::system_clock::now();
  auto tElapsed = tCurrentTime - fBegin;
  double tProgressPercent = 100. * tCurrent / fMaxEvents;
  
  auto tEstimatedTotal = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
      tElapsed * (100.0 / tProgressPercent)
  );
  
  std::string tElapsedStr = formatDuration(std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(tElapsed));
  std::string tEstimatedStr = formatDuration(tEstimatedTotal);
  
  std::string tProgress = Form("Loop: %d / %lld | %.2f %% | %s / %s",
                                tCurrent, fMaxEvents, tProgressPercent,
                               tElapsedStr.c_str(), tEstimatedStr.c_str());
  
  std::cout << tProgress << std::endl;
}

DYLoopMUMU::DYLoopMUMU(options* fOpt) {
  
  fOpt->GetVariable("config", &fConfigPath);
  fOpt->GetVariable("id", &fJobID);
  fOpt->GetVariable("sample", &fSampleName);
  fOpt->GetVariable("OutputDir", &fOutputDir);

  if (fOutputDir == "null") fOutputDir = ".";

  fConfig = YAML::LoadFile(fConfigPath);
  
  std::string fEra = fConfig["Info"]["Era"].as<std::string>();

  YAML::Node fConfigSample = YAML::LoadFile(std::string("../../Input/dataset.yml"));
  fIsMC = fConfigSample[fEra][fSampleName]["IsMC"].as<bool>();

  std::uint32_t fSeedForMuon = MakeRandomSeed(fEra, fSampleName, std::to_string(fJobID), "Muon");
  std::uint32_t fSeedForJet = MakeRandomSeed(fEra, fSampleName, std::to_string(fJobID), "Jet");
  std::uint32_t fSeedForMuonSmearing = MakeRandomSeed(fEra, fSampleName, std::to_string(fJobID), "MuonSmearing");

  std::unique_ptr<NT> tNtuples = std::make_unique<NT>(fEra, fIsMC, fSampleName, fJobID);
  fMaxEntries = tNtuples->GetEntries();

  fPool = std::make_unique<EventContentPool>(std::move(tNtuples));
  fPool->SetEventInfo(EventInfo{fEra, fSampleName, fJobID, fOutputDir, fIsMC, fSeedForMuon, fSeedForJet, fSeedForMuonSmearing});
  fPool->SetTrigger(fConfig["Info"]["Trigger"].as<std::vector<std::string>>());
  fPool->SetNoiseFilter();
  
  fTimeProgress = std::make_unique<TimeProgress>();
  fTimeProgress->Init(fMaxEntries);

  std::unordered_map<std::string, const YAML::Node> tSystConfigs;
  for (const auto& tSystConfig : fConfig["Systematics"])
    tSystConfigs.emplace(tSystConfig["Name"].as<std::string>(), tSystConfig);

  fPool->RegisterSystematics(std::move(tSystConfigs));

  for (const auto& tProducerConfig : fConfig["ObjectProducer"])
    fProducers.push_back(MakeProducer(tProducerConfig));
  
  for (auto& tProducer : fProducers)
    tProducer->Init(*fPool);

  const auto& tSystematics = fPool->GetSystematics();

  std::cout << "######################################################################" << std::endl;
  std::cout << " Loop Information " << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " Config Path: " << fConfigPath << std::endl;
  std::cout << " Era: " << fEra << std::endl;
  std::cout << " Sample Name: " << fSampleName << std::endl;
  std::cout << " Is MC: " << fIsMC << std::endl;
  std::cout << " Job ID: " << fJobID << std::endl;
  std::cout << " Max Entries: " << fMaxEntries << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;

  std::cout << "######################################################################" << std::endl;
  std::cout << " Producers Information " << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  for (const auto& fProducer : fProducers)
    std::cout << " - " << fProducer->Name() << ": " << fProducer->InName() << " -> " << fProducer->OutputName() << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;

  std::cout << "######################################################################" << std::endl;
  std::cout << " Systematics Information " << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  for (const auto& tSyst : tSystematics)
    std::cout << " - " << tSyst.second["Name"].as<std::string>() << ": " << tSyst.second["SystType"].as<std::string>() << " - " << tSyst.second["Iter"].as<int>()<< " Iter" << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;
}

void DYLoopMUMU::Loop() {

  long long tMaxLoop = 0;
  while (fPool->Next()) { // Event loop starts here
    tMaxLoop++;

    if (tMaxLoop % 10000 == 0) fTimeProgress->Print(tMaxLoop);

    if (tMaxLoop == 10) break;

    for (const auto& fProducer : fProducers)
      fProducer->Produce(*fPool);

    if (fPool->GetEventInfo().sampleName == "NNLO_MUMU_inc" && fIsMC) {
      const auto& tLHEContent = fPool->Get<GEN::LHEContent>("LHECollection");
      if (tLHEContent.fMass > 100) continue;
    }

    const NT& nt = fPool->Source();

    bool tPassingTrigger = fPool->PassingTrigger();
    bool tPassingNoiseFilter = fPool->PassingNoiseFilter();

    fPool->Put("BaselineInfo", BaselineInfo{tPassingTrigger, tPassingNoiseFilter});


  }
}
