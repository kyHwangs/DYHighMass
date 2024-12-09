#! /bin/sh

SAMPLES=(
NNLO_inc
NNLO_10to50
NNLO_10to50_v2
NNLO_100to200
NNLO_200to400
NNLO_400to500
NNLO_500to700
NNLO_700to800
NNLO_800to1000
NNLO_1000to1500
NNLO_1500to2000
NNLO_2000toInf
NNLO_tautau
ST_s
ST_t_AntiTop
ST_t_Top
ST_tW_AntiTop
ST_tW_Top
TTTo2L2Nu
WJetsToLNu
WW
WZ
ZZ
)

ERAS=(
2016_preVFP
2016_postVFP
2017
2018
)

for era in ${ERAS[@]}
do
  for sample in ${SAMPLES[@]}
  do
    mkdir -p $era/$sample
    mkdir -p log/$era/$sample
  done
done

DATA=(
"2016_preVFP/Run2016B_APV_ver2"
"2016_preVFP/Run2016C_APV"
"2016_preVFP/Run2016D_APV"
"2016_preVFP/Run2016E_APV"
"2016_preVFP/Run2016F_APV"
"2016_postVFP/Run2016F"
"2016_postVFP/Run2016G"
"2016_postVFP/Run2016H"
"2017/Run2017B"
"2017/Run2017C"
"2017/Run2017D"
"2017/Run2017E"
"2017/Run2017F"
"2018/Run2018A"
"2018/Run2018B"
"2018/Run2018C"
"2018/Run2018D"
)

for dataset in ${DATA[@]}
do
  mkdir -p $dataset
  mkdir -p log/$dataset
done
