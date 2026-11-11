#include "JetProducer.h"
#include "EventContentPool.h"

#include <iostream>

JetProducer::JetProducer(const YAML::Node& fConfig) {

  fName = fConfig["Name"].as<std::string>();
  fOutputName = fConfig["OutputName"].as<std::string>();

  fPt = fConfig["Pt"].as<float>();
  fPUId = fConfig["PUId"].as<int>();
  fSrc = fConfig["src"].as<std::string>();
  fMCSrc = fConfig["MCsrc"].as<std::string>();

  fJetEnergyCorrectionFactory = std::make_unique<JetEnergyCorrectionFactory>(fConfig["Correction"]["JetEnergyCorrection"]);

  std::cout << "######################################################################" << std::endl;
  std::cout << " " << Name() << ": " << InName() << " -> " << OutputName() << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  std::cout << " fPt: " << fPt << std::endl;
  std::cout << " fPUId: " << fPUId << std::endl;
  std::cout << " fSrc: " << fSrc << std::endl;
  std::cout << " fMCSrc: " << fMCSrc << std::endl;
  
  std::cout << " fCorrectionFactories: " << std::endl;
  std::cout << "  - " << fJetEnergyCorrectionFactory->Name() << " " << fJetEnergyCorrectionFactory->InName() << std::endl;
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl; 
}

void JetProducer::Init(EventContentPool& pool) {

  fJetEnergyCorrectionFactory->Init(pool);
  fIsMC = pool.GetEventInfo().isMC;

  const auto& tSystematics = pool.GetSystematics();

  fCorrectionRegister.push_back(
    JetSystConfigs{
      "Nominal",
      fOutputName,
      "None",
      0,
      0
    }
  );

  if (fIsMC) {
    for (const auto& tSystematic : tSystematics) {
      if (tSystematic.first.find("JetEnergySmeringSyst") != std::string::npos) {
        fCorrectionRegister.push_back(
          JetSystConfigs{
            tSystematic.second["input"].as<std::string>(), 
            tSystematic.second["JetSrc"].as<std::string>() + "_1",
            "Smearing",
            0., 
            -1.
          }
        );
        fCorrectionRegister.push_back(
          JetSystConfigs{
            tSystematic.second["input"].as<std::string>(), 
            tSystematic.second["JetSrc"].as<std::string>() + "_2",
            "Smearing",
            0., 
            1.
          }
        );
      }

      if (tSystematic.first.find("JetEnergyScaleSyst") != std::string::npos) {
        fCorrectionRegister.push_back(
          JetSystConfigs{
            tSystematic.second["input"].as<std::string>(), 
            tSystematic.second["JetSrc"].as<std::string>() + "_1",
            "Scale",
            -1., 
            0.
          }
        );
        fCorrectionRegister.push_back(
          JetSystConfigs{
            tSystematic.second["input"].as<std::string>(), 
            tSystematic.second["JetSrc"].as<std::string>() + "_2",
            "Scale",
            1., 
            0.
          }
        );
      }
    }
  }

  std::cout << "######################################################################" << std::endl;
  std::cout << " Jet associated systematics:" << std::endl;
  std::cout << "----------------------------------------------------------------------" << std::endl;
  for (const auto& tCorrection : fCorrectionRegister) {
    std::string tSmearing = "";
    if (tCorrection.fSmearingSF == -1) tSmearing = " -1 dSmearing";
    if (tCorrection.fSmearingSF == 0) tSmearing = " No dSmearing";
    if (tCorrection.fSmearingSF == 1) tSmearing = " +1 dSmearing";

    std::string tScale = "";
    if (tCorrection.fScalingUnc == -1) tScale = " -1 dScale";
    if (tCorrection.fScalingUnc == 0) tScale = " No dScale";
    if (tCorrection.fScalingUnc == 1) tScale = " +1 dScale";

    std::cout << " - " << tCorrection.fSystName << " : " << tCorrection.fOutputName << " " << tSmearing << tScale << std::endl;
  }
  std::cout << "######################################################################" << std::endl;
  std::cout << " " << std::endl;
}

void JetProducer::Produce(EventContentPool& pool) {

  const auto& tRawJets = pool.Get<JET::RawJets>(fSrc);

  GEN::Jets tGenJets = {};
  if (fIsMC) tGenJets = pool.Get<GEN::Jets>(fMCSrc);

  const auto& tCorrectedJets = fJetEnergyCorrectionFactory->Apply(tRawJets, tGenJets, fCorrectionRegister);


  for (unsigned int i = 0; i < tCorrectedJets.size(); i++) {
    const JET::StdJets& tJetCollection = tCorrectedJets.at(i);
    std::string tOutputName = fCorrectionRegister.at(i).fOutputName;

    JET::StdJets tJets = {};
    JET::StdJets tBJets = {};
    JET::StdJets tHSJets = {};

    for (const auto& tJet : tJetCollection) {

      if (tJet.fVec.Pt() < fPt) continue;
      if ((tJet.fVec.Pt() < 50 && ((tJet.fJetPUId & 4) != 0)) || tJet.fVec.Pt() >= 50) {
        tJets.push_back(tJet);
        if (tJet.fIsBJet) tBJets.push_back(tJet);
      }

      if (tJet.fVec.Pt() < 50 && tJet.fGenJetIdx != -1) tHSJets.push_back(tJet);
    }

    pool.Put(tOutputName, JET::JetsCollection{tJets, tBJets, tHSJets, static_cast<int>(tJets.size()), static_cast<int>(tBJets.size())});
  }
}

