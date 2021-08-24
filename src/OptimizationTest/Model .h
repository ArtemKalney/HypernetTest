#pragma once

#include "../HypernetModel/Hypernet.h"
#include "../HypernetModel/Helpers/DataHelper.h"

class Model {
private:
    H _hypernet;
    std::vector<Branch> _solution;
    int _objFunctionValue{};
    double _reliability{};
public:
    Model() = default;

    Model(H &hypernet, std::vector<Branch> &solution) :
            _hypernet(hypernet),
            _solution(std::move(solution))
    {
        std::vector<Node> newNodes;
        // добавляем b-nodes
        for(auto &item : _solution) {
            // находим нужную ветсь для нашей копии гиперсети
            newNodes.push_back(AddNode(*std::find(_hypernet.GetFN().begin(), _hypernet.GetFN().end(), item)));
        }
        // добавляем c-node
        Node newNode = Node::GetSimpleElement(GetUniqueId(_hypernet.GetNodes()), p, false,
                                              _hypernet.GetFN().front().GetC().size());
        _hypernet.GetNodes().push_back(newNode);
        for(auto &item : newNodes) {
            Branch newBranch = Branch::GetSimpleElement(GetUniqueId(_hypernet.GetFN()), newNode.GetId(), item.GetId(),
                                                        _hypernet.GetFN().front().GetC().size());
            std::vector<int> newVector {newNode.GetId(), item.GetId()};
            auto ptr = std::make_shared<std::vector<int>>(newVector);
            Route newRoute = Route(GetUniqueId(_hypernet.GetF()), ptr);
            newBranch.GetRoutes().push_back(newRoute);
            newBranch.SetIsReliable(true);
            _hypernet.GetFN().push_back(newBranch);
            _hypernet.GetF().emplace_back(newRoute);
        }
        _hypernet.RenumerateNodes(0, newNode.GetId());
        if (IS_DEBUG == 1) {
            _hypernet.LogHypernet();
        }
        if (!_hypernet.IsValidHypernet()) {
            throw std::runtime_error("Generated not valid hypernet in model");
        }
        _objFunctionValue = _solution.size();
    }

    double GetObjFunctionValue() const {
        return _objFunctionValue;
    }

    double GetReliability() const {
        return _reliability;
    }

    std::vector<Branch>& GetSolution() {
        return _solution;
    }

    bool CheckConditions();

    Node AddNode(Branch &branch);

    void PrintModel();

    void PrintModel(const std::string& header);

    static bool ModelCompare(const Model &a, const Model &b)
    {
        return a.GetObjFunctionValue() < b.GetObjFunctionValue();
    }
};

bool operator ==(Model &firstElement, Model &secondElement);
bool operator !=(Model &firstElement, Model &secondElement);