# DYHighMass
DY high mass differential cross section measurement tool

## How to install
```sh
git clone https://github.com/kyHwangs/DYHighMass.git
cd DYHighMass
source envset.sh
mkdir install build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=../install
make -j4 install
```

## How to submit jobs
```sh
mkdir DYHighMass/Batch
cd Batch
generate_script --base <baseDir> --channel <channel>
cd <baseDir>
condor_submit condor_submit.sub
```

`<baseDir>`: base workspace name

`<channel>`: channel to analysis **(ALLOWED: MUMU, EMU, EE)**

## How to make plots
When the condor jobs are done, you should merge all of them and check the status of jobs.
```sh
hadd -j8 output.root ./ROOT/*.root
job_summary
```
When the jobs are done without any problem, no warning sign appears.
### **UNDER CONSTRUCTION**



