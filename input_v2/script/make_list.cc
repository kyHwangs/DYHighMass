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

static std::map<std::string, std::map<std::string, std::map<std::string, double>>> MeanTimeMap = {
  {
    "2016_postVFP",
    {
      {
        "Data",
        {
          {"Run2016F_SingleMuon", 1.44935e-05},
          {"Run2016G_SingleMuon", 1.50055e-05},
          {"Run2016H_SingleMuon", 1.31414e-05},
        },
      },
      {
        "MC",
        {
          {"NNLO_MUMU_10to50", 3.03654e-05},
          {"NNLO_MUMU_10to50_v2", 3.03654e-05},
          {"NNLO_MUMU_inc", 0.000272971},
          {"NNLO_MUMU_100to200", 4.18371e-05},
          {"NNLO_MUMU_200to400", 0.000189},
          {"NNLO_MUMU_400to500", 0.000421796},
          {"NNLO_MUMU_500to700", 0.00022161},
          {"NNLO_MUMU_700to800", 0.0003},
          {"NNLO_MUMU_800to1000", 0.000323992},
          {"NNLO_MUMU_1000to1500", 0.00058032},
          {"NNLO_MUMU_1500to2000", 0.00134885},
          {"NNLO_MUMU_2000toInf", 0.00175524},
          {"NNLO_tautau", 0.000145838},
          {"ST_s", 3.23524e-05},
          {"ST_t_AntiTop", 3.61985e-05},
          {"ST_t_Top", 0.000128106},
          {"ST_tW_AntiTop", 4.69851e-05},
          {"ST_tW_Top", 4.7772e-05},
          {"TTTo2L2Nu", 0.000101111},
          {"WW", 4.10345e-05},
          {"WZ", 3.78428e-05},
          {"ZZ", 3.2146e-05},
          {"GGToMuMu_10to30_ElEl", 2.5e-05},
          {"GGToMuMu_10to30_InelElElInel", 3.5e-05},
          {"GGToMuMu_10to30_InelInel", 3.5e-05},
          {"GGToMuMu_30to50_ElEl", 2.5e-05},
          {"GGToMuMu_30to50_InelElElInel", 2.5e-05},
          {"GGToMuMu_30to50_InelInel", 2.5227e-05},
          {"GGToMuMu_1500toInf_ElEl", 0.000341453},
          {"GGToMuMu_1500toInf_InelElElInel", 0.000205973},
          {"GGToMuMu_1500toInf_InelInel", 0.000253595},
          {"GGToMuMu_200to1500_ElEl", 0.00024734},
          {"GGToMuMu_200to1500_InelElElInel", 0.000543377},
          {"GGToMuMu_200to1500_InelInel", 0.00056},
          {"GGToMuMu_50to200_ElEl", 4.01899e-05},
          {"GGToMuMu_50to200_InelElElInel", 4.5e-05},
          {"GGToMuMu_50to200_InelInel", 4e-05},
        },
      },
    },
  },
  {
    "2016_preVFP",
    {
      {
        "Data",
        {
          {"Run2016B_SingleMuon_v2", 9.13639e-05},
          {"Run2016C_SingleMuon", 8.73091e-05},
          {"Run2016D_SingleMuon", 7.87934e-05},
          {"Run2016E_SingleMuon", 0.000163095},
          {"Run2016F_SingleMuon", 1.80037e-05},
        },
      },
      {
        "MC",
        {
          {"NNLO_MUMU_10to50", 5.15942e-05},
          {"NNLO_MUMU_10to50_v2", 5.15942e-05},
          {"NNLO_MUMU_inc", 0.000144847},
          {"NNLO_MUMU_100to200", 5.50715e-05},
          {"NNLO_MUMU_200to400", 0.000360792},
          {"NNLO_MUMU_400to500", 0.000356667},
          {"NNLO_MUMU_500to700", 0.000287604},
          {"NNLO_MUMU_700to800", 0.00039838},
          {"NNLO_MUMU_800to1000", 0.00043711},
          {"NNLO_MUMU_1000to1500", 0.000557838},
          {"NNLO_MUMU_1500to2000", 0.00059},
          {"NNLO_MUMU_2000toInf", 0.000458737},
          {"NNLO_tautau", 3.34646e-05},
          {"ST_s", 3.49764e-05},
          {"ST_t_AntiTop", 2.63022e-05},
          {"ST_t_Top", 2.26408e-05},
          {"ST_tW_AntiTop", 1.6087e-05},
          {"ST_tW_Top", 3.04348e-05},
          {"TTTo2L2Nu", 6.11385e-05},
          {"WW", 7.25646e-05},
          {"WZ", 0.000308798},
          {"ZZ", 3.35413e-05},
          {"GGToMuMu_10to30_ElEl", 5.2e-05},
          {"GGToMuMu_10to30_InelElElInel", 4e-05},
          {"GGToMuMu_10to30_InelInel", 8e-05},
          {"GGToMuMu_30to50_ElEl", 6.4e-05},
          {"GGToMuMu_30to50_InelElElInel", 4.8e-05},
          {"GGToMuMu_30to50_InelInel", 3.6e-05},
          {"GGToMuMu_1500toInf_ElEl", 0.00042},
          {"GGToMuMu_1500toInf_InelElElInel", 0.000436},
          {"GGToMuMu_1500toInf_InelInel", 0.000391763},
          {"GGToMuMu_200to1500_ElEl", 0.000352},
          {"GGToMuMu_200to1500_InelElElInel", 0.000368},
          {"GGToMuMu_200to1500_InelInel", 0.000276},
          {"GGToMuMu_50to200_ElEl", 3.6e-05},
          {"GGToMuMu_50to200_InelElElInel", 4e-05},
          {"GGToMuMu_50to200_InelInel", 3.2e-05},
        },
      },
    },
  },
  {
    "2017",
    {
      {
        "Data",
        {
          {"Run2017B_SingleMuon", 1.75731e-05},
          {"Run2017C_SingleMuon", 1.90452e-05},
          {"Run2017D_SingleMuon", 2.17277e-05},
          {"Run2017E_SingleMuon", 1.63887e-05},
          {"Run2017F_SingleMuon", 1.37907e-05},
        },
      },
      {
        "MC",
        {
          {"NNLO_MUMU_10to50", 6.38572e-05},
          {"NNLO_MUMU_10to50_v2", 6.38572e-05},
          {"NNLO_MUMU_inc", 0.000130176},
          {"NNLO_MUMU_100to200", 7.8402e-05},
          {"NNLO_MUMU_200to400", 0.000393},
          {"NNLO_MUMU_400to500", 0.000689346},
          {"NNLO_MUMU_500to700", 0.00125},
          {"NNLO_MUMU_700to800", 0.000406667},
          {"NNLO_MUMU_800to1000", 0.000283333},
          {"NNLO_MUMU_1000to1500", 0.000427632},
          {"NNLO_MUMU_1500to2000", 0.00052},
          {"NNLO_MUMU_2000toInf", 0.00044},
          {"NNLO_tautau", 0.000101635},
          {"ST_s", 3.68576e-05},
          {"ST_t_AntiTop", 3.50752e-05},
          {"ST_t_Top", 3.26089e-05},
          {"ST_tW_AntiTop", 0.000192809},
          {"ST_tW_Top", 0.00135493},
          {"TTTo2L2Nu", 0.000185235},
          {"WW", 4.81826e-05},
          {"WZ", 5.64077e-05},
          {"ZZ", 3.51072e-05},
          {"GGToMuMu_10to30_ElEl", 2.6e-05},
          {"GGToMuMu_10to30_InelElElInel", 3e-05},
          {"GGToMuMu_10to30_InelInel", 2.2e-05},
          {"GGToMuMu_30to50_ElEl", 2.8e-05},
          {"GGToMuMu_30to50_InelElElInel", 2.6e-05},
          {"GGToMuMu_30to50_InelInel", 2.2e-05},
          {"GGToMuMu_1500toInf_ElEl", 0.00045},
          {"GGToMuMu_1500toInf_InelElElInel", 0.000884},
          {"GGToMuMu_1500toInf_InelInel", 0.0004},
          {"GGToMuMu_200to1500_ElEl", 0.000386},
          {"GGToMuMu_200to1500_InelElElInel", 0.00053},
          {"GGToMuMu_200to1500_InelInel", 0.000736},
          {"GGToMuMu_50to200_ElEl", 4.4e-05},
          {"GGToMuMu_50to200_InelElElInel", 3.6e-05},
          {"GGToMuMu_50to200_InelInel", 3.4e-05},
        },
      },
    },
  },
  {
    "2018",
    {
      {
        "Data",
        {
          {"Run2018A_SingleMuon", 1.97674e-05},
          {"Run2018B_SingleMuon", 2.01636e-05},
          {"Run2018C_SingleMuon", 1.67727e-05},
          {"Run2018D_SingleMuon", 6.92743e-05},
        },
      },
      {
        "MC",
        {
          {"NNLO_MUMU_10to50", 4.63409e-05},
          {"NNLO_MUMU_10to50_v2", 4.63409e-05},
          {"NNLO_MUMU_inc", 0.000205292},
          {"NNLO_MUMU_100to200", 5.68783e-05},
          {"NNLO_MUMU_200to400", 0.00034},
          {"NNLO_MUMU_400to500", 0.000456667},
          {"NNLO_MUMU_500to700", 0.00125333},
          {"NNLO_MUMU_700to800", 0.000663333},
          {"NNLO_MUMU_800to1000", 0.000599826},
          {"NNLO_MUMU_1000to1500", 0.000592778},
          {"NNLO_MUMU_1500to2000", 0.00032},
          {"NNLO_MUMU_2000toInf", 0.000460002},
          {"NNLO_tautau", 0.000111256},
          {"ST_s", 2.5457e-05},
          {"ST_t_AntiTop", 0.000201669},
          {"ST_t_Top", 3.41602e-05},
          {"ST_tW_AntiTop", 4.02633e-05},
          {"ST_tW_Top", 5.36702e-05},
          {"TTTo2L2Nu", 0.000229424},
          {"WW", 4.3326e-05},
          {"WZ", 3.92947e-05},
          {"ZZ", 3.48837e-05},
          {"GGToMuMu_10to30_ElEl", 2.4e-05},
          {"GGToMuMu_10to30_InelElElInel", 0.000228},
          {"GGToMuMu_10to30_InelInel", 4e-05},
          {"GGToMuMu_30to50_ElEl", 0.000168},
          {"GGToMuMu_30to50_InelElElInel", 3e-05},
          {"GGToMuMu_30to50_InelInel", 3.2e-05},
          {"GGToMuMu_1500toInf_ElEl", 0.000446},
          {"GGToMuMu_1500toInf_InelElElInel", 0.0004},
          {"GGToMuMu_1500toInf_InelInel", 0.000468},
          {"GGToMuMu_200to1500_ElEl", 0.000768},
          {"GGToMuMu_200to1500_InelElElInel", 0.000636},
          {"GGToMuMu_200to1500_InelInel", 0.000484},
          {"GGToMuMu_50to200_ElEl", 3.8e-05},
          {"GGToMuMu_50to200_InelElElInel", 0.000158},
          {"GGToMuMu_50to200_InelInel", 3.8e-05},
        },
      },
    },
  },
};

