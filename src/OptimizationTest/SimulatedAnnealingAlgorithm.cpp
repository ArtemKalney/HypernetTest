#include "SimulatedAnnealingAlgorithm.h"

std::shared_ptr<Model> SimulatedAnnealingAlgorithm::GetMinModel() {
    _t = T_MAX;
    SetInitialState();
    int count = 0;
    while (_t > T_MIN) {
        if (IS_DEBUG == 1 && count > MAX_ITERATION_COUNT) {
            return std::make_shared<Model>(_currentMinModel);
        }

        count++;
        srand(_seed++);
        auto neighborhood = GetNeighborhood(_currentMinModel);
        auto newModel = GenerateStateCandidate(neighborhood);
        if (newModel.GetObjFunctionValue() < _currentMinModel.GetObjFunctionValue()) {

            _currentMinModel = newModel;
        } else {
            double p = exp(-((newModel.GetObjFunctionValue() - _currentMinModel.GetObjFunctionValue()) / _t));
            if ((double) rand() / RAND_MAX <= p) {
                _currentMinModel = newModel;
            }
        }
        _t = T_MAX * 0.1 / count;
        if (IS_DEBUG == 1) {
            _currentMinModel.PrintModel("_currentMinModel");
        }
    }

    return std::make_shared<Model>(_currentMinModel);
}

void SimulatedAnnealingAlgorithm::SetInitialState() {
    std::vector<Branch> newSolution;
    auto vector = _hypernet.GetFN();
    while (newSolution.size() < MAX_BRANCH_COUNT - 1) {
        auto it = std::max_element(vector.begin(), vector.end(),
                                    [](Branch &a, Branch &b) -> bool { return a.GetBranchSaturation() < b.GetBranchSaturation(); });
        newSolution.push_back(*it);
        vector.erase(it);
    }
    auto model = new Model(_hypernet, newSolution);
    if (model->CheckConditions()) {
        _currentMinModel = *model;
    } else {
        throw std::runtime_error("Wrong initial state.");
    }
}

// одноточеччное случайное изменение
Model SimulatedAnnealingAlgorithm::GenerateStateCandidate(std::vector<Model>& neighborhood) {
    srand(_seed++);
    auto index = rand() % neighborhood.size();
    auto changeCandidate = neighborhood[index];
    if (changeCandidate.CheckConditions()) {
        return changeCandidate;
    } else {
        neighborhood.erase(neighborhood.begin() + index);

        return GenerateStateCandidate(neighborhood);
    }
}

std::vector<Model> SimulatedAnnealingAlgorithm::GetNeighborhood(Model& model) {
    std::vector<Model> neighborhood;
    std::vector<std::vector<Branch>> combinations;
    std::vector<Branch> vector;
    // та же окрестность
    ComputeCombinations(_hypernet.GetFN(), combinations, vector, 0, model.GetSolution().size());
    for(auto &item : combinations) {
        if (!std::equal(model.GetSolution().begin(), model.GetSolution().end(), item.begin())) {
            auto neighborModel = new Model(_hypernet, item);
            neighborhood.push_back(*neighborModel);
        }
    }
    for (int i = 1; i <= SIMULATED_ANNEALING_LOCAL_DISTANCE; ++i) {
        // окрестность поменьше
        vector.clear();
        combinations.clear();
        if (model.GetSolution().size() - i > 0) {
            ComputeCombinations(model.GetSolution(), combinations, vector, 0, model.GetSolution().size() - i);
            for(auto &item : combinations) {
                auto neighborModel = new Model(_hypernet, item);
                neighborhood.push_back(*neighborModel);
            }
        }
        // окрестность побольше
        vector.clear();
        combinations.clear();
        // сразу проверяем максимальный размер
        if (model.GetSolution().size() + i > MAX_BRANCH_COUNT - 1) {
            continue;
        }

        ComputeCombinations(_hypernet.GetFN(), combinations, vector, 0, i);
        for(auto &combination : combinations) {
            auto newSolution = model.GetSolution();
            for(auto &item : combination) {
                if (std::find(newSolution.begin(), newSolution.end(), item) == newSolution.end()) {
                    newSolution.push_back(item);
                }
            }
            if (newSolution.size() > model.GetSolution().size()) {
                auto neighborModel = new Model(_hypernet, newSolution);
                neighborhood.push_back(*neighborModel);
            }
        }
    }

    return neighborhood;
}