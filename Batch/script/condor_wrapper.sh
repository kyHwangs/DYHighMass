#!/bin/sh

echo "$@"
cd /u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241001/DYHighMass

source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh

export INSTALL_DIR_PATH=$PWD/install

export PATH=$PATH:$INSTALL_DIR_PATH/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_DIR_PATH/lib
export YAMLPATH=/cvmfs/sft.cern.ch/lcg/releases/yamlcpp/0.6.3-d05b2/x86_64-centos7-gcc11-opt/lib

cd /u/user/kyhwang/WorkingDir/CMS/HighMassDY/NanoAOD/dev_241001/DYHighMass/Batch/241115_2018_noCorrection
echo $PWD

echo "$@"
eval "$@"
