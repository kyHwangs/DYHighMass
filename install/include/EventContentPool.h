#ifndef EVENTCONTENTPOOL_H
#define EVENTCONTENTPOOL_H

#include "NT.h"
#include "EventProducts.h"

#include "yaml-cpp/yaml.h"

#include <cxxabi.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

class EventContentPool {
public:
  EventContentPool() = default;

  explicit EventContentPool(std::unique_ptr<NT> source) {
    ResetSource(std::move(source));
  }

  void ResetSource(std::unique_ptr<NT> source) {
    if (!source) {
      throw std::invalid_argument(
          "EventContentPool requires a non-null NT source");
    }
    fSource = std::move(source);
  }

  template <typename T>
  void Put(std::string label, T product) {
    if (fProduct.find(label) != fProduct.end()) {
      throw std::runtime_error("Duplicate event product label '" + label +
                               "': " + Demangle(fProduct.find(label)->second->Type().name()));
    }

    fProduct.emplace(
        std::move(label),
        std::make_unique<Product<T>>(std::move(product)));
  }

  template <typename T>
  const T& Get(const std::string& label) const {
    const auto found = fProduct.find(label);
    if (found == fProduct.end()) {
      throw std::runtime_error("Missing event product label '" + label + "'");
    }
    if (found->second->Type() != std::type_index(typeid(T))) {
      throw std::runtime_error(
          "Wrong type requested for event product '" + label +
          "': requested " + Demangle(typeid(T).name()) + ", stored " +
          Demangle(found->second->Type().name()));
    }

    return static_cast<const Product<T>&>(*found->second).value;
  }

  template <typename T>
  bool Contains(const std::string& label) const {
    const auto found = fProduct.find(label);
    return found != fProduct.end() &&
           found->second->Type() == std::type_index(typeid(T));
  }

  void Clear() { fProduct.clear(); }

  const NT& Source() const {
    if (!fSource) {
      throw std::runtime_error("EventContentPool has no NT source");
    }
    return *fSource;
  }

  bool Next() {
    Clear();
    return fSource->GetNext();
  }

  void SetEventInfo(const EventInfo& fEventInfo_) {
    fEventInfo = fEventInfo_;
  }

  const EventInfo& GetEventInfo() const {
    
    return fEventInfo;
  }

  void SetTrigger(const std::vector<std::string>& fTriggers) {

    std::unordered_map<std::string, TTreeReaderValue<bool>*> fTriggerMaps = {
     {"HLT_Mu50", fSource->HLT_Mu50},
     {"HLT_TkMu50", fSource->HLT_TkMu50},
     {"HLT_TkMu100", fSource->HLT_TkMu100},
     {"HLT_OldMu100", fSource->HLT_OldMu100},
    };

    std::cout << "######################################################################" << std::endl;
    std::cout << " Trigger Requirement " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << " Requested: ";
    for (const auto& fTrigger : fTriggers)
      std::cout << fTrigger << " ";

    std::cout << std::endl;
  
    for (const auto& fTrigger : fTriggers) {
      auto found = fTriggerMaps.find(fTrigger);
      
      if (found == fTriggerMaps.end()) {
        throw std::runtime_error("Trigger " + fTrigger + " not found");
      }
      
      if (found->second != nullptr) {
        fTriggerVec.push_back(found->second);
        std::cout << " Reserved: " << fTrigger << std::endl;
      } else {
        std::cout << " Not Found, Skipping: " << fTrigger << " (nullptr)" << std::endl;
      }
    }
    
    if (fTriggerVec.empty()) {
      throw std::runtime_error("No requested trigger exists in this ntuple");
    }
    std::cout << "######################################################################" << std::endl;
    std::cout << std::endl;
  }

  bool PassingTrigger() const {
    for (auto* fTrigger : fTriggerVec) {
      if (**fTrigger == true) return true;
    }
    return false;
  }

