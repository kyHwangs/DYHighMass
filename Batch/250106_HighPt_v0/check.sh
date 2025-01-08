#! /bin/sh

SAMPLES=(
NNLO_10to50
NNLO_10to50_v2
NNLO_inc
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

INPUT=(
DYJetsToMuMu_M-10to50_v1
DYJetsToMuMu_M-10to50_ext1-v1
DYJetsToMuMu_M-50
DYJetsToMuMu_M-100to200
DYJetsToMuMu_M-200to400
DYJetsToMuMu_M-400to500
DYJetsToMuMu_M-500to700
DYJetsToMuMu_M-700to800
DYJetsToMuMu_M-800to1000
DYJetsToMuMu_M-1000to1500
DYJetsToMuMu_M-1500to2000
DYJetsToMuMu_M-2000toInf
DYJetsToTauTau_M-50_AtLeastOneEorMuDecay
ST_s-channel_4f_leptonDecays
ST_t-channel_anitop_4f_InclusiveDecays
ST_t-channel_top_4f_InclusiveDecays
ST_tW_antitop_5f_inclusiveDecays
ST_tW_top_5f_inclusiveDecays
TTTo2L2Nu
WJetsToLNu
WWTo2L2Nu
WZ
ZZ
)

for era in ${ERAS[@]}
do
  for i in {0..22}
  do
    ROOTNUM=`ll $era/${SAMPLES[$i]} | grep root | wc -l`
    LISTNUM=`ll ../../input/$era/${INPUT[$i]} | grep list | wc -l`
    if [ $ROOTNUM -ne $LISTNUM ]
    then
      echo $era ${SAMPLES[$i]} : $LISTNUM $ROOTNUM : ${INPUT[$i]} ${SAMPLES[$i]}
    fi
  done
done


DATAINPUT=(
"2016_preVFP/SingleMuon_Run2016B_APV_ver2"
"2016_preVFP/SingleMuon_Run2016C_APV"
"2016_preVFP/SingleMuon_Run2016D_APV"
"2016_preVFP/SingleMuon_Run2016E_APV"
"2016_preVFP/SingleMuon_Run2016F_APV"
"2016_postVFP/SingleMuon_Run2016F"
"2016_postVFP/SingleMuon_Run2016G"
"2016_postVFP/SingleMuon_Run2016H"
"2017/SingleMuon_Run2017B"
"2017/SingleMuon_Run2017C"
"2017/SingleMuon_Run2017D"
"2017/SingleMuon_Run2017E"
"2017/SingleMuon_Run2017F"
"2018/SingleMuon_Run2018A"
"2018/SingleMuon_Run2018B"
"2018/SingleMuon_Run2018C"
"2018/SingleMuon_Run2018D"
)

DATASAMPLE=(
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

for i in {0..16}
do
  ROOTNUM=`ll ${DATASAMPLE[$i]} | grep root | wc -l`
  LISTNUM=`ll ../../input/${DATAINPUT[$i]} | grep list | wc -l`
  echo ${DATASAMPLE[$i]} : $LISTNUM $ROOTNUM : ${DATAINPUT[$i]} ${DATASAMPLE[$i]}
done
