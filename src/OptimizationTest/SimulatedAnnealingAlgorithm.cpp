#include "SimulatedAnnealingAlgorithm.h"

std::shared_ptr<Model> SimulatedAnnealingAlgorithm::GetMinModel() {
    _t = AppSettings.TMax;
    SetInitialState();
    int count = 0;
    while (_t > AppSettings.TMin) {
        if (IS_DEBUG == 1 && count == MAX_ITERATION_COUNT) {
            return std::make_shared<Model>(_currentMinModel);
        }

        count++;
        srand(_seed++);
        auto solution = _currentMinModel.GetSolution();
        auto newModel = GenerateStateCandidate(solution);
        bool changed = false;
        if (newModel.GetObjFunctionValue() < _currentMinModel.GetObjFunctionValue()) {
            changed = true;
            _currentMinModel = newModel;
        } else {
            double p = exp(-((newModel.GetObjFunctionValue() - _currentMinModel.GetObjFunctionValue()) / _t));
            if ((double) rand() / RAND_MAX <= p) {
                changed = true;
                _currentMinModel = newModel;
            }
        }
        _t = AppSettings.TMax * 0.1 / count;
        if (IS_DEBUG == 1 && changed) {
            _currentMinModel.PrintModel("_currentMinModel");
        }
    }

    return std::make_shared<Model>(_currentMinModel);
}

void SimulatedAnnealingAlgorithm::SetInitialState() {
    std::vector<Branch> newSolution;
    auto vector = _hypernet.GetFN();
    while (newSolution.size() < AppSettings.InitialBranchCount) {
        auto it = std::max_element(vector.begin(), vector.end(),
                                    [](Branch &a, Branch &b) -> bool { return a.GetSaturation() <
                                            b.GetSaturation(); });
        newSolution.push_back(*it);
        vector.erase(it);
    }
    auto model = new Model(_hypernet, newSolution);
    _currentMinModel = *model;
}

// одноточечное изменение
Model SimulatedAnnealingAlgorithm::GenerateStateCandidate(std::vector<Branch>& solution) {
    srand(_seed++);
    auto changeCandidate = _hypernet.GetFN()[rand() % _hypernet.GetFN().size()];
    auto it = std::find(solution.begin(), solution.end(), changeCandidate);
    if (it == solution.end()) {
        solution.push_back(changeCandidate);
    } else {
        // не допускаем пустое решение
        if (solution.size() == 1) {
            return GenerateStateCandidate(solution);
        }

        solution.erase(it);
    }
    // отбрасываем проверенные варианты
    for(auto &item : _unacceptedSolutions) {
        if (VectorEqual(solution, item)) {
            return GenerateStateCandidate(solution);
        }
    }

    for(auto &item : _acceptedSolutions) {
        if (VectorEqual(solution, item)) {
            auto model = new Model(_hypernet, solution);

            return *model;
        }
    }

    auto model = new Model(_hypernet, solution);
    if (model->CheckConditions()) {
        CheckedConditions++;
        _acceptedSolutions.push_back(model->GetSolution());

        return *model;
    } else {
        UncheckedConditions++;
        _unacceptedSolutions.push_back(model->GetSolution());

        return GenerateStateCandidate(model->GetSolution());
    }
}