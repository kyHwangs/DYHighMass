import os, ROOT, sys, pickle, argparse
import uuid
import numpy as np
import cmsstyle as CMS
import array

ROOT.gROOT.SetBatch(True)

def SetMoreLogXLabels(pad):
    for obj in pad.GetListOfPrimitives():
        if obj.InheritsFrom("TH1"):
            obj.GetXaxis().SetMoreLogLabels(True)
            obj.GetXaxis().SetNoExponent(True)
            break

    pad.Modified()
    pad.Update()

def SetError0(hist):
    for bin in range(1, hist.GetNbinsX() + 1):
        hist.SetBinError(bin, 1e-6)

    return hist

def MakingRatio(hDict, hRef):
    hTargets = {}

    for hTargetName, hTargetObject in list(hDict.items())[1:]:
        hRatio = hTargetObject.Clone(
            f"{hTargetObject.GetName()}_ratioToRef"
        )
        hRatio.Divide(hRef)

        hRatio = SetError0(hRatio)

        hTargets[hTargetName] = {
            "object": hTargetObject,
            "ratioToRef": hRatio,
        }

    return hTargets

def MakingUpperPadYRange(hDict, xMin, xMax, logy = False):

    yMin = None
    yMax = None

    for name, hist in hDict.items():
        for bin in range(1, hist.GetNbinsX() + 1):
            
            if hist.GetBinCenter(bin) < xMin or hist.GetBinCenter(bin) > xMax:
                continue
            
            binContent = hist.GetBinContent(bin)
            if yMin is None or binContent < yMin:
                yMin = binContent
            if yMax is None or binContent > yMax:
                yMax = binContent

    if yMin is None or yMax is None:
        raise ValueError(f"No histogram bins found in x range [{xMin}, {xMax}]")

    if logy:
        if yMin <= 0 : yMin = 1e-3
        else: yMin = yMin * 0.1
        yMax = yMax * 10
    else:
        if yMin <= 0 : yMin = 0
        else: yMin = yMin * 0.9
        yMax = yMax * 1.3

    return yMin, yMax

def MakingRatioPadYRange(hDict, xMin, xMax):

    rRasidual = 0

    for name, target in hDict.items():
        hTargetObject = target["object"]
        hRatioObject = target["ratioToRef"]

        for bin in range(1, hTargetObject.GetNbinsX() + 1):
            if hTargetObject.GetBinCenter(bin) < xMin or hTargetObject.GetBinCenter(bin) > xMax:
                continue
            
            if abs(1 - hRatioObject.GetBinContent(bin)) > rRasidual:
                rRasidual = abs(1 - hRatioObject.GetBinContent(bin))
    
    if rRasidual <= 0 or rRasidual > 0.5:
        rRasidual = 0.5

    else: 
        rRasidual = rRasidual * 1.1

    return 1 - rRasidual, 1 + rRasidual

