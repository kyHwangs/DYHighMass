#include "NT.h"
#include "options.h"
#include "DYLoopEMU.h"

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "TFile.h"
#include "TROOT.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TStopwatch.h"
#include "TChain.h"
#include "TLorentzVector.h"

#include "yaml-cpp/yaml.h"

int main(int argc, char* argv[]) {

  options* fOpt = new options(argc, argv);

  DYLoopEMU* fLoops = new DYLoopEMU(fOpt);
  fLoops->Loop();
  
  return 1;
}
