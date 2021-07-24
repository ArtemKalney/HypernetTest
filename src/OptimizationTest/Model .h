#pragma once

#include "../HypernetModel/Hypernet.h"

class Model {
private:
    H _hypernet;
    std::vector<Branch> _solution;
    int _objFunctionValue;
    bool _isConditionsChecked;
public:
    Model() = default;

    Model(H &hypernet, std::vector<Branch> &solution) :
            _hypernet(hypernet),
            _solution(solution),
            _objFunctionValue(0)
    {
        std::vector<Node> newNodes;
        // добавляем b-nodes
        for(auto &item : solution) {
            newNodes.push_back(AddNode(item));
        }
        // добавляем c-node
        Node newNode = Node::GetSimpleElement(_hypernet.GetNodes().size() + 1, p, false);
        _hypernet.GetNodes().push_back(newNode);
        for(auto &item : newNodes) {
            Branch newBranch = Branch::GetSimpleElement(_hypernet.GetFN().size() + 1, newNode.GetId(), item.GetId());
            std::vector<int> newVector {newNode.GetId(), item.GetId()};
            auto ptr = std::make_shared<std::vector<int>>(newVector);
            Route newRoute = Route(_hypernet.GetF().size(), ptr);
            newBranch.GetRoutes().push_back(newRoute);
            _hypernet.GetFN().push_back(newBranch);
        }
        _objFunctionValue = solution.size();
        _isConditionsChecked = CheckConditions();
    }

    double GetObjFunctionValue() const {
        return _objFunctionValue;
    }

    std::vector<Branch>& GetSolution() {
        return _solution;
    }

    bool GetIsConditionsChecked() const {
        return _isConditionsChecked;
    }

    bool CheckConditions();

    Node AddNode(Branch &branch);

    void PrintModel();

    static bool ModelCompare(const Model &a, const Model &b)
    {
        return a.GetObjFunctionValue() < b.GetObjFunctionValue();
    }
};

bool operator ==(Model &firstElement, Model &secondElement);
bool operator !=(Model &firstElement, Model &secondElement);