#ifndef EFFICIENCYFACTORYMAKER_H
#define EFFICIENCYFACTORYMAKER_H

#include "EfficiencyFactory.h"

#include "yaml-cpp/yaml.h"

#include <memory>

std::unique_ptr<EfficiencyFactory> MakeEfficiencyFactory(const YAML::Node& fConfig);

#endif
