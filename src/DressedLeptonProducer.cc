#include "DressedLeptonProducer.h"
#include "EventContentPool.h"
#include "EventProducts.h"
#include "NT.h"

#include <cmath>
#include <iostream>
#include <vector>

DressedLeptonProducer::DressedLeptonProducer(const YAML::Node& fConfig) {

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

void DressedLeptonProducer::Init(EventContentPool& pool) {

  fIsMC = pool.GetEventInfo().isMC;
  fSampleName = pool.GetEventInfo().sampleName;
}

void DressedLeptonProducer::Produce(EventContentPool& pool) {

  if (!fIsMC || fSampleName.find("NNLO_MUMU") == std::string::npos) return;

  const auto& nt = pool.Source();

  std::vector<StoredDressedLepton> tDressedLeptonCollection = {};

  for (unsigned int iGenLep = 0; iGenLep < **nt.nGenDressedLepton; iGenLep++) {

    if (std::abs(nt.GenDressedLepton_pdgId->At(iGenLep)) != fPid) continue;
    if (std::abs(nt.GenDressedLepton_eta->At(iGenLep)) > fEta) continue;
    if (nt.GenDressedLepton_pt->At(iGenLep) < fSubleadingPt) continue;

    TLorentzVector fVec;
    fVec.SetPtEtaPhiM(
      nt.GenDressedLepton_pt->At(iGenLep), 
      nt.GenDressedLepton_eta->At(iGenLep), 
      nt.GenDressedLepton_phi->At(iGenLep), 
      nt.GenDressedLepton_mass->At(iGenLep)
    );

    int fCharge = nt.GenDressedLepton_pdgId->At(iGenLep) > 0 ? -1 : 1;

    tDressedLeptonCollection.push_back(StoredDressedLepton{fVec, fCharge});
  }

  bool find = false;
  for (unsigned int i = 0; i < tDressedLeptonCollection.size(); i++) {
    for (unsigned int j = i + 1; j < tDressedLeptonCollection.size(); j++) {
      const auto& tLep1 = tDressedLeptonCollection[i];
      const auto& tLep2 = tDressedLeptonCollection[j];
      
      if (
        !(
          (tLep1.fVec.Pt() > fLeadingPt && tLep2.fVec.Pt() > fSubleadingPt) ||
          (tLep1.fVec.Pt() > fSubleadingPt && tLep2.fVec.Pt() > fLeadingPt)
        )
      ) continue;
    
      if (tLep1.fCharge * tLep2.fCharge > 0) continue;

      find = true;
      if (find) pool.Put(fOutputName, GEN::DressedMuon{tLep1.fVec, tLep2.fVec, 2, (tLep1.fVec + tLep2.fVec).M()});
      if (find) break;
    }
    if (find) break;
  }

  if (!find) pool.Put(fOutputName, GEN::DressedMuon{TLorentzVector(), TLorentzVector(), 0, -999});
}

