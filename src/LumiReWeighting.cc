#include <iostream>
#include <string>
#include <algorithm>

#include "TRandom1.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include "TH1.h"
#include "TFile.h"

#include "LumiReWeighting.h"

LumiReWeighting::LumiReWeighting( std::string generatedFile,
		   std::string dataFile,
		   std::string GenHistName = "pileup",
		   std::string DataHistName = "pileup" )
: generatedFileName_( generatedFile), dataFileName_( dataFile ), GenHistName_( GenHistName ), DataHistName_( DataHistName )
{
	generatedFile_ =  new TFile(generatedFileName_.c_str());
	dataFile_      =  new TFile(dataFileName_.c_str());
	Data_distr_ = static_cast<TH1*>(dataFile_->Get( DataHistName_.c_str() )->Clone() );
	MC_distr_ = static_cast<TH1*>(generatedFile_->Get( GenHistName_.c_str() )->Clone() );

	Data_distr_->Scale( 1.0/ Data_distr_->Integral() );
	MC_distr_->Scale( 1.0/ MC_distr_->Integral() );

	weights_ = static_cast<TH1*>(Data_distr_->Clone());
	weights_->SetName("lumiWeights");

	TH1* den = dynamic_cast<TH1*>(MC_distr_->Clone());

	//den->Scale(1.0/ den->Integral());
	weights_->Divide( den );

	int NBins = weights_->GetNbinsX();

	// for(int ibin = 1; ibin<NBins+1; ++ibin){
	//   std::cout << "   " << ibin-1 << " " << weights_->GetBinContent(ibin) << std::endl;
	// }
	//
	FirstWarning_ = true;
	OldLumiSection_ = -1;
}

double LumiReWeighting::weight( int npv ) {
  int bin = weights_->GetXaxis()->FindBin( npv );
  return weights_->GetBinContent( bin );
}

double LumiReWeighting::weight( float npv ) {
  int bin = weights_->GetXaxis()->FindBin( npv );
  return weights_->GetBinContent( bin );
}
