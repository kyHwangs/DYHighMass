import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import uuid as uuid
import Config

ROOT.TH1.AddDirectory(False)
ROOT.TH1.SetDefaultSumw2()
ROOT.TDirectory.AddDirectory(0);

class PlotMerger():

    def __init__(self, file_name):
        self.file_input = ROOT.TFile(file_name, "READ")
        self.file_output_name = file_name.replace(".root", "_Merged.root")
        self.file_output = ROOT.TFile(self.file_output_name, "RECREATE")

    def GetPlots(self, sample, histName, systName, iter, isCorrelated):

        if systName == "Nominal":

            print(f"2016_preVFP/{sample}/{histName}")
        
            aHist = self.file_input.Get(f"2016_preVFP/{sample}/{histName}").Clone(histName + "_merged" + uuid.uuid4().hex)
            aHist.Add(self.file_input.Get(f"2016_postVFP/{sample}/{histName}"))
            aHist.Add(self.file_input.Get(f"2017/{sample}/{histName}"))
            aHist.Add(self.file_input.Get(f"2018/{sample}/{histName}"))
        
            return aHist

        if isCorrelated:
            print(f"2016_preVFP/{sample}/{systName}/{histName}_{iter}")

            aHist = self.file_input.Get(f"2016_preVFP/{sample}/{systName}/{histName}_{iter}").Clone(histName + "_merged" + uuid.uuid4().hex)
            aHist.Add(self.file_input.Get(f"2016_postVFP/{sample}/{systName}/{histName}_{iter}"))
            aHist.Add(self.file_input.Get(f"2017/{sample}/{systName}/{histName}_{iter}"))
            aHist.Add(self.file_input.Get(f"2018/{sample}/{systName}/{histName}_{iter}"))

            return aHist

        else:
            aHist_2016preVFP = self.file_input.Get(f"2016_preVFP/{sample}/{systName}/{histName}_{iter}").Clone(histName + "_2016preVFP_merged" + uuid.uuid4().hex)
            aHist_2016preVFP.Add(self.file_input.Get(f"2016_postVFP/{sample}/{histName}"))
            aHist_2016preVFP.Add(self.file_input.Get(f"2017/{sample}/{histName}"))
            aHist_2016preVFP.Add(self.file_input.Get(f"2018/{sample}/{histName}"))

            aHist_2016postVFP = self.file_input.Get(f"2016_postVFP/{sample}/{systName}/{histName}_{iter}").Clone(histName + "_2016postVFP_merged" + uuid.uuid4().hex)
            aHist_2016postVFP.Add(self.file_input.Get(f"2016_preVFP/{sample}/{histName}"))
            aHist_2016postVFP.Add(self.file_input.Get(f"2017/{sample}/{histName}"))
            aHist_2016postVFP.Add(self.file_input.Get(f"2018/{sample}/{histName}"))

            aHist_2017 = self.file_input.Get(f"2017/{sample}/{systName}/{histName}_{iter}").Clone(histName + "_2017_merged" + uuid.uuid4().hex)
            aHist_2017.Add(self.file_input.Get(f"2016_preVFP/{sample}/{histName}"))
            aHist_2017.Add(self.file_input.Get(f"2016_postVFP/{sample}/{histName}"))
            aHist_2017.Add(self.file_input.Get(f"2018/{sample}/{histName}"))

            aHist_2018 = self.file_input.Get(f"2018/{sample}/{systName}/{histName}_{iter}").Clone(histName + "_2018_merged" + uuid.uuid4().hex)
            aHist_2018.Add(self.file_input.Get(f"2016_preVFP/{sample}/{histName}"))
            aHist_2018.Add(self.file_input.Get(f"2016_postVFP/{sample}/{histName}"))
            aHist_2018.Add(self.file_input.Get(f"2017/{sample}/{histName}"))

            return [aHist_2016preVFP, aHist_2016postVFP, aHist_2017, aHist_2018]

    def GetDir(self, fDir, fName):
        fDirTemp =  fDir.GetDirectory(fName)

        if not fDirTemp:
            fDirTemp = fDir.mkdir(fName)

        if not fDirTemp:
            raise RuntimeError("Cannot create output directory")

        return fDirTemp

    def Store(self):

        for sample_name, sample_list in samples_for_merger.items():
            tDirSample = self.GetDir(self.file_output, sample_name)

            plotName = "h_dimuonMass"
            if sample_name == "DY_Gen": plotName = "h_dimuonMassGen"
            if sample_name == "DY_Fake": plotName = "h_dimuonMassFake"
            if sample_name == "ResponseMatrix": plotName = "h_ResponseMatrix"

            for syst_name, syst_val in syst_input.items():

                print(sample_name, syst_name)

                if syst_name == "Nominal":

                    tDirSample.cd()
                    
                    aHist = self.GetPlots(sample_name, plotName, syst_name, 1, False)
                    aHist.Write(plotName)

                    if sample_name != "DY_Gen" and sample_name != "DY_Fake" and sample_name != "ResponseMatrix":

                        caselist = ["SS", "OS_inverted", "SS_inverted"]
                        for case in caselist:
                            tDirTmp = self.GetDir(self.file_output, case)
                            tDitTmpSample = self.GetDir(tDirTmp, sample_name)

                            aHist = self.GetPlots(f"{case}/{sample_name}", plotName, syst_name, 1, False)
                            tDitTmpSample.cd()
                            aHist.Write(plotName)


                else:
                    if sample_name == "Data":
                        continue

                    if not syst_val["isCor"]:

                        tDirSyst_2016preVFP = self.GetDir(tDirSample, f"{syst_name}_2016_preVFP")
                        tDirSyst_2016postVFP = self.GetDir(tDirSample, f"{syst_name}_2016_postVFP")
                        tDirSyst_2017 = self.GetDir(tDirSample, f"{syst_name}_2017")
                        tDirSyst_2018 = self.GetDir(tDirSample, f"{syst_name}_2018")

                        for iTer in range(1, syst_val["Iter"] + 1):

                            aHist = self.GetPlots(sample_name, plotName, syst_name, iTer, False)
                            
                            tDirSyst_2016preVFP.cd()
                            aHist[0].Write(f"{plotName}_{iTer}")

                            tDirSyst_2016postVFP.cd()
                            aHist[1].Write(f"{plotName}_{iTer}")

                            tDirSyst_2017.cd()
                            aHist[2].Write(f"{plotName}_{iTer}")

                            tDirSyst_2018.cd()
                            aHist[3].Write(f"{plotName}_{iTer}")

                    else:
                        tDirSyst = self.GetDir(tDirSample, f"{syst_name}")

                        for iTer in range(1, syst_val["Iter"] + 1):
                            aHist = self.GetPlots(sample_name, plotName, syst_name, iTer, True)
                            tDirSyst.cd()
                            aHist.Write(f"{plotName}_{iTer}")
