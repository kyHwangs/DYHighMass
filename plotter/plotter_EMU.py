import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array

parser = argparse.ArgumentParser()
parser.add_argument('-dir', help=' : output directory') 
parser.add_argument('-era', help=' : era to plot')
args = parser.parse_args()


CMS.SetExtraText("Preliminary")
CMS.SetEnergy("13")
ROOT.gROOT.SetBatch(ROOT.kTRUE)

mcList = ["NNLO_10to50", "NNLO_inc", "NNLO_100to200",
    "NNLO_200to400", "NNLO_400to500", "NNLO_500to700",
    "NNLO_700to800", "NNLO_800to1000", "NNLO_1000to1500",
    "NNLO_1500to2000", "NNLO_2000toInf", "NNLO_tautau",
    "ST_s", "ST_t_AntiTop", "ST_t_Top", "ST_tW_AntiTop",
    "ST_tW_Top", "TTTo2L2Nu", "WW", "WZ", "ZZ"]

dylist = ["NNLO_10to50", "NNLO_inc", "NNLO_100to200",
    "NNLO_200to400", "NNLO_400to500", "NNLO_500to700",
    "NNLO_700to800", "NNLO_800to1000", "NNLO_1000to1500",
    "NNLO_1500to2000", "NNLO_2000toInf"]

stlist = ["ST_s", "ST_t_AntiTop", "ST_t_Top", "ST_tW_AntiTop", "ST_tW_Top"]

ewlist = ["WW", "WZ", "ZZ"]

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
    "ZZ": 12.17
}

normFactor = {}


