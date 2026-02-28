#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cassert>
#include <cctype>

#include "NT.h"
#include "options.h"
#include "DYLoopMUMU.h"

#include <iostream>
#include <string>

#include "TFile.h"
#include "TROOT.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TStopwatch.h"
#include "TChain.h"
#include "TLorentzVector.h"

#include "yaml-cpp/yaml.h"

#include <filesystem>

// #include "time.h"


static std::map<std::string, std::map<std::string, std::map<std::string, std::vector<std::string>>>> InputMap = {
  {
    {
      "MUMU", {
        {
          "2016_preVFP", 
          {
            {"Data", {
              "Run2016B_SingleMuon_v2",
              "Run2016C_SingleMuon",
              "Run2016D_SingleMuon",
              "Run2016E_SingleMuon",
              "Run2016F_SingleMuon"
            }},
            {"MC", {
              "NNLO_MUMU_10to50",
              "NNLO_MUMU_10to50_v2",
              "NNLO_MUMU_inc",
              "NNLO_MUMU_100to200",
              "NNLO_MUMU_200to400",
              "NNLO_MUMU_400to500",
              "NNLO_MUMU_500to700",
              "NNLO_MUMU_700to800",
              "NNLO_MUMU_800to1000",
              "NNLO_MUMU_1000to1500",
              "NNLO_MUMU_1500to2000",
              "NNLO_MUMU_2000toInf",
              "NNLO_tautau",
              "ST_s",
              "ST_t_AntiTop",
              "ST_t_Top",
              "ST_tW_AntiTop",
              "ST_tW_Top",
              "TTTo2L2Nu",
              "WW",
              "WZ",
              "ZZ",
              "GGToMuMu_10to30_ElEl",
              "GGToMuMu_10to30_InelElElInel",
              "GGToMuMu_10to30_InelInel",
              "GGToMuMu_30to50_ElEl",
              "GGToMuMu_30to50_InelElElInel",
              "GGToMuMu_30to50_InelInel",
              "GGToMuMu_1500toInf_ElEl",
              "GGToMuMu_1500toInf_InelElElInel",
              "GGToMuMu_1500toInf_InelInel",
              "GGToMuMu_200to1500_ElEl",
              "GGToMuMu_200to1500_InelElElInel",
              "GGToMuMu_200to1500_InelInel",
              "GGToMuMu_50to200_ElEl",
              "GGToMuMu_50to200_InelElElInel",
              "GGToMuMu_50to200_InelInel",
            }}
          }
        },
        {
          "2016_postVFP",
          {
            {"Data", {
              "Run2016F_SingleMuon",
              "Run2016G_SingleMuon",
              "Run2016H_SingleMuon"
            }},
            {"MC", {
              "NNLO_MUMU_10to50",
              "NNLO_MUMU_10to50_v2",
              "NNLO_MUMU_inc",
              "NNLO_MUMU_100to200",
              "NNLO_MUMU_200to400",
              "NNLO_MUMU_400to500",
              "NNLO_MUMU_500to700",
              "NNLO_MUMU_700to800",
              "NNLO_MUMU_800to1000",
              "NNLO_MUMU_1000to1500",
              "NNLO_MUMU_1500to2000",
              "NNLO_MUMU_2000toInf",
              "NNLO_tautau",
              "ST_s",
              "ST_t_AntiTop",
              "ST_t_Top",
              "ST_tW_AntiTop",
              "ST_tW_Top",
              "TTTo2L2Nu",
              "WW",
              "WZ",
              "ZZ",
              "GGToMuMu_10to30_ElEl",
              "GGToMuMu_10to30_InelElElInel",
              "GGToMuMu_10to30_InelInel",
              "GGToMuMu_30to50_ElEl",
              "GGToMuMu_30to50_InelElElInel",
              "GGToMuMu_30to50_InelInel",
              "GGToMuMu_1500toInf_ElEl",
              "GGToMuMu_1500toInf_InelElElInel",
              "GGToMuMu_1500toInf_InelInel",
              "GGToMuMu_200to1500_ElEl",
              "GGToMuMu_200to1500_InelElElInel",
              "GGToMuMu_200to1500_InelInel",
              "GGToMuMu_50to200_ElEl",
              "GGToMuMu_50to200_InelElElInel",
              "GGToMuMu_50to200_InelInel",
            }}
          }
        },
        {
          "2017",
          {
            {"Data", {
              "Run2017B_SingleMuon",
              "Run2017C_SingleMuon",
              "Run2017D_SingleMuon",
              "Run2017E_SingleMuon",
              "Run2017F_SingleMuon"
            }},
            {"MC", {
              "NNLO_MUMU_10to50",
              "NNLO_MUMU_10to50_v2",
              "NNLO_MUMU_inc",
              "NNLO_MUMU_100to200",
              "NNLO_MUMU_200to400",
              "NNLO_MUMU_400to500",
              "NNLO_MUMU_500to700",
              "NNLO_MUMU_700to800",
              "NNLO_MUMU_800to1000",
              "NNLO_MUMU_1000to1500",
              "NNLO_MUMU_1500to2000",
              "NNLO_MUMU_2000toInf",
              "NNLO_tautau",
              "ST_s",
              "ST_t_AntiTop",
              "ST_t_Top",
              "ST_tW_AntiTop",
              "ST_tW_Top",
              "TTTo2L2Nu",
              "WW",
              "WZ",
              "ZZ",
              "GGToMuMu_10to30_ElEl",
              "GGToMuMu_10to30_InelElElInel",
              "GGToMuMu_10to30_InelInel",
              "GGToMuMu_30to50_ElEl",
              "GGToMuMu_30to50_InelElElInel",
              "GGToMuMu_30to50_InelInel",
              "GGToMuMu_1500toInf_ElEl",
              "GGToMuMu_1500toInf_InelElElInel",
              "GGToMuMu_1500toInf_InelInel",
              "GGToMuMu_200to1500_ElEl",
              "GGToMuMu_200to1500_InelElElInel",
              "GGToMuMu_200to1500_InelInel",
              "GGToMuMu_50to200_ElEl",
              "GGToMuMu_50to200_InelElElInel",
              "GGToMuMu_50to200_InelInel",
            }}
          }
        },
        {
          "2018",
          {
            {"Data", {
              "Run2018A_SingleMuon",
              "Run2018B_SingleMuon",
              "Run2018C_SingleMuon",
              "Run2018D_SingleMuon"
            }},
            {"MC", {
              "NNLO_MUMU_10to50",
              "NNLO_MUMU_10to50_v2",
              "NNLO_MUMU_inc",
              "NNLO_MUMU_100to200",
              "NNLO_MUMU_200to400",
              "NNLO_MUMU_400to500",
              "NNLO_MUMU_500to700",
              "NNLO_MUMU_700to800",
              "NNLO_MUMU_800to1000",
              "NNLO_MUMU_1000to1500",
              "NNLO_MUMU_1500to2000",
              "NNLO_MUMU_2000toInf",
              "NNLO_tautau",
              "ST_s",
              "ST_t_AntiTop",
              "ST_t_Top",
              "ST_tW_AntiTop",
              "ST_tW_Top",
              "TTTo2L2Nu",
              "WW",
              "WZ",
              "ZZ",
              "GGToMuMu_10to30_ElEl",
              "GGToMuMu_10to30_InelElElInel",
              "GGToMuMu_10to30_InelInel",
              "GGToMuMu_30to50_ElEl",
              "GGToMuMu_30to50_InelElElInel",
              "GGToMuMu_30to50_InelInel",
              "GGToMuMu_1500toInf_ElEl",
              "GGToMuMu_1500toInf_InelElElInel",
              "GGToMuMu_1500toInf_InelInel",
              "GGToMuMu_200to1500_ElEl",
              "GGToMuMu_200to1500_InelElElInel",
              "GGToMuMu_200to1500_InelInel",
              "GGToMuMu_50to200_ElEl",
              "GGToMuMu_50to200_InelElElInel",
              "GGToMuMu_50to200_InelInel",
            }}
          }
        }
      }
    },
    {
      "EE", {
        {
          "2016_preVFP", 
          {
            {"Data", {
              "Run2016B_DoubleEG_v2",
              "Run2016C_DoubleEG",
              "Run2016D_DoubleEG",
              "Run2016E_DoubleEG",
              "Run2016F_DoubleEG",
            }},
            {"MC", {
              "NNLO_EE_10to50",
              "NNLO_EE_inc",
              "NNLO_EE_100to200",
              "NNLO_EE_200to400",
              "NNLO_EE_400to500",
              "NNLO_EE_500to700",
              "NNLO_EE_700to800",
              "NNLO_EE_800to1000",
              "NNLO_EE_1000to1500",
              "NNLO_EE_1500to2000",
              "NNLO_EE_2000toInf",
              "NNLO_tautau",
              "ST_s",
              "ST_t_AntiTop",
              "ST_t_Top",
              "ST_tW_AntiTop",
              "ST_tW_Top",
              "TTTo2L2Nu",
              "WW",
              "WZ",
              "ZZ",
              "GGToEE_10to30_ElEl",
              "GGToEE_10to30_InelElElInel",
              "GGToEE_10to30_InelInel",
              "GGToEE_30to50_ElEl",
              "GGToEE_30to50_InelElElInel",
              "GGToEE_30to50_InelInel",
              "GGToEE_1500toInf_ElEl",
              "GGToEE_1500toInf_InelElElInel",
              "GGToEE_1500toInf_InelInel",
              "GGToEE_200to1500_ElEl",
              "GGToEE_200to1500_InelElElInel",
              "GGToEE_200to1500_InelInel",
              "GGToEE_50to200_InelElElInel",
              "GGToEE_50to200_InelInel",
              "GGToEE_50to200_ElEl",
            }}
          }
        },
        {
          "2016_postVFP", 
          {
            {"Data", {
              "Run2016F_DoubleEG",
              "Run2016G_DoubleEG",
              "Run2016H_DoubleEG",
            }},
            {"MC", {
              "NNLO_EE_10to50",
              "NNLO_EE_inc",
              "NNLO_EE_100to200",
              "NNLO_EE_200to400",
              "NNLO_EE_400to500",
              "NNLO_EE_500to700",
              "NNLO_EE_700to800",
              "NNLO_EE_800to1000",
              "NNLO_EE_1000to1500",
              "NNLO_EE_1500to2000",
              "NNLO_EE_2000toInf",
              "NNLO_tautau",
              "ST_s",
              "ST_t_AntiTop",
              "ST_t_Top",
              "ST_tW_AntiTop",
              "ST_tW_Top",
              "TTTo2L2Nu",
              "WW",
              "WZ",
              "ZZ",
              "GGToEE_10to30_ElEl",
              "GGToEE_10to30_InelElElInel",
              "GGToEE_10to30_InelInel",
              "GGToEE_30to50_ElEl",
              "GGToEE_30to50_InelElElInel",
              "GGToEE_30to50_InelInel",
              "GGToEE_1500toInf_ElEl",
              "GGToEE_1500toInf_InelElElInel",
              "GGToEE_1500toInf_InelInel",
              "GGToEE_200to1500_ElEl",
              "GGToEE_200to1500_InelElElInel",
              "GGToEE_200to1500_InelInel",
              "GGToEE_50to200_InelElElInel",
              "GGToEE_50to200_InelInel",
              "GGToEE_50to200_ElEl",
            }}
          }
        },
        {
          "2017", 
          {
            {"Data", {
              "Run2017B_DoubleEG",
              "Run2017C_DoubleEG",
              "Run2017D_DoubleEG",
              "Run2017E_DoubleEG",
              "Run2017F_DoubleEG",
            }},
            {"MC", {
              "NNLO_EE_10to50",
              "NNLO_EE_inc",
              "NNLO_EE_100to200",
              "NNLO_EE_200to400",
              "NNLO_EE_400to500",
              "NNLO_EE_500to700",
              "NNLO_EE_700to800",
              "NNLO_EE_800to1000",
              "NNLO_EE_1000to1500",
              "NNLO_EE_1500to2000",
              "NNLO_EE_2000toInf",
              "NNLO_tautau",
              "ST_s",
              "ST_t_AntiTop",
              "ST_t_Top",
              "ST_tW_AntiTop",
              "ST_tW_Top",
              "TTTo2L2Nu",
              "WW",
              "WZ",
              "ZZ",
              "GGToEE_10to30_ElEl",
              "GGToEE_10to30_InelElElInel",
              "GGToEE_10to30_InelInel",
              "GGToEE_30to50_ElEl",
              "GGToEE_30to50_InelElElInel",
              "GGToEE_30to50_InelInel",
              "GGToEE_1500toInf_ElEl",
              "GGToEE_1500toInf_InelElElInel",
              "GGToEE_1500toInf_InelInel",
              "GGToEE_200to1500_ElEl",
              "GGToEE_200to1500_InelElElInel",
              "GGToEE_200to1500_InelInel",
              "GGToEE_50to200_InelElElInel",
              "GGToEE_50to200_InelInel",
              "GGToEE_50to200_ElEl",
            }}
          }
        },
        {
          "2018", 
          {
            {"Data", {
              "Run2018A_EGamma",
              "Run2018B_EGamma",
              "Run2018C_EGamma",
              "Run2018D_EGamma"
            }},
            {"MC", {
              "NNLO_EE_10to50",
              "NNLO_EE_inc",
              "NNLO_EE_100to200",
              "NNLO_EE_200to400",
              "NNLO_EE_400to500",
              "NNLO_EE_500to700",
              "NNLO_EE_700to800",
              "NNLO_EE_800to1000",
              "NNLO_EE_1000to1500",
              "NNLO_EE_1500to2000",
              "NNLO_EE_2000toInf",
              "NNLO_tautau",
              "ST_s",
              "ST_t_AntiTop",
              "ST_t_Top",
              "ST_tW_AntiTop",
              "ST_tW_Top",
              "TTTo2L2Nu",
              "WW",
              "WZ",
              "ZZ",
              "GGToEE_10to30_ElEl",
              "GGToEE_10to30_InelElElInel",
              "GGToEE_10to30_InelInel",
              "GGToEE_30to50_ElEl",
              "GGToEE_30to50_InelElElInel",
              "GGToEE_30to50_InelInel",
              "GGToEE_1500toInf_ElEl",
              "GGToEE_1500toInf_InelElElInel",
              "GGToEE_1500toInf_InelInel",
              "GGToEE_200to1500_ElEl",
              "GGToEE_200to1500_InelElElInel",
              "GGToEE_200to1500_InelInel",
              "GGToEE_50to200_InelElElInel",
              "GGToEE_50to200_InelInel",
              "GGToEE_50to200_ElEl",
            }}
          }
        }
      }
    }
  }
};


