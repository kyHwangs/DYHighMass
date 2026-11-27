#include "NormalizationFactory.h"
#include "EventContentPool.h"

NormalizationFactory::NormalizationFactory(const YAML::Node& fConfig_) {
  
  fName = fConfig_["Name"].as<std::string>();

}

void NormalizationFactory::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;
  fEra = pool.GetEventInfo().era;
  fSampleName = pool.GetEventInfo().sampleName;
  
  if (fSampleName.find("NNLO_MUMU_10to50") != std::string::npos) fSampleName = "NNLO_MUMU_10to50";

  SetNormFactor();
}

float NormalizationFactory::GetValue(EventContentPool& pool, const WeightInput& fWeightInput, const std::string& fSyst, const int fSystIdx) {

  std::cout << "NormalizationFactory: " << fSyst << " " << fSystIdx << std::endl;
  
  if (!fIsMC) return 1.;

  if (fSystIdx == 0 && fSyst != "Nominal") throw std::runtime_error("NormalizationFactory: SystIdx is 0 but Syst is not Nominal");
  if (fSystIdx != 0 && fSyst == "Nominal") throw std::runtime_error("NormalizationFactory: SystIdx is not 0 but Syst is Nominal");

  if (fSystIdx == 0) return fNormFactor["Nominal"].at(0);
  else return fNormFactor[fSyst].at(fSystIdx - 1);
  
  return 1.;
}

