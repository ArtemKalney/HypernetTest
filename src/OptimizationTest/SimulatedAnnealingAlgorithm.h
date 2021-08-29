#pragma once

#include "../HypernetModel/Hypernet.h"
#include "Model .h"

class SimulatedAnnealingAlgorithm {
private:
    H _hypernet;
    double _t{};
    int _seed = time(0);
    Model _currentMinModel;
    std::vector<std::vector<Branch>> _acceptedSolutions;
    std::vector<std::vector<Branch>> _unacceptedSolutions;
public:
    SimulatedAnnealingAlgorithm() = default;

    SimulatedAnnealingAlgorithm(H& hypernet) : _hypernet(hypernet) {}

    std::shared_ptr<Model> GetMinModel();

    void SetInitialState();

    Model GenerateStateCandidate(std::vector<Model>& neighborhood);

    Model GenerateStateCandidate(std::vector<Branch>& solution);

    std::vector<Model> GetNeighborhood(Model &model);
};