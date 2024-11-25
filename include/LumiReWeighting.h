#ifndef LumiReWeighting_h
#define LumiReWeighting_h

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "TH1.h"
#include "TFile.h"

class LumiReWeighting {
public:

  LumiReWeighting( std::string generatedFile,
	     std::string dataFile,
	     std::string GenHistName,
	     std::string DataHistName);

  double weight( int npv ) ;
  double weight( float npv ) ;

protected:

  std::string generatedFileName_;
  std::string dataFileName_;
  std::string GenHistName_;
  std::string DataHistName_;

  TH1* weights_;
  TFile* dataFile_;
  TFile* generatedFile_;

  TH1* MC_distr_;
  TH1* Data_distr_;

  int  OldLumiSection_;
  bool FirstWarning_;
};

#endif
