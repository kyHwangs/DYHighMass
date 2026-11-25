import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import uuid as uuid
import Config

ROOT.TH1.AddDirectory(False)
ROOT.TH1.SetDefaultSumw2()
ROOT.TDirectory.AddDirectory(0);

class PlotContainter:

    def __init__ (self, file_name):

        self.template_1D = ROOT.TH1F("template_1D", "", 42, 0.5, 42.5)
        self.template_2D = ROOT.TH2F("template_2D", "", 42, 0.5, 42.5, 42, 0.5, 42.5)

        self.file_input = ROOT.TFile(file_name, "READ")
        self.file_output_name = file_name.replace(".root", "_FlatCollection.root")
        self.file_output = ROOT.TFile(self.file_output_name, "RECREATE")

    def GetDir(self, fDir, fName):
        fDirTemp =  fDir.GetDirectory(fName)

        if not fDirTemp:
            fDirTemp = fDir.mkdir(fName)

        if not fDirTemp:
            raise RuntimeError("Cannot create output directory")

        return fDirTemp

    def SanityCheck(self, hist):

        returnHist = self.template_1D.Clone(hist.GetName() + "_sanityCheck" + uuid.uuid4().hex)
        
        for i in range(1, hist.GetNbinsX() + 1):
            if hist.GetBinContent(i) < 0:
                returnHist.SetBinContent(i, 0)
                returnHist.SetBinError(i, hist.GetBinError(i))
            else:
                returnHist.SetBinContent(i, hist.GetBinContent(i))
                returnHist.SetBinError(i, hist.GetBinError(i))
        
        return returnHist

    
    def DecomposeSystIter(self, histOrigin, nIter):

        hist_name = histOrigin.GetName()
        if hist_name.find("ResponseMatrix") != -1:
   
            hist_2D = ROOT.TH2F(hist_name + uuid.uuid4().hex, "", 42, 0.5, 42.5, 42, 0.5, 42.5)

            for x_idx in range(0, 42 + 1):
                for y_idx in range(1, 42 + 1):
                    hist_2D.SetBinContent(x_idx, y_idx, histOrigin.GetBinContent(x_idx, y_idx, nIter))
                    hist_2D.SetBinError(x_idx, y_idx, histOrigin.GetBinError(x_idx, y_idx, nIter))

            return hist_2D

        else:

            hist_1D = ROOT.TH1F(hist_name + uuid.uuid4().hex, "", 42, 0.5, 42.5)
            
            for bin_idx in range(1, 42 + 1):
                hist_1D.SetBinContent(bin_idx, histOrigin.GetBinContent(bin_idx, nIter))
                hist_1D.SetBinError(bin_idx, histOrigin.GetBinError(bin_idx, nIter))

            return hist_1D
    
    def Store(self):

        for era in eras:
            tDirEra = self.GetDir(self.file_output, era)
            
            for syst_name, syst_val in syst_input.items():
                for sample_name, sample_list in samples.items():

                    if syst_name == "Nominal":
                        if args.input == "MUMU_OS_Syst.root":
                            tDirSample = self.GetDir(tDirEra, sample_name)
                            returnHist = self.file_input.Get(f"{era}/{sample_list[0]}/OS/h_dimuonMass").Clone(f"h_dimuonMass")
                            # returnHist = self.SanityCheck(returnHist)
                            tDirSample.cd()
                            returnHist.Write("h_dimuonMass")

                        if args.input == "MUMU_SS_Syst.root":
                            tDirSS = self.GetDir(tDirEra, "SS")
                            tDirSSSample = self.GetDir(tDirSS, sample_name)
                            tDirSSSample.cd()
                            returnHist = self.file_input.Get(f"{era}/{sample_list[0]}/OS/h_dimuonMass").Clone(f"h_dimuonMass")
                            # returnHist = self.SanityCheck(returnHist)
                            returnHist.Write("h_dimuonMass")

                        if args.input == "MUMU_OS_inverted_Syst.root":
                            tDirSS = self.GetDir(tDirEra, "OS_inverted")
                            tDirSSSample = self.GetDir(tDirSS, sample_name)
                            tDirSSSample.cd()
                            returnHist = self.file_input.Get(f"{era}/{sample_list[0]}/OS/h_dimuonMass").Clone(f"h_dimuonMass")
                            # returnHist = self.SanityCheck(returnHist)
                            returnHist.Write("h_dimuonMass")

                        if args.input == "MUMU_SS_inverted_Syst.root":
                            tDirSS = self.GetDir(tDirEra, "SS_inverted")
                            tDirSSSample = self.GetDir(tDirSS, sample_name)
                            tDirSSSample.cd()
                            returnHist = self.file_input.Get(f"{era}/{sample_list[0]}/OS/h_dimuonMass").Clone(f"h_dimuonMass")
                            # returnHist = self.SanityCheck(returnHist)
                            returnHist.Write("h_dimuonMass")



                    else:
                        if sample_name == "Data":
                            continue

                        tDirSample = self.GetDir(tDirEra, sample_name)
                        tDirSyst = self.GetDir(tDirSample, syst_name)
                        tDirSyst.cd()
                        
                        returnHist = self.file_input.Get(f"{era}/{sample_list[0]}/OS/syst/{syst_name}/h_dimuonMass")
                        returnHist.Write("h_dimuonMass_syst")

                        for iTer in range(1, syst_val["Iter"] + 1):
                            aTmpHist = self.DecomposeSystIter(returnHist, iTer)
                            # aTmpHist = self.SanityCheck(aTmpHist)
                            aTmpHist.Write(f"h_dimuonMass_{iTer}")

                    if sample_name == "DY":

                        if args.input == "MUMU_OS_Syst.root":

                            tDirGen = self.GetDir(tDirEra, "DY_Gen")
                            tDirFake = self.GetDir(tDirEra, "DY_Fake")
                            tDirResponse = self.GetDir(tDirEra, "ResponseMatrix")

                            if syst_name == "Nominal":

                                tDirGen = self.GetDir(tDirEra, "DY_Gen")
                                tDirGen.cd()
                                returnHist = self.file_input.Get(f"{era}/NNLO_MUMU/OS/h_dimuonMuonGen")
                                returnHist.Write("h_dimuonMassGen")

                                tDirFake.cd()
                                returnHist = self.file_input.Get(f"{era}/NNLO_MUMU/OS/h_dimuonMuonFake")
                                returnHist.Write("h_dimuonMassFake")

                                tDirResponse.cd()
                                returnHist = self.file_input.Get(f"{era}/NNLO_MUMU/OS/h_ResponseMatrix")
                                returnHist.Write("h_ResponseMatrix")

                            else:

                                if not args.detail:
                                    continue

                                tDirGenSyst = self.GetDir(tDirGen, syst_name)
                                tDirGenSyst.cd()
                                
                                tmpHist = self.file_input.Get(f"{era}/NNLO_MUMU/OS/syst/{syst_name}/h_dimuonMuonGen")
                                tmpHist.Write("h_dimuonMuonGen_Syst")
                                for iTer in range(1, syst_val["Iter"] + 1):
                                    iTerHist = self.DecomposeSystIter(tmpHist, iTer)
                                    iTerHist.Write(f"h_dimuonMassGen_{iTer}")


                                tDirFakeSyst = self.GetDir(tDirFake, syst_name)
                                tDirFakeSyst.cd()

                                tmpHist = self.file_input.Get(f"{era}/NNLO_MUMU/OS/syst/{syst_name}/h_dimuonMuonFake")
                                tmpHist.Write("h_dimuonMuonFake_Syst")
                                for iTer in range(1, syst_val["Iter"] + 1):
                                    iTerHist = self.DecomposeSystIter(tmpHist, iTer)
                                    iTerHist.Write(f"h_dimuonMassFake_{iTer}")

                                tDirResponseSyst = self.GetDir(tDirResponse, syst_name)
                                tDirResponseSyst.cd()

                                tmpHist = self.file_input.Get(f"{era}/NNLO_MUMU/OS/syst/{syst_name}/h_ResponseMatrix")
                                tmpHist.Write("h_ResponseMatrix_Syst")
                                for iTer in range(1, syst_val["Iter"] + 1):
                                    iTerHist = self.DecomposeSystIter(tmpHist, iTer)
                                    iTerHist.Write(f"h_ResponseMatrix_{iTer}")


        self.file_input.Close()
        self.file_output.Close()
