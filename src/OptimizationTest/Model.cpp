#include "Model .h"
#include "../HypernetTest/ComputeMENC.h"

Node Model::AddNode(Branch &branch) {
    // добавляем вершину
    Node newNode = Node::GetSimpleElement(_hypernet.GetNodes().size() + 1, p, false);
    _hypernet.GetNodes().push_back(newNode);
    // добавляем ветвь
    int secondNode = branch.GetSecondNode();
    branch.SetSecondNode(newNode.GetId());
    Branch newBranch = Branch::GetSimpleElement(_hypernet.GetFN().size() + 1, newNode.GetId(), secondNode);
    // добавляем маршруты
    for(auto &route : branch.GetRoutes()) {
        auto newVector = *route.Ptr;
        auto it = std::find(route.Ptr->begin(), route.Ptr->end(), secondNode);
        route.Ptr->erase(it, route.Ptr->end());
        newVector.erase(route.Ptr->begin(), it);
        if (std::find(route.Ptr->begin(), route.Ptr->end(), branch.GetFirstNode()) != route.Ptr->end()) {
            route.Ptr->push_back(newNode.GetId());
            newVector.insert(newVector.begin(), newNode.GetId());
        } else {
            route.Ptr->insert(route.Ptr->begin(), newNode.GetId());
            newVector.push_back(newNode.GetId());
        }
        auto ptr = std::make_shared<std::vector<int>>(newVector);
        Route newRoute = Route(_hypernet.GetF().size(), ptr);
        // заменяем старый маршрут новым
        for(auto &item : _hypernet.GetFN()) {
            for(auto &node : newVector) {
                auto branchIt = std::find(item.GetRoutes().begin(), item.GetRoutes().end(), node);
                if (branchIt != item.GetRoutes().end()) {
                    item.GetRoutes()[branchIt - item.GetRoutes().begin()] = newRoute;
                }
            }
        }
        newBranch.GetRoutes().push_back(newRoute);
        _hypernet.GetF().emplace_back(newRoute);
    }
    _hypernet.GetFN().push_back(newBranch);

    return newNode;
}

bool Model::CheckConditions() {
    if (_solution.size() > MAX_BRANCH_COUNT) {
        return false;
    }

    Branch branchSum = Branch::GetZero();
    Node nodeSum = Node::GetZero();
    if (IS_NODES_RELIABLE == 1) {
        ComputeMENC(branchSum, _hypernet);
        if (branchSum.GetPolynomialValue(p) < MIN_MENC_VALUE) {
            return false;
        }
    } else {
        ComputeMENC(nodeSum, _hypernet);
        if (nodeSum.GetPolynomialValue(p) < MIN_MENC_VALUE) {
            return false;
        }
    }

    return true;
}

bool operator ==(Model &firstElement, Model &secondElement) {
    auto firstSolution = firstElement.GetSolution();
    auto secondSolution = secondElement.GetSolution();
    for (int i = 0; i < firstSolution.size(); ++i) {
        if (firstSolution[i] == secondSolution[i]) {
            continue;
        } else {
            return false;
        }
    }

    return true;
}

bool operator !=(Model &firstElement, Model &secondElement) {
    if (firstElement == secondElement) {
        return false;
    }

    return true;
}

void Model::PrintModel() {
    std::cout << "Route:";
    PrintVector(_solution);
    std::cout << "ObjFunctionValue=" << GetObjFunctionValue() << std::endl;
}