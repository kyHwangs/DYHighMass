import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import uuid as uuid
import Config

ROOT.TH1.AddDirectory(False)
ROOT.TH1.SetDefaultSumw2()
ROOT.TDirectory.AddDirectory(0);

class Plotter:
    def __init__(self, file_name):
        self.file_name = file_name
        self.file_input = ROOT.TFile(self.file_name, "READ")
        self.file_output = ROOT.TFile(self.file_name.replace(".root", "_plot.root"), "RECREATE")

        self.vBasic = {
            "xLog": False,
            "yLog": False,
            "lumi": 137.88,
            "xMin": 0.5,
            "xMax": 42.5,
            "AutoYRange": True,
            "AutoRatioYRange": True,
            "xTitle": "Mass Bins",
            "yTitle": "Rel. Unc.",
            "rTitle": "Ratio",
            "outputPath": "./temp.pdf",
            "latex": [],
            "hist": {},
        }

    def MakeUncertaintyEnvelope(self, lHist):

        tempHist2 = ROOT.TH1D("tempHist2", "", 42, 0.5, 42.5)
        tempHist = ROOT.TH1D("tempHist", "", 42, 0.5, 42.5)

        for hCol_name, hCol_hist in lHist.items():
            for idx in range(1, hCol_hist.GetNbinsX() + 1):
                tempHist2.Fill(idx, hCol_hist.GetBinContent(idx) ** 2)

        for idx in range(1, tempHist2.GetNbinsX() + 1):
            tempHist.SetBinContent(idx, tempHist2.GetBinContent(idx) ** 0.5)

        return tempHist

    def Store(self):

        hColTotal = {}
        hColMuonEff = {}
        hColMuonP = {}
        hColJetE = {}

        for syst_name, syst_val in syst_group.items():

            # if syst_name == "PDF": continue
            
            hCol = {}
            for syst_case in syst_val["input"]:
                hCol[syst_input[syst_case]["pretty"]] = self.file_input.Get(f"{syst_case}/h_Uncertainty")
    
            tempHist = self.MakeUncertaintyEnvelope(hCol)
            hCol["Total"] = tempHist

            if syst_name.find("MuonEff") != -1: hColMuonEff[syst_val["name"]] = tempHist
            elif syst_name == "MuonSmearing" or syst_name == "MuonScaling": hColMuonP[syst_val["name"]] = tempHist
            elif syst_name == "JetEnergySmearing" or syst_name == "JetEnergyScale": hColJetE[syst_val["name"]] = tempHist
            else: hColTotal[syst_val["name"]] = tempHist

            vCol = self.vBasic.copy()
            vCol["hist"] = hCol
            vCol["latex"] = [syst_val["name"]]

            vCanvas = SHE.GetSingleCanvasUnordered(vCol)

            CMS.SaveCanvas(vCanvas, f"./plots/unc_{syst_name}.pdf")

        hColTotal["Muon Efficiency"] = self.MakeUncertaintyEnvelope(hColMuonEff)
        hColTotal["Muon Momentum"] = self.MakeUncertaintyEnvelope(hColMuonP)
        hColTotal["Jet Energy"] = self.MakeUncertaintyEnvelope(hColJetE)

        hColTotal["Total"] = self.MakeUncertaintyEnvelope(hColTotal)

        vCol = self.vBasic.copy()
        vCol["hist"] = hColTotal
        vCol["latex"] = ["Total Uncertainty"]

        print(len(hColTotal))
        print(hColTotal.keys())

        vCanvas = SHE.GetSingleCanvasUnordered(vCol)

        CMS.SaveCanvas(vCanvas, f"./plots/unc_Total.pdf")

