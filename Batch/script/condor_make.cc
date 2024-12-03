#include <utility>
#include <vector>
#include <iostream>
#include <fstream>

int condor_make() {


  // 2016_preVFP
  std::vector<std::string> fileVec_2016_preVFP = {};
  fileVec_2016_preVFP.push_back("Run2016B_APV_ver2");
  fileVec_2016_preVFP.push_back("Run2016C_APV");
  fileVec_2016_preVFP.push_back("Run2016D_APV");
  fileVec_2016_preVFP.push_back("Run2016E_APV");
  fileVec_2016_preVFP.push_back("Run2016F_APV");
  fileVec_2016_preVFP.push_back("NNLO_inc");
  fileVec_2016_preVFP.push_back("NNLO_10to50");
  fileVec_2016_preVFP.push_back("NNLO_10to50_v2");
  fileVec_2016_preVFP.push_back("NNLO_100to200");
  fileVec_2016_preVFP.push_back("NNLO_200to400");
  fileVec_2016_preVFP.push_back("NNLO_400to500");
  fileVec_2016_preVFP.push_back("NNLO_500to700");
  fileVec_2016_preVFP.push_back("NNLO_700to800");
  fileVec_2016_preVFP.push_back("NNLO_800to1000");
  fileVec_2016_preVFP.push_back("NNLO_1000to1500");
  fileVec_2016_preVFP.push_back("NNLO_1500to2000");
  fileVec_2016_preVFP.push_back("NNLO_2000toInf");
  fileVec_2016_preVFP.push_back("NNLO_tautau");
  fileVec_2016_preVFP.push_back("ST_s");
  fileVec_2016_preVFP.push_back("ST_t_AntiTop");
  fileVec_2016_preVFP.push_back("ST_t_Top");
  fileVec_2016_preVFP.push_back("ST_tW_AntiTop");
  fileVec_2016_preVFP.push_back("ST_tW_Top");
  fileVec_2016_preVFP.push_back("TTTo2L2Nu");
  fileVec_2016_preVFP.push_back("WJetsToLNu");
  fileVec_2016_preVFP.push_back("WW");
  fileVec_2016_preVFP.push_back("WZ");
  fileVec_2016_preVFP.push_back("ZZ");

  std::map<std::string, int> fileMap_2016_preVFP = {};
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_inc",  6));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_10to50",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_10to50_v2",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_100to200",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_200to400",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_400to500",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_500to700",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_700to800",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_800to1000",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_1000to1500",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_1500to2000",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_2000toInf",  1));
  fileMap_2016_preVFP.insert(std::make_pair("NNLO_tautau",  4));
  fileMap_2016_preVFP.insert(std::make_pair("ST_s",  2));
  fileMap_2016_preVFP.insert(std::make_pair("ST_t_AntiTop",  5));
  fileMap_2016_preVFP.insert(std::make_pair("ST_t_Top",  5));
  fileMap_2016_preVFP.insert(std::make_pair("ST_tW_AntiTop",  1));
  fileMap_2016_preVFP.insert(std::make_pair("ST_tW_Top",  1));
  fileMap_2016_preVFP.insert(std::make_pair("TTTo2L2Nu",  4));
  fileMap_2016_preVFP.insert(std::make_pair("WJetsToLNu",  6));
  fileMap_2016_preVFP.insert(std::make_pair("WW",  2));
  fileMap_2016_preVFP.insert(std::make_pair("WZ",  2));
  fileMap_2016_preVFP.insert(std::make_pair("ZZ",  1));
  fileMap_2016_preVFP.insert(std::make_pair("Run2016B_APV_ver2", 7));
  fileMap_2016_preVFP.insert(std::make_pair("Run2016C_APV", 3));
  fileMap_2016_preVFP.insert(std::make_pair("Run2016D_APV", 4));
  fileMap_2016_preVFP.insert(std::make_pair("Run2016E_APV", 5));
  fileMap_2016_preVFP.insert(std::make_pair("Run2016F_APV", 5));



  // 2016_postVFP
  std::vector<std::string> fileVec_2016_postVFP = {};
  fileVec_2016_postVFP.push_back("Run2016F");
  fileVec_2016_postVFP.push_back("Run2016G");
  fileVec_2016_postVFP.push_back("Run2016H");
  fileVec_2016_postVFP.push_back("NNLO_inc");
  fileVec_2016_postVFP.push_back("NNLO_10to50");
  fileVec_2016_postVFP.push_back("NNLO_10to50_v2");
  fileVec_2016_postVFP.push_back("NNLO_100to200");
  fileVec_2016_postVFP.push_back("NNLO_200to400");
  fileVec_2016_postVFP.push_back("NNLO_400to500");
  fileVec_2016_postVFP.push_back("NNLO_500to700");
  fileVec_2016_postVFP.push_back("NNLO_700to800");
  fileVec_2016_postVFP.push_back("NNLO_800to1000");
  fileVec_2016_postVFP.push_back("NNLO_1000to1500");
  fileVec_2016_postVFP.push_back("NNLO_1500to2000");
  fileVec_2016_postVFP.push_back("NNLO_2000toInf");
  fileVec_2016_postVFP.push_back("NNLO_tautau");
  fileVec_2016_postVFP.push_back("ST_s");
  fileVec_2016_postVFP.push_back("ST_t_AntiTop");
  fileVec_2016_postVFP.push_back("ST_t_Top");
  fileVec_2016_postVFP.push_back("ST_tW_AntiTop");
  fileVec_2016_postVFP.push_back("ST_tW_Top");
  fileVec_2016_postVFP.push_back("TTTo2L2Nu");
  fileVec_2016_postVFP.push_back("WJetsToLNu");
  fileVec_2016_postVFP.push_back("WW");
  fileVec_2016_postVFP.push_back("WZ");
  fileVec_2016_postVFP.push_back("ZZ");

  std::map<std::string, int> fileMap_2016_postVFP = {};
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_inc", 6));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_10to50", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_10to50_v2", 2));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_100to200", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_200to400", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_400to500", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_500to700", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_700to800", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_800to1000", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_1000to1500", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_1500to2000", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_2000toInf", 1));
  fileMap_2016_postVFP.insert(std::make_pair("NNLO_tautau", 9));
  fileMap_2016_postVFP.insert(std::make_pair("ST_s", 2));
  fileMap_2016_postVFP.insert(std::make_pair("ST_t_AntiTop", 4));
  fileMap_2016_postVFP.insert(std::make_pair("ST_t_Top", 9));
  fileMap_2016_postVFP.insert(std::make_pair("ST_tW_AntiTop", 3));
  fileMap_2016_postVFP.insert(std::make_pair("ST_tW_Top", 3));
  fileMap_2016_postVFP.insert(std::make_pair("TTTo2L2Nu", 5));
  fileMap_2016_postVFP.insert(std::make_pair("WJetsToLNu", 3));
  fileMap_2016_postVFP.insert(std::make_pair("WW", 1));
  fileMap_2016_postVFP.insert(std::make_pair("WZ", 2));
  fileMap_2016_postVFP.insert(std::make_pair("ZZ", 2));
  fileMap_2016_postVFP.insert(std::make_pair("Run2016F", 1));
  fileMap_2016_postVFP.insert(std::make_pair("Run2016G", 7));
  fileMap_2016_postVFP.insert(std::make_pair("Run2016H", 9));


  // 2017
  std::vector<std::string> fileVec_2017 = {};
  fileVec_2017.push_back("Run2017B");
  fileVec_2017.push_back("Run2017C");
  fileVec_2017.push_back("Run2017D");
  fileVec_2017.push_back("Run2017E");
  fileVec_2017.push_back("Run2017F");
  fileVec_2017.push_back("NNLO_inc");
  fileVec_2017.push_back("NNLO_10to50");
  fileVec_2017.push_back("NNLO_10to50_v2");
  fileVec_2017.push_back("NNLO_100to200");
  fileVec_2017.push_back("NNLO_200to400");
  fileVec_2017.push_back("NNLO_400to500");
  fileVec_2017.push_back("NNLO_500to700");
  fileVec_2017.push_back("NNLO_700to800");
  fileVec_2017.push_back("NNLO_800to1000");
  fileVec_2017.push_back("NNLO_1000to1500");
  fileVec_2017.push_back("NNLO_1500to2000");
  fileVec_2017.push_back("NNLO_2000toInf");
  fileVec_2017.push_back("NNLO_tautau");
  fileVec_2017.push_back("ST_s");
  fileVec_2017.push_back("ST_t_AntiTop");
  fileVec_2017.push_back("ST_t_Top");
  fileVec_2017.push_back("ST_tW_AntiTop");
  fileVec_2017.push_back("ST_tW_Top");
  fileVec_2017.push_back("TTTo2L2Nu");
  fileVec_2017.push_back("WJetsToLNu");
  fileVec_2017.push_back("WW");
  fileVec_2017.push_back("WZ");
  fileVec_2017.push_back("ZZ");

  std::map<std::string, int> fileMap_2017 = {};
  fileMap_2017.insert(std::make_pair("NNLO_inc", 17));
  fileMap_2017.insert(std::make_pair("NNLO_10to50", 1));
  fileMap_2017.insert(std::make_pair("NNLO_10to50_v2", 3));
  fileMap_2017.insert(std::make_pair("NNLO_100to200", 1));
  fileMap_2017.insert(std::make_pair("NNLO_200to400", 1));
  fileMap_2017.insert(std::make_pair("NNLO_400to500", 1));
  fileMap_2017.insert(std::make_pair("NNLO_500to700", 1));
  fileMap_2017.insert(std::make_pair("NNLO_700to800", 1));
  fileMap_2017.insert(std::make_pair("NNLO_800to1000", 1));
  fileMap_2017.insert(std::make_pair("NNLO_1000to1500", 1));
  fileMap_2017.insert(std::make_pair("NNLO_1500to2000", 1));
  fileMap_2017.insert(std::make_pair("NNLO_2000toInf", 1));
  fileMap_2017.insert(std::make_pair("NNLO_tautau", 6));
  fileMap_2017.insert(std::make_pair("ST_s", 2));
  fileMap_2017.insert(std::make_pair("ST_t_AntiTop", 6));
  fileMap_2017.insert(std::make_pair("ST_t_Top", 20));
  fileMap_2017.insert(std::make_pair("ST_tW_AntiTop", 5));
  fileMap_2017.insert(std::make_pair("ST_tW_Top", 5));
  fileMap_2017.insert(std::make_pair("TTTo2L2Nu", 10));
  fileMap_2017.insert(std::make_pair("WJetsToLNu", 5));
  fileMap_2017.insert(std::make_pair("WW", 5));
  fileMap_2017.insert(std::make_pair("WZ", 3));
  fileMap_2017.insert(std::make_pair("ZZ", 1));
  fileMap_2017.insert(std::make_pair("Run2017B", 6));
  fileMap_2017.insert(std::make_pair("Run2017C", 10));
  fileMap_2017.insert(std::make_pair("Run2017D", 4));
  fileMap_2017.insert(std::make_pair("Run2017E", 9));
  fileMap_2017.insert(std::make_pair("Run2017F", 14));


  // 2018
  std::vector<std::string> fileVec_2018 = {};
  fileVec_2018.push_back("Run2018A");
  fileVec_2018.push_back("Run2018B");
  fileVec_2018.push_back("Run2018C");
  fileVec_2018.push_back("Run2018D");
  fileVec_2018.push_back("NNLO_inc");
  fileVec_2018.push_back("NNLO_10to50");
  fileVec_2018.push_back("NNLO_10to50_v2");
  fileVec_2018.push_back("NNLO_100to200");
  fileVec_2018.push_back("NNLO_200to400");
  fileVec_2018.push_back("NNLO_400to500");
  fileVec_2018.push_back("NNLO_500to700");
  fileVec_2018.push_back("NNLO_700to800");
  fileVec_2018.push_back("NNLO_800to1000");
  fileVec_2018.push_back("NNLO_1000to1500");
  fileVec_2018.push_back("NNLO_1500to2000");
  fileVec_2018.push_back("NNLO_2000toInf");
  fileVec_2018.push_back("NNLO_tautau");
  fileVec_2018.push_back("ST_s");
  fileVec_2018.push_back("ST_t_AntiTop");
  fileVec_2018.push_back("ST_t_Top");
  fileVec_2018.push_back("ST_tW_AntiTop");
  fileVec_2018.push_back("ST_tW_Top");
  fileVec_2018.push_back("TTTo2L2Nu");
  fileVec_2018.push_back("WJetsToLNu");
  fileVec_2018.push_back("WW");
  fileVec_2018.push_back("WZ");
  fileVec_2018.push_back("ZZ");

  std::map<std::string, int> fileMap_2018 = {};
  fileMap_2018.insert(std::make_pair("NNLO_inc", 29));
  fileMap_2018.insert(std::make_pair("NNLO_10to50", 1));
  fileMap_2018.insert(std::make_pair("NNLO_10to50_v2", 4));
  fileMap_2018.insert(std::make_pair("NNLO_100to200", 1));
  fileMap_2018.insert(std::make_pair("NNLO_200to400", 1));
  fileMap_2018.insert(std::make_pair("NNLO_400to500", 1));
  fileMap_2018.insert(std::make_pair("NNLO_500to700", 1));
  fileMap_2018.insert(std::make_pair("NNLO_700to800", 1));
  fileMap_2018.insert(std::make_pair("NNLO_800to1000", 1));
  fileMap_2018.insert(std::make_pair("NNLO_1000to1500", 1));
  fileMap_2018.insert(std::make_pair("NNLO_1500to2000", 1));
  fileMap_2018.insert(std::make_pair("NNLO_2000toInf", 1));
  fileMap_2018.insert(std::make_pair("NNLO_tautau", 12));
  fileMap_2018.insert(std::make_pair("ST_s", 2));
  fileMap_2018.insert(std::make_pair("ST_t_AntiTop", 13));
  fileMap_2018.insert(std::make_pair("ST_t_Top", 15));
  fileMap_2018.insert(std::make_pair("ST_tW_AntiTop", 3));
  fileMap_2018.insert(std::make_pair("ST_tW_Top", 6));
  fileMap_2018.insert(std::make_pair("TTTo2L2Nu", 16));
  fileMap_2018.insert(std::make_pair("WJetsToLNu", 6));
  fileMap_2018.insert(std::make_pair("WW", 4));
  fileMap_2018.insert(std::make_pair("WZ", 2));
  fileMap_2018.insert(std::make_pair("ZZ", 1));
  fileMap_2018.insert(std::make_pair("Run2018A", 10));
  fileMap_2018.insert(std::make_pair("Run2018B", 6));
  fileMap_2018.insert(std::make_pair("Run2018C", 7));
  fileMap_2018.insert(std::make_pair("Run2018D", 21));


  std::fstream file_out;
  file_out.open("joblist.txt", std::ios_base::out);
  if (!file_out.is_open()) {
    std::cout << "failed to open " << std::endl;
    return 1;
  }

  std::cout << "fileVec_2016_preVFP " << fileVec_2016_preVFP.size() << std::endl;
  for (int i = 0; i < fileVec_2016_preVFP.size(); i++) {
    int fileNum = fileMap_2016_preVFP.at(fileVec_2016_preVFP.at(i));
    std::cout << i << " " << fileVec_2016_preVFP.at(i) << " " << fileNum << std::endl;

    for (int j = 1; j <= fileNum; j++) {

      file_out << "../../config/UL2016_preVFP.yml 2016_preVFP " << fileVec_2016_preVFP.at(i) << " " << j << std::endl;

    }
  }


  std::cout << "fileVec_2016_postVFP " << fileVec_2016_postVFP.size() << std::endl;
  for (int i = 0; i < fileVec_2016_postVFP.size(); i++) {
    int fileNum = fileMap_2016_postVFP.at(fileVec_2016_postVFP.at(i));
    std::cout << fileVec_2016_postVFP.at(i) << " " << fileNum << std::endl;

    for (int j = 1; j <= fileNum; j++) {

      file_out << "../../config/UL2016_postVFP.yml 2016_postVFP " << fileVec_2016_postVFP.at(i) << " " << j << std::endl;

    }
  }

  std::cout << "fileVec_2017 " << fileVec_2017.size() << std::endl;
  for (int i = 0; i < fileVec_2017.size(); i++) {
    int fileNum = fileMap_2017.at(fileVec_2017.at(i));
    std::cout << fileVec_2017.at(i) << " " << fileNum << std::endl;

    for (int j = 1; j <= fileNum; j++) {

      file_out << "../../config/UL2017.yml 2017 " << fileVec_2017.at(i) << " " << j << std::endl;

    }
  }

  std::cout << "fileVec_2018 " << fileVec_2018.size() << std::endl;
  for (int i = 0; i < fileVec_2018.size(); i++) {
    int fileNum = fileMap_2018.at(fileVec_2018.at(i));
    std::cout << fileVec_2018.at(i) << " " << fileNum << std::endl;

    for (int j = 1; j <= fileNum; j++) {

      file_out << "../../config/UL2018.yml 2018 " << fileVec_2018.at(i) << " " << j << std::endl;

    }
  }

  file_out.close();

  // file_out <<  ../../config/UL2018.yml

  // arguments             = ../../install/bin/analysis --config $(config) --base ../../ --era $(era) --sample $(sample) --id $(id) --output ./
  // queue config,era,sample,id from joblist.txt


}
