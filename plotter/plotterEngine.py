#!/usr/bin/env python3

import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array

parser = argparse.ArgumentParser()
parser.add_argument('--era', help=' : era to plot')
args = parser.parse_args()


# CMS.SetExtraText("Preliminary")
# CMS.SetEnergy("13")
ROOT.gROOT.SetBatch(ROOT.kTRUE)

TotalMCList = [
    "NNLO_MUMU_10to50",
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
    "GGToMuMu_30to50_ElEl",
    "GGToMuMu_50to200_ElEl",
    "GGToMuMu_200to1500_ElEl",
    "GGToMuMu_1500toInf_ElEl",

    "GGToMuMu_10to30_InelElElInel",
    "GGToMuMu_30to50_InelElElInel",
    "GGToMuMu_50to200_InelElElInel",
    "GGToMuMu_200to1500_InelElElInel",
    "GGToMuMu_1500toInf_InelElElInel",

    "GGToMuMu_10to30_InelInel",
    "GGToMuMu_30to50_InelInel",
    "GGToMuMu_50to200_InelInel",
    "GGToMuMu_200to1500_InelInel",
    "GGToMuMu_1500toInf_InelInel",
]

BkgMCList = [
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
    "GGToMuMu_30to50_ElEl",
    "GGToMuMu_50to200_ElEl",
    "GGToMuMu_200to1500_ElEl",
    "GGToMuMu_1500toInf_ElEl",

    "GGToMuMu_10to30_InelElElInel",
    "GGToMuMu_30to50_InelElElInel",
    "GGToMuMu_50to200_InelElElInel",
    "GGToMuMu_200to1500_InelElElInel",
    "GGToMuMu_1500toInf_InelElElInel",

    "GGToMuMu_10to30_InelInel",
    "GGToMuMu_30to50_InelInel",
    "GGToMuMu_50to200_InelInel",
    "GGToMuMu_200to1500_InelInel",
    "GGToMuMu_1500toInf_InelInel",
]

DYMCList = [
    "NNLO_MUMU_10to50",
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
]

refLumi = {
    "2016_preVFP": 19.5,
    "2016_postVFP": 16.8,
    "2017": 41.5,
    "2018": 59.8
}

xSec = {
    "NNLO_10to50": 7012.53,
    "NNLO_inc": 1925.65,
    "NNLO_100to200": 77.95,
    "NNLO_200to400": 2.78,
    "NNLO_400to500": 0.15,
    "NNLO_500to700": 0.084,
    "NNLO_700to800": 0.013,
    "NNLO_800to1000": 0.011,
    "NNLO_1000to1500": 0.006,
    "NNLO_1500to2000": 0.00081,
    "NNLO_2000toInf": 0.0002,

    "NNLO_MUMU_10to50": 7012.53,
    "NNLO_MUMU_inc": 1925.65,
    "NNLO_MUMU_100to200": 77.95,
    "NNLO_MUMU_200to400": 2.78,
    "NNLO_MUMU_400to500": 0.15,
    "NNLO_MUMU_500to700": 0.084,
    "NNLO_MUMU_700to800": 0.013,
    "NNLO_MUMU_800to1000": 0.011,
    "NNLO_MUMU_1000to1500": 0.006,
    "NNLO_MUMU_1500to2000": 0.00081,
    "NNLO_MUMU_2000toInf": 0.0002,

    "NNLO_EE_10to50": 7012.53,
    "NNLO_EE_inc": 1925.65,
    "NNLO_EE_100to200": 77.95,
    "NNLO_EE_200to400": 2.78,
    "NNLO_EE_400to500": 0.15,
    "NNLO_EE_500to700": 0.084,
    "NNLO_EE_700to800": 0.013,
    "NNLO_EE_800to1000": 0.011,
    "NNLO_EE_1000to1500": 0.006,
    "NNLO_EE_1500to2000": 0.00081,
    "NNLO_EE_2000toInf": 0.0002,
    
    "NNLO_tautau": 1164.31,

    "ST_s": 10.32,
    "ST_t_AntiTop": 80.0,
    "ST_t_Top": 134.2,
    "ST_tW_AntiTop": 39.65,
    "ST_tW_Top": 39.65,
    
    "TTTo2L2Nu": 88.51,
    
    "WJetsToLNu": 61526.7,
    
    "WW": 11.09,
    "WZ": 27.59,
    "ZZ": 12.17,

    "GGToMuMu_10to30_ElEl": 11.39,
    "GGToMuMu_30to50_ElEl": 0.6716,
    "GGToMuMu_50to200_ElEl": 0.2473,
    "GGToMuMu_200to1500_ElEl": 0.005637,
    "GGToMuMu_1500toInf_ElEl": 0.000003813,

    "GGToMuMu_10to30_InelElElInel": 10.52,
    "GGToMuMu_30to50_InelElElInel": 0.8958,
    "GGToMuMu_50to200_InelElElInel": 0.3694,
    "GGToMuMu_200to1500_InelElElInel": 0.0111,
    "GGToMuMu_1500toInf_InelElElInel": 0.000008786,

    "GGToMuMu_10to30_InelInel": 10.40,
    "GGToMuMu_30to50_InelInel": 1.171,
    "GGToMuMu_50to200_InelInel": 0.5615,
    "GGToMuMu_200to1500_InelInel": 0.02173,
    "GGToMuMu_1500toInf_InelInel": 0.00002023,

    "GGToEE_10to30_ElEl": 11.39,
    "GGToEE_30to50_ElEl": 0.6716,
    "GGToEE_50to200_ElEl": 0.2473,
    "GGToEE_200to1500_ElEl": 0.005637,
    "GGToEE_1500toInf_ElEl": 0.000003813,

    "GGToEE_10to30_InelElElInel": 10.52,
    "GGToEE_30to50_InelElElInel": 0.8958,
    "GGToEE_50to200_InelElElInel": 0.3694,
    "GGToEE_200to1500_InelElElInel": 0.0111,
    "GGToEE_1500toInf_InelElElInel": 0.000008786,

    "GGToEE_10to30_InelInel": 10.40,
    "GGToEE_30to50_InelInel": 1.171,
    "GGToEE_50to200_InelInel": 0.5615,
    "GGToEE_200to1500_InelInel": 0.02173,
    "GGToEE_1500toInf_InelInel": 0.00002023,
}