int main() {


  YAML::Node fConfigRef = YAML::LoadFile(std::string("../../input/dataset.yml"));
  YAML::Node fConfigTarget = YAML::LoadFile(std::string("./input_v2/dataset.yml"));
  
  const auto& fMapMUMU = InputMap["MUMU"];

  for (const auto& [fYear, fCase] : fMapMUMU) {
    for (const auto& [fSample, fFiles] : fCase) {
      for (const auto& fFile : fFiles) {

        std::cout << "Running: " << fYear << " " << fFile << std::endl;

        int fRefNevent = fConfigRef[fYear][fFile]["nEvent"].as<int>();
        int fTargetNevent = fConfigTarget[fYear][fFile]["nEvent"].as<int>();

        bool fRefIsMC = fConfigRef[fYear][fFile]["IsMC"].as<bool>();
        bool fTargetIsMC = fConfigTarget[fYear][fFile]["IsMC"].as<bool>();

        int fTargetNlist = fConfigTarget[fYear][fFile]["nList"].as<int>();

        std::string fFileLocation = fConfigTarget[fYear][fFile]["Name"].as<std::string>();

        int nList = 0;
        for (int i = 1; i < 9999; i++) {
          std::string fFileNmae = "./input_v2/" + fYear + "/" + fFileLocation + "/input_" + std::to_string(i) + ".list";

          if (!std::filesystem::exists(fFileNmae))
            break;

          nList++;
        }

        if (fRefNevent != fTargetNevent) 
          std::cout << "Number of events mismatch: " << fRefNevent << " != " << fTargetNevent << std::endl;

        if (fTargetNlist != nList)
          std::cout << "Number of lists mismatch: " << fTargetNlist << " != " << nList << std::endl;

        if (fRefIsMC != fTargetIsMC)
          std::cout << "MC status mismatch: " << fRefIsMC << " != " << fTargetIsMC << std::endl;
      }
    }
  }



  return 0;
}