#include "FullEnumerationAlgorithm.h"
#include "../HypernetModel/Helpers/DataHelper.h"

std::shared_ptr<Model> FullEnumerationAlgorithm::GetMinModel() {
    std::vector<std::vector<Branch>> combinations;
    std::vector<Branch> vector;
    for (int i = 1; i < _hypernet.GetFN().size(); ++i) {
        vector.clear();
        ComputeCombinations(_hypernet.GetFN(), combinations, vector, 0, i);
    }
    std::vector<Model> models;
    for(auto &solution : combinations) {
        auto model = new Model(_hypernet, solution);
        if (model->CheckConditions()) {
            CheckedConditions++;
            models.push_back(*model);
        } else {
            UncheckedConditions++;
        }
        if (IS_DEBUG == 1) {
            model->PrintModel();
        }
    }
    if (models.empty()) {
        throw std::runtime_error("There ara no models satisfying the task conditions. Please check model conditions.");
    }
    auto minModelIt = std::min_element(models.begin(), models.end(), Model::ModelCompare);

    return std::make_shared<Model>(*minModelIt);
}