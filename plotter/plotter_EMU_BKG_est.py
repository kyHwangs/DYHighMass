#!/usr/bin/env python3

import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array

parser = argparse.ArgumentParser()
parser.add_argument('--era', help=' : era to plot')
args = parser.parse_args()


CMS.SetExtraText("Preliminary")
CMS.SetEnergy("13")
ROOT.gROOT.SetBatch(ROOT.kTRUE)

mcList = [
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

dylist = [
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
    "NNLO_MUMU_2000toInf"
]

stlist = [
    "ST_s",
    "ST_t_AntiTop",
    "ST_t_Top",
    "ST_tW_AntiTop",
    "ST_tW_Top"
]

ewlist = ["WW", "WZ", "ZZ"]

GG_ElEl_list = [
    "GGToMuMu_10to30_ElEl",
    "GGToMuMu_30to50_ElEl",
    "GGToMuMu_50to200_ElEl",
    "GGToMuMu_200to1500_ElEl",
    "GGToMuMu_1500toInf_ElEl",
]

GG_InelElElInel_list = [
    "GGToMuMu_10to30_InelElElInel",
    "GGToMuMu_30to50_InelElElInel",
    "GGToMuMu_50to200_InelElElInel",
    "GGToMuMu_200to1500_InelElElInel",
    "GGToMuMu_1500toInf_InelElElInel",
]

GG_InelInel_list = [
    "GGToMuMu_10to30_InelInel",
    "GGToMuMu_30to50_InelInel",
    "GGToMuMu_50to200_InelInel",
    "GGToMuMu_200to1500_InelInel",
    "GGToMuMu_1500toInf_InelInel",
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
    def __init__(self, era, path):
        self.rootPath = path
        self.era = era
        self.outputPath = "./plots/plot_" + era + "/"

        os.makedirs(self.outputPath, exist_ok=True)

        self.lumi = refLumi[self.era]
        CMS.SetLumi(self.lumi)

        self.PrepareFiles()
        self.PrepareNorm()


    def PrepareFiles(self):
        self.fileSet = ROOT.TFile(self.rootPath, "READ");

    def PrepareNorm(self):
        for mcSet in mcList:
            nEvent = self.fileSet.Get(self.era + "/" + mcSet + "/h_EventInfo").GetBinContent(4)
            normFactor[mcSet] = (1000. * self.lumi * xSec[mcSet]) / nEvent;

    def Plot(self, histName, 
        case,
        massbin,
        hook,
        xmin = -1, 
        xmax = -1, 
        ymin = -1, 
        ymax = -1, 
        yrmin = -1, 
        yrmax = -1,
        xTitle = "", 
        yTitle = "Events", 
        ratioTitle = "Data/Pred.",
        logy = True, 
        logx = False
        ):

        doAutoXrange = False
        doAutoYrange = False
        doAutoYRatiorange = False

        if (xmin == -1 and xmax == -1): doAutoXrange = True
        if (ymin == -1 and ymax == -1): doAutoYrange = True
        if (yrmin == -1 and yrmax == -1): doAutoYRatiorange = True

        self.histName = ""

        if case != "" and massbin != "":
            self.histName += case + massbin + "/" + histName + case + massbin
        if case != "" and massbin == "":
            self.histName += case + "/" + histName + case
        if case == "" and massbin != "":
            self.histName += massBin + "/" + histName + massBin
        if case == "" and massbin == "":
            self.histName = histName

        self.histOutputName = histName + case + massbin
        canvasName = self.era + "_" + histName


        data = self.fileSet.Get(self.era + "/Data/" + self.histName)
        # DY = self.GetMCHist("DY")
        TT = self.GetMCHist("TTTo2L2Nu")
        ST = self.GetMCHist("ST")
        DY_tau = self.GetMCHist("NNLO_tautau")
        EW = self.GetMCHist("EW")
        GG_ElEl = self.GetMCHist("GG_ElEl")
        GG_InelElElInel = self.GetMCHist("GG_InelElElInel")
        GG_InelInel = self.GetMCHist("GG_InelInel")
        GG = GG_ElEl.Clone(f"GG_{uuid.uuid4()}")
        GG.Add(GG_InelElElInel)
        GG.Add(GG_InelInel)
        # WJets = self.GetMCHist("WJetsToLNu")
        
        MC = TT.Clone(f"MC_{uuid.uuid4()}")
        # MC.Add(TT)
        MC.Add(ST)
        MC.Add(DY_tau)
        MC.Add(EW)
        MC.Add(GG_ElEl)
        MC.Add(GG_InelElElInel)
        MC.Add(GG_InelInel)

        dataOmc = data.Clone(f"ratio_{uuid.uuid4()}")
        dataOmc.Divide(MC)

        if doAutoXrange:
            xmin = MC.GetBinLowEdge(1)
            xmax = MC.GetBinLowEdge(MC.GetNbinsX()) +  MC.GetBinWidth(MC.GetNbinsX())

        if doAutoYrange:
            ymin = 2e-2
            ymax = data.GetBinContent(data.GetMaximumBin()) * 1e3

        if "dimuonMass_wide" in histName: 
            ymin = 2e-2

        if doAutoYRatiorange:
            nBinsX = dataOmc.GetNbinsX()
            residual = -999
            for i in range(1, nBinsX + 1):
                if dataOmc.GetBinCenter(i) > xmin and dataOmc.GetBinCenter(i) < xmax:
                    if dataOmc.GetBinContent(i) > 0.2 and dataOmc.GetBinContent(i) < 1.8:
                        if (residual < abs(1 - dataOmc.GetBinContent(i))):
                            residual = abs(1 - dataOmc.GetBinContent(i))

            if residual < 0.05:
                yrmin = 1 - 0.06
                yrmax = 1 + 0.06

            elif residual < 0.1:
                yrmin = 1 - 0.12
                yrmax = 1 + 0.12
            
            elif residual < 0.15:
                yrmin = 1 - 0.18
                yrmax = 1 + 0.18

            elif residual < 0.2:
                yrmin = 1 - 0.24
                yrmax = 1 + 0.24
            
            elif residual < 0.3:
                yrmin = 1 - 0.36
                yrmax = 1 + 0.36

            elif residual < 0.4:
                yrmin = 1 - 0.48
                yrmax = 1 + 0.48

            else:
                yrmin = 0.5
                yrmax = 1.5

        dicanv = CMS.cmsDiCanvas(
            canvasName,
            xmin,
            xmax,
            ymin,
            ymax,
            yrmin,
            yrmax,
            xTitle,
            yTitle,
            ratioTitle,
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        dicanv.cd(1)
        if logy: dicanv.cd(1).SetLogy(True)
        if logx: dicanv.cd(1).SetLogx(True)

        stack = ROOT.THStack("stack", "Stacked")

        leg = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        leg.AddEntry(data, "Data", "lp")

        stackSeet = {
            # "WJets": WJets,
            "DY_tautau": DY_tau,
            "GG": GG,
            # "#gamma#gamma_ElEl": GG_ElEl,
            # "#gamma#gamma_InelElElInel": GG_InelElElInel,
            # "#gamma#gamma_InelInel": GG_InelInel,
            "Single Top": ST,
            "VV": EW,
            "TT": TT,
            # "DY": DY
        }

        CMS.cmsDrawStack(stack, leg, stackSeet)
        CMS.cmsDraw(data, "P", mcolor=ROOT.kBlack)

        latex = ROOT.TLatex()
        latex.SetTextAlign(14);
        latex.SetTextSize(0.04);
        latex.SetTextFont(42);
        for idx, addon in enumerate(hook):
            latex.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        dicanv.cd(2)
        if logx: dicanv.cd(2).SetLogx(True)

        CMS.cmsDraw(dataOmc, "P", mcolor=ROOT.kBlack)


        ref_line = ROOT.TLine(xmin, 1, xmax, 1)
        CMS.cmsDrawLine(ref_line, lcolor=ROOT.kRed, lstyle=ROOT.kDotted)

        CMS.SaveCanvas(dicanv, os.path.join(self.outputPath, self.era + "_" + self.histOutputName + ".pdf"))
        # CMS.SaveCanvas(dicanv, os.path.join(self.outputPath, self.era + "_" + histName + ".png"))

    def GetDataHist(self):
        hist = self.fileSet.Get(self.era + "/" + "Data" + "/" + self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
        hist.SetDirectory(0)
        hist.SetStats(0);

        hist = self.CheckSanity(hist)

        return hist

    def CheckSanity(self, hist):

        hist_clone = hist.Clone(f"{hist.GetName()}_{uuid.uuid4()}_cl")
        for i in range(1, hist_clone.GetNbinsX() + 1):
            if hist_clone.GetBinContent(i) <= 0:
                hist_clone.SetBinContent(i, 0)
                hist_clone.SetBinError(i, 1e-10)

        return hist_clone

    def GetMCHist(self, mcName):
        if mcName == "DY":
            return self.GetDYHist()
        elif mcName == "ST":
            return self.GetSingleTopHist()
        elif mcName == "EW":
            return self.GetEWHist()
        elif mcName == "GG":
            return self.GetGGHist()
        elif mcName == "GG_ElEl":
            return self.GetGG_ElEl()
        elif mcName == "GG_InelElElInel":
            return self.GetGG_InelElElInel()
        elif mcName == "GG_InelInel":
            return self.GetGG_InelInel()
        else:
            hist = self.fileSet.Get(self.era + "/" + mcName + "/" + self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            hist.SetDirectory(0)
            hist.SetStats(0);
            hist.Scale(normFactor[mcName]);
            hist = self.CheckSanity(hist)

            return hist

    def GetTotalMC(self, mcList):
        histoSet = {}
        for mc in mcList:
            histoSet[mc] = self.GetMCHist(mc).Clone(f"{mc}_{uuid.uuid4()}")
            histoSet[mc] = self.CheckSanity(histoSet[mc])

        returnHist = histoSet[mcList[0]].Clone(f"TotalMC_{uuid.uuid4()}")
        for mc in mcList:
            if (mc != mcList[0]):
                returnHist.Add(histoSet[mc])

        return returnHist

    def GetDYHist(self):
        histoSet = {}
        for mc in dylist:
            histoSet[mc] = self.fileSet.Get(self.era + "/" + mc + "/" + self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            histoSet[mc].SetDirectory(0)
            histoSet[mc].SetStats(0);
            histoSet[mc].Scale(normFactor[mc]);
            histoSet[mc] = self.CheckSanity(histoSet[mc])

        returnHist = histoSet["NNLO_MUMU_inc"].Clone(f"DY_{uuid.uuid4()}")
        for mc in dylist:
            if (mc != "NNLO_MUMU_inc"):
                returnHist.Add(histoSet[mc])

        return returnHist

    def GetGG_ElEl(self):
        histoSet = {}
        for mc in GG_ElEl_list:
            histoSet[mc] = self.fileSet.Get(self.era + "/" + mc + "/" + self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            histoSet[mc].SetDirectory(0)
            histoSet[mc].SetStats(0);
            histoSet[mc].Scale(normFactor[mc]);
            histoSet[mc] = self.CheckSanity(histoSet[mc])

        returnHist = histoSet["GGToMuMu_10to30_ElEl"].Clone(f"GG_ElEl_{uuid.uuid4()}")
        for mc in GG_ElEl_list:
            if (mc != "GGToMuMu_10to30_ElEl"):
                returnHist.Add(histoSet[mc])

        return returnHist

    def GetGG_InelElElInel(self):
        histoSet = {}
        for mc in GG_InelElElInel_list:
            histoSet[mc] = self.fileSet.Get(self.era + "/" + mc + "/" + self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            histoSet[mc].SetDirectory(0)
            histoSet[mc].SetStats(0);
            histoSet[mc].Scale(normFactor[mc]);
            histoSet[mc] = self.CheckSanity(histoSet[mc])

        returnHist = histoSet["GGToMuMu_10to30_InelElElInel"].Clone(f"GG_InelElElInel_{uuid.uuid4()}")
        for mc in GG_InelElElInel_list:
            if (mc != "GGToMuMu_10to30_InelElElInel"):
                returnHist.Add(histoSet[mc])

        return returnHist

    def GetGG_InelInel(self):
        histoSet = {}
        for mc in GG_InelInel_list:
            histoSet[mc] = self.fileSet.Get(self.era + "/" + mc + "/" + self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            histoSet[mc].SetDirectory(0)
            histoSet[mc].SetStats(0);
            histoSet[mc].Scale(normFactor[mc]);
            histoSet[mc] = self.CheckSanity(histoSet[mc])

        returnHist = histoSet["GGToMuMu_10to30_InelInel"].Clone(f"GG_InelInel_{uuid.uuid4()}")
        for mc in GG_InelInel_list:
            if (mc != "GGToMuMu_10to30_InelInel"):
                returnHist.Add(histoSet[mc])

        return returnHist

    def GetSingleTopHist(self):
        histoSet = {}
        for mc in stlist:
            histoSet[mc] = self.fileSet.Get(self.era + "/" + mc + "/" + self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            histoSet[mc].SetDirectory(0)
            histoSet[mc].SetStats(0);
            histoSet[mc].Scale(normFactor[mc]);
            histoSet[mc] = self.CheckSanity(histoSet[mc])

        returnHist = histoSet["ST_s"].Clone(f"SingleTop_{uuid.uuid4()}")
        for mc in stlist:
            if (mc != "ST_s"):
                returnHist.Add(histoSet[mc])

        return returnHist

    def GetEWHist(self):
        histoSet = {}
        for mc in ewlist:
            histoSet[mc] = self.fileSet.Get(self.era + "/" + mc + "/" + self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            histoSet[mc].SetDirectory(0)
            histoSet[mc].SetStats(0);
            histoSet[mc].Scale(normFactor[mc]);
            histoSet[mc] = self.CheckSanity(histoSet[mc])

        returnHist = histoSet["WW"].Clone(f"EW_{uuid.uuid4()}")
        for mc in ewlist:
            if (mc != "WW"):
                returnHist.Add(histoSet[mc])

        return returnHist

def GetOSFromSS(EMU_SS_FAKE, SStoOS):

    EMU_OS_FAKE_DataDriven = EMU_SS_FAKE.Clone(f"EMU_OS_FAKE_DataDriven_{uuid.uuid4()}")
    EMU_OS_FAKE_DataDriven.Reset("ICES");

    for i in range(1, SStoOS.GetNbinsX() + 1):
        if SStoOS.GetBinContent(i) == 0 or EMU_SS_FAKE.GetBinContent(i) <= 0:
            EMU_OS_FAKE_DataDriven.SetBinContent(i, 0)
        else:
            EMU_OS_FAKE_DataDriven.SetBinContent(i, EMU_SS_FAKE.GetBinContent(i) * SStoOS.GetBinContent(i))

    return EMU_OS_FAKE_DataDriven


def main(args):

    # cases = ["", "_0J", "_1J", "_mtJ", "_0BJ", "_1BJ", "_mt1BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    cases = ["", "_0BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    # cases = ["_0BJ"]
    # cases = [""]
    # massBins = ["", "_m200_220", "_m220_243", "_m243_273", "_m273_320", "_m320_380", "_m380_440", "_m440_510", "_m510_600", "_m600_700", "_m700_830", "_m830_1000", "_m1000_1500", "_m1500_4000"]
    massBins = [""] 

    addon_hook = {
        "": "",
        "_0J": "N(jet) = 0",
        "_1J": "N(jet) = 1",
        "_mt1J": "N(jet) > 1",
        "_0BJ": "b-veto",
        "_1BJ": "N(b-jet) = 1",
        "_mt1BJ": "N(b-jet) > 1",
        "_bVeto_0J": "b-veto, N(jet) = 0",
        "_bVeto_1J": "b-veto, N(jet) = 1",
        "_bVeto_mt1J": "b-veto, N(jet) > 1",
    }

    yrmax_vec = {
        "": -1,
        "_0J": -1,
        "_1J": -1,
        "_mtJ": -1,
        "_0BJ": 1.15,
        "_1BJ": -1,
        "_mt1BJ": -1,
        "_bVeto_0J": 1.3,
        "_bVeto_1J": 1.5,
        "_bVeto_mt1J": 1.5,    
    }

    yrmin_vec = {
        "": -1,
        "_0J": -1,
        "_1J": -1,
        "_mt1J": -1,
        "_0BJ": 0.85,
        "_1BJ": -1,
        "_mt1BJ": -1,
        "_bVeto_0J": 0.7,
        "_bVeto_1J": 0.5,
        "_bVeto_mt1J": 0.5,  
    }

    addon_hook_mass = {
        "": "M_{e#mu} > 200 GeV",
        "_m200_220": "200 < M_{e#mu} < 220 GeV",
        "_m220_243": "220 < M_{e#mu} < 243 GeV",
        "_m243_273": "243 < M_{e#mu} < 273 GeV",
        "_m273_320": "273 < M_{e#mu} < 320 GeV",
        "_m320_380": "320 < M_{e#mu} < 380 GeV",
        "_m380_440": "380 < M_{e#mu} < 440 GeV",
        "_m440_510": "440 < M_{e#mu} < 510 GeV",
        "_m510_600": "510 < M_{e#mu} < 600 GeV",
        "_m600_700": "600 < M_{e#mu} < 700 GeV",
        "_m700_830": "700 < M_{e#mu} < 830 GeV",
        "_m830_1000": "830 < M_{e#mu} < 1000 GeV",
        "_m1000_1500": "1000 < M_{e#mu} < 1500 GeV",
        "_m1500_4000": "1500 < M_{e#mu} < 4000 GeV"
    }

    latex = [
        args.era,
        "p_{T}(#mu) > 52 GeV, |#eta(#mu)| < 2.4",
        "p_{T}(e) > 20 GeV, |#eta(e)| < 2.5",
        "",
        "",
    ]

    histoName = "h_PairMass"
    histoName_MUMU = "h_dimuonMass"
    outputPath = "./EMU_BKG_251201/plot/"
    outputRoot = "./EMU_BKG_251201/EMU_bkg.root"

    outputFile = ROOT.TFile(outputRoot, "Update")
    outputFile.cd()
    outputFile.mkdir(f"{args.era}/FAKE_EMU_OS")
    outputFile.mkdir(f"{args.era}/FAKE_EMU_SS")
    outputFile.mkdir(f"{args.era}/FAKE_EMU_SStoOS")
    outputFile.mkdir(f"{args.era}/EMU_OS_SingleTop")
    outputFile.mkdir(f"{args.era}/EMU_OS_TT")
    outputFile.mkdir(f"{args.era}/EMU_OS_WW")
    outputFile.mkdir(f"{args.era}/EMU_OS_TOP")
    outputFile.mkdir(f"{args.era}/SingleTop_EMUtoMUMU")
    outputFile.mkdir(f"{args.era}/TT_EMUtoMUMU")
    outputFile.mkdir(f"{args.era}/WW_EMUtoMUMU")
    outputFile.mkdir(f"{args.era}/TOP_EMUtoMUMU")
    outputFile.mkdir(f"{args.era}/SingleTop_MUMU")
    outputFile.mkdir(f"{args.era}/TT_MUMU")
    outputFile.mkdir(f"{args.era}/WW_MUMU")
    outputFile.mkdir(f"{args.era}/TOP_MUMU")

    EMU_OS = Plotter(args.era, "./EMU_BKG_251201/root/EMU_OS.root")
    EMU_SS = Plotter(args.era, "./EMU_BKG_251201/root/EMU_SS.root")
    EMU_OS_inverted = Plotter(args.era, "./EMU_BKG_251201/root/EMU_OS_inverted.root")
    EMU_SS_inverted = Plotter(args.era, "./EMU_BKG_251201/root/EMU_SS_inverted.root")
    MUMU = Plotter(args.era, "./EMU_BKG_251201/root/MUMU_OS.root")

    for case in cases:

        latex_temp = latex.copy()
        latex_temp[3] = addon_hook[case]

        tmp_histName = ""
        tmp_histName_MUMU = ""
        if case != "":
            tmp_histName = case + "/" + histoName + case
            tmp_histName_MUMU = case + "/" + histoName_MUMU + case
        if case == "":
            tmp_histName = histoName
            tmp_histName_MUMU = histoName_MUMU

        EMU_OS.histName = tmp_histName
        EMU_SS.histName = tmp_histName
        EMU_OS_inverted.histName = tmp_histName
        EMU_SS_inverted.histName = tmp_histName
        MUMU.histName = tmp_histName_MUMU

        emu_mc_list = ["TTTo2L2Nu", "ST", "NNLO_tautau", "EW", "GG_ElEl", "GG_InelElElInel", "GG_InelInel"]
        emu_mc_list_ex_TT = ["ST", "NNLO_tautau", "EW", "GG_ElEl", "GG_InelElElInel", "GG_InelInel"]
        emu_mc_list_ex_ST = ["TTTo2L2Nu", "NNLO_tautau", "EW", "GG_ElEl", "GG_InelElElInel", "GG_InelInel"]
        emu_mc_list_ex_WW = ["TTTo2L2Nu", "ST", "NNLO_tautau", "WZ", "ZZ", "GG_ElEl", "GG_InelElElInel", "GG_InelInel"]
        emu_mc_list_ex_TOP = ["NNLO_tautau", "EW", "GG_ElEl", "GG_InelElElInel", "GG_InelInel"]


        EMU_OS_TotalMC = EMU_OS.GetTotalMC(emu_mc_list)
        EMU_OS_data = EMU_OS.GetDataHist()

        EMU_OS_TT = EMU_OS.GetMCHist("TTTo2L2Nu")
        EMU_OS_TT.SetName("EMU_OS_TT" + case)
        EMU_OS_ST = EMU_OS.GetMCHist("ST")
        EMU_OS_ST.SetName("EMU_OS_ST" + case)
        EMU_OS_WW = EMU_OS.GetMCHist("WW")
        EMU_OS_WW.SetName("EMU_OS_WW" + case)

        EMU_OS_TOP = EMU_OS_TT.Clone("EMU_OS_TOP" + case)
        EMU_OS_TOP.Add(EMU_OS_ST)

        MUMU_OS_TT = MUMU.GetMCHist("TTTo2L2Nu")
        MUMU_OS_TT.SetName("MUMU_OS_TT" + case)
        MUMU_OS_ST = MUMU.GetMCHist("ST")
        MUMU_OS_ST.SetName("MUMU_OS_ST" + case)
        MUMU_OS_WW = MUMU.GetMCHist("WW")
        MUMU_OS_WW.SetName("MUMU_OS_WW" + case)

        MUMU_OS_TOP = MUMU_OS_TT.Clone("MUMU_OS_TOP" + case)
        MUMU_OS_TOP.Add(MUMU_OS_ST)

        TT_EMUtoMUMU = MUMU_OS_TT.Clone(f"TT_EMUtoMUMU_{uuid.uuid4()}")
        TT_EMUtoMUMU.Divide(EMU_OS_TT)
        TT_EMUtoMUMU.SetName("TT_EMUtoMUMU" + case)

        ST_EMUtoMUMU = MUMU_OS_ST.Clone(f"ST_EMUtoMUMU_{uuid.uuid4()}")
        ST_EMUtoMUMU.Divide(EMU_OS_ST)
        ST_EMUtoMUMU.SetName("ST_EMUtoMUMU" + case)

        WW_EMUtoMUMU = MUMU_OS_WW.Clone(f"WW_EMUtoMUMU_{uuid.uuid4()}")
        WW_EMUtoMUMU.Divide(EMU_OS_WW)
        WW_EMUtoMUMU.SetName("WW_EMUtoMUMU" + case)

        TOP_EMUtoMUMU = MUMU_OS_TOP.Clone(f"TOP_EMUtoMUMU_{uuid.uuid4()}")
        TOP_EMUtoMUMU.Divide(EMU_OS_TOP)
        TOP_EMUtoMUMU.SetName("TOP_EMUtoMUMU" + case)

        EMU_OS_FAKE = EMU_OS_data.Clone(f"EMU_OS_FAKE_{uuid.uuid4()}")
        EMU_OS_FAKE.Add(EMU_OS_TotalMC, -1)
        EMU_OS_FAKE.SetName("EMU_OS_FAKE" + case)

        EMU_SS_TotalMC = EMU_SS.GetTotalMC(emu_mc_list)
        EMU_SS_data = EMU_SS.GetDataHist()
        
        EMU_SS_FAKE = EMU_SS_data.Clone(f"EMU_SS_FAKE_{uuid.uuid4()}")
        EMU_SS_FAKE.Add(EMU_SS_TotalMC, -1)
        EMU_SS_FAKE.SetName("EMU_SS_FAKE" + case)

        EMU_OS_inverted_TotalMC = EMU_OS_inverted.GetTotalMC(emu_mc_list)
        EMU_OS_inverted_data = EMU_OS_inverted.GetDataHist()
        
        EMU_OS_inverted_FAKE = EMU_OS_inverted_data.Clone(f"EMU_OS_inverted_FAKE_{uuid.uuid4()}")
        EMU_OS_inverted_FAKE.Add(EMU_OS_inverted_TotalMC, -1)

        EMU_SS_inverted_TotalMC = EMU_SS_inverted.GetTotalMC(emu_mc_list)
        EMU_SS_inverted_data = EMU_SS_inverted.GetDataHist()

        EMU_SS_inverted_FAKE = EMU_SS_inverted_data.Clone(f"EMU_SS_inverted_FAKE_{uuid.uuid4()}")
        EMU_SS_inverted_FAKE.Add(EMU_SS_inverted_TotalMC, -1)

        EMU_inverted_SStoOS = EMU_OS_inverted_FAKE.Clone(f"EMU_inverted_SStoOS_{uuid.uuid4()}")
        EMU_inverted_SStoOS.Divide(EMU_SS_inverted_FAKE)
        EMU_inverted_SStoOS.SetName("EMU_inverted_SStoOS" + case)

        EMU_OS_FAKE_DataDriven = GetOSFromSS(EMU_SS_FAKE, EMU_inverted_SStoOS)
        EMU_OS_FAKE_DataDriven.SetName("EMU_OS_FAKE_DataDriven" + case)

        EMU_OS_TotalMC_ex_TT = EMU_OS.GetTotalMC(emu_mc_list_ex_TT)
        EMU_OS_TotalMC_ex_TT.Add(EMU_OS_FAKE_DataDriven)

        EMU_OS_TT_DataDriven = EMU_OS_data.Clone(f"EMU_OS_TT_DataDriven_{uuid.uuid4()}")
        EMU_OS_TT_DataDriven.Add(EMU_OS_TotalMC_ex_TT, -1)
        EMU_OS_TT_DataDriven.SetName("EMU_OS_TT_DataDriven" + case)

        MUMU_OS_TT_DataDriven = GetOSFromSS(EMU_OS_TT_DataDriven, TT_EMUtoMUMU)
        MUMU_OS_TT_DataDriven.SetName("MUMU_OS_TT_DataDriven" + case)
        MUMU_OS_TT_DataDriven_Ratio = MUMU_OS_TT_DataDriven.Clone(f"MUMU_OS_TT_DataDriven_Ratio_{uuid.uuid4()}")
        MUMU_OS_TT_DataDriven_Ratio.Divide(MUMU_OS_TT)

        EMU_OS_TotalMC_ex_ST = EMU_OS.GetTotalMC(emu_mc_list_ex_ST)
        EMU_OS_TotalMC_ex_ST.Add(EMU_OS_FAKE_DataDriven)

        EMU_OS_ST_DataDriven = EMU_OS_data.Clone(f"EMU_OS_ST_DataDriven_{uuid.uuid4()}")
        EMU_OS_ST_DataDriven.Add(EMU_OS_TotalMC_ex_ST, -1)
        EMU_OS_ST_DataDriven.SetName("EMU_OS_ST_DataDriven" + case)

        MUMU_OS_ST_DataDriven = GetOSFromSS(EMU_OS_ST_DataDriven, ST_EMUtoMUMU)
        MUMU_OS_ST_DataDriven.SetName("MUMU_OS_ST_DataDriven" + case)
        MUMU_OS_ST_DataDriven_Ratio = MUMU_OS_ST_DataDriven.Clone(f"MUMU_OS_ST_DataDriven_Ratio_{uuid.uuid4()}")
        MUMU_OS_ST_DataDriven_Ratio.Divide(MUMU_OS_ST)

        EMU_OS_TotalMC_ex_WW = EMU_OS.GetTotalMC(emu_mc_list_ex_WW)
        EMU_OS_TotalMC_ex_WW.Add(EMU_OS_FAKE_DataDriven)

        EMU_OS_WW_DataDriven = EMU_OS_data.Clone(f"EMU_OS_WW_DataDriven_{uuid.uuid4()}")
        EMU_OS_WW_DataDriven.Add(EMU_OS_TotalMC_ex_WW, -1)
        EMU_OS_WW_DataDriven.SetName("EMU_OS_WW_DataDriven" + case)

        MUMU_OS_WW_DataDriven = GetOSFromSS(EMU_OS_WW_DataDriven, WW_EMUtoMUMU)
        MUMU_OS_WW_DataDriven.SetName("MUMU_OS_WW_DataDriven" + case)
        MUMU_OS_WW_DataDriven_Ratio = MUMU_OS_WW_DataDriven.Clone(f"MUMU_OS_WW_DataDriven_Ratio_{uuid.uuid4()}")
        MUMU_OS_WW_DataDriven_Ratio.Divide(MUMU_OS_WW)


        EMU_OS_TotalMC_ex_TOP = EMU_OS.GetTotalMC(emu_mc_list_ex_TOP)
        EMU_OS_TotalMC_ex_TOP.Add(EMU_OS_FAKE_DataDriven)

        EMU_OS_TOP_DataDriven = EMU_OS_data.Clone(f"EMU_OS_TOP_DataDriven_{uuid.uuid4()}")
        EMU_OS_TOP_DataDriven.Add(EMU_OS_TotalMC_ex_TOP, -1)
        EMU_OS_TOP_DataDriven.SetName("EMU_OS_TOP_DataDriven" + case)

        MUMU_OS_TOP_DataDriven = GetOSFromSS(EMU_OS_TOP_DataDriven, TOP_EMUtoMUMU)
        MUMU_OS_TOP_DataDriven.SetName("MUMU_OS_TOP_DataDriven" + case)
        MUMU_OS_TOP_DataDriven_Ratio = MUMU_OS_TOP_DataDriven.Clone(f"MUMU_OS_TOP_DataDriven_Ratio_{uuid.uuid4()}")
        MUMU_OS_TOP_DataDriven_Ratio.Divide(MUMU_OS_TOP)

        outputFile.cd(f"{args.era}/FAKE_EMU_SStoOS")
        EMU_inverted_SStoOS.Write()
        outputFile.cd(f"{args.era}/FAKE_EMU_SS")
        EMU_SS_FAKE.Write()
        outputFile.cd(f"{args.era}/FAKE_EMU_OS")
        EMU_OS_FAKE_DataDriven.Write()
        outputFile.cd(f"{args.era}/TT_MUMU")
        MUMU_OS_TT_DataDriven.Write()
        outputFile.cd(f"{args.era}/SingleTop_MUMU")
        MUMU_OS_ST_DataDriven.Write()
        outputFile.cd(f"{args.era}/WW_MUMU")
        MUMU_OS_WW_DataDriven.Write()
        outputFile.cd(f"{args.era}/TOP_MUMU")
        MUMU_OS_TOP_DataDriven.Write()
        outputFile.cd(f"{args.era}/TT_EMUtoMUMU")
        TT_EMUtoMUMU.Write()
        outputFile.cd(f"{args.era}/SingleTop_EMUtoMUMU")
        ST_EMUtoMUMU.Write()
        outputFile.cd(f"{args.era}/WW_EMUtoMUMU")
        WW_EMUtoMUMU.Write()
        outputFile.cd(f"{args.era}/TOP_EMUtoMUMU")
        TOP_EMUtoMUMU.Write()


        #################################################################
        # Fake SS
        #################################################################
        Canv_EMU_SS_FAKE = CMS.cmsCanvas(
            "Canv_EMU_SS_FAKE",
            200,
            4000,
            0,
            EMU_SS_FAKE.GetMaximum() * 1.3,
            "M(e#mu) [GeV]",
            "Events",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_EMU_SS_FAKE.cd(1).SetLogx(True)
        Canv_EMU_SS_FAKE.cd(2).SetLogx(True)

        EMU_SS_FAKE_Stack = ROOT.THStack("EMU_SS_FAKE_Stack", "Stacked")

        Leg_EMU_SS_FAKE_Stack = CMS.cmsLeg(0.80, 0.80, 0.89, 0.89, textSize=0.03)

        EMU_SS_FAKE_StackSheet = {
            "Fake": EMU_SS_FAKE,
        }

        CMS.cmsDrawStack(EMU_SS_FAKE_Stack, Leg_EMU_SS_FAKE_Stack, EMU_SS_FAKE_StackSheet)
        Canv_EMU_SS_FAKE.cd(1).RedrawAxis()

        latex_EMU_SS_FAKE_item = latex_temp.copy()
        latex_EMU_SS_FAKE_item[0] = f"{args.era}, Fake SS"
        Latex_EMU_SS_FAKE = ROOT.TLatex()
        Latex_EMU_SS_FAKE.SetTextAlign(14);
        Latex_EMU_SS_FAKE.SetTextSize(0.04);
        Latex_EMU_SS_FAKE.SetTextFont(42);
        for idx, addon in enumerate(latex_EMU_SS_FAKE_item):
            Latex_EMU_SS_FAKE.DrawLatexNDC(0.25, 0.86 - idx * 0.065, addon.encode('utf-8'))

        CMS.SaveCanvas(Canv_EMU_SS_FAKE, os.path.join(outputPath, args.era + "_FakeSS" + case + ".pdf"))


        #################################################################
        # Fake OS
        #################################################################
        Canv_EMU_OS_FAKE = CMS.cmsCanvas(
            "Canv_EMU_OS_FAKE",
            200,
            4000,
            0,
            EMU_OS_FAKE_DataDriven.GetMaximum() * 1.3,
            "M(e#mu) [GeV]",
            "Events",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_EMU_OS_FAKE.cd(1).SetLogx(True)
        Canv_EMU_OS_FAKE.cd(2).SetLogx(True)

        EMU_OS_FAKE_Stack = ROOT.THStack("EMU_OS_FAKE_Stack", "Stacked")

        Leg_EMU_OS_FAKE_Stack = CMS.cmsLeg(0.80, 0.80, 0.89, 0.89, textSize=0.03)

        EMU_OS_FAKE_StackSheet = {
            "Fake": EMU_OS_FAKE_DataDriven,
        }

        CMS.cmsDrawStack(EMU_OS_FAKE_Stack, Leg_EMU_OS_FAKE_Stack, EMU_OS_FAKE_StackSheet)
        Canv_EMU_OS_FAKE.cd(1).RedrawAxis()

        latex_EMU_OS_FAKE_item = latex_temp.copy()
        latex_EMU_OS_FAKE_item[0] = f"{args.era}, Fake OS data-driven"
        Latex_EMU_OS_FAKE = ROOT.TLatex()
        Latex_EMU_OS_FAKE.SetTextAlign(14);
        Latex_EMU_OS_FAKE.SetTextSize(0.04);
        Latex_EMU_OS_FAKE.SetTextFont(42);
        for idx, addon in enumerate(latex_EMU_OS_FAKE_item):
            Latex_EMU_OS_FAKE.DrawLatexNDC(0.25, 0.86 - idx * 0.065, addon.encode('utf-8'))

        CMS.SaveCanvas(Canv_EMU_OS_FAKE, os.path.join(outputPath, args.era + "_FakeOSDataDriven" + case + ".pdf"))

        #################################################################
        # Fake SStoOS
        #################################################################
        Canv_SStoOS = CMS.cmsDiCanvas(
            "Canv_SStoOS",
            200,
            4000,
            2e-2,
            EMU_OS_inverted_FAKE.GetMaximum() * 1e3,
            1 - 0.24,
            1 + 0.24,
            "M(e#mu) [GeV]",
            "Events",
            "OS/SS",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )


        Canv_SStoOS.cd(1).SetLogy(True)
        Canv_SStoOS.cd(1).SetLogx(True)
        Canv_SStoOS.cd(2).SetLogx(True)

        Canv_SStoOS.cd(1)

        Leg_SStoOS = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        Leg_SStoOS.AddEntry(EMU_OS_inverted_FAKE, "Data - MC, OS", "lp")
        Leg_SStoOS.AddEntry(EMU_SS_inverted_FAKE, "Data - MC, SS", "lp")

        Canv_SStoOS.cd(1)
        CMS.cmsDraw(EMU_OS_inverted_FAKE, "P", mcolor=ROOT.kBlack)
        CMS.cmsDraw(EMU_SS_inverted_FAKE, "P", mcolor=ROOT.kRed)


        latex_SStoOS_item = latex_temp.copy()
        latex_SStoOS_item[0] = f"{args.era}, OS/SS, inverted ID and ISO"
        Latex_SStoOS = ROOT.TLatex()
        Latex_SStoOS.SetTextAlign(14);
        Latex_SStoOS.SetTextSize(0.04);
        Latex_SStoOS.SetTextFont(42);
        for idx, addon in enumerate(latex_SStoOS_item):
            Latex_SStoOS.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        Canv_SStoOS.cd(2)
        CMS.cmsDraw(EMU_inverted_SStoOS, "P", mcolor=ROOT.kBlack)

        CMS.SaveCanvas(Canv_SStoOS, os.path.join(outputPath, args.era + "_SStoOS" + case + ".pdf"))


        #################################################################
        # TT EMUtoMUMU
        #################################################################
        Canv_TT_EMUtoMUMU = CMS.cmsDiCanvas(
            "Canv_TT_EMUtoMUMU",
            200,
            4000,
            2e-2,
            MUMU_OS_TT.GetMaximum() * 1e3,
            1 - 0.55,
            1 + 0.55,
            "Mass [GeV]",
            "Events",
            "#mu#mu/e#mu",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_TT_EMUtoMUMU.cd(1).SetLogy(True)
        Canv_TT_EMUtoMUMU.cd(1).SetLogx(True)
        Canv_TT_EMUtoMUMU.cd(2).SetLogx(True)

        Canv_TT_EMUtoMUMU.cd(1)

        Leg_TT_EMUtoMUMU = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        Leg_TT_EMUtoMUMU.AddEntry(MUMU_OS_TT, "#mu#mu, TT MC", "lp")
        Leg_TT_EMUtoMUMU.AddEntry(EMU_OS_TT, "e#mu, TT MC", "lp")

        Canv_TT_EMUtoMUMU.cd(1)
        CMS.cmsDraw(MUMU_OS_TT, "P", mcolor=ROOT.kBlack)
        CMS.cmsDraw(EMU_OS_TT, "P", mcolor=ROOT.kRed)


        latex_TT_EMUtoMUMU_item = latex_temp.copy()
        latex_TT_EMUtoMUMU_item[0] = f"{args.era}, #mu#mu/e#mu"
        latex_TT_EMUtoMUMU_item[1] = latex_TT_EMUtoMUMU_item[3]
        latex_TT_EMUtoMUMU_item[2] = ""
        latex_TT_EMUtoMUMU_item[3] = ""

        Latex_TT_EMUtoMUMU = ROOT.TLatex()
        Latex_TT_EMUtoMUMU.SetTextAlign(14);
        Latex_TT_EMUtoMUMU.SetTextSize(0.04);
        Latex_TT_EMUtoMUMU.SetTextFont(42);
        for idx, addon in enumerate(latex_TT_EMUtoMUMU_item):
            Latex_TT_EMUtoMUMU.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        Canv_TT_EMUtoMUMU.cd(2)
        CMS.cmsDraw(TT_EMUtoMUMU, "P", mcolor=ROOT.kBlack)

        CMS.SaveCanvas(Canv_TT_EMUtoMUMU, os.path.join(outputPath, args.era + "_TT_EMUtoMUMU" + case + ".pdf"))


        #################################################################
        # ST EMUtoMUMU
        #################################################################
        Canv_ST_EMUtoMUMU = CMS.cmsDiCanvas(
            "Canv_ST_EMUtoMUMU",
            200,
            4000,
            2e-2,
            MUMU_OS_ST.GetMaximum() * 1e3,
            1 - 0.55,
            1 + 0.55,
            "Mass [GeV]",
            "Events",
            "#mu#mu/e#mu",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_ST_EMUtoMUMU.cd(1).SetLogy(True)
        Canv_ST_EMUtoMUMU.cd(1).SetLogx(True)
        Canv_ST_EMUtoMUMU.cd(2).SetLogx(True)

        Canv_ST_EMUtoMUMU.cd(1)

        Leg_ST_EMUtoMUMU = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        Leg_ST_EMUtoMUMU.AddEntry(MUMU_OS_ST, "#mu#mu, Single Top MC", "lp")
        Leg_ST_EMUtoMUMU.AddEntry(EMU_OS_ST, "e#mu, Single Top MC", "lp")

        Canv_ST_EMUtoMUMU.cd(1)
        CMS.cmsDraw(MUMU_OS_ST, "P", mcolor=ROOT.kBlack)
        CMS.cmsDraw(EMU_OS_ST, "P", mcolor=ROOT.kRed)


        latex_ST_EMUtoMUMU_item = latex_temp.copy()
        latex_ST_EMUtoMUMU_item[0] = f"{args.era}, #mu#mu/e#mu"
        latex_ST_EMUtoMUMU_item[1] = latex_ST_EMUtoMUMU_item[3]
        latex_ST_EMUtoMUMU_item[2] = ""
        latex_ST_EMUtoMUMU_item[3] = ""

        Latex_ST_EMUtoMUMU = ROOT.TLatex()
        Latex_ST_EMUtoMUMU.SetTextAlign(14);
        Latex_ST_EMUtoMUMU.SetTextSize(0.04);
        Latex_ST_EMUtoMUMU.SetTextFont(42);
        for idx, addon in enumerate(latex_ST_EMUtoMUMU_item):
            Latex_ST_EMUtoMUMU.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        Canv_ST_EMUtoMUMU.cd(2)
        CMS.cmsDraw(ST_EMUtoMUMU, "P", mcolor=ROOT.kBlack)

        CMS.SaveCanvas(Canv_ST_EMUtoMUMU, os.path.join(outputPath, args.era + "_ST_EMUtoMUMU" + case + ".pdf"))

        #################################################################
        # WW EMUtoMUMU
        #################################################################
        Canv_WW_EMUtoMUMU = CMS.cmsDiCanvas(
            "Canv_WW_EMUtoMUMU",
            200,
            4000,
            2e-2,
            MUMU_OS_WW.GetMaximum() * 1e3,
            1 - 0.55,
            1 + 0.55,
            "Mass [GeV]",
            "Events",
            "#mu#mu/e#mu",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_WW_EMUtoMUMU.cd(1).SetLogy(True)
        Canv_WW_EMUtoMUMU.cd(1).SetLogx(True)
        Canv_WW_EMUtoMUMU.cd(2).SetLogx(True)

        Canv_WW_EMUtoMUMU.cd(1)

        Leg_WW_EMUtoMUMU = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        Leg_WW_EMUtoMUMU.AddEntry(MUMU_OS_WW, "#mu#mu, WW MC", "lp")
        Leg_WW_EMUtoMUMU.AddEntry(EMU_OS_WW, "e#mu, WW MC", "lp")

        Canv_WW_EMUtoMUMU.cd(1)
        CMS.cmsDraw(MUMU_OS_WW, "P", mcolor=ROOT.kBlack)
        CMS.cmsDraw(EMU_OS_WW, "P", mcolor=ROOT.kRed)


        latex_WW_EMUtoMUMU_item = latex_temp.copy()
        latex_WW_EMUtoMUMU_item[0] = f"{args.era}, #mu#mu/e#mu"
        latex_WW_EMUtoMUMU_item[1] = latex_WW_EMUtoMUMU_item[3]
        latex_WW_EMUtoMUMU_item[2] = ""
        latex_WW_EMUtoMUMU_item[3] = ""

        Latex_WW_EMUtoMUMU = ROOT.TLatex()
        Latex_WW_EMUtoMUMU.SetTextAlign(14);
        Latex_WW_EMUtoMUMU.SetTextSize(0.04);
        Latex_WW_EMUtoMUMU.SetTextFont(42);
        for idx, addon in enumerate(latex_WW_EMUtoMUMU_item):
            Latex_WW_EMUtoMUMU.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        Canv_WW_EMUtoMUMU.cd(2)
        CMS.cmsDraw(WW_EMUtoMUMU, "P", mcolor=ROOT.kBlack)

        CMS.SaveCanvas(Canv_WW_EMUtoMUMU, os.path.join(outputPath, args.era + "_WW_EMUtoMUMU" + case + ".pdf"))


        #################################################################
        # TOP EMUtoMUMU
        #################################################################
        Canv_TOP_EMUtoMUMU = CMS.cmsDiCanvas(
            "Canv_TOP_EMUtoMUMU",
            200,
            4000,
            2e-2,
            MUMU_OS_TOP.GetMaximum() * 1e3,
            1 - 0.55,
            1 + 0.55,
            "Mass [GeV]",
            "Events",
            "#mu#mu/e#mu",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_TOP_EMUtoMUMU.cd(1).SetLogy(True)
        Canv_TOP_EMUtoMUMU.cd(1).SetLogx(True)
        Canv_TOP_EMUtoMUMU.cd(2).SetLogx(True)

        Canv_TOP_EMUtoMUMU.cd(1)

        Leg_TOP_EMUtoMUMU = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        Leg_TOP_EMUtoMUMU.AddEntry(MUMU_OS_TOP, "#mu#mu, TTbar + Single Top MC", "lp")
        Leg_TOP_EMUtoMUMU.AddEntry(EMU_OS_TOP, "e#mu, TTbar + Single Top MC", "lp")

        Canv_TOP_EMUtoMUMU.cd(1)
        CMS.cmsDraw(MUMU_OS_TOP, "P", mcolor=ROOT.kBlack)
        CMS.cmsDraw(EMU_OS_TOP, "P", mcolor=ROOT.kRed)


        latex_TOP_EMUtoMUMU_item = latex_temp.copy()
        latex_TOP_EMUtoMUMU_item[0] = f"{args.era}, #mu#mu/e#mu"
        latex_TOP_EMUtoMUMU_item[1] = latex_TOP_EMUtoMUMU_item[3]
        latex_TOP_EMUtoMUMU_item[2] = ""
        latex_TOP_EMUtoMUMU_item[3] = ""

        Latex_TOP_EMUtoMUMU = ROOT.TLatex()
        Latex_TOP_EMUtoMUMU.SetTextAlign(14);
        Latex_TOP_EMUtoMUMU.SetTextSize(0.04);
        Latex_TOP_EMUtoMUMU.SetTextFont(42);
        for idx, addon in enumerate(latex_TOP_EMUtoMUMU_item):
            Latex_TOP_EMUtoMUMU.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        Canv_TOP_EMUtoMUMU.cd(2)
        CMS.cmsDraw(TOP_EMUtoMUMU, "P", mcolor=ROOT.kBlack)

        CMS.SaveCanvas(Canv_TOP_EMUtoMUMU, os.path.join(outputPath, args.era + "_TOP_EMUtoMUMU" + case + ".pdf"))

        #################################################################
        # TT Comparison
        #################################################################
        Canv_TT_Comparision = CMS.cmsDiCanvas(
            "Canv_TT_Comparision",
            200,
            4000,
            2e-2,
            MUMU_OS_TT_DataDriven.GetMaximum() * 1e3,
            1 - 0.55,
            1 + 0.55,
            "M(#mu#mu) [GeV]",
            "Events",
            "Data/Pred.",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_TT_Comparision.cd(1).SetLogy(True)
        Canv_TT_Comparision.cd(1).SetLogx(True)
        Canv_TT_Comparision.cd(2).SetLogx(True)

        Canv_TT_Comparision.cd(1)

        Leg_TT_Comparision = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        Leg_TT_Comparision.AddEntry(MUMU_OS_TT_DataDriven, "TT, data-diven", "lp")

        TT_Comparision_Stack = ROOT.THStack("TT_Comparision_Stack", "Stacked")
        TT_Comparision_StackSheet = {
            "TT, MC": MUMU_OS_TT,
        }

        CMS.cmsDrawStack(TT_Comparision_Stack, Leg_TT_Comparision, TT_Comparision_StackSheet)
        
        Canv_TT_Comparision.cd(1)
        CMS.cmsDraw(MUMU_OS_TT_DataDriven, "P", mcolor=ROOT.kBlack)
        Canv_TT_Comparision.cd(1).RedrawAxis()

        latex_TT_Comparision_item = latex_temp.copy()
        latex_TT_Comparision_item[0] = f"{args.era}, TTbar"
        latex_TT_Comparision_item[1] = "p_{T}(#mu) > 52 (15) GeV, |#eta(#mu)| < 2.4"
        latex_TT_Comparision_item[2] = latex_TT_Comparision_item[3]
        latex_TT_Comparision_item[3] = ""
    
        Latex_TT_Comparision = ROOT.TLatex()
        Latex_TT_Comparision.SetTextAlign(14);
        Latex_TT_Comparision.SetTextSize(0.04);
        Latex_TT_Comparision.SetTextFont(42);
        for idx, addon in enumerate(latex_TT_Comparision_item):
            Latex_TT_Comparision.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        Canv_TT_Comparision.cd(2)
        CMS.cmsDraw(MUMU_OS_TT_DataDriven_Ratio, "P", mcolor=ROOT.kBlack)

        CMS.SaveCanvas(Canv_TT_Comparision, os.path.join(outputPath, args.era + "_TT_DataDriven" + case + ".pdf"))


        #################################################################
        # ST Comparison
        #################################################################
        Canv_ST_Comparision = CMS.cmsDiCanvas(
            "Canv_ST_Comparision",
            200,
            4000,
            2e-2,
            MUMU_OS_ST_DataDriven.GetMaximum() * 1e3,
            1 - 0.55,
            1 + 0.55,
            "M(#mu#mu) [GeV]",
            "Events",
            "Data/Pred.",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_ST_Comparision.cd(1).SetLogy(True)
        Canv_ST_Comparision.cd(1).SetLogx(True)
        Canv_ST_Comparision.cd(2).SetLogx(True)

        Canv_ST_Comparision.cd(1)

        Leg_ST_Comparision = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        Leg_ST_Comparision.AddEntry(MUMU_OS_ST_DataDriven, "Single Top, data-diven", "lp")

        ST_Comparision_Stack = ROOT.THStack("ST_Comparision_Stack", "Stacked")
        ST_Comparision_StackSheet = {
            "Single Top, MC": MUMU_OS_ST,
        }

        CMS.cmsDrawStack(ST_Comparision_Stack, Leg_ST_Comparision, ST_Comparision_StackSheet)
        
        Canv_ST_Comparision.cd(1)
        CMS.cmsDraw(MUMU_OS_ST_DataDriven, "P", mcolor=ROOT.kBlack)
        Canv_ST_Comparision.cd(1).RedrawAxis()

        latex_ST_Comparision_item = latex_temp.copy()
        latex_ST_Comparision_item[0] = f"{args.era}, Single Top"
        latex_ST_Comparision_item[1] = "p_{T}(#mu) > 52 (15) GeV, |#eta(#mu)| < 2.4"
        latex_ST_Comparision_item[2] = latex_ST_Comparision_item[3]
        latex_ST_Comparision_item[3] = ""
    
        Latex_ST_Comparision = ROOT.TLatex()
        Latex_ST_Comparision.SetTextAlign(14);
        Latex_ST_Comparision.SetTextSize(0.04);
        Latex_ST_Comparision.SetTextFont(42);
        for idx, addon in enumerate(latex_ST_Comparision_item):
            Latex_ST_Comparision.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        Canv_ST_Comparision.cd(2)
        CMS.cmsDraw(MUMU_OS_ST_DataDriven_Ratio, "P", mcolor=ROOT.kBlack)

        CMS.SaveCanvas(Canv_ST_Comparision, os.path.join(outputPath, args.era + "_ST_DataDriven" + case + ".pdf"))

        #################################################################
        # WW Comparison
        #################################################################
        Canv_WW_Comparision = CMS.cmsDiCanvas(
            "Canv_WW_Comparision",
            200,
            4000,
            2e-2,
            MUMU_OS_WW_DataDriven.GetMaximum() * 1e3,
            1 - 0.55,
            1 + 0.55,
            "M(#mu#mu) [GeV]",
            "Events",
            "Data/Pred.",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_WW_Comparision.cd(1).SetLogy(True)
        Canv_WW_Comparision.cd(1).SetLogx(True)
        Canv_WW_Comparision.cd(2).SetLogx(True)

        Canv_WW_Comparision.cd(1)

        Leg_WW_Comparision = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        Leg_WW_Comparision.AddEntry(MUMU_OS_WW_DataDriven, "WW, data-diven", "lp")

        WW_Comparision_Stack = ROOT.THStack("WW_Comparision_Stack", "Stacked")
        WW_Comparision_StackSheet = {
            "WW, MC": MUMU_OS_WW,
        }

        CMS.cmsDrawStack(WW_Comparision_Stack, Leg_WW_Comparision, WW_Comparision_StackSheet)
        
        Canv_WW_Comparision.cd(1)
        CMS.cmsDraw(MUMU_OS_WW_DataDriven, "P", mcolor=ROOT.kBlack)
        Canv_WW_Comparision.cd(1).RedrawAxis()

        latex_WW_Comparision_item = latex_temp.copy()
        latex_WW_Comparision_item[0] = f"{args.era}, Single Top"
        latex_WW_Comparision_item[1] = "p_{T}(#mu) > 52 (15) GeV, |#eta(#mu)| < 2.4"
        latex_WW_Comparision_item[2] = latex_WW_Comparision_item[3]
        latex_WW_Comparision_item[3] = ""
    
        Latex_WW_Comparision = ROOT.TLatex()
        Latex_WW_Comparision.SetTextAlign(14);
        Latex_WW_Comparision.SetTextSize(0.04);
        Latex_WW_Comparision.SetTextFont(42);
        for idx, addon in enumerate(latex_WW_Comparision_item):
            Latex_WW_Comparision.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        Canv_WW_Comparision.cd(2)
        CMS.cmsDraw(MUMU_OS_WW_DataDriven_Ratio, "P", mcolor=ROOT.kBlack)

        CMS.SaveCanvas(Canv_WW_Comparision, os.path.join(outputPath, args.era + "_WW_DataDriven" + case + ".pdf"))

        #################################################################
        # TOP Comparison
        #################################################################
        Canv_TOP_Comparision = CMS.cmsDiCanvas(
            "Canv_TOP_Comparision",
            200,
            4000,
            2e-2,
            MUMU_OS_TOP_DataDriven.GetMaximum() * 1e3,
            1 - 0.55,
            1 + 0.55,
            "M(#mu#mu) [GeV]",
            "Events",
            "Data/Pred.",
            square = CMS.kSquare,
            extraSpace = 0.1,
            iPos = 0,
        )

        Canv_TOP_Comparision.cd(1).SetLogy(True)
        Canv_TOP_Comparision.cd(1).SetLogx(True)
        Canv_TOP_Comparision.cd(2).SetLogx(True)

        Canv_TOP_Comparision.cd(1)

        Leg_TOP_Comparision = CMS.cmsLeg(0.70, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.03)
        Leg_TOP_Comparision.AddEntry(MUMU_OS_TOP_DataDriven, "TTbar + Single Top, data-diven", "lp")

        TOP_Comparision_Stack = ROOT.THStack("TOP_Comparision_Stack", "Stacked")
        TOP_Comparision_StackSheet = {
            "TT, MC": MUMU_OS_TT,
            "Single Top, MC": MUMU_OS_ST,
        }

        CMS.cmsDrawStack(TOP_Comparision_Stack, Leg_TOP_Comparision, TOP_Comparision_StackSheet)
        
        Canv_TOP_Comparision.cd(1)
        CMS.cmsDraw(MUMU_OS_TOP_DataDriven, "P", mcolor=ROOT.kBlack)
        Canv_TOP_Comparision.cd(1).RedrawAxis()

        latex_TOP_Comparision_item = latex_temp.copy()
        latex_TOP_Comparision_item[0] = f"{args.era}, TTbar + Single Top"
        latex_TOP_Comparision_item[1] = "p_{T}(#mu) > 52 (15) GeV, |#eta(#mu)| < 2.4"
        latex_TOP_Comparision_item[2] = latex_TOP_Comparision_item[3]
        latex_TOP_Comparision_item[3] = ""
    
        Latex_TOP_Comparision = ROOT.TLatex()
        Latex_TOP_Comparision.SetTextAlign(14);
        Latex_TOP_Comparision.SetTextSize(0.04);
        Latex_TOP_Comparision.SetTextFont(42);
        for idx, addon in enumerate(latex_TOP_Comparision_item):
            Latex_TOP_Comparision.DrawLatexNDC(0.18, 0.86 - idx * 0.065, addon.encode('utf-8'))

        Canv_TOP_Comparision.cd(2)
        CMS.cmsDraw(MUMU_OS_TOP_DataDriven_Ratio, "P", mcolor=ROOT.kBlack)

        CMS.SaveCanvas(Canv_TOP_Comparision, os.path.join(outputPath, args.era + "_TOP_DataDriven" + case + ".pdf"))



    outputFile.Close()


if __name__ == "__main__" :
    ROOT.TH1.AddDirectory(False)
    main(args)