class Plotter:
    def __init__(self, baseDir, era):
        self.baseDir = baseDir
        self.rootPath = "../Batch/" + baseDir + "/" + era + "/"
        self.era = era
        self.outputPath = "./pdf/" + baseDir + "/plot_" + era + "/"

        os.makedirs(self.outputPath, exist_ok=True)

        self.lumi = refLumi[self.era]
        CMS.SetLumi(self.lumi)

        self.PrepareFiles()
        self.PrepareNorm()


    def PrepareFiles(self):
        self.fileSet = {}
        self.fileSet["Data"] = ROOT.TFile(self.rootPath + "Data.root", "READ")
        self.fileSet["NNLO_10to50"] = ROOT.TFile(self.rootPath + "NNLO_10to50.root", "READ")
        self.fileSet["NNLO_inc"] = ROOT.TFile(self.rootPath + "NNLO_inc.root", "READ")
        self.fileSet["NNLO_100to200"] = ROOT.TFile(self.rootPath + "NNLO_100to200.root", "READ")
        self.fileSet["NNLO_200to400"] = ROOT.TFile(self.rootPath + "NNLO_200to400.root", "READ")
        self.fileSet["NNLO_400to500"] = ROOT.TFile(self.rootPath + "NNLO_400to500.root", "READ")
        self.fileSet["NNLO_500to700"] = ROOT.TFile(self.rootPath + "NNLO_500to700.root", "READ")
        self.fileSet["NNLO_700to800"] = ROOT.TFile(self.rootPath + "NNLO_700to800.root", "READ")
        self.fileSet["NNLO_800to1000"] = ROOT.TFile(self.rootPath + "NNLO_800to1000.root", "READ")
        self.fileSet["NNLO_1000to1500"] = ROOT.TFile(self.rootPath + "NNLO_1000to1500.root", "READ")
        self.fileSet["NNLO_1500to2000"] = ROOT.TFile(self.rootPath + "NNLO_1500to2000.root", "READ")
        self.fileSet["NNLO_2000toInf"] = ROOT.TFile(self.rootPath + "NNLO_2000toInf.root", "READ")
        self.fileSet["NNLO_tautau"] = ROOT.TFile(self.rootPath + "NNLO_tautau.root", "READ")
        self.fileSet["ST_s"] = ROOT.TFile(self.rootPath + "ST_s.root", "READ")
        self.fileSet["ST_tW_AntiTop"] = ROOT.TFile(self.rootPath + "ST_tW_AntiTop.root", "READ")
        self.fileSet["ST_tW_Top"] = ROOT.TFile(self.rootPath + "ST_tW_Top.root", "READ")
        self.fileSet["ST_t_AntiTop"] = ROOT.TFile(self.rootPath + "ST_t_AntiTop.root", "READ")
        self.fileSet["ST_t_Top"] = ROOT.TFile(self.rootPath + "ST_t_Top.root", "READ")
        self.fileSet["TTTo2L2Nu"] = ROOT.TFile(self.rootPath + "TTTo2L2Nu.root", "READ")
        # self.fileSet["WJetsToLNu"] = ROOT.TFile(self.rootPath + "WJetsToLNu.root", "READ")
        self.fileSet["WW"] = ROOT.TFile(self.rootPath + "WW.root", "READ")
        self.fileSet["WZ"] = ROOT.TFile(self.rootPath + "WZ.root", "READ")
        self.fileSet["ZZ"] = ROOT.TFile(self.rootPath + "ZZ.root", "READ")

    def PrepareNorm(self):
        for mcSet in mcList:
            nEvent = self.fileSet[mcSet].Get("h_EventInfo").GetBinContent(4)
            normFactor[mcSet] = (1000. * self.lumi * xSec[mcSet]) / nEvent;

    def Plot(self, histName, hook,
        xmin = -1, 
        xmax = -1, 
        ymin = -1, 
        ymax = -1, 
        yrmin = -1, 
        yrmax = -1,
        xTitle = "", 
        yTitle = "Events", 
        ratioTitle = "Pred./Data",
        logy = True, 
        logx = False
        ):

        doAutoXrange = False
        doAutoYrange = False
        doAutoYRatiorange = False

        if (xmin == -1 and xmax == -1): doAutoXrange = True
        if (ymin == -1 and ymax == -1): doAutoYrange = True
        if (yrmin == -1 and yrmax == -1): doAutoYRatiorange = True

        self.histName = histName
        canvasName = self.era + "_" + histName

        data = self.fileSet["Data"].Get(self.histName)
        DY = self.GetMCHist("DY")
        TT = self.GetMCHist("TTTo2L2Nu")
        ST = self.GetMCHist("ST")
        DY_tau = self.GetMCHist("NNLO_tautau")
        EW = self.GetMCHist("EW")
        # WJets = self.GetMCHist("WJetsToLNu")

        # nXBins = 13
        # XBins = array.array('d', [200, 220, 243, 273, 320, 380, 440, 510, 600, 700, 830, 1000, 1500, 4000])

        # nPtBins = 98
        # XPtBin = array.array('d', [0,   15,  21,  27,  32,  37,  42,  47,  52,  57, 
        #                           62,  67,  72,  77,  82,  87,  92,  97,  102, 107, 
        #                           112, 117, 122, 127, 132, 137, 142, 147, 152, 157, 
        #                           162, 167, 172, 177, 182, 187, 192, 197, 202, 207, 
        #                           212, 217, 222, 227, 232, 237, 242, 247, 252, 257, 
        #                           262, 267, 272, 277, 282, 287, 292, 297, 302, 307, 
        #                           312, 317, 322, 327, 332, 337, 342, 347, 352, 357, 
        #                           362, 367, 372, 377, 382, 387, 392, 397, 402, 407, 
        #                           412, 417, 422, 427, 432, 437, 442, 447, 452, 457, 
        #                           462, 467, 472, 477, 482, 487, 492, 497, 500])

        # WJets = self.GetMCHist("WJetsToLNu")

        # if "dimuonMass_wide" in histName: 
        #     data = data.Rebin(nXBins, "data", XBins)
        #     DY = DY.Rebin(nXBins, "DY", XBins)
        #     TT = TT.Rebin(nXBins, "TT", XBins)
        #     ST = ST.Rebin(nXBins, "ST", XBins)
        #     DY_tau = DY_tau.Rebin(nXBins, "DY_tau", XBins)
        #     EW = EW.Rebin(nXBins, "EW", XBins)
        
        # if "h_JetPt" in histName: 
        #     data = data.Rebin(5, "data")
        #     DY = DY.Rebin(5, "DY")
        #     TT = TT.Rebin(5, "TT")
        #     ST = ST.Rebin(5, "ST")
        #     DY_tau = DY_tau.Rebin(5, "DY_tau")
        #     EW = EW.Rebin(5, "EW")

        # if "h_BJetPt" in histName: 
        #     data = data.Rebin(5, "data")
        #     DY = DY.Rebin(5, "DY")
        #     TT = TT.Rebin(5, "TT")
        #     ST = ST.Rebin(5, "ST")
        #     DY_tau = DY_tau.Rebin(5, "DY_tau")
        #     EW = EW.Rebin(5, "EW")

        # if "h_LeadingMuonPt" in histName: 
        #     data = data.Rebin(nPtBins, "data", XPtBin)
        #     DY = DY.Rebin(nPtBins, "DY", XPtBin)
        #     TT = TT.Rebin(nPtBins, "TT", XPtBin)
        #     ST = ST.Rebin(nPtBins, "ST", XPtBin)
        #     DY_tau = DY_tau.Rebin(nPtBins, "DY_tau", XPtBin)
        #     EW = EW.Rebin(nPtBins, "EW", XPtBin)

        # if "h_SubleadingMuonPt" in histName: 
        #     data = data.Rebin(nPtBins, "data", XPtBin)
        #     DY = DY.Rebin(nPtBins, "DY", XPtBin)
        #     TT = TT.Rebin(nPtBins, "TT", XPtBin)
        #     ST = ST.Rebin(nPtBins, "ST", XPtBin)
        #     DY_tau = DY_tau.Rebin(nPtBins, "DY_tau", XPtBin)
        #     EW = EW.Rebin(nPtBins, "EW", XPtBin)

        # if "h_MuonPt" in histName: 
        #     data = data.Rebin(nPtBins, "data", XPtBin)
        #     DY = DY.Rebin(nPtBins, "DY", XPtBin)
        #     TT = TT.Rebin(nPtBins, "TT", XPtBin)
        #     ST = ST.Rebin(nPtBins, "ST", XPtBin)
        #     DY_tau = DY_tau.Rebin(nPtBins, "DY_tau", XPtBin)
        #     EW = EW.Rebin(nPtBins, "EW", XPtBin)

        # if "h_dimuonPt" in histName: 
        #     data = data.Rebin(5, "data")
        #     DY = DY.Rebin(5, "DY")
        #     TT = TT.Rebin(5, "TT")
        #     ST = ST.Rebin(5, "ST")
        #     DY_tau = DY_tau.Rebin(5, "DY_tau")
        #     EW = EW.Rebin(5, "EW")

        
        MC = TT.Clone(f"MC_{uuid.uuid4()}")
        MC.Add(ST)
        MC.Add(DY_tau)
        MC.Add(EW)

        if "Pt" in histName:
            XPtBin = array.array('d', [15, 32, 52, 82, 112, 142, 172, 202, 252, 302, 352, 402, 452, 500, 550, 600, 700, 800, 900, 1000, 1120, 1240, 1360, 1500, 1520])

            nXPtBin = len(XPtBin) - 1

            MC = MC.Rebin(nXPtBin, f"MC_{uuid.uuid4()}", XPtBin)
            data = data.Rebin(nXPtBin, f"data_{uuid.uuid4()}", XPtBin)
            DY = DY.Rebin(nXPtBin, f"DY_{uuid.uuid4()}", XPtBin)
            TT = TT.Rebin(nXPtBin, f"TT_{uuid.uuid4()}", XPtBin)
            ST = ST.Rebin(nXPtBin, f"ST_{uuid.uuid4()}", XPtBin)
            DY_tau = DY_tau.Rebin(nXPtBin, f"DY_tau_{uuid.uuid4()}", XPtBin)
            EW = EW.Rebin(nXPtBin, f"EW_{uuid.uuid4()}", XPtBin)

        dataOmc = MC.Clone(f"ratio_{uuid.uuid4()}")
        dataOmc.Divide(data)

        if doAutoXrange:
            xmin = MC.GetBinLowEdge(1)
            xmax = MC.GetBinLowEdge(MC.GetNbinsX()) +  MC.GetBinWidth(MC.GetNbinsX())

        if doAutoYrange:
            ymin = 2e-2
            ymax = data.GetBinContent(data.GetMaximumBin()) * 1e5



        if "dimuonMass_wide" in histName: 
            ymin = 2e-2


        # if doAutoYRatiorange:
        #     nBinsX = dataOmc.GetNbinsX()
        #     residual = -999
        #     for i in range(1, nBinsX + 1):
        #         if dataOmc.GetBinCenter(i) > xmin and dataOmc.GetBinCenter(i) < xmax:
        #             if dataOmc.GetBinContent(i) > 0.2 and dataOmc.GetBinContent(i) < 1.8:
        #                 if (residual < abs(1 - dataOmc.GetBinContent(i))):
        #                     residual = abs(1 - dataOmc.GetBinContent(i))

        #     if residual < 0.05:
        #         yrmin = 1 - 0.05
        #         yrmax = 1 + 0.05

        #     elif residual < 0.1:
        #         yrmin = 1 - 0.10
        #         yrmax = 1 + 0.10
            
        #     elif residual < 0.2:
        #         yrmin = 1 - 0.20
        #         yrmax = 1 + 0.20
            
        #     elif residual < 0.4:
        #         yrmin = 1 - 0.40
        #         yrmax = 1 + 0.40

        #     else:
        #         yrmin = 0.5
        #         yrmax = 1.5

        yrmin = 0.0
        yrmax = 0.2

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

        leg = CMS.cmsLeg(0.60, 0.89 - 0.05 * 6, 0.89, 0.89, textSize=0.05)
        leg.AddEntry(data, "Data", "lp")

        stackSeet = {
            # "WJets": WJets,
            "DY_tautau": DY_tau,
            "Single Top": ST,
            "WW + WZ + ZZ": EW,
            "TT": TT
            # "DY": DY
        }

        CMS.cmsDrawStack(stack, leg, stackSeet)
        CMS.cmsDraw(data, "P", mcolor=ROOT.kBlack)

        # TLatex latex;
        # Latex_Preliminary_13p6TeV( latex );
        # //latex.DrawLatexNDC( 0.45,0.96, "#scale[0.8]{#font[42]{"+SAMPLE+"}}");
        # latex.DrawLatexNDC(0.16, 0.90, "#font[42]{#scale[0.6]{"+L3str+"}}");
        # latex.DrawLatexNDC(0.735, 0.89, "#font[42]{#scale[0.8]{"+etas_str_long.at(i_eta)+"}}");
        # if(v_var[ivar] != "pt" ) latex.DrawLatexNDC(0.68, 0.84, "#font[42]{#scale[0.8]{"+v_pts_str.at(ipt)+"}}");

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

        CMS.SaveCanvas(dicanv, os.path.join(self.outputPath, self.era + "_" + histName + ".pdf"))

    def GetMCHist(self, mcName):
        if mcName == "DY":
            return self.GetDYHist()
        elif mcName == "ST":
            return self.GetSingleTopHist()
        elif mcName == "EW":
            return self.GetEWHist()
        else:
            hist = self.fileSet[mcName].Get(self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            hist.SetDirectory(0)
            hist.SetStats(0);
            hist.Scale(normFactor[mcName]);

            return hist

    def GetDYHist(self):
        histoSet = {}
        for mc in dylist:
            histoSet[mc] = self.fileSet[mc].Get(self.histName)
            histoSet[mc].SetDirectory(0)
            histoSet[mc].SetStats(0);
            histoSet[mc].Scale(normFactor[mc]);

        returnHist = histoSet["NNLO_inc"].Clone(f"DY_{uuid.uuid4()}")
        for mc in dylist:
            if (mc != "NNLO_inc"):
                returnHist.Add(histoSet[mc])

        return returnHist


    def GetSingleTopHist(self):
        histoSet = {}
        for mc in stlist:
            histoSet[mc] = self.fileSet[mc].Get(self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            histoSet[mc].SetDirectory(0)
            histoSet[mc].SetStats(0);
            histoSet[mc].Scale(normFactor[mc]);

        returnHist = histoSet["ST_s"].Clone(f"SingleTop_{uuid.uuid4()}")
        for mc in stlist:
            if (mc != "ST_s"):
                returnHist.Add(histoSet[mc])

        return returnHist

    def GetEWHist(self):
        histoSet = {}
        for mc in ewlist:
            histoSet[mc] = self.fileSet[mc].Get(self.histName).Clone(f"{self.histName}_{uuid.uuid4()}")
            histoSet[mc].SetDirectory(0)
            histoSet[mc].SetStats(0);
            histoSet[mc].Scale(normFactor[mc]);

        returnHist = histoSet["WW"].Clone(f"EW_{uuid.uuid4()}")
        for mc in ewlist:
            if (mc != "WW"):
                returnHist.Add(histoSet[mc])

        return returnHist




def main(args):
    plotter = Plotter(args.dir, args.era)

    # cases = ["", "_0J", "_1J", "_mtJ", "_0BJ", "_1BJ", "_mt1BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    cases = ["", "_0BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    # cases = ["_0BJ"]
    # cases = [""]
    # massBins = ["", "_m200_220","_m220_243","_m243_273","_m273_320","_m320_380","_m380_440","_m440_510","_m510_600","_m600_700","_m700_830","_m830_1000","_m1000_1500","_m1500_4000"] 
    # massBins = ["", "_m700_830","_m830_1000","_m1000_1500","_m1500_4000"] 
    massBins = [""] 

    addon_hook = {
        "": "",
        "_0J": "N(jet) = 0",
        "_1J": "N(jet) = 1",
        "_mtJ": "N(jet) > 1",
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
        "_mtJ": -1,
        "_0BJ": 0.85,
        "_1BJ": -1,
        "_mt1BJ": -1,
        "_bVeto_0J": 0.7,
        "_bVeto_1J": 0.5,
        "_bVeto_mt1J": 0.5,  
    }

    addon_hook_mass = {
        "": "200 GeV < M_{e#mu}",
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
        "p_{T}(#mu(e)) > 52 (20) GeV, |#eta(l)| < 2.4",
        "Opposite-sign e#mu pair",
        "",
        "",
    ]

    # plotter.Plot("h_PileUp_Count_Interaction_before", latex, xTitle = "# of True Interaction", xmin = 0, xmax = 100)
    # plotter.Plot("h_PileUp_Count_Interaction_after", latex, xTitle = "# of True Interaction", xmin = 0, xmax = 100)

    for case in cases:

        latex_temp = latex.copy()
        latex_temp[3] = addon_hook[case]

        plotter.Plot("h_PairMass" + case                                  , latex_temp, xTitle = "M(e#mu) [GeV]"   ,xmin = 200, xmax = 4000, logy = True, logx = True)

        for massbin in massBins:

            latex_temp[4] = addon_hook_mass[massbin]

            plotter.Plot("h_JetPt" + case + massbin                                     , latex_temp, xTitle = "pT(jet) [GeV]"          ,xmin = 0, xmax = 500, logy = True)
            plotter.Plot("h_JetEta" + case + massbin                                    , latex_temp, xTitle = "#eta(jet)"              ,xmin = -2.4, xmax = 2.4, logy = True)
            plotter.Plot("h_JetPhi" + case + massbin                                    , latex_temp, xTitle = "#phi(jet)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

            plotter.Plot("h_BJetPt" + case + massbin                                    , latex_temp, xTitle = "pT(b-jet) [GeV]"          ,xmin = 0, xmax = 500, logy = True)
            plotter.Plot("h_BJetEta" + case + massbin                                   , latex_temp, xTitle = "#eta(b-jet)"              ,xmin = -2.4, xmax = 2.4, logy = True)
            plotter.Plot("h_BJetPhi" + case + massbin                                   , latex_temp, xTitle = "#phi(b-jet)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

            plotter.Plot("h_MuonPt" + case + massbin                                    , latex_temp, xTitle = "pT(#mu) [GeV]"          ,xmin = 15, xmax = 1520, logy = True, logx = True)
            plotter.Plot("h_MuonEta" + case + massbin                                   , latex_temp, xTitle = "#eta(#mu)"              ,xmin = -2.4, xmax = 2.4, logy = True)
            plotter.Plot("h_MuonPhi" + case + massbin                                   , latex_temp, xTitle = "#phi(#mu)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

            plotter.Plot("h_ElecPt" + case + massbin                                    , latex_temp, xTitle = "pT(e) [GeV]"           ,xmin = 15, xmax = 1520, logy = True, logx = True)
            plotter.Plot("h_ElecEta" + case + massbin                                   , latex_temp, xTitle = "#eta(e)"               ,xmin = -2.4, xmax = 2.4, logy = True)
            plotter.Plot("h_ElecPhi" + case + massbin                                   , latex_temp, xTitle = "#phi(e)"               ,xmin = -3.141593, xmax = 3.141593, logy = True)

            plotter.Plot("h_PairDeltaR" + case + massbin                                , latex_temp, xTitle = "#DeltaR(e, #mu)"       ,xmin = 0, xmax = 6.4, logy = True)

            plotter.Plot("h_PairPt" + case + massbin                                    , latex_temp, xTitle = "pT(e#mu) [GeV]"        ,xmin = 0, xmax = 500, logy = True)
            plotter.Plot("h_PairRap" + case + massbin                                   , latex_temp, xTitle = "rapidity(e#mu)"        ,xmin = -2.4, xmax = 2.4, logy = True)



if __name__ == "__main__" :
    ROOT.TH1.AddDirectory(False)
    main(args)