def PlottingSingleCanvas(var):

    CMS.SetExtraText("Private Work")
    CMS.SetEnergy("13")
    CMS.SetLumi(var["lumi"])

    hLatex = var["latex"]
    hHist = var["hist"]

    if not hHist:
        raise ValueError("var['hist'] must contain at least one histogram")

    hRefName, hRefObject = next(iter(hHist.items()))

    xLog = var.get("xLog", False)
    yLog = var.get("yLog", False)
    
    xMin = var.get("xMin", 200)
    xMax = var.get("xMax", 4000)
    
    yMin = 1e-3 if yLog and var.get("yMin", 0) == 0 else var.get("yMin", 0)
    yMax = var.get("yMax", hRefObject.GetBinContent(2) * 1.3)

    if var.get("AutoYRange", False):
        yMin, yMax = MakingUpperPadYRange(hHist, xMin, xMax, yLog)

    xTitle = var.get("xTitle", "M(e#mu) [GeV]")
    yTitle = var.get("yTitle", "Events")

    fOutputPath = var.get("outputPath", "./temp.pdf")

    fCanvs = CMS.cmsCanvas(
        f"fCanv_{hRefObject.GetName()}_{uuid.uuid4()}",
        xMin,
        xMax,
        yMin,
        yMax,
        xTitle,
        yTitle,
        square = True,
        extraSpace = 0.05,
        iPos = 0,
        yTitOffset = 1.25
    )

    if xLog: fCanvs.cd().SetLogx(True)
    if yLog: fCanvs.cd().SetLogy(True)

    fCanvs.cd()

    fLegend = CMS.cmsLeg(0.80, 0.6, 0.89, 0.7, textSize=0.03)    
    for idx, (hHistName, hHistObject) in enumerate(hHist.items()):
        if idx == 0: CMS.cmsDraw(hHistObject, "PE", mcolor=ROOT.kBlack)
        else: CMS.cmsDraw(hHistObject, "PE", mcolor=ROOT.TColor.GetColor(CMS.petroff_6[idx - 1]))
        fLegend.AddEntry(hHistObject, hHistName, "lp")
    
    fCanvs.cd().RedrawAxis()

    rLatex = ROOT.TLatex()
    rLatex.SetTextAlign(14);
    rLatex.SetTextSize(0.04);
    rLatex.SetTextFont(42);
    for idx, hook in enumerate(hLatex):
        rLatex.DrawLatexNDC(0.20, 0.89 - idx * 0.065, hook.encode('utf-8'))

    CMS.SaveCanvas(fCanvs, fOutputPath)




def PlottingDiCanvas(var):

    CMS.SetExtraText("Private Work")
    CMS.SetEnergy("13")
    CMS.SetLumi(var["lumi"])

    hLatex = var["latex"]
    hHist = var["hist"]

    if len(hHist) < 2:
        raise ValueError("var['hist'] must contain a reference and at least one target histogram")

    hRefName, hRefObject = next(iter(hHist.items()))
    hTargets = MakingRatio(hHist, hRefObject)

    xLog = var.get("xLog", False)
    yLog = var.get("yLog", False)
    
    xMin = var.get("xMin", 200)
    xMax = var.get("xMax", 4000)
    
    yMin = 1e-3 if yLog and var.get("yMin", 0) == 0 else var.get("yMin", 0)
    yMax = var.get("yMax", hRefObject.GetBinContent(2) * 1.3)

    if var.get("AutoYRange", False):
        yMin, yMax = MakingUpperPadYRange(hHist, xMin, xMax, yLog)

    rMin = var.get("rMin", 0.5)
    rMax = var.get("rMax", 1.5)

    if var.get("AutoRatioYRange", False):
        rMin, rMax = MakingRatioPadYRange(hTargets, xMin, xMax)
    
    xTitle = var.get("xTitle", "M(#mu#mu) [GeV]")
    yTitle = var.get("yTitle", "Events")
    rTitle = var.get("rTitle", "Ratio")

    fOutputPath = var.get("outputPath", "./temp.pdf")

    fCanvs = CMS.cmsDiCanvas(
        f"fCanv_{hRefObject.GetName()}_{uuid.uuid4()}",
        xMin,
        xMax,
        yMin,
        yMax,
        rMin,
        rMax,
        xTitle,
        yTitle,
        rTitle,
        square = CMS.kSquare,
        extraSpace = 0.1,
        iPos = 0,
    )

    if xLog: 
        fCanvs.cd(1).SetLogx(True)
        fCanvs.cd(2).SetLogx(True)
    
    if yLog: 
        fCanvs.cd(1).SetLogy(True)

    fCanvs.cd(1)

    fLegend = CMS.cmsLeg(0.70, 0.65, 0.89, 0.89, textSize=0.045, textFont=42)    
    
    
    CMS.cmsDraw(hRefObject, "PE", mcolor=ROOT.kBlack)
    fLegend.AddEntry(hRefObject, hRefName, "lp")

    for idx, (hTargetsName, target) in enumerate(hTargets.items()):
        hTargetObject = target["object"]
        CMS.cmsDraw(hTargetObject, "PE", mcolor=ROOT.TColor.GetColor(CMS.petroff_6[idx]))
        fLegend.AddEntry(hTargetObject, hTargetsName, "lp")
    
    if xLog:
        SetMoreLogXLabels(fCanvs.cd(1))
    fCanvs.cd(1).RedrawAxis()

    rLatexFirst = ROOT.TLatex()
    rLatexFirst.SetTextAlign(14);
    rLatexFirst.SetTextSize(0.08);
    rLatexFirst.SetTextFont(42);
    rLatexFirst.DrawLatexNDC(0.25, 0.28, hLatex[0])

    rLatexOthers = ROOT.TLatex()
    rLatexOthers.SetTextAlign(14);
    rLatexOthers.SetTextSize(0.05);
    rLatexOthers.SetTextFont(42);
    for idx, hook in enumerate(hLatex[1:]):
        rLatexOthers.DrawLatexNDC(0.25, 0.25 - (idx + 1) * 0.065, hook.encode('utf-8'))
    

    fCanvs.cd(2)
    
    for idx, (hTargetsName, target) in enumerate(hTargets.items()):
        hRatioObject = target["ratioToRef"]
        CMS.cmsDraw(hRatioObject, "PE", mcolor=ROOT.TColor.GetColor(CMS.petroff_6[idx]))
    
    ref_line = ROOT.TLine(xMin, 1, xMax, 1)
    CMS.cmsDrawLine(ref_line, lcolor=ROOT.kRed, lstyle=ROOT.kDotted)
    
    if xLog:
        SetMoreLogXLabels(fCanvs.cd(2))
    fCanvs.cd(2).RedrawAxis()

    CMS.SaveCanvas(fCanvs, fOutputPath)



