#include "NT.h"
#include "options.h"
#include "DYLoop.h"

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

  options* opt = new options(argc, argv);

  DYLoop* fLoops = new DYLoop(opt);
  fLoops->Loop();
  
  return 1;
}
