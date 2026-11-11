#ifndef DYLoopMUMU_h
#define DYLoopMUMU_h 1

#include "EventContentPool.h"
#include "Producer.h"

#include "yaml-cpp/yaml.h"

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

class options;

class TimeProgress
{
public:
  
  TimeProgress() {};
  ~TimeProgress() {};

  std::string getCurrentTimeString();
  std::string formatDuration(const std::chrono::high_resolution_clock::duration& tDuration);
  void Print(int tCurrent);

  void Init(long long tMaxEvents) { 
    fBegin = std::chrono::system_clock::now(); 
    fMaxEvents = tMaxEvents; 
  }

private:
  std::chrono::time_point<std::chrono::system_clock> fBegin;
  long long fMaxEvents;
};

class DYLoopMUMU
{
public:

  DYLoopMUMU(options* fOpt);

  void EndOfJob() {}

  void Loop();


private:

  static std::uint32_t MakeRandomSeed(
    const std::string& fEra, 
    const std::string& fSampleName, 
    const std::string& fJobID, 
    const std::string& fSuffix
  ) {
    std::string tKey = fEra + ":" + fSampleName + ":" + fJobID + ":" + fSuffix;

    std::uint32_t tHash = 2166136261u;
    for (const unsigned char tCharacter : tKey) {
      tHash ^= tCharacter;
      tHash *= 16777619u;
    }

    return tHash == 0 ? 1u : tHash;
  }

  YAML::Node fConfig;

  std::string fConfigPath;
  int fJobID;
  std::string fSampleName;
  std::string fOutputDir;
  bool fIsMC;

  std::unique_ptr<EventContentPool> fPool;
  std::unique_ptr<TimeProgress> fTimeProgress;
  
  ProducerList fProducers;
  std::unordered_map<std::string, std::unique_ptr<Producer>> fIndivProducers;

  double fMaxEntries;
};

#endif
