#!/usr/bin/env python3

import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import plotterEngine_MUMU as plotterEngine


def main():

    # eras = ["merged", "2018", "2017", "2016_postVFP", "2016_preVFP"]
    eras = ["merged"]

    # cases = ["", "_0J", "_1J", "_mtJ", "_0BJ", "_1BJ", "_mt1BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    # cases = ["", "_0BJ", "_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    cases = ["_bVeto_0J", "_bVeto_1J", "_bVeto_mt1J"]
    # cases = ["_0BJ"]
    # cases = [""]

    # massBins = ["", "_m200_220", "_m220_243", "_m243_273", "_m273_320", "_m320_380", "_m380_440", "_m440_510", "_m510_600", "_m600_700", "_m700_830", "_m830_1000", "_m1000_1500", "_m1500_4000"]
    massBins = [""] 

    yrmax_vec = {
        "": 1 + 0.18,
        "_0J": 1 + 0.18,
        "_1J": 1 + 0.24,
        "_mtJ": 1 + 0.48,
        "_0BJ": 1 + 0.18,
        "_1BJ": 1 + 0.48,
        "_mt1BJ": 1 + 0.48,
        "_bVeto_0J": 1 + 0.48,
        "_bVeto_1J": 1 + 0.48,
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
        "_bVeto_0J": 1 - 0.48,
        "_bVeto_1J": 1 - 0.48,
        "_bVeto_mt1J": 1 - 0.48,
    }
 
    for era in eras:

        # plotter = plotterEngine.Plotter(era, 
        #                                 rootPath = <path_to_root_file>,
        #                                 outputPath = <path_to_output_pdf>,
        #                                 channel = <channel>, "EMU" or "MUMU"
        #                                 region = <region>) "OS", "SS", "OS_inverted", "SS_inverted"

        # plotter = plotterEngine.Plotter(era, 
        #                                 rootPath = f"./Bck_260512/ROOT/EMU_OS.root", 
        #                                 outputPath = f"./plots_260514/EMU_OSwithFake/plots" + era + "/",
        #                                 channel = "EMU", 
        #                                 region = "OS")

        type_list = ["OS", "SS", "OS_inverted", "SS_inverted"]
        # type_list = ["OS"]

        for type in type_list:
            plotter = plotterEngine.Plotter(era, 
                                            rootPath = f"./root/260918_EMU_base_merged.root", 
                                            outputPath = f"./plots_260923/emu_withFake/EMU_{type}/plots_" + era + "/",
                                            channel = "EMU", 
                                            region = f"{type}")

            plotter.SetFakes(rootPath = "./Bck/EMU_FAKE.root")

            for case in cases:
                # plotter.Plot("h_PairMass", case, "", xTitle = "M(e#mu) [GeV]", xmin = 200, xmax = 4000, yrmin = yrmin_vec[case], yrmax = yrmax_vec[case], logy = True, logx = True)
                plotter.Plot("PairMass", case, "", xTitle = "M(e#mu) [GeV]", xmin = 200, xmax = 4000, logy = True, logx = True)
                
                if not plotter.HasFakes:
                    for massbin in massBins:

                        plotter.Plot("JetPt", case, massbin                 , xTitle = "pT(jet) [GeV]"          ,xmin = 0, xmax = 500, logy = True)
                        plotter.Plot("JetEta", case, massbin                , xTitle = "#eta(jet)"              ,xmin = -2.5, xmax = 2.5, logy = True)
                        plotter.Plot("JetPhi", case, massbin                , xTitle = "#phi(jet)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

                        plotter.Plot("BJetPt", case, massbin                , xTitle = "pT(b-jet) [GeV]"          ,xmin = 0, xmax = 500, logy = True)
                        plotter.Plot("BJetEta", case, massbin               , xTitle = "#eta(b-jet)"              ,xmin = -2.5, xmax = 2.5, logy = True)
                        plotter.Plot("BJetPhi", case, massbin               , xTitle = "#phi(b-jet)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

                        plotter.Plot("MuonPt", case, massbin                , xTitle = "pT(#mu) [GeV]"          ,xmin = 15, xmax = 1520, logy = True, logx = True)
                        plotter.Plot("MuonEta", case, massbin               , xTitle = "#eta(#mu)"              ,xmin = -2.5, xmax = 2.5, logy = True)
                        plotter.Plot("MuonPhi", case, massbin               , xTitle = "#phi(#mu)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

                        plotter.Plot("ElecPt", case, massbin                , xTitle = "pT(e) [GeV]"          ,xmin = 15, xmax = 1520, logy = True, logx = True)
                        plotter.Plot("ElecEta", case, massbin               , xTitle = "#eta(e)"              ,xmin = -2.5, xmax = 2.5, logy = True)
                        plotter.Plot("ElecPhi", case, massbin               , xTitle = "#phi(e)"              ,xmin = -3.141593, xmax = 3.141593, logy = True)

                        plotter.Plot("PairPt", case, massbin              , xTitle = "pT(e#mu) [GeV]" ,xmin = 0, xmax = 500, logy = True)
                        plotter.Plot("PairRap", case, massbin             , xTitle = "rapidity(e#mu)"      ,xmin = -2.8, xmax = 2.8, logy = True)


if __name__ == "__main__" :
    ROOT.TH1.AddDirectory(False)
    main()