void NormalizationFactory::SetNormFactor() {

  std::map<std::string, std::map<std::string, std::map<std::string, std::vector<float>>>> tTotalFactor = {
    {"NNLO_MUMU_10to50", {
      {"2016_preVFP", {
        {"Nominal", {8.953835386132878}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {6.696763261990173}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {8.30347047605725}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {8.478099138965206}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_inc", {
      {"2016_preVFP", {
        {"Nominal", {0.4215513491101474}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.13415297766728698}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.5338187137508883}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.5509086907640744}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_100to200", {
      {"2016_preVFP", {
        {"Nominal", {1.2383600146645484}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.9703520819934024}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {1.1069897131759685}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {1.1052278414354115}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_200to400", {
      {"2016_preVFP", {
        {"Nominal", {0.06435307413246215}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0532354583898508}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.1333330296833082}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.18829581187622477}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_400to500", {
      {"2016_preVFP", {
        {"Nominal", {0.01098179087666604}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.00997482544055479}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.02379284632939422}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.03353527832306685}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_500to700", {
      {"2016_preVFP", {
        {"Nominal", {0.006291482300884956}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.005501303986808098}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.013263257334362982}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.018733962575591602}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_700to800", {
      {"2016_preVFP", {
        {"Nominal", {0.0009547303404639952}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0008196905893215033}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.0020546958257658763}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.002896304901813821}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_800to1000", {
      {"2016_preVFP", {
        {"Nominal", {0.0008127401277650215}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0006947943062960094}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.0017387584157078201}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.0024687046992027057}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_1000to1500", {
      {"2016_preVFP", {
        {"Nominal", {0.0004472186440483608}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0003849561577709205}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.0009543698523052987}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.001351463991270611}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_1500to2000", {
      {"2016_preVFP", {
        {"Nominal", {6.0682315878443275e-05}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {5.511944977762655e-05}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.000130991269091663}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.00018454949603341893}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_MUMU_2000toInf", {
      {"2016_preVFP", {
        {"Nominal", {1.5981772583474028e-05}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {1.3947696139476963e-05}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {3.423638713453145e-05}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {4.833824575668259e-05}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"NNLO_tautau", {
      {"2016_preVFP", {
        {"Nominal", {0.47961110764787024}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.15513431281953255}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.16196906531815308}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.1650264618665613}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"ST_s", {
      {"2016_preVFP", {
        {"Nominal", {0.010269311816973582}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.008921670428731914}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.008988124957897413}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.008921768832221068}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"ST_t_AntiTop", {
      {"2016_preVFP", {
        {"Nominal", {0.0007863440536620225}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0006866660949077951}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.0007550300241948225}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.000777766013456906}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"ST_t_Top", {
      {"2016_preVFP", {
        {"Nominal", {0.00043995301592914064}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0003363106503096246}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.0004093644072775737}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.0004208797760939243}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"ST_tW_AntiTop", {
      {"2016_preVFP", {
        {"Nominal", {0.010341217553520375}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.00802320798989329}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.009054439897047876}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.009357571812149898}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"ST_tW_Top", {
      {"2016_preVFP", {
        {"Nominal", {0.010360850098116514}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.008241873012192561}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.009111814827007894}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.009131541793817559}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"WJetsToLNu", {
      {"2016_preVFP", {
        {"Nominal", {0.00024045816112795335}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.00021211904341278688}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.000568325653433938}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.000728911359574558}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"TTTo2L2Nu", {
      {"2016_preVFP", {
        {"Nominal", {0.0006381687375488736}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0004735374884150164}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.00048442280500276175}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.0005031686064640101}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"WW", {
      {"2016_preVFP", {
        {"Nominal", {0.0064637667958500926}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0057956120688575745}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.005936243144471701}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.0059506189944155815}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"WZ", {
      {"2016_preVFP", {
        {"Nominal", {0.06781005797832115}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.06111708860759494}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.14730520978577766}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.20657751889168766}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"ZZ", {
      {"2016_preVFP", {
        {"Nominal", {0.18511310452418098}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.1776333622936577}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.18943104212860312}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.20519186046511628}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_10to30_ElEl", {
      {"2016_preVFP", {
        {"Nominal", {0.88842}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.95676}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.9594936000000001}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {1.354271}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_30to50_ElEl", {
      {"2016_preVFP", {
        {"Nominal", {0.052384799999999995}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.056414399999999996}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.056575584}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.07985323999999999}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_50to200_ElEl", {
      {"2016_preVFP", {
        {"Nominal", {0.019289399999999998}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.02087181934641179}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.020832552}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.029403969999999998}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_200to1500_ElEl", {
      {"2016_preVFP", {
        {"Nominal", {0.000439686}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.00047803016536434673}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.00047486087999999997}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.0006702393}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_1500toInf_ElEl", {
      {"2016_preVFP", {
        {"Nominal", {2.97414e-07}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {3.2646213433900724e-07}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {3.2120712000000004e-07}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {4.5336570000000006e-07}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_10to30_InelElElInel", {
      {"2016_preVFP", {
        {"Nominal", {0.82056}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.88368}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.8862047999999999}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {1.250828}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_30to50_InelElElInel", {
      {"2016_preVFP", {
        {"Nominal", {0.06987240000000002}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0752472}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.07546219200000001}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.10651062000000001}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_50to200_InelElElInel", {
      {"2016_preVFP", {
        {"Nominal", {0.0288132}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0310296}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.031118256}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.04392166}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_200to1500_InelElElInel", {
      {"2016_preVFP", {
        {"Nominal", {0.0008658000000000001}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0009934208773939217}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.0009350640000000001}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.00131979}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_1500toInf_InelElElInel", {
      {"2016_preVFP", {
        {"Nominal", {6.85308e-07}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {7.415277184697697e-07}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {7.4013264e-07}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {1.0446554000000001e-06}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_10to30_InelInel", {
      {"2016_preVFP", {
        {"Nominal", {0.8112}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.8736}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.876096}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {1.23656}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_30to50_InelInel", {
      {"2016_preVFP", {
        {"Nominal", {0.091338}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.09925731584258325}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.09864504}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.1392319}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_50to200_InelInel", {
      {"2016_preVFP", {
        {"Nominal", {0.043797}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.04716600000000001}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.047300760000000004}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.06676235000000001}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_200to1500_InelInel", {
      {"2016_preVFP", {
        {"Nominal", {0.00169494}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0018253199999999999}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.0018305352}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.0025836970000000003}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"GGToMuMu_1500toInf_InelInel", {
      {"2016_preVFP", {
        {"Nominal", {1.981341034655952e-06}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {1.7237542160119697e-06}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {1.7041752000000002e-06}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {2.4053470000000003e-06}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-15To20_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {11481.634932058249}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {10371.371757913494}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {13059.002921823947}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {17827.429006137307}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-20To30_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {1537.9072804711716}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {1371.0482671475593}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {1641.1533670913836}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {2468.5269643270203}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-30To50_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {925.8555080021379}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {644.5478135472761}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {977.5869428060516}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {1377.5046841725743}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-50To80_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {373.4969701375811}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {295.3303251428193}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {394.6393902039262}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {561.1901697791775}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-80To120_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {78.61679657693597}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {67.64790157369511}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {81.16882750251145}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {115.79066371328553}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-120To170_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {21.469225242909204}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {17.90175993449009}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {22.527350529346606}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {32.94339354092295}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-170To300_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {3.691167529796758}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {3.4496108542250443}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {4.045877115672248}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {5.805953735926829}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-300To470_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {0.42637041036873263}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.35059242528005796}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.4466550288301493}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.6276815384996126}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-470To600_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {0.058276119896870586}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.05001371921001022}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.06277699686971752}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.09099905116082113}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-600To800_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {0.018128136318111}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.016850179157344623}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.01951649327856377}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.029119325159746776}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-800To1000_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {0.0016277571702221763}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0014031219102728723}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.0017501632891225645}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.0024474939783446013}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
    {"QCD_Pt-1000_MuEnrichedPt5", {
      {"2016_preVFP", {
        {"Nominal", {0.0014274212303214428}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2016_postVFP", {
        {"Nominal", {0.0012081597382780818}},
        {"uncorrelated", {0.99, 1.01}},
        {"correlated_161718", {0.994, 1.006}},
        {"correlated_1718", {1, 1}},
      }},
      {"2017", {
        {"Nominal", {0.0015328474245815957}},
        {"uncorrelated", {0.98, 1.02}},
        {"correlated_161718", {0.991, 1.009}},
        {"correlated_1718", {0.994, 1.006}},
      }},
      {"2018", {
        {"Nominal", {0.002167561826556406}},
        {"uncorrelated", {0.985, 1.015}},
        {"correlated_161718", {0.98, 1.02}},
        {"correlated_1718", {0.998, 1.002}},
      }},
    }},
  };

  fNormFactor = tTotalFactor[fSampleName][fEra];
}


