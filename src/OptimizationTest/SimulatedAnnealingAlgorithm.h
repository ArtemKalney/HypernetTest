#pragma once

#include "../HypernetModel/Hypernet.h"
#include "Model .h"

class SimulatedAnnealingAlgorithm {
private:
    H _hypernet;
    double _t;
    int _seed = time(0);
    Model _currentMinModel;
public:
    SimulatedAnnealingAlgorithm() = default;

    SimulatedAnnealingAlgorithm(H& hypernet) : _hypernet(hypernet) {}

    std::shared_ptr<Model> GetMinModel();

    void SetInitialState();

    Model GenerateStateCandidate(std::vector<Model>& neighborhood);

    Model GenerateStateCandidate(std::vector<Branch>& solution, std::vector<std::vector<Branch>>& checkedSolutions);

    std::vector<Model> GetNeighborhood(Model &model);
};