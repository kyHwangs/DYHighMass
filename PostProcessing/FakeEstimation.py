import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import uuid as uuid
import Config

ROOT.TH1.AddDirectory(False)
ROOT.TH1.SetDefaultSumw2()
ROOT.TDirectory.AddDirectory(0);


class FakeEstimation():

    def __init__(self, file_name):

        self.file_name = file_name
        
        os.system(f"cp {self.file_name} {self.file_name.replace('.root', '_FakeEstimation.root')}")

        self.file_output = ROOT.TFile(self.file_name.replace(".root", "_FakeEstimation.root"), "UPDATE")

    def GetDir(self, fDir, fName):
        fDirTemp =  fDir.GetDirectory(fName)

        if not fDirTemp:
            fDirTemp = fDir.mkdir(fName)

        if not fDirTemp:
            raise RuntimeError("Cannot create output directory")

        return fDirTemp

    def Store(self):

        aHist_SSInv_Data = self.file_output.Get("SS_inverted/Data/h_dimuonMass").Clone("aHist_SSInv_Data")

        aHist_SSInv_MC = aHist_SSInv_Data.Clone("aHist_SSInv_MC")
        aHist_SSInv_MC.Reset("ICESM")
        aHist_SSInv_MC.Add(self.file_output.Get("SS_inverted/DY_tautau/h_dimuonMass"))
        aHist_SSInv_MC.Add(self.file_output.Get("SS_inverted/TT/h_dimuonMass"))
        aHist_SSInv_MC.Add(self.file_output.Get("SS_inverted/WW/h_dimuonMass"))
        aHist_SSInv_MC.Add(self.file_output.Get("SS_inverted/WZ/h_dimuonMass"))
        aHist_SSInv_MC.Add(self.file_output.Get("SS_inverted/ZZ/h_dimuonMass"))
        aHist_SSInv_MC.Add(self.file_output.Get("SS_inverted/GG/h_dimuonMass"))

        aHist_SSInv_DataOnly = aHist_SSInv_Data.Clone("aHist_SSInv_DataOnly")
        aHist_SSInv_DataOnly.Add(aHist_SSInv_MC, -1)

        aHist_OSInv_Data = self.file_output.Get("OS_inverted/Data/h_dimuonMass").Clone("aHist_OSInv_Data")

        aHist_OSInv_MC = aHist_OSInv_Data.Clone("aHist_OSInv_MC")
        aHist_OSInv_MC.Reset("ICESM")
        aHist_OSInv_MC.Add(self.file_output.Get("OS_inverted/DY_tautau/h_dimuonMass"))
        aHist_OSInv_MC.Add(self.file_output.Get("OS_inverted/TT/h_dimuonMass"))
        aHist_OSInv_MC.Add(self.file_output.Get("OS_inverted/WW/h_dimuonMass"))
        aHist_OSInv_MC.Add(self.file_output.Get("OS_inverted/WZ/h_dimuonMass"))
        aHist_OSInv_MC.Add(self.file_output.Get("OS_inverted/ZZ/h_dimuonMass"))
        aHist_OSInv_MC.Add(self.file_output.Get("OS_inverted/GG/h_dimuonMass"))

        aHist_OSInv_DataOnly = aHist_OSInv_Data.Clone("aHist_OSInv_DataOnly")
        aHist_OSInv_DataOnly.Add(aHist_OSInv_MC, -1)

        aHist_SS_Data = self.file_output.Get("SS/Data/h_dimuonMass").Clone("aHist_SS_Data")
        
        aHist_SS_MC = aHist_SS_Data.Clone("aHist_SS_MC")
        aHist_SS_MC.Reset("ICESM")
        aHist_SS_MC.Add(self.file_output.Get("SS/DY_tautau/h_dimuonMass"))
        aHist_SS_MC.Add(self.file_output.Get("SS/TT/h_dimuonMass"))
        aHist_SS_MC.Add(self.file_output.Get("SS/WW/h_dimuonMass"))
        aHist_SS_MC.Add(self.file_output.Get("SS/WZ/h_dimuonMass"))
        aHist_SS_MC.Add(self.file_output.Get("SS/ZZ/h_dimuonMass"))
        aHist_SS_MC.Add(self.file_output.Get("SS/GG/h_dimuonMass"))

        aHist_SS_DataOnly = aHist_SS_Data.Clone("aHist_SS_DataOnly")
        aHist_SS_DataOnly.Add(aHist_SS_MC, -1)

        aHist_SStoOS = aHist_OSInv_DataOnly.Clone("aHist_SStoOS")
        aHist_SStoOS.Divide(aHist_SSInv_DataOnly)

        aHist_OS_Fake = aHist_SS_DataOnly.Clone("h_dimuonMass_Fake")
        aHist_OS_Fake.Multiply(aHist_SStoOS)

        tDir = self.GetDir(self.file_output, "Fake")
        tDir.cd()
        aHist_OS_Fake.Write("h_dimuonMass_Fake")
