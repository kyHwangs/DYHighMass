import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import uuid as uuid
import Config

ROOT.TH1.AddDirectory(False)
ROOT.TH1.SetDefaultSumw2()
ROOT.TDirectory.AddDirectory(0);

class Unfolding():

    def __init__(self, file_name):


        self.file_name = file_name
        self.file_input = ROOT.TFile(self.file_name, "READ")
        self.file_output = ROOT.TFile(self.file_name.replace("_FakeEstimation.root", "_unfolded.root"), "RECREATE")

        self.fake_hist = self.file_input.Get("Fake/h_dimuonMass_Fake")

    def GetDir(self, fDir, fName):
        fDirTemp =  fDir.GetDirectory(fName)

        if not fDirTemp:
            fDirTemp = fDir.mkdir(fName)

        if not fDirTemp:
            raise RuntimeError("Cannot create output directory")

        return fDirTemp

    def DoUnfolding(self, hist_data, hist_mc, hist_response, hist_fake):

        ############################################################
        # ERegMode
        # kRegModeNone : regularization 없음
        # kRegModeSize : 결과값 크기 자체를 억제 (0차 미분)
        # kRegModeDerivative : 인접 bin 간 1차 미분 억제
        # kRegModeCurvature : 인접 bin 간 2차 미분(곡률) 억제
        # kRegModeMixed : 사용자가 직접 regularization 조건을 추가할 때 사용
        # -----------------------------------------------------------
        # EConstraint
        # kEConstraintNone : 전체 이벤트 수 보존 안 함
        # kEConstraintArea : 전체 적분이 입력 데이터와 일치하도록 강제
        # -----------------------------------------------------------
        # EDensityMode
        # kDensityModeNone : bin 폭 보정 없음
        # kDensityModeBinWidth : bin 폭으로 나눈 밀도 기준으로 regularization
        # kDensityModeUser : 사용자 정의 밀도 factor 사용
        # kDensityModeBinWidthAndUser : bin 폭 + 사용자 정의 factor 동시 적용
        ############################################################

        regMode = ROOT.TUnfold.kRegModeNone
        constraint = ROOT.TUnfold.kEConstraintNone
        densityMode = ROOT.TUnfoldDensity.kDensityModeNone

        unfold = ROOT.TUnfoldDensity(
            hist_response,
            ROOT.TUnfold.kHistMapOutputVert,  # truth on vertical (Y) axis
            regMode,
            constraint,
            densityMode,
        )

        if unfold.SetInput(hist_data) >= 10000:
            print("WARNING: input data badly constrained by response matrix")

        unfold.SubtractBackground(hist_mc, "Bkg", 1.0, 0.0)
        unfold.SubtractBackground(hist_fake, "Fake", 1.0, 0.0)

        unfold.DoUnfold(0.0)

        hUnfolded = unfold.GetOutput("hUnfolded")
        # hFoldedBack = unfold.GetFoldedOutput("hFoldedBack")
        # hEmatTot = unfold.GetEmatrixTotal("hEmatTot")
        # hRhoIJ = unfold.GetRhoIJtotal("hRhoIJ")

        hUnfolded.SetName(f"hUnfolded_{uuid.uuid4().hex}")
        # hFoldedBack.SetName("FoldedBack_Run2")
        # hEmatTot.SetName("EmatTot_Run2")
        # hRhoIJ.SetName("RhoIJ_Run2")

        return hUnfolded


    def CheckTotalRunNumber(self):
        total_run_number = 0
        for syst_name, syst_val in syst_input.items():
            if syst_val["isCor"]:
                total_run_number += syst_val["Iter"]
            else:
                total_run_number += 4 * syst_val["Iter"]
        
        
        return total_run_number

    def Store(self):

        current_run = 0
        total_run_number = self.CheckTotalRunNumber()

        for syst_name, syst_val in syst_input.items():

            if syst_name == "Nominal":

                tDir = self.GetDir(self.file_output, syst_name)

                current_run += 1
                print(f"[{current_run}/{total_run_number}] {syst_name}")

                aHist_TotalMC = self.file_input.Get(f"DY_tautau/h_dimuonMass")
                aHist_TotalMC.Add(self.file_input.Get(f"TT/h_dimuonMass"))
                aHist_TotalMC.Add(self.file_input.Get(f"WW/h_dimuonMass"))
                aHist_TotalMC.Add(self.file_input.Get(f"WZ/h_dimuonMass"))
                aHist_TotalMC.Add(self.file_input.Get(f"ZZ/h_dimuonMass"))
                aHist_TotalMC.Add(self.file_input.Get(f"GG/h_dimuonMass"))
                aHist_TotalMC.Add(self.fake_hist)

                aHist_Data = self.file_input.Get(f"Data/h_dimuonMass")

                aHist_Response = self.file_input.Get(f"ResponseMatrix/h_ResponseMatrix")

                aHist_Fake = self.file_input.Get(f"DY_Fake/h_dimuonMassFake")

                # def DoUnfolding(self, hist_data, hist_mc, hist_response, hist_fake):
                aHist_Unfolded = self.DoUnfolding(aHist_Data, aHist_TotalMC, aHist_Response, aHist_Fake)

                tDir.cd()
                aHist_Unfolded.Write(f"hUnfolded")

            else: 

                if not syst_val["isCor"]:

                    for iTer in range(1, syst_val["Iter"] + 1):
                        for era in ["2016_preVFP", "2016_postVFP", "2017", "2018"]:

                            tDir = self.GetDir(self.file_output, f"{syst_name}_{era}")
                            
                            current_run += 1
                            print(f"[{current_run}/{total_run_number}] {syst_name} {era} {iTer}")

                            aHist_TotalMC = self.file_input.Get(f"DY_tautau/{syst_name}_{era}/h_dimuonMass_{iTer}")
                            aHist_TotalMC.Add(self.file_input.Get(f"TT/{syst_name}_{era}/h_dimuonMass_{iTer}"))
                            aHist_TotalMC.Add(self.file_input.Get(f"WW/{syst_name}_{era}/h_dimuonMass_{iTer}"))
                            aHist_TotalMC.Add(self.file_input.Get(f"WZ/{syst_name}_{era}/h_dimuonMass_{iTer}"))
                            aHist_TotalMC.Add(self.file_input.Get(f"ZZ/{syst_name}_{era}/h_dimuonMass_{iTer}"))
                            aHist_TotalMC.Add(self.file_input.Get(f"GG/{syst_name}_{era}/h_dimuonMass_{iTer}"))
                            aHist_TotalMC.Add(self.fake_hist)

                            aHist_Data = self.file_input.Get(f"Data/h_dimuonMass")

                            aHist_Response = self.file_input.Get(f"ResponseMatrix/{syst_name}_{era}/h_ResponseMatrix_{iTer}")

                            aHist_Fake = self.file_input.Get(f"DY_Fake/{syst_name}_{era}/h_dimuonMassFake_{iTer}")

                            aHist_Unfolded = self.DoUnfolding(aHist_Data, aHist_TotalMC, aHist_Response, aHist_Fake)

                            tDir.cd()
                            aHist_Unfolded.Write(f"hUnfolded_{iTer}")

                else: 
                    for iTer in range(1, syst_val["Iter"] + 1):

                        tDir = self.GetDir(self.file_output, f"{syst_name}")
                            
                        current_run += 1
                        print(f"[{current_run}/{total_run_number}] {syst_name} {iTer}")

                        aHist_TotalMC = self.file_input.Get(f"DY_tautau/{syst_name}/h_dimuonMass_{iTer}")
                        aHist_TotalMC.Add(self.file_input.Get(f"TT/{syst_name}/h_dimuonMass_{iTer}"))
                        aHist_TotalMC.Add(self.file_input.Get(f"WW/{syst_name}/h_dimuonMass_{iTer}"))
                        aHist_TotalMC.Add(self.file_input.Get(f"WZ/{syst_name}/h_dimuonMass_{iTer}"))
                        aHist_TotalMC.Add(self.file_input.Get(f"ZZ/{syst_name}/h_dimuonMass_{iTer}"))
                        aHist_TotalMC.Add(self.file_input.Get(f"GG/{syst_name}/h_dimuonMass_{iTer}"))
                        aHist_TotalMC.Add(self.fake_hist)

                        aHist_Data = self.file_input.Get(f"Data/h_dimuonMass")

                        aHist_Response = self.file_input.Get(f"ResponseMatrix/{syst_name}/h_ResponseMatrix_{iTer}")

                        aHist_Fake = self.file_input.Get(f"DY_Fake/{syst_name}/h_dimuonMassFake_{iTer}")

                        aHist_Unfolded = self.DoUnfolding(aHist_Data, aHist_TotalMC, aHist_Response, aHist_Fake)

                        tDir.cd()
                        aHist_Unfolded.Write(f"hUnfolded_{iTer}")
