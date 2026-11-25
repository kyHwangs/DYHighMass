#!/usr/bin/env python3

import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import uuid as uuid

import SingleHistEngine as SHE


def GetYProjection(hist):
    
    returnHist = ROOT.TH1F(f"hYProjection_{uuid.uuid4().hex}", "", 42, 0.5, 42.5)
    for j in range(1, hist.GetNbinsY() + 1):
        
        yContent = 0.0
        for i in range(0, hist.GetNbinsX() + 1):
            yContent += hist.GetBinContent(i, j)

        returnHist.SetBinContent(j, yContent)
    
    return returnHist


def DoUnfolding(hist_data, hist_response, hist_fake):


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

    unfold.SubtractBackground(hist_fake, "Fake", 1.0, 0.0)

    unfold.DoUnfold(0.0)

    hUnfolded = unfold.GetOutput("hUnfolded")
    hFoldedBack = unfold.GetFoldedOutput("hFoldedBack")
    # hEmatTot = unfold.GetEmatrixTotal("hEmatTot")
    # hRhoIJ = unfold.GetRhoIJtotal("hRhoIJ")

    hUnfolded.SetName(f"hUnfolded_{uuid.uuid4().hex}")
    hFoldedBack.SetName(f"hFoldedBack_{uuid.uuid4().hex}")
    # hEmatTot.SetName("EmatTot_Run2")
    # hRhoIJ.SetName("RhoIJ_Run2")

    return hUnfolded, hFoldedBack



file_input = ROOT.TFile("MUMU_all_v2_Merged_v2.root", "READ")

# aHist_TotalMC = file_input.Get(f"DY_tautau/h_dimuonMass")
# aHist_TotalMC.Add(file_input.Get(f"TT/h_dimuonMass"))
# aHist_TotalMC.Add(file_input.Get(f"WW/h_dimuonMass"))
# aHist_TotalMC.Add(file_input.Get(f"WZ/h_dimuonMass"))
# aHist_TotalMC.Add(file_input.Get(f"ZZ/h_dimuonMass"))
# aHist_TotalMC.Add(file_input.Get(f"GG/h_dimuonMass"))
# aHist_TotalMC.Add(file_input.Get("Fake/h_dimuonMass_Fake"))

aHist_Data = file_input.Get(f"DY/h_dimuonMass")
aHist_Gen = file_input.Get(f"DY_Gen/h_dimuonMassGen")

aHist_Response = file_input.Get(f"ResponseMatrix/h_ResponseMatrix")

aHist_Fake = file_input.Get(f"DY_Fake/h_dimuonMassFake")

# def DoUnfolding(self, hist_data, hist_mc, hist_response, hist_fake):
aHist_Unfolded, aHist_FoldedBack = DoUnfolding(aHist_Data, aHist_Response, aHist_Fake)

file_output = ROOT.TFile("valudation.root", "RECREATE")
file_output.cd()

aHist_Data.Write("DY_Reco")
aHist_Gen.Write("DY_Gen")
aHist_Unfolded.Write("DY_Unfolded")
aHist_FoldedBack.Write("DY_FoldedBack")

aHist_Data_noFake = aHist_Data.Clone("DY_Reco_noFake")
aHist_Data_noFake.Add(aHist_Fake, -1)



vBasic = {
    "xLog": False,
    "yLog": True,
    "lumi": 137.88,
    "xMin": 0.5,
    "xMax": 42.5,
    "AutoYRange": True,
    "AutoRatioYRange": True,
    "xTitle": "M(#mu#mu) [GeV]",
    "yTitle": "Events",
    "rTitle": "Ratio",
    "outputPath": "./temp.pdf",
}

vUnfolding = vBasic.copy()
vUnfolding["latex"] = [
    "Run2, DY #rightarrow #mu#mu",
    "Unfolding validataion",
    "Unfolded DY Reco vs. DY Gen"
]
vUnfolding["outputPath"] = "validation_unfolding.pdf"
vUnfolding["hist"] = {
    "DY_Reco (unfolded)": aHist_Unfolded,
    "DY_Gen": aHist_Gen,
}

SHE.PlottingDiCanvas(vUnfolding)

vFoldedBack = vBasic.copy()
vFoldedBack["latex"] = [
    "Run2, DY #rightarrow #mu#mu",
    "Unfolding validataion",
    "DY Reco vs. DY Folded Back"
]
vFoldedBack["outputPath"] = "validation_foldedback.pdf"
vFoldedBack["hist"] = {
    "DY_Reco (no Fake)": aHist_Data_noFake,
    "DY_FoldedBack": aHist_FoldedBack,
}

SHE.PlottingDiCanvas(vFoldedBack)

aHist_Response_YProjection = GetYProjection(aHist_Response)

vGenTest = vBasic.copy()
vGenTest["latex"] = [
    "Run2, DY #rightarrow #mu#mu",
    "Unfolding validataion",
    "DY Gen vs. Y Projection"
]
vGenTest["outputPath"] = "validation_response_yprojection.pdf"
vGenTest["hist"] = {
    "DY_Gen": aHist_Gen,
    "Y Projection": aHist_Response_YProjection,
}

SHE.PlottingDiCanvas(vGenTest)

