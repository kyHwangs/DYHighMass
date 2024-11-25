#include <utility>
#include <vector>
#include <iostream>
#include <fstream>

int condor_make() {

  std::vector<std::string> fileVec = {};
  fileVec.push_back("Run2018A");
  fileVec.push_back("Run2018B");
  fileVec.push_back("Run2018C");
  fileVec.push_back("Run2018D");
  fileVec.push_back("NNLO_inc");
  fileVec.push_back("NNLO_10to50");
  fileVec.push_back("NNLO_10to50_v2");
  fileVec.push_back("NNLO_100to200");
  fileVec.push_back("NNLO_200to400");
  fileVec.push_back("NNLO_400to500");
  fileVec.push_back("NNLO_500to700");
  fileVec.push_back("NNLO_700to800");
  fileVec.push_back("NNLO_800to1000");
  fileVec.push_back("NNLO_1000to1500");
  fileVec.push_back("NNLO_1500to2000");
  fileVec.push_back("NNLO_2000toInf");
  fileVec.push_back("NNLO_tautau");
  fileVec.push_back("ST_s");
  fileVec.push_back("ST_t_AntiTop");
  fileVec.push_back("ST_t_Top");
  fileVec.push_back("ST_tW_AntiTop");
  fileVec.push_back("ST_tW_Top");
  fileVec.push_back("TTTo2L2Nu");
  fileVec.push_back("WJetsToLNu");
  fileVec.push_back("WW");
  fileVec.push_back("WZ");
  fileVec.push_back("ZZ");

  std::map<std::string, int> fileMap = {};
  fileMap.insert(std::make_pair("NNLO_inc", 29));
  fileMap.insert(std::make_pair("NNLO_10to50", 1));
  fileMap.insert(std::make_pair("NNLO_10to50_v2", 4));
  fileMap.insert(std::make_pair("NNLO_100to200", 1));
  fileMap.insert(std::make_pair("NNLO_200to400", 1));
  fileMap.insert(std::make_pair("NNLO_400to500", 1));
  fileMap.insert(std::make_pair("NNLO_500to700", 1));
  fileMap.insert(std::make_pair("NNLO_700to800", 1));
  fileMap.insert(std::make_pair("NNLO_800to1000", 1));
  fileMap.insert(std::make_pair("NNLO_1000to1500", 1));
  fileMap.insert(std::make_pair("NNLO_1500to2000", 1));
  fileMap.insert(std::make_pair("NNLO_2000toInf", 1));
  fileMap.insert(std::make_pair("NNLO_tautau", 12));
  fileMap.insert(std::make_pair("ST_s", 2));
  fileMap.insert(std::make_pair("ST_t_AntiTop", 13));
  fileMap.insert(std::make_pair("ST_t_Top", 15));
  fileMap.insert(std::make_pair("ST_tW_AntiTop", 3));
  fileMap.insert(std::make_pair("ST_tW_Top", 6));
  fileMap.insert(std::make_pair("TTTo2L2Nu", 16));
  fileMap.insert(std::make_pair("WJetsToLNu", 6));
  fileMap.insert(std::make_pair("WW", 4));
  fileMap.insert(std::make_pair("WZ", 2));
  fileMap.insert(std::make_pair("ZZ", 1));
  fileMap.insert(std::make_pair("Run2018A", 10));
  fileMap.insert(std::make_pair("Run2018B", 6));
  fileMap.insert(std::make_pair("Run2018C", 7));
  fileMap.insert(std::make_pair("Run2018D", 21));


  std::fstream file_out;
  file_out.open("joblist.txt", std::ios_base::out);
  if (!file_out.is_open()) std::cout << "failed to open " << std::endl;


  for (int i = 0; i < fileVec.size(); i++) {
    int fileNum = fileMap.at(fileVec.at(i));
    std::cout << fileVec.at(i) << " " << fileNum << std::endl;

    for (int j = 1; j <= fileNum; j++) {

      file_out << "../../config/UL2018.yml 2018 " << fileVec.at(i) << " " << j << std::endl;

    }
  }

  file_out.close();

  // file_out <<  ../../config/UL2018.yml

  // arguments             = ../../install/bin/analysis --config $(config) --base ../../ --era $(era) --sample $(sample) --id $(id) --output ./
  // queue config,era,sample,id from joblist.txt


}
