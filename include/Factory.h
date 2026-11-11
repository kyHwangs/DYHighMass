#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include <string>
#include <unordered_map>

class EventContentPool;

class Factory {
public:
  virtual ~Factory() = default;

  virtual const std::string& Name() const = 0;
  virtual void Init(EventContentPool& pool) = 0;

  virtual const std::string& InName() const = 0;
};

using FactoryList = std::unordered_map<std::string, std::unique_ptr<Factory>>;

#endif