def GetSingleCanvas(var):

    CMS.SetExtraText("Private Work")
    CMS.SetEnergy("13")
    CMS.SetLumi(var["lumi"])

    hLatex = var["latex"]
    hHist = var["hist"]

    if not hHist:
        raise ValueError("var['hist'] must contain at least one histogram")

    hRefName, hRefObject = next(iter(hHist.items()))

    xLog = var.get("xLog", False)
    yLog = var.get("yLog", False)
    
    xMin = var.get("xMin", 200)
    xMax = var.get("xMax", 4000)
    
    yMin = 1e-3 if yLog and var.get("yMin", 0) == 0 else var.get("yMin", 0)
    yMax = var.get("yMax", hRefObject.GetBinContent(2) * 1.3)

    if var.get("AutoYRange", False):
        yMin, yMax = MakingUpperPadYRange(hHist, xMin, xMax, yLog)

    xTitle = var.get("xTitle", "M(e#mu) [GeV]")
    yTitle = var.get("yTitle", "Events")

    fCanvs = CMS.cmsCanvas(
        f"fCanv_{hRefObject.GetName()}_{uuid.uuid4()}",
        xMin,
        xMax,
        yMin,
        yMax,
        xTitle,
        yTitle,
        square = True,
        extraSpace = 0.05,
        iPos = 0,
        yTitOffset = 1.25
    )

    if xLog: fCanvs.cd().SetLogx(True)
    if yLog: fCanvs.cd().SetLogy(True)

    fCanvs.cd()

    fLegend = CMS.cmsLeg(0.80, 0.6, 0.89, 0.7, textSize=0.03)    
    for idx, (hHistName, hHistObject) in enumerate(hHist.items()):

        if len(hHist) > 7:
            if idx == 0: 
                CMS.cmsDraw(hHistObject, "PE", mcolor=ROOT.kBlack)
                fLegend.AddEntry(hHistObject, hHistName, "lp")
            else:
                CMS.cmsDraw(hHistObject, "PE", mcolor=ROOT.kGray)

        else: 
            if idx == 0: CMS.cmsDraw(hHistObject, "PE", mcolor=ROOT.kBlack)
            else: CMS.cmsDraw(hHistObject, "PE", mcolor=ROOT.TColor.GetColor(CMS.petroff_6[idx - 1]))
            fLegend.AddEntry(hHistObject, hHistName, "lp")

    CMS.cmsDraw(hHist["Nominal"], "PE", mcolor=ROOT.kBlack)
    
    fCanvs.cd().RedrawAxis()

    if len(hLatex) > 0:
        rLatex = ROOT.TLatex()
        rLatex.SetTextAlign(14);
        rLatex.SetTextSize(0.04);
        rLatex.SetTextFont(42);
        for idx, hook in enumerate(hLatex):
            rLatex.DrawLatexNDC(0.20, 0.89 - idx * 0.065, hook.encode('utf-8'))

    fCanvs._keep = list(hHist.values())
    return fCanvs