normFactor = {}


class Plotter:
    def __init__(self, era, rootPath = "output.root"):
        self.rootPath = rootPath
        self.era = era
        self.outputPath = "./plots/plot_" + era + "/"

        self.lumi = refLumi[self.era]
        CMS.SetLumi(self.lumi)

        self.PrepareFiles()
        self.PrepareNorm()


    def PrepareFiles(self):
        self.fileSet = ROOT.TFile(self.rootPath, "READ");

    def PrepareNorm(self):
        for mcSet in TotalMCList:
            nEvent = self.fileSet.Get(self.era + "/" + mcSet + "/h_EventInfo").GetBinContent(4)
            normFactor[mcSet] = (1000. * self.lumi * xSec[mcSet]) / nEvent;

    def CheckSanity(self, hist):

        hist_clone = hist.Clone(f"{hist.GetName()}_{uuid.uuid4()}_cl")
        for i in range(1, hist_clone.GetNbinsX() + 1):
            if hist_clone.GetBinContent(i) < 0:
                hist_clone.SetBinContent(i, 0)
                hist_clone.SetBinError(i, 0)
            
        return hist_clone

    def CheckSanity2D(self, hist):

        hist_clone = hist.Clone(f"{hist.GetName()}_{uuid.uuid4()}_cl")
        for i in range(1, hist_clone.GetNbinsX() + 1):
            for j in range(1, hist_clone.GetNbinsY() + 1):
                if hist_clone.GetBinContent(i, j) < 0:
                    hist_clone.SetBinContent(i, j, 0)
                    hist_clone.SetBinError(i, j, 0)
            
        return hist_clone

    def GetMCHist(self, histName, list):
        
        histSet = {}
        for mc in list:
            hist = self.fileSet.Get(self.era + "/" + mc + "/" + histName).Clone(f"{histName}_{uuid.uuid4()}")
            hist.SetDirectory(0)
            hist.SetStats(0);
            hist.Sumw2();
            if (self.era != "merged"):
                hist.Scale(normFactor[mc]);
            hist = self.CheckSanity(hist)

            histSet[mc] = hist

        return_hist = histSet[list[0]].Clone(f"{histName}_{uuid.uuid4()}")
        for mc in list[1:]:
            return_hist.Add(histSet[mc])
        
        return return_hist

    def GetMCHist2D(self, histName, list):
        
        histSet = {}
        for mc in list:
            hist = self.fileSet.Get(self.era + "/" + mc + "/" + histName).Clone(f"{histName}_{uuid.uuid4()}")
            hist.SetDirectory(0)
            hist.SetStats(0);
            hist.Sumw2();
            if (self.era != "merged"):
                hist.Scale(normFactor[mc]);
            hist = self.CheckSanity2D(hist)

            histSet[mc] = hist

        return_hist = histSet[list[0]].Clone(f"{histName}_{uuid.uuid4()}")
        for mc in list[1:]:
            return_hist.Add(histSet[mc])
        
        return return_hist

    def GetSingleHist(self, histname, sample):


        hist = self.fileSet.Get(self.era + "/" + sample + "/" + histname).Clone(f"{histname}_{uuid.uuid4()}")
        hist.SetDirectory(0)
        hist.Sumw2()
        if sample != "Data":
            hist.Scale(normFactor[sample]);
        hist = self.CheckSanity(hist)

        return hist

    def GetDataHist(self, histName):
        hist = self.fileSet.Get(self.era + "/Data/" + histName).Clone(f"{histName}_{uuid.uuid4()}")
        hist.SetDirectory(0)
        hist.SetStats(0);
        
        return hist

    def GetSignalFraction(self, histName):

        data = self.GetDataHist(histName)
        totalBKG = self.GetMCHist(histName, BkgMCList)

        data_signalOnly = data.Clone(f"{histName}_signalOnly_{uuid.uuid4()}")
        data_signalOnly.Add(totalBKG, -1)

        signalFraction = data_signalOnly.Clone(f"{histName}_signalFraction_{uuid.uuid4()}")
        signalFraction.Divide(data)

        return data_signalOnly, signalFraction


