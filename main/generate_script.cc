#include "options.h"

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <filesystem>
#include <cstdlib>
#include <stdexcept>
#include <array>
#include <fstream>

namespace fs = std::filesystem;
static std::map<std::string, std::map<std::string, std::map<std::string, int>>> InputMap = {
  {
    {
      "MUMU", {
      {
        "2016_preVFP", 
        {
          {"Data", 1},
          {"MC", 1}
        }
      },
      {
        "2016_postVFP",
        {
          {"Data", 1},
          {"MC", 1}
        }
      },
      {
        "2017",
        {
          {"Data", 1},
          {"MC", 1}
        }
      },
      {
        "2018",
        {
          {"Data", 1},
          {"MC", 1}
        }
      }
    }
    },
    {
      "EE", {
        {
          "2016_preVFP", {
            {"Data", 1},
            {"MC", 1}
          }
        },
        {
          "2016_postVFP", {
            {"Data", 1},
            {"MC", 1}
          }
        },
        {
          "2017", {
            {"Data", 1},
            {"MC", 1}
          }
        },
        {
          "2018", {
            {"Data", 1},
            {"MC", 1}
          }
        }
      }
    }
  }
};

int main(int argc, char* argv[]) {

  options* fOpt = new options(argc, argv);

  std::string fBaseDir = "";
  fOpt->GetVariable("base", &fBaseDir);
  if (fBaseDir == "") {
    std::cout << "Base directory is not set" << std::endl;
    return -1;
  }
  
  std::string fChannel = "";
  fOpt->GetVariable("channel", &fChannel);
  if (fChannel == "") {
    std::cout << "Channel is not set: EMU, MUMU, EE" << std::endl;
    return -1;
  }

  std::string fChannelTemp = fChannel;
  if (fChannelTemp == "EMU") fChannelTemp = "MUMU";


  const char* fWorkspaceBatch = getenv("DY_HIGHMASS_WORKSPACE_BATCH");
  if (fWorkspaceBatch == nullptr) {
    std::cout << "ENV set is needed before running the script" << std::endl;
    return -1;
  }
  std::string fWorkspaceBatchStr = std::string(fWorkspaceBatch);

  const char* fWorkspace = getenv("DY_HIGHMASS_WORKSPACE");
  if (fWorkspace == nullptr) {
    std::cout << "ENV set is needed before running the script" << std::endl;
    return -1;
  }
  std::string fWorkspaceStr = std::string(fWorkspace);

  std::string fBaseDirStr = fWorkspaceBatchStr + "/" + std::string(fBaseDir);
  fs::path fBaseDirFS(fBaseDirStr.data());
  if( !(fs::exists(fBaseDirFS)) ) fs::create_directory(fBaseDirFS);

  std::string fOutputDirStr = fBaseDirStr + "/ROOT";
  fs::path fOutputDirFS(fOutputDirStr.data());
  if( !(fs::exists(fOutputDirFS)) ) fs::create_directory(fOutputDirFS);

  std::string fLogDirStr = fBaseDirStr + "/log";
  fs::path fLogDirFS(fLogDirStr.data());
  if( !(fs::exists(fLogDirFS)) ) fs::create_directory(fLogDirFS);

  std::string fCondorSubmit = R"(universe              = vanilla
executable            = condor_wrapper.sh
getenv                = True
arguments             = analysis_)" + fChannel + R"( --config $(config) --era $(era) --sample $(sample) --id $(id)

request_memory        = 500 MB
should_transfer_files = YES
transfer_input_files = )" + fWorkspaceStr + R"(/envset.sh, \
                       )" + fWorkspaceStr + R"(/install/lib, \
                       )" + fWorkspaceStr + R"(/install/bin

JobBatchName          = )" + fBaseDir + R"(
+JobType = "long"

output                = log/$(era)_$(sample)_$(id).out
error                 = log/$(era)_$(sample)_$(id).err
log                   = log/$(era)_$(sample)_$(id).log

queue config,era,sample,id from joblist.txt
)";

  std::string fCondorSubmitStr = fBaseDirStr + "/condor_submit.sub";
  std::ofstream fCondorSubmitStream(fCondorSubmitStr);
  if (fCondorSubmitStream.is_open()) {
    fCondorSubmitStream << fCondorSubmit;
    fCondorSubmitStream.close();
  } else {
    std::cout << "Failed to create config file: " << fCondorSubmitStr << std::endl;
    return -1;
  }

  std::string fCondorWrapper = R"(#!/bin/sh

echo "$@"
cd )" + fWorkspaceStr + R"(

source /cvmfs/sft.cern.ch/lcg/views/LCG_107/x86_64-el9-gcc13-opt/setup.sh
export INSTALL_DIR_PATH=$PWD/install

export PATH=$PATH:$INSTALL_DIR_PATH/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_DIR_PATH/lib
export YAMLPATH=/cvmfs/sft.cern.ch/lcg/releases/yamlcpp/0.6.3-d05b2/x86_64-el9-gcc13-opt/lib
export CORRECTIONLIBPATH=/cvmfs/sft.cern.ch/lcg/releases/correctionlib/2.6.4-b8584/x86_64-el9-gcc13-opt/lib

export DY_HIGHMASS_WORKSPACE=$PWD
export DY_HIGHMASS_WORKSPACE_INSTALL=$PWD/install
export DY_HIGHMASS_WORKSPACE_BATCH=$PWD/Batch

cd )" + fBaseDirStr + R"(

echo $PWD

echo "$@"
eval "$@"
)";

  std::string fConfigWrapperStr = fBaseDirStr + "/condor_wraper.sh";
  std::ofstream fConfigWrapperStream(fConfigWrapperStr);
  if (fConfigWrapperStream.is_open()) {
    fConfigWrapperStream << fCondorWrapper;
    fConfigWrapperStream.close();
  } else {
    std::cout << "Failed to create config file: " << fConfigWrapperStr << std::endl;
    return -1;
  }

  std::string fJobList = "";

  for (auto tEra : InputMap[fChannel]) {
    for (auto tSample : tEra.second) {
      for (int i = 0; i < tSample.second; i++)
        fJobList += R"(../../config/)" + fChannelTemp + R"(/UL)" + tEra.first + R"(.yml )" + tEra.first + R"( )" + tSample.first + R"( )" + std::to_string(i) + "\n";
    }
  }

  std::string fJobListStr = fBaseDirStr + "/joblist.txt";
  std::ofstream fJobListStream(fJobListStr);
  if (fJobListStream.is_open()) {
    fJobListStream << fJobList;
    fJobListStream.close();
  } else {
    std::cout << "Failed to create config file: " << fJobListStr << std::endl;
    return -1;
  }

  return 1;
}