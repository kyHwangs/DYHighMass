#ifndef FACTORYMAKER_H
#define FACTORYMAKER_H

#include "Factory.h"

#include "yaml-cpp/yaml.h"

#include <memory>

std::unique_ptr<Factory> MakeFactory(const YAML::Node& fConfig);

#endif