int main() {


  YAML::Node fConfigPath = YAML::LoadFile(std::string("./path.yaml"));
  YAML::Node fConfigDataset = YAML::LoadFile(std::string("../../input/dataset.yml"));
  const auto& fMapMUMU = InputMap["MUMU"];

  system("mkdir input_v2");
  std::ofstream fFileDataset("./input_v2/dataset.yml", std::ofstream::out);

  for (const auto& [fYear, fCase] : fMapMUMU) {
    system(("mkdir -p input_v2/" + fYear).c_str());

    fFileDataset << fYear + ":" << std::endl;
    
    for (const auto& [fSample, fFiles] : fCase) {
      for (const auto& fFile : fFiles) {

        std::cout << "Running: " << fYear << " " << fFile << std::endl;

        double fMeanTime = MeanTimeMap[fYear][fSample][fFile];
        std::string fFileLocation = fConfigPath[std::string(fYear)][std::string(fFile)]["Path"].as<std::string>();
        std::string fFileName = fConfigPath[std::string(fYear)][std::string(fFile)]["Name"].as<std::string>();

        system(("mkdir -p input_v2/" + fYear + "/" + fFileName).c_str());
        
        double fMaxEvent = 0;
        double fTotalEvent = 0;
        int fListFile = 1;

        std::string fListName = "./input_v2/" + fYear + "/" + fFileName + "/input_" + std::to_string(fListFile) + ".list";
        std::ofstream tFile(fListName, std::ofstream::out);
        std::cout << "    -- List file created: " << fListName << std::endl;
      
        for (int i = 1; i < 9999; i++) {
          std::string fRootName = fFileLocation + "0000/tree_" + std::to_string(i) + ".root";
          if (i >= 1000) fRootName = fFileLocation + "0001/tree_" + std::to_string(i) + ".root";
          if (i >= 2000) fRootName = fFileLocation + "0002/tree_" + std::to_string(i) + ".root";

          if (!std::filesystem::exists(fRootName)) {
            if (i < 10) continue;
            std::cout << "no file! " << fRootName << std::endl;
            tFile.close();
            break;
          }

          tFile << fRootName << std::endl;

          TChain* fChain = new TChain("Events");
          fChain->Add((TString)fRootName);

          double tMaxEvent = fChain->GetEntries();
          fMaxEvent += tMaxEvent;
          fTotalEvent += tMaxEvent;
          delete fChain;

          double tEstTime = fMaxEvent * fMeanTime;

          if (tEstTime > 900) {
            std::cout << "Time over 900 seconds! " << tEstTime << " = " << fMaxEvent << " * " << fMeanTime << std::endl;
            tFile.close();
            fListFile++;
            fMaxEvent = 0;
            fListName = "./input_v2/" + fYear + "/" + fFileName + "/input_" + std::to_string(fListFile) + ".list";
            std::cout << "    -- List file created: " << fListName << std::endl;
            tFile.open(fListName, std::ofstream::out);
          } 
        }







        fFileDataset << "  " + fFile + ":" << std::endl;
        fFileDataset << "    Name: " + fFileName << std::endl;
        fFileDataset << "    nList: " + std::to_string(fListFile) << std::endl;
        fFileDataset << "    nEvent: " + std::to_string(static_cast<int>(fTotalEvent)) << std::endl;
        if (fConfigDataset[fYear][fFile]["IsMC"].as<bool>()) fFileDataset << "    IsMC: true"  << std::endl;
        else fFileDataset << "    IsMC: false"  << std::endl;
        fFileDataset << "" << std::endl;
      }
    }
  }

  fFileDataset.close();


  return 0;
}