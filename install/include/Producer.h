#ifndef PRODUCER_H
#define PRODUCER_H

#include <memory>
#include <string>
#include <vector>

class EventContentPool;

class Producer {
public:
  virtual ~Producer() = default;

  virtual const std::string& Name() const = 0;
  virtual void Init(EventContentPool& pool) = 0;
  virtual void Produce(EventContentPool& pool) = 0;
  virtual const std::string& InName() const = 0;
  virtual const std::string& OutputName() const = 0;
};

using ProducerList = std::vector<std::unique_ptr<Producer>>;

#endif
