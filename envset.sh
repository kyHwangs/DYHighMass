#! /bin/bash

source /cvmfs/sft.cern.ch/lcg/views/LCG_107/x86_64-el9-gcc13-opt/setup.sh
export INSTALL_DIR_PATH=$PWD/install

export PATH=$PATH:$INSTALL_DIR_PATH/lib:$INSTALL_DIR_PATH/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_DIR_PATH/lib
export YAMLPATH=/cvmfs/sft.cern.ch/lcg/releases/yamlcpp/0.6.3-d05b2/x86_64-el9-gcc13-opt/lib
export CORRECTIONLIBPATH=/cvmfs/sft.cern.ch/lcg/releases/correctionlib/2.6.4-b8584/x86_64-el9-gcc13-opt/lib

export DY_HIGHMASS_WORKSPACE=$PWD
export DY_HIGHMASS_WORKSPACE_INSTALL=$PWD/install
export DY_HIGHMASS_WORKSPACE_BATCH=$PWD/Batch
