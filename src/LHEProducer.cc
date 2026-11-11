#include "LHEProducer.h"
#include "EventContentPool.h"
#include "EventProducts.h"
#include "NT.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

LHEProducer::LHEProducer(const YAML::Node& fConfig) {

  fName = fConfig["Name"].as<std::string>();
  fOutputName = fConfig["OutputName"].as<std::string>();

  fLeadingPt = fConfig["LeadingPt"].as<float>();
  fSubleadingPt = fConfig["SubleadingPt"].as<float>();
  fEta = fConfig["Eta"].as<float>();
  
  fPid = fConfig["Pid"].as<int>();

  std::cout << "######################################################################" << std::endl;
  std::cout << " " << Name() << ": " << InName() << " -> " << OutputName() << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " fLeadingPt: " << fLeadingPt << std::endl;
  std::cout << " fSubleadingPt: " << fSubleadingPt << std::endl;
  std::cout << " fEta: " << fEta << std::endl;
  std::cout << " fPid: " << fPid << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl; 
}

void LHEProducer::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;
  fSampleName = pool.GetEventInfo().sampleName;
}

void LHEProducer::Produce(EventContentPool& pool) {

  if (!fIsMC || fSampleName.find("NNLO_MUMU") == std::string::npos) return;

  const NT& nt = pool.Source();

  std::vector<TLorentzVector> returnVec = {};

  for (unsigned int i = 0; i < **nt.nLHEPart; i++) {

    if (std::abs(nt.LHEPart_pdgId->At(i)) == fPid) {

      TLorentzVector lvec;
      lvec.SetPtEtaPhiM(nt.LHEPart_pt->At(i), nt.LHEPart_eta->At(i), nt.LHEPart_phi->At(i), nt.LHEPart_mass->At(i));
      returnVec.push_back(lvec);
    }
  }

  std::sort(returnVec.begin(), returnVec.end(), [](const TLorentzVector& a, const TLorentzVector& b) {
    return a.Pt() > b.Pt();
  });

  if (returnVec.size() < 2)
    pool.Put<GEN::LHEContent>(fOutputName, GEN::LHEContent{TLorentzVector(), TLorentzVector(), 0, -9999});
  else 
    pool.Put<GEN::LHEContent>(fOutputName, GEN::LHEContent{returnVec[0], returnVec[1], static_cast<int>(returnVec.size()), (returnVec[0] + returnVec[1]).M()});
}