  void SetNoiseFilter() {
    if (fEventInfo.era == "2016_preVFP" || fEventInfo.era == "2016_postVFP") {
    
      fNoiseFilterVec.push_back(fSource->Flag_goodVertices);
      fNoiseFilterVec.push_back(fSource->Flag_globalSuperTightHalo2016Filter);
      fNoiseFilterVec.push_back(fSource->Flag_HBHENoiseFilter);
      fNoiseFilterVec.push_back(fSource->Flag_HBHENoiseIsoFilter);
      fNoiseFilterVec.push_back(fSource->Flag_EcalDeadCellTriggerPrimitiveFilter);
      fNoiseFilterVec.push_back(fSource->Flag_BadPFMuonFilter);
      fNoiseFilterVec.push_back(fSource->Flag_BadPFMuonDzFilter);
      fNoiseFilterVec.push_back(fSource->Flag_hfNoisyHitsFilter);
      fNoiseFilterVec.push_back(fSource->Flag_eeBadScFilter);
      fNoiseFilterVec.push_back(fSource->Flag_ecalBadCalibFilter);

    } else if (fEventInfo.era == "2017" || fEventInfo.era == "2018") {
  
      fNoiseFilterVec.push_back(fSource->Flag_goodVertices);
      fNoiseFilterVec.push_back(fSource->Flag_globalSuperTightHalo2016Filter);
      fNoiseFilterVec.push_back(fSource->Flag_HBHENoiseFilter);
      fNoiseFilterVec.push_back(fSource->Flag_HBHENoiseIsoFilter);
      fNoiseFilterVec.push_back(fSource->Flag_EcalDeadCellTriggerPrimitiveFilter);
      fNoiseFilterVec.push_back(fSource->Flag_BadPFMuonFilter);
      fNoiseFilterVec.push_back(fSource->Flag_BadPFMuonDzFilter);
      fNoiseFilterVec.push_back(fSource->Flag_eeBadScFilter);
      fNoiseFilterVec.push_back(fSource->Flag_hfNoisyHitsFilter);
    } else {

      throw std::runtime_error("Noise filter not available for this era");
    }
  }

  bool PassingNoiseFilter() const {
    for (auto* fNoiseFilter : fNoiseFilterVec) {
      if (**fNoiseFilter == false) return false;
    }
    return true;
  }

  void RegisterSystematics(std::unordered_map<std::string, const YAML::Node> tSystematics) {
    fSystematics = std::move(tSystematics);
  }

  const std::unordered_map<std::string, const YAML::Node>& GetSystematics() const {
    return fSystematics;
  }

  void PrintPool() {
    std::cout << "######################################################################" << std::endl;
    std::cout << " Event Content Pool: Size = " << fProduct.size() << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    for (const auto& fProduct : fProduct) {
      std::cout << " " << fProduct.first << " : " << Demangle(fProduct.second->Type().name()) << std::endl;
    }
    std::cout << "######################################################################" << std::endl;
    std::cout << std::endl;
  }


private:
  static std::string Demangle(const char* fMangledName) {
    int tStatus = 0;
    std::unique_ptr<char, void (*)(void*)> tDemangled(
        abi::__cxa_demangle(fMangledName, nullptr, nullptr, &tStatus), std::free);
    return (tStatus == 0 && tDemangled) ? tDemangled.get() : fMangledName;
  }

  class ProductBase {
  public:
    virtual ~ProductBase() = default;
    virtual std::type_index Type() const = 0;
  };

  template <typename T>
  class Product final : public ProductBase {
  public:
    explicit Product(T product) : value(std::move(product)) {}

    std::type_index Type() const override {
      return std::type_index(typeid(T));
    }

    T value;
  };

  EventInfo fEventInfo;

  std::unordered_map<std::string, std::unique_ptr<ProductBase>> fProduct;
  std::unique_ptr<NT> fSource;

  std::unordered_map<std::string, const YAML::Node> fSystematics;

  std::vector<TTreeReaderValue<bool>*> fTriggerVec;
  std::vector<TTreeReaderValue<bool>*> fNoiseFilterVec;
};

#endif
