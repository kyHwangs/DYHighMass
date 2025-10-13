# DYHighMass
DY high mass differential cross section measurement tool

## How to install
```sh
# install analyzer
git clone https://github.com/kyHwangs/DYHighMass.git
cd DYHighMass
source envset.sh
mkdir install build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=../install
make -j4 install

# install plotter
cd DYHighMass
source envset.sh
pip3 install --user 
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
hadd output.root ./ROOT/*.root # taks ~ 10 min
job_summary --channel <channel>

or 

hadd <output>.root ./ROOT/*.root # taks ~ 10 min
job_summary --channel <channel> --input <output>
```
When the jobs are done without any problem, no warning sign appears.
### **<UNDER CONSTRUCTION>**



