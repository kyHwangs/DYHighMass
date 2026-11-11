#ifndef PRODUCERMAKER_H
#define PRODUCERMAKER_H

#include "Producer.h"

#include "yaml-cpp/yaml.h"

#include <memory>

std::unique_ptr<Producer> MakeProducer(const YAML::Node& fConfig);

#endif
