#pragma once

#include "../HypernetModel/Hypernet.h"
#include "Model .h"

class GeneticAlgorithm {
private:
    int _seed = time(0);
    H _hypernet;
    Model _currentMinModel;
    std::vector<Model> _population;
public:
    GeneticAlgorithm() = default;

    GeneticAlgorithm(H &hypernet) : _hypernet(hypernet) {}

    std::vector<Model> GetInitialPopulation();

    Model ChooseParent();

    Model CrossOperator(Model &parent1, Model &parent2);

    Model MutationOperator(Model &model);

    std::vector<Model> GetNeighborhood(Model &model);

    Model LocalDescentAlgorithm(Model &model);

    std::shared_ptr<Model> GetMinModel();
};