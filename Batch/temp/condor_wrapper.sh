#!/bin/sh

echo "$@"
cd /u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241202_JetFolding/DYHighMass

source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh

export INSTALL_DIR_PATH=$PWD/install

export PATH=$PATH:$INSTALL_DIR_PATH/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_DIR_PATH/lib
export YAMLPATH=/cvmfs/sft.cern.ch/lcg/releases/yamlcpp/0.6.3-d05b2/x86_64-centos7-gcc11-opt/lib

cd /u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241202_JetFolding/DYHighMass/Batch/241202_FullRun2_v5_trigger
echo $PWD

echo "$@"
eval "$@"
