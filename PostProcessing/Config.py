syst_input = {
    "Nominal": {
        "Iter": 1,
        "isCor": False,
    },
    "PDF_Hessian": {
        "Iter": 100,
        "isCor": True,
        "method": "EnsembleAdd",
        "pretty": "PDF replica"
    },
    "PDF_alphaS": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "#alpha_{S} variation"
    },
    "PDF_scale": {
        "Iter": 6,
        "isCor": True,
        "method": "Max",
        "pretty": "Scale variation"
    },
    "Norm_uncorrelated": {
        "Iter": 2,
        "isCor": False,
        "method": "Max",
        "pretty": "Uncorrelated"
    },
    "Norm_correlated_161718": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "Correlated (16, 17, 18)"
    },
    "Norm_correlated_1718": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "Correlated (17, 18)"
    },
    "MuonEffReco_stat": {
        "Iter": 100,
        "isCor": False,
        "method": "Ensemble",
        "pretty": "Stat. uncertainty"
    },
    "MuonEffReco_tagIso": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "Tag Muon Iso. variation"
    },
    "MuonEffReco_massRange": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "Mass Range variation"
    },
    "MuonEffId_stat": {
        "Iter": 100,
        "isCor": False,
        "method": "Ensemble",
        "pretty": "Stat. variation"
    },
    "MuonEffId_tagIso": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "Tag Muon Iso. variation"
    },
    "MuonEffId_massRange": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "Mass Range variation"
    },
    "MuonEffIso_stat": {
        "Iter": 100,
        "isCor": False,
        "method": "Ensemble",
        "pretty": "Stat. variation"
    },
    "MuonEffIso_tagIso": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "Tag Muon Iso. variation"
    },
    "MuonEffIso_massRange": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "Mass Range variation"
    },
    "MuonEffTrigData_stat": {
        "Iter": 100,
        "isCor": False,
        "method": "Ensemble",
        "pretty": "Data Stat. variation"
    },
    "MuonEffTrigData_tagIso": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "Data Tag Muon Iso. variation"
    },
    "MuonEffTrigData_massRange": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "Data Mass Range variation"
    },
    "MuonEffTrigMC_stat": {
        "Iter": 100,
        "isCor": False,
        "method": "Ensemble",
        "pretty": "MC Stat. variation"
    },
    "MuonEffTrigMC_tagIso": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "MC Tag Muon Iso. variation"
    },
    "MuonEffTrigMC_massRange": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "MC Mass Range variation"
    },
    "PileUpReweighting": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "Pile Up Reweighting"
    },
    "JetPuID": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "Jet PU ID"
    },
    "BTagging_bc_correlated": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "b-tag: b,c corr"
    },
    "BTagging_light_correlated": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "b-tag: light corr"
    },
    "BTagging_bc_uncorrelated": {
        "Iter": 2,
        "isCor": False,
        "method": "Max",
        "pretty": "b-tag: b,c uncorr"
    },
    "BTagging_light_uncorrelated": {
        "Iter": 2,
        "isCor": False,
        "method": "Max",
        "pretty": "b-tag: light uncorr"
    },
    "L1Prefiring": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "L1 Prefiring"
    },
    "MuonSmearing": {
        "Iter": 1,
        "isCor": True,
        "method": "diff",
        "pretty": "Muon p_{T} Smearing"
    },
    "MuonScaling": {
        "Iter": 100,
        "isCor": True,
        "method": "Ensemble",
        "pretty": "Muon p_{T} Scaling"
    },
    "JetEnergySmearing": {
        "Iter": 2,
        "isCor": False,
        "method": "Max",
        "pretty": "Jet Energy Smearing"
    },
    "JetEnergyScale_FlavorQCD": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "JEC: Flavor QCD, corr"
    },
    "JetEnergyScale_RelativeBal": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "JEC: Relative Balance, corr"
    },
    "JetEnergyScale_HF": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "JEC: HF, corr"
    },
    "JetEnergyScale_BBEC1": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "JEC: BBEC1, corr"
    },
    "JetEnergyScale_EC2": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "JEC: EC2, corr"
    },
    "JetEnergyScale_Absolute": {
        "Iter": 2,
        "isCor": True,
        "method": "Max",
        "pretty": "JEC: Absolute, corr"
    },
    "JetEnergyScale_RelativeSample_Uncor": {
        "Iter": 2,
        "isCor": False,
        "method": "Max",
        "pretty": "JEC: Relative Sample, uncorr"
    },
    "JetEnergyScale_HF_Uncor": {
        "Iter": 2,
        "isCor": False,
        "method": "Max",
        "pretty": "JEC: HF, uncorr"
    },
    "JetEnergyScale_BBEC1_Uncor": {
        "Iter": 2,
        "isCor": False,
        "method": "Max",
        "pretty": "JEC: BBEC1, uncorr"
    },
    "JetEnergyScale_EC2_Uncor": {
        "Iter": 2,
        "isCor": False,
        "method": "Max",
        "pretty": "JEC: EC2, uncorr"
    },
    "JetEnergyScale_Absolute_Uncor": {
        "Iter": 2,
        "isCor": False,
        "method": "Max",
        "pretty": "JEC: Absolute, uncorr"
    },
}