def GetDiCanvas(var):

    CMS.SetExtraText("Private Work")
    CMS.SetEnergy("13")
    CMS.SetLumi(var["lumi"])

    hLatex = var["latex"]
    hHist = var["hist"]

    if len(hHist) < 2:
        raise ValueError("var['hist'] must contain a reference and at least one target histogram")

    hRefName, hRefObject = next(iter(hHist.items()))
    hTargets = MakingRatio(hHist, hRefObject)

    xLog = var.get("xLog", False)
    yLog = var.get("yLog", False)
    
    xMin = var.get("xMin", 200)
    xMax = var.get("xMax", 4000)
    
    yMin = 1e-3 if yLog and var.get("yMin", 0) == 0 else var.get("yMin", 0)
    yMax = var.get("yMax", hRefObject.GetBinContent(2) * 1.3)

    if var.get("AutoYRange", False):
        yMin, yMax = MakingUpperPadYRange(hHist, xMin, xMax, yLog)

    rMin = var.get("rMin", 0.5)
    rMax = var.get("rMax", 1.5)

    if var.get("AutoRatioYRange", False):
        rMin, rMax = MakingRatioPadYRange(hTargets, xMin, xMax)
    
    xTitle = var.get("xTitle", "M(#mu#mu) [GeV]")
    yTitle = var.get("yTitle", "Events")
    rTitle = var.get("rTitle", "Ratio")

    fCanvs = CMS.cmsDiCanvas(
        f"fCanv_{hRefObject.GetName()}_{uuid.uuid4()}",
        xMin,
        xMax,
        yMin,
        yMax,
        rMin,
        rMax,
        xTitle,
        yTitle,
        rTitle,
        square = CMS.kSquare,
        extraSpace = 0.1,
        iPos = 0,
    )

    if xLog: 
        fCanvs.cd(1).SetLogx(True)
        fCanvs.cd(2).SetLogx(True)
    
    if yLog: 
        fCanvs.cd(1).SetLogy(True)

    fCanvs.cd(1)

    fLegend = CMS.cmsLeg(0.70, 0.65, 0.89, 0.89, textSize=0.045, textFont=42)    

    for idx, (hTargetsName, target) in enumerate(hTargets.items()):

        if len(hTargets) < 7:
            hTargetObject = target["object"]
            CMS.cmsDraw(hTargetObject, "PE", mcolor=ROOT.TColor.GetColor(CMS.petroff_6[idx]))
            fLegend.AddEntry(hTargetObject, hTargetsName, "lp")
        else:
            hTargetObject = target["object"]
            CMS.cmsDraw(hTargetObject, "PE", mcolor=ROOT.kGray)

    CMS.cmsDraw(hRefObject, "PE", mcolor=ROOT.kBlack)
    fLegend.AddEntry(hRefObject, hRefName, "lp")
    
    if xLog:
        SetMoreLogXLabels(fCanvs.cd(1))
    fCanvs.cd(1).RedrawAxis()

    if len(hLatex) > 0:
        rLatexFirst = ROOT.TLatex()
        rLatexFirst.SetTextAlign(14);
        rLatexFirst.SetTextSize(0.08);
        rLatexFirst.SetTextFont(42);
        rLatexFirst.DrawLatexNDC(0.25, 0.28, hLatex[0])

    if len(hLatex) > 1:
        rLatexOthers = ROOT.TLatex()
        rLatexOthers.SetTextAlign(14);
        rLatexOthers.SetTextSize(0.05);
        rLatexOthers.SetTextFont(42);
        for idx, hook in enumerate(hLatex[1:]):
            rLatexOthers.DrawLatexNDC(0.25, 0.25 - (idx + 1) * 0.065, hook.encode('utf-8'))
        
    fCanvs.cd(2)
    
    for idx, (hTargetsName, target) in enumerate(hTargets.items()):
        hRatioObject = target["ratioToRef"]
        if len(hTargets) < 7:
            CMS.cmsDraw(hRatioObject, "PE", mcolor=ROOT.TColor.GetColor(CMS.petroff_6[idx]))
        else:
            CMS.cmsDraw(hRatioObject, "PE", mcolor=ROOT.kGray)

    ref_line = ROOT.TLine(xMin, 1, xMax, 1)
    CMS.cmsDrawLine(ref_line, lcolor=ROOT.kRed, lstyle=ROOT.kDotted)
    
    if xLog:
        SetMoreLogXLabels(fCanvs.cd(2))
    
    fCanvs.cd(2).RedrawAxis()

    fCanvs._keep = list(hTargets.values())
    fCanvs._keep.append(hRefObject)
    fCanvs._keep.append(ref_line)
    return fCanvs



