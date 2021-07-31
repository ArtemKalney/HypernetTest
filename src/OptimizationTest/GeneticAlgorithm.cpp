#include "GeneticAlgorithm.h"
#include "../HypernetModel/Helpers/DataHelper.h"

std::shared_ptr<Model> GeneticAlgorithm::GetMinModel() {
    _population = GetInitialPopulation();
    if (_population.empty()) {
        throw std::runtime_error("There ara no models satisfying the task conditions. Please check task conditions.");
    }
    auto minModelIt = std::min_element(_population.begin(), _population.end(), Model::ModelCompare);
    _currentMinModel = *minModelIt;
    if (IS_DEBUG == 1) {
        std::cout << "_currentMinModel:" << VectorToString(_currentMinModel.GetSolution()) << std::endl;
        std::cout << "ObjFunctionValue=" << _currentMinModel.GetObjFunctionValue() << std::endl << std::endl;
    }
    while (_currentMinModel.GetObjFunctionValue() > REQUIRED_ROUTE_DISTANCE) {
        GeneticAlgorithmIterations++;
        auto parent1Model = ChooseParent();
        if (IS_DEBUG == 1) {
            std::cout << "parent1Model:" << std::string(4, ' ') << VectorToString(parent1Model.GetSolution()) << std::endl;
        }
        auto parent2Model = ChooseParent();
        if (IS_DEBUG == 1 && parent1Model != parent2Model) {
            std::cout << "parent2Model:" << std::string(4, ' ') << VectorToString(parent2Model.GetSolution()) << std::endl;
        }
        while (parent1Model == parent2Model) {
            parent2Model = ChooseParent();
            if (IS_DEBUG == 1 && parent1Model != parent2Model) {
                std::cout << "parent2Model:" << std::string(4, ' ') << VectorToString(parent2Model.GetSolution()) << std::endl;
            }
        }
        auto crossedModel = CrossOperator(parent1Model, parent2Model);
        if (IS_DEBUG == 1) {
            // todo возможно надо модифицировать чтобы получать удовлетворяющие услвиям модели, либо не проверять модели вовсе
            if (!crossedModel.GetIsConditionsChecked()) {
                std::cout << "IsCheckConditions=false" << std::endl;
            }
            std::cout << "crossedModel:" << std::string(4, ' ') << VectorToString(crossedModel.GetSolution()) << std::endl;
        }
        auto mutatedModel = MutationOperator(crossedModel);
        if (IS_DEBUG == 1 && mutatedModel != crossedModel) {
            if (!crossedModel.GetIsConditionsChecked()) {
                std::cout << "IsCheckConditions=false" << std::endl;
            }
            std::cout << "mutatedModel:" << std::string(4, ' ') << VectorToString(mutatedModel.GetSolution()) << std::endl;
        }
        auto newMinModel = LocalDescentAlgorithm(mutatedModel);
        if (newMinModel.GetObjFunctionValue() < _currentMinModel.GetObjFunctionValue()) {
            _currentMinModel = newMinModel;
            if (IS_DEBUG == 1) {
                std::cout << "_currentMinModel:" << VectorToString(newMinModel.GetSolution()) << std::endl;
                std::cout << "ObjFunctionValue=" << _currentMinModel.GetObjFunctionValue() << std::endl << std::endl;
            }
        }
        auto maxModelIt = std::max_element(_population.begin(), _population.end(), Model::ModelCompare);
        _population.erase(maxModelIt);
        _population.push_back(newMinModel);
    }

    return std::make_shared<Model>(_currentMinModel);
}

//Выбираем случайно из всех возможных комбинаций
std::vector<Model> GeneticAlgorithm::GetInitialPopulation() {
    std::vector<std::vector<Branch>> combinations;
    std::vector<Branch> vector;
    if (USE_MAX_POPULATION_CAPACITY == 1) {
        ComputeCombinations(_hypernet.GetFN(), combinations, vector, 0, MAX_BRANCH_COUNT);
    } else {
        for (int i = 1; i < MAX_BRANCH_COUNT; ++i) {
            vector.clear();
            ComputeCombinations(_hypernet.GetFN(), combinations, vector, 0, i);
        }
    }
    std::vector<Model> models;
    for(auto &solution : combinations) {
        auto model = new Model(_hypernet, solution);
        auto isConditionsChecked = model->GetIsConditionsChecked();
        if (isConditionsChecked && models.size() < MAX_INITIAL_POPULATION_COUNT) {
            models.push_back(*model);
        }
    }

    return models;
}