syst_group = {
    "PDF":{
        "input": ["PDF_Hessian", "PDF_alphaS", "PDF_scale"],
        "name": "PDF"
    },
    "Luminosity":{
        "input": ["Norm_uncorrelated", "Norm_correlated_161718", "Norm_correlated_1718"],
        "name": "Luminosity"
    },
    "MuonEffReco":{
        "input": ["MuonEffReco_stat", "MuonEffReco_tagIso", "MuonEffReco_massRange"],
        "name": "Muon Efficiency (Reco)"
    },
    "MuonEffId":{
        "input": ["MuonEffId_stat", "MuonEffId_tagIso", "MuonEffId_massRange"],
        "name": "Muon Efficiency (ID)"
    },
    "MuonEffIso":{
        "input": ["MuonEffIso_stat", "MuonEffIso_tagIso", "MuonEffIso_massRange"],
        "name": "Muon Efficiency (Iso)"
    },
    "MuonEffTrig":{
        "input": ["MuonEffTrigData_stat", "MuonEffTrigData_tagIso", "MuonEffTrigData_massRange", "MuonEffTrigMC_stat", "MuonEffTrigMC_tagIso", "MuonEffTrigMC_massRange"],
        "name": "Muon Efficiency (Trig)"
    },
    "PileUp":{
        "input": ["PileUpReweighting"],
        "name": "Pile Up"
    },
    "JetPuID":{
        "input": ["JetPuID"],
        "name": "Jet PileUp ID"
    },
    "BTagging":{
        "input": ["BTagging_bc_correlated", "BTagging_light_correlated", "BTagging_bc_uncorrelated", "BTagging_light_uncorrelated"],
        "name": "b-tagging Efficiency"
    },
    "L1Prefiring":{
        "input": ["L1Prefiring"],
        "name": "L1 Prefiring"
    },
    "MuonSmearing":{
        "input": ["MuonSmearing"],
        "name": "Muon Smearing"
    },
    "MuonScaling":{
        "input": ["MuonScaling"],
        "name": "Muon Scaling"
    },
    "JetEnergySmearing":{
        "input": ["JetEnergySmearing"],
        "name": "Jet Energy Resolution"
    },
    "JetEnergyScale":{
        "input": ["JetEnergyScale_FlavorQCD", "JetEnergyScale_RelativeBal", "JetEnergyScale_HF", "JetEnergyScale_BBEC1", "JetEnergyScale_EC2", "JetEnergyScale_Absolute", "JetEnergyScale_RelativeSample_Uncor", "JetEnergyScale_HF_Uncor", "JetEnergyScale_BBEC1_Uncor", "JetEnergyScale_EC2_Uncor", "JetEnergyScale_Absolute_Uncor"],
        "name": "Jet Energy Scale"
    },
}

eras = ["2016_preVFP", "2016_postVFP", "2017", "2018"]

samples = {
    "Data": ["Data"],
    "DY": ["NNLO_MUMU"],
    "DY_tautau": ["NNLO_tautau"],
    "TT": ["TTTo2L2Nu"],
    "WW": ["WW"],
    "WZ": ["WZ"],
    "ZZ": ["ZZ"],
    "GG": ["GG"]
}

samples_for_merger = {
    "Data": ["Data"],
    "DY": ["NNLO_MUMU"],
    "DY_Gen": ["NNLO_MUMU"],
    "DY_Fake": ["NNLO_MUMU"],
    "ResponseMatrix": ["NNLO_MUMU"],
    "DY_tautau": ["NNLO_tautau"],
    "TT": ["TTTo2L2Nu"],
    "WW": ["WW"],
    "WZ": ["WZ"],
    "ZZ": ["ZZ"],
    "GG": ["GG"] 
}
