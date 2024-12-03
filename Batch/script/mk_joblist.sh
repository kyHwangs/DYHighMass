ERA=$1

echo $ERA 'job lists'

# NNLO_inc
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-50 | grep list | wc -l`
echo -e NNLO_inc $RUNNUMBER
# NNLO_10to50
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-10to50_v1 | grep list | wc -l`
echo -e NNLO_10to50 $RUNNUMBER
# NNLO_10to50_v2
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-10to50_ext1-v1 | grep list | wc -l`
echo -e NNLO_10to50_v2 $RUNNUMBER
# NNLO_100to200
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-100to200 | grep list | wc -l`
echo -e NNLO_100to200 $RUNNUMBER
# NNLO_200to400
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-200to400 | grep list | wc -l`
echo -e NNLO_200to400 $RUNNUMBER
# NNLO_400to500
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-400to500 | grep list | wc -l`
echo -e NNLO_400to500 $RUNNUMBER
# NNLO_500to700
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-500to700 | grep list | wc -l`
echo -e NNLO_500to700 $RUNNUMBER
# NNLO_700to800
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-700to800 | grep list | wc -l`
echo -e NNLO_700to800 $RUNNUMBER
# NNLO_800to1000
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-800to1000 | grep list | wc -l`
echo -e NNLO_800to1000 $RUNNUMBER
# NNLO_1000to1500
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-1000to1500 | grep list | wc -l`
echo -e NNLO_1000to1500 $RUNNUMBER
# NNLO_1500to2000
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-1500to2000 | grep list | wc -l`
echo -e NNLO_1500to2000 $RUNNUMBER
# NNLO_2000toInf
RUNNUMBER=`ll ../../input/$ERA/DYJetsToMuMu_M-2000toInf | grep list | wc -l`
echo -e NNLO_2000toInf $RUNNUMBER
# NNLO_tautau
RUNNUMBER=`ll ../../input/$ERA/DYJetsToTauTau_M-50_AtLeastOneEorMuDecay | grep list | wc -l`
echo -e NNLO_tautau $RUNNUMBER
# ST_s
RUNNUMBER=`ll ../../input/$ERA/ST_s-channel_4f_leptonDecays | grep list | wc -l`
echo -e ST_s $RUNNUMBER
# ST_t_AntiTop
RUNNUMBER=`ll ../../input/$ERA/ST_t-channel_anitop_4f_InclusiveDecays | grep list | wc -l`
echo -e ST_t_AntiTop $RUNNUMBER
# ST_t_Top
RUNNUMBER=`ll ../../input/$ERA/ST_t-channel_top_4f_InclusiveDecays | grep list | wc -l`
echo -e ST_t_Top $RUNNUMBER
# ST_tW_AntiTop
RUNNUMBER=`ll ../../input/$ERA/ST_tW_antitop_5f_inclusiveDecays | grep list | wc -l`
echo -e ST_tW_AntiTop $RUNNUMBER
# ST_tW_Top
RUNNUMBER=`ll ../../input/$ERA/ST_tW_top_5f_inclusiveDecays | grep list | wc -l`
echo -e ST_tW_Top $RUNNUMBER
# TTTo2L2Nu
RUNNUMBER=`ll ../../input/$ERA/TTTo2L2Nu | grep list | wc -l`
echo -e TTTo2L2Nu $RUNNUMBER
# WJetsToLNu
RUNNUMBER=`ll ../../input/$ERA/WJetsToLNu | grep list | wc -l`
echo -e WJetsToLNu $RUNNUMBER
# WW
RUNNUMBER=`ll ../../input/$ERA/WWTo2L2Nu | grep list | wc -l`
echo -e WW $RUNNUMBER
# WZ
RUNNUMBER=`ll ../../input/$ERA/WZ | grep list | wc -l`
echo -e WZ $RUNNUMBER
# ZZ
RUNNUMBER=`ll ../../input/$ERA/ZZ | grep list | wc -l`
echo -e ZZ $RUNNUMBER




RUNNUMBER=`ll ../../input/2016_preVFP/SingleMuon_Run2016B_APV_ver1 | grep list | wc -l`
echo -e Run2016B_APV_ver1 $RUNNUMBER

RUNNUMBER=`ll ../../input/2016_preVFP/SingleMuon_Run2016B_APV_ver2 | grep list | wc -l`
echo -e Run2016B_APV_ver2 $RUNNUMBER

RUNNUMBER=`ll ../../input/2016_preVFP/SingleMuon_Run2016C_APV | grep list | wc -l`
echo -e Run2016C $RUNNUMBER

RUNNUMBER=`ll ../../input/2016_preVFP/SingleMuon_Run2016D_APV | grep list | wc -l`
echo -e Run2016D $RUNNUMBER

RUNNUMBER=`ll ../../input/2016_preVFP/SingleMuon_Run2016E_APV | grep list | wc -l`
echo -e Run2016E $RUNNUMBER

RUNNUMBER=`ll ../../input/2016_preVFP/SingleMuon_Run2016F_APV | grep list | wc -l`
echo -e Run2016F $RUNNUMBER


RUNNUMBER=`ll ../../input/2016_postVFP/SingleMuon_Run2016F | grep list | wc -l`
echo -e Run2016F $RUNNUMBER

RUNNUMBER=`ll ../../input/2016_postVFP/SingleMuon_Run2016G | grep list | wc -l`
echo -e Run2016G $RUNNUMBER

RUNNUMBER=`ll ../../input/2016_postVFP/SingleMuon_Run2016H | grep list | wc -l`
echo -e Run2016H $RUNNUMBER



RUNNUMBER=`ll ../../input/2017/SingleMuon_Run2017B | grep list | wc -l`
echo -e Run2017B $RUNNUMBER

RUNNUMBER=`ll ../../input/2017/SingleMuon_Run2017C | grep list | wc -l`
echo -e Run2017C $RUNNUMBER

RUNNUMBER=`ll ../../input/2017/SingleMuon_Run2017D | grep list | wc -l`
echo -e Run2017D $RUNNUMBER

RUNNUMBER=`ll ../../input/2017/SingleMuon_Run2017E | grep list | wc -l`
echo -e Run2017E $RUNNUMBER

RUNNUMBER=`ll ../../input/2017/SingleMuon_Run2017F | grep list | wc -l`
echo -e Run2017F $RUNNUMBER

RUNNUMBER=`ll ../../input/2017/SingleMuon_Run2017G | grep list | wc -l`
echo -e Run2017G $RUNNUMBER

RUNNUMBER=`ll ../../input/2017/SingleMuon_Run2017H | grep list | wc -l`
echo -e Run2017H $RUNNUMBER


# Run2018A
RUNNUMBER=`ll ../../input/2018/SingleMuon_Run2018A | grep list | wc -l`
echo -e Run2018A $RUNNUMBER
# Run2018B
RUNNUMBER=`ll ../../input/2018/SingleMuon_Run2018B | grep list | wc -l`
echo -e Run2018B $RUNNUMBER
# Run2018C
RUNNUMBER=`ll ../../input/2018/SingleMuon_Run2018C | grep list | wc -l`
echo -e Run2018C $RUNNUMBER
# Run2018D
RUNNUMBER=`ll ../../input/2018/SingleMuon_Run2018D | grep list | wc -l`
echo -e Run2018D $RUNNUMBER
