#pragma once

#include "../HypernetModel/Hypernet.h"
#include "Model .h"

class FullEnumerationAlgorithm {
private:
    H _hypernet;
public:
    FullEnumerationAlgorithm() = default;

    FullEnumerationAlgorithm(H &hypernet) : _hypernet(hypernet) {}

    std::shared_ptr<Model> GetMinModel();
};