//Турнирная селекция
Model GeneticAlgorithm::ChooseParent() {
    srand(_seed++);
    std::vector<Model> vector;
    for(auto &item : _population) {
        // p=1/2
        if (rand() % 2) {
            vector.push_back(item);
        }
    }
    // если вдруг не выбрали ничего то берём первого из полпуляции
    if (vector.empty()) {
        return _population.front();
    }

    auto minModelIt = std::min_element(vector.begin(), vector.end(), Model::ModelCompare);

    return *minModelIt;
}

// Одноточечный оператор скрещивания
Model GeneticAlgorithm::CrossOperator(Model &parent1, Model &parent2) {
    srand(_seed++);
    auto position = rand() % _hypernet.GetFN().size();
    std::vector<Branch> vector;
//    for(auto &item : parent1.GetSolution()) {
//        if (item.GetPosition() <= position) {
//            vector.push_back(item);
//        }
//    }
//    for(auto &item : parent2.GetSolution()) {
//        if (item.GetPosition() > position) {
//            vector.push_back(item);
//        }
//    }
    auto model = new Model(_hypernet, vector);

    return *model;
}

//Вероятностный оператор мутации
Model GeneticAlgorithm::MutationOperator(Model &model) {
    srand(_seed++);
    auto mutatedSolution = model.GetSolution();
    for(auto &item : _hypernet.GetFN()) {
        // p=1/_data.Bins.size()
        if (rand() % _hypernet.GetFN().size() == 0) {
            auto it = std::find(mutatedSolution.begin(), mutatedSolution.end(), item);
            if (it != mutatedSolution.end()) {
                mutatedSolution.erase(it);
            } else {
                mutatedSolution.push_back(item);
            }
        }
    }
    auto mutatedModel = new Model(_hypernet, mutatedSolution);

    return *mutatedModel;
}

std::vector<Model> GeneticAlgorithm::GetNeighborhood(Model &model) {
    std::vector<Model> neighborhood;
    for (int i = 1; i <= LOCAL_DISTANCE; ++i) {
        std::vector<std::vector<Branch>> combinations;
        std::vector<Branch> vector;
        ComputeCombinations(model.GetSolution(), combinations, vector, 0, model.GetSolution().size() - i);
        for(auto &item : combinations) {
            auto neighborModel = new Model(_hypernet, item);
            if (neighborModel->GetIsConditionsChecked()) {
                neighborhood.push_back(*neighborModel);
            }
        }
        vector.clear();
        combinations.clear();
        ComputeCombinations(_hypernet.GetFN(), combinations, vector, 0, i);
        for(auto &combination : combinations) {
            auto solution = model.GetSolution();
            for(auto &item : combination) {
                if (std::find(solution.begin(), solution.end(), item) == solution.end()) {
                    solution.push_back(item);
                }
            }
            if (solution.size() > model.GetSolution().size()) {
                auto neighborModel = new Model(_hypernet, solution);
                if (neighborModel->GetIsConditionsChecked()) {
                    neighborhood.push_back(*neighborModel);
                }
            }
        }
    }

    return neighborhood;
}

Model GeneticAlgorithm::LocalDescentAlgorithm(Model &model) {
    auto neighborhood = GetNeighborhood(model);
    if (neighborhood.empty()) {
        throw std::runtime_error("Empty Neighborhood for model " + VectorToString(model.GetSolution()));
    }
    auto minModelIt = std::min_element(neighborhood.begin(), neighborhood.end(), Model::ModelCompare);
    if (minModelIt->GetObjFunctionValue() < model.GetObjFunctionValue()) {
        return LocalDescentAlgorithm(*minModelIt);
    }// Если ранее получили модель не удовлетворяющую условиям, то продолжаем поиск
    else if (!model.GetIsConditionsChecked()) {
        return LocalDescentAlgorithm(*minModelIt);
    }

    return model;
}