#!/usr/bin/env python3

import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import plotterEngine_MUMU as plotterEngine


def main():

    # eras = ["merged"]
    eras = ["2018", "2017", "2016_postVFP", "2016_preVFP", "merged"]

    # cases = ["", "_0J", "_1J", "_mtJ", "_0BJ", "_1BJ", "_mt1BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    # cases = ["", "_0BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    cases = ["_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    # cases = ["_0BJ"]
    # cases = [""]

    # massBins = ["", "_m200_220", "_m220_243", "_m243_273", "_m273_320", "_m320_380", "_m380_440", "_m440_510", "_m510_600", "_m600_700", "_m700_830", "_m830_1000", "_m1000_1500", "_m1500_4000"]
    # massBins = ["", "_m830_1000", "_m1000_1500", "_m1500_4000"]
    massBins = [""] 

    yrmax_vec = {
        "": 1 + 0.18,
        "_0J": 1 + 0.18,
        "_1J": 1 + 0.24,
        "_mtJ": 1 + 0.48,
        "_0BJ": 1 + 0.18,
        "_1BJ": 1 + 0.48,
        "_mt1BJ": 1 + 0.48,
        "_bVeto_0J": 1 + 0.18,
        "_bVeto_1J": 1 + 0.24,
        "_bVeto_mt1J": 1 + 0.48 
    }

    yrmin_vec = {
        "": 1 - 0.18,
        "_0J": 1 - 0.18,
        "_1J": 1 - 0.24,
        "_mt1J": 1 - 0.48,
        "_0BJ": 1 - 0.18,
        "_1BJ": 1 - 0.48,
        "_mt1BJ": 1 - 0.48,
        "_bVeto_0J": 1 - 0.18,
        "_bVeto_1J": 1 - 0.24,
        "_bVeto_mt1J": 1 - 0.48,
    }
 
    for era in eras:

        type_list = ["OS", "OS_inverted", "SS_inverted", "SS"]
        # type_list = ["SS"]
        # type_list = ["OS"]

        # input_file = "MUMU_BothInverted"
        # input_file = "MUMU_OneInverted"
        input_file = "./root/260922_mumu_bugfix_merged.root"

        for type in type_list:
            plotter = plotterEngine.Plotter(era, 
                                            rootPath = f"{input_file}", 
                                            outputPath = f"./plots_260925_v1/MUMU/{type}/plots_{era}/",
                                            channel = "MUMU", 
                                            region = f"{type}")

            # plotter.SetBackground(rootPath = "./Bck/EMU_FAKE.root", mcList = ["TOP"])
            # plotter.SetFakes(rootPath = "./Bck/MUMU_FAKE.root")

            # plotter.Plot("h_nJet",  "", "", xTitle = "N_{jet}", xmin = 0, xmax = 14)
            # plotter.Plot("h_nBJet", "", "", xTitle = "N_{b-jet}", xmin = 0, xmax = 14)

            for case in cases:
                if type == "OS": plotter.Plot("dimuonMass", case, "", xTitle = "M(#mu#mu) [GeV]", xmin = 200, xmax = 4000, yrmin = yrmin_vec[case], yrmax = yrmax_vec[case], logy = True, logx = True)
                else: plotter.Plot("dimuonMass", case, "", xTitle = "M(#mu#mu) [GeV]", xmin = 200, xmax = 4000, logy = True, logx = True)

                # plotter.Plot("nJet",  case, "", xTitle = "N_{jet}", xmin = 0, xmax = 14)
                # plotter.Plot("nBJet", case, "", xTitle = "N_{b-jet}", xmin = 0, xmax = 14)

                # if type == "OS" and not plotter.HasFakes and not plotter.HasTopBkg:
                #     for massbin in massBins:

                #         plotter.Plot("JetPt", case, massbin               , xTitle = "pT(jet) [GeV]"          ,xmin = 0, xmax = 500, logy = True)
                #         plotter.Plot("JetEta", case, massbin              , xTitle = "#eta(jet)"              ,xmin = -2.5, xmax = 2.5, logy = True)
                #         plotter.Plot("JetPhi", case, massbin              , xTitle = "#phi(jet)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

                #         plotter.Plot("BJetPt", case, massbin              , xTitle = "pT(b-jet) [GeV]"          ,xmin = 0, xmax = 500, logy = True)
                #         plotter.Plot("BJetEta", case, massbin             , xTitle = "#eta(b-jet)"              ,xmin = -2.5, xmax = 2.5, logy = True)
                #         plotter.Plot("BJetPhi", case, massbin             , xTitle = "#phi(b-jet)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

                #         plotter.Plot("LeadingMuonPt", case, massbin       , xTitle = "pT(#mu) [GeV]"          ,xmin = 15, xmax = 1520, logy = True, logx = True)
                #         plotter.Plot("LeadingMuonEta", case, massbin      , xTitle = "#eta(#mu)"              ,xmin = -2.5, xmax = 2.5, logy = True)
                #         plotter.Plot("LeadingMuonPhi", case, massbin      , xTitle = "#phi(#mu)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

                #         plotter.Plot("SubleadingMuonPt", case, massbin    , xTitle = "pT(#mu) [GeV]"          ,xmin = 15, xmax = 1520, logy = True, logx = True)
                #         plotter.Plot("SubleadingMuonEta", case, massbin   , xTitle = "#eta(#mu)"              ,xmin = -2.5, xmax = 2.5, logy = True)
                #         plotter.Plot("SubleadingMuonPhi", case, massbin   , xTitle = "#phi(#mu)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

                #         plotter.Plot("MuonPt", case, massbin              , xTitle = "pT(#mu) [GeV]"          ,xmin = 15, xmax = 1520, logy = True, logx = True)
                #         plotter.Plot("MuonEta", case, massbin             , xTitle = "#eta(#mu)"              ,xmin = -2.5, xmax = 2.5, logy = True)
                #         plotter.Plot("MuonPhi", case, massbin             , xTitle = "#phi(#mu)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

                #         plotter.Plot("dimuonPt", case, massbin            , xTitle = "pT(#mu#mu) [GeV]" ,xmin = 0, xmax = 500, logy = True)
                #         plotter.Plot("dimuonRap", case, massbin           , xTitle = "rapidity(#mu#mu)"      ,xmin = -2.8, xmax = 2.8, logy = True)


if __name__ == "__main__" :
    ROOT.TH1.AddDirectory(False)
    ROOT.TH1.SetDefaultSumw2()
    
    main()
