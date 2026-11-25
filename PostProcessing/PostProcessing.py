#!/usr/bin/env python3

import os, ROOT, sys, pickle, argparse
import uuid
import cmsstyle as CMS
import array
import uuid as uuid

import SingleHistEngine as SHE

# import plotterEngine_MUMU as PE
# import EraMerger as EM
# import Plotter as PL

parser = argparse.ArgumentParser()
parser.add_argument(
    '--input',
    default = "output.root",
    help=' : input root file'
)

args = parser.parse_args()

def main(args):

    # pc = PlotContainter(file_name = args.input)
    # pc.Store()

    pm = PlotMerger(file_name = "MUMU_all.root")
    pm.Store()

    # fe = FakeEstimation(file_name = "MUMU_all_Merged.root")
    # fe.Store()

    # uf = Unfolding(file_name = "MUMU_all_Merged_FakeEstimation.root")
    # uf.Store()

    # syst = GetSystematic(file_name = "MUMU_all_Merged_unfolded.root")
    # syst.Store()

    # plotter = Plotter(file_name = "MUMU_all_Merged_systematic.root")
    # plotter.Store()



if __name__ == "__main__" :
    ROOT.TH1.AddDirectory(False)
    ROOT.TH1.SetDefaultSumw2()
    ROOT.TDirectory.AddDirectory(0);

    main(args)
