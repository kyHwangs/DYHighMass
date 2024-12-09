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


  //     std::string highPtID_test = "null";
  //     // if (tNtuples->Muon_highPtId->At(i) == (UChar_t)(1)) highPtID_test = "tracker high pT";
  //     // if (tNtuples->Muon_highPtId->At(i) == (UChar_t)(2)) highPtID_test = "global high pT";
  //
  return 1;
}