def GetSingleCanvasUnordered(var):

    CMS.SetExtraText("Private Work")
    CMS.SetEnergy("13")
    CMS.SetLumi(var["lumi"])

    hLatex = var["latex"]
    hHist = var["hist"]

    if not hHist:
        raise ValueError("var['hist'] must contain at least one histogram")

    xLog = var.get("xLog", False)
    yLog = var.get("yLog", False)
    
    xMin = var.get("xMin", 200)
    xMax = var.get("xMax", 4000)
    
    yMin = 1e-3 if yLog and var.get("yMin", 0) == 0 else var.get("yMin", 0)
    yMax = var.get("yMax", hHist["Total"].GetBinContent(2) * 1.3)

    if var.get("AutoYRange", False):
        yMin, yMax = MakingUpperPadYRange(hHist, xMin, xMax, yLog)

    xTitle = var.get("xTitle", "M(e#mu) [GeV]")
    yTitle = var.get("yTitle", "Events")

    fCanvs = CMS.cmsCanvas(
        f"fCanv_{hHist['Total'].GetName()}_{uuid.uuid4()}",
        xMin,
        xMax,
        yMin,
        yMax,
        xTitle,
        yTitle,
        square = True,
        extraSpace = 0.05,
        iPos = 11,
        yTitOffset = 1.25
    )

    if xLog: fCanvs.cd().SetLogx(True)
    if yLog: fCanvs.cd().SetLogy(True)

    fCanvs.cd()

    fLegend = CMS.cmsLeg(0.2, 0.66 - 0.035 * len(hHist), 0.5, 0.66, textSize=0.02)    
    for idx, (hHistName, hHistObject) in enumerate(hHist.items()):
        if hHistName == "Total": 
            CMS.cmsDraw(hHistObject, "lp", mcolor=ROOT.kBlack, fstyle=0)
            fLegend.AddEntry(hHistObject, hHistName, "lp")
        else:
            if (len(hHist) > 6): CMS.cmsDraw(hHistObject, "lp", mcolor=ROOT.TColor.GetColor(CMS.petroff_10[idx - 1]), fstyle=0)
            else: CMS.cmsDraw(hHistObject, "lp", mcolor=ROOT.TColor.GetColor(CMS.petroff_6[idx - 1]), fstyle=0)
            fLegend.AddEntry(hHistObject, hHistName, "lp")
    
    fCanvs.cd().RedrawAxis()

    rLatex = ROOT.TLatex()
    rLatex.SetTextAlign(14);
    rLatex.SetTextSize(0.04);
    rLatex.SetTextFont(42);
    for idx, hook in enumerate(hLatex):
        rLatex.DrawLatexNDC(0.22, 0.70 - idx * 0.065, hook.encode('utf-8'))

    fCanvs._keep = list(hHist.values())
    fCanvs._keep.append(fLegend)
    return fCanvs
