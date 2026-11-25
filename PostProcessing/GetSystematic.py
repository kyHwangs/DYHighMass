import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import uuid as uuid
import Config

ROOT.TH1.AddDirectory(False)
ROOT.TH1.SetDefaultSumw2()
ROOT.TDirectory.AddDirectory(0);

class GetSystematic():

    def __init__(self, file_name):
        self.file_name = file_name
        self.file_input = ROOT.TFile(self.file_name, "READ")
        self.file_output = ROOT.TFile(self.file_name.replace("_unfolded.root", "_systematic.root"), "RECREATE")

        self.Nominal = self.file_input.Get("Nominal/hUnfolded")
        self.var = {
            "Nominal": self.Nominal
        }


        # self.vBasic.copy()
        self.vBasic = {
            "xLog": False,
            "yLog": True,
            "lumi": 137.88,
            "xMin": 0.5,
            "xMax": 42.5,
            "AutoYRange": True,
            "AutoRatioYRange": True,
            "xTitle": "Mass Bins",
            "yTitle": "Events",
            "rTitle": "Ratio",
            "outputPath": "./temp.pdf",
            "latex": [],
            "hist": {},
        }

    def GetDir(self, fDir, fName):
        fDirTemp =  fDir.GetDirectory(fName)

        if not fDirTemp:
            fDirTemp = fDir.mkdir(fName)

        if not fDirTemp:
            raise RuntimeError("Cannot create output directory")

        return fDirTemp


    def GetMax(self, lHist):
        
        if len(lHist) < 2:
            raise ValueError(f"GetMax: more than one histogram is allowed, only have {len(lHist)}")

        returnHist = lHist[0].Clone("returnHist")
        returnHist.Reset("ICESM")

        for idx in range(1, returnHist.GetNbinsX() + 1):

            max_diff = -9999
            
            for aHist in lHist:
                if abs(aHist.GetBinContent(idx) - self.Nominal.GetBinContent(idx)) > max_diff:
                    max_diff = abs(aHist.GetBinContent(idx) - self.Nominal.GetBinContent(idx))

            returnHist.SetBinContent(idx, max_diff)

        for idx in range(1, returnHist.GetNbinsX() + 1):
            returnHist.SetBinContent(idx, returnHist.GetBinContent(idx) / self.Nominal.GetBinContent(idx))

        return returnHist
    
    def GetDiff(self, lHist):

        if len(lHist) != 1:
            raise ValueError(f"GetDiff: only one histogram is allowed, only have {len(lHist)}")

        aHist = lHist[0]
        returnHist = aHist.Clone("returnHist")
        returnHist.Reset("ICESM")

        for idx in range(1, returnHist.GetNbinsX() + 1):
            returnHist.SetBinContent(idx, abs(aHist.GetBinContent(idx) - self.Nominal.GetBinContent(idx)))

        for idx in range(1, returnHist.GetNbinsX() + 1):
            returnHist.SetBinContent(idx, returnHist.GetBinContent(idx) / self.Nominal.GetBinContent(idx))

        return returnHist

    def GetEnsemble(self, lHist):

        if len(lHist) != 100:
            raise ValueError(f"GetEnsemble: 100 histograms are required, only have {len(lHist)}")

        histMap = []
        for idx in range(1, lHist[0].GetNbinsX() + 1):
            histMap.append(ROOT.TH1D(f"histMap_{idx}", "", 1000, -500, 500))

        for idx in range(1, lHist[0].GetNbinsX() + 1):
            for aHist in lHist:
                histMap[idx - 1].Fill(idx, aHist.GetBinContent(idx) - self.Nominal.GetBinContent(idx))

        returnHist = lHist[0].Clone("returnHist")
        returnHist.Reset("ICESM")

        for idx in range(1, lHist[0].GetNbinsX() + 1):
            returnHist.SetBinContent(idx, histMap[idx - 1].GetStdDev())

        for idx in range(1, returnHist.GetNbinsX() + 1):
            returnHist.SetBinContent(idx, returnHist.GetBinContent(idx) / self.Nominal.GetBinContent(idx))

        return returnHist

    def GetEnsembleAdd(self, lHist):
        
        if len(lHist) != 100:
            raise ValueError(f"GetEnsembleAdd: 100 histograms are required, only have {len(lHist)}")

        tmpHist = lHist[0].Clone("tmpHist")
        tmpHist.Reset("ICESM")

        for aHist in lHist:
            for idx in range(1, tmpHist.GetNbinsX() + 1):
                tmpValue = aHist.GetBinContent(idx) - self.Nominal.GetBinContent(idx)
                tmpHist.Fill(idx, tmpValue * tmpValue)

        returnHist = tmpHist.Clone("returnHist")
        returnHist.Reset("ICESM")

        for idx in range(1, returnHist.GetNbinsX() + 1):
            returnHist.SetBinContent(idx, tmpHist.GetBinContent(idx) ** 0.5)
        
        for idx in range(1, returnHist.GetNbinsX() + 1):
            returnHist.SetBinContent(idx, returnHist.GetBinContent(idx) / self.Nominal.GetBinContent(idx))

        return returnHist


    def Store(self):
        
        for syst_name, syst_val in syst_input.items():
        
            if syst_name == "Nominal":
                continue

            if not syst_val["isCor"]:
                for era in ["2016_preVFP", "2016_postVFP", "2017", "2018"]:

                    syst_name_era = f"{syst_name}_{era}"

                    tDir = self.GetDir(self.file_output, syst_name_era)

                    lhist = []
                    vHist = self.var.copy()
                    for iTer in range(1, syst_val["Iter"] + 1):
                        tmpHist = self.file_input.Get(f"{syst_name_era}/hUnfolded_{iTer}")
                        lhist.append(tmpHist)
                        vHist[f"h_{iTer}"] = tmpHist

                    h_Uncertainty = None

                    if syst_val["method"] == "Max":
                        h_Uncertainty = self.GetMax(lhist)
                    
                    elif syst_val["method"] == "diff":
                        h_Uncertainty = self.GetDiff(lhist)
                    
                    elif syst_val["method"] == "Ensemble":
                        h_Uncertainty = self.GetEnsemble(lhist)
                    
                    elif syst_val["method"] == "EnsembleAdd":
                        h_Uncertainty = self.GetEnsembleAdd(lhist)
                    else:
                        raise ValueError(f"Invalid method: {syst_val['method']}")

                    vCase = self.vBasic.copy()
                    vCase["hist"] = vHist
                    vCanvas_Single = SHE.GetSingleCanvas(vCase)
                    vCanvas_Double = SHE.GetDiCanvas(vCase)

                    tDir.cd()
                    h_Uncertainty.Write(f"h_Uncertainty")
                    vCanvas_Single.Write(f"Canvas_Single")
                    vCanvas_Double.Write(f"Canvas_Double")
            
            else:
                tDir = self.GetDir(self.file_output, f"{syst_name}")

                lhist = []
                vHist = self.var.copy()
                for iTer in range(1, syst_val["Iter"] + 1):
                    tmpHist = self.file_input.Get(f"{syst_name}/hUnfolded_{iTer}")
                    lhist.append(tmpHist)
                    vHist[f"h_{iTer}"] = tmpHist

                h_Uncertainty = None

                if syst_val["method"] == "Max":
                    h_Uncertainty = self.GetMax(lhist)
                
                elif syst_val["method"] == "diff":
                    h_Uncertainty = self.GetDiff(lhist)
                
                elif syst_val["method"] == "Ensemble":
                    h_Uncertainty = self.GetEnsemble(lhist)
                
                elif syst_val["method"] == "EnsembleAdd":
                    h_Uncertainty = self.GetEnsembleAdd(lhist)
                else:
                    raise ValueError(f"Invalid method: {syst_val['method']}")

                vCase = self.vBasic.copy()
                vCase["hist"] = vHist
                vCanvas_Single = SHE.GetSingleCanvas(vCase)
                vCanvas_Double = SHE.GetDiCanvas(vCase)

                tDir.cd()
                h_Uncertainty.Write(f"h_Uncertainty")
                vCanvas_Single.Write(f"Canvas_Single")
                vCanvas_Double.Write(f"Canvas_Double")

        for syst_name, syst_val in syst_input.items():
            if syst_name == "Nominal":
                continue

            if not syst_val["isCor"]:

                h_total_uncertainty = ROOT.TH1D("h_Uncertainty", "", 42, 0.5, 42.5)

                h2016_preVFP = self.file_output.Get(f"{syst_name}_2016_preVFP/h_Uncertainty")
                h2016_postVFP = self.file_output.Get(f"{syst_name}_2016_postVFP/h_Uncertainty")
                h2017 = self.file_output.Get(f"{syst_name}_2017/h_Uncertainty")
                h2018 = self.file_output.Get(f"{syst_name}_2018/h_Uncertainty")

                for idx in range(1, h_total_uncertainty.GetNbinsX() + 1):
                    h_total_uncertainty.SetBinContent(
                        idx,
                        (
                            h2016_preVFP.GetBinContent(idx) ** 2 +
                            h2016_postVFP.GetBinContent(idx) ** 2 +
                            h2017.GetBinContent(idx) ** 2 +
                            h2018.GetBinContent(idx) ** 2
                        ) ** 0.5
                    )

                tDir = self.GetDir(self.file_output, f"{syst_name}")
                tDir.cd()
                h_total_uncertainty.Write(f"h_Uncertainty")
