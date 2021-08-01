#include "Model .h"
#include "../HypernetTest/ComputeMENC.h"
#include "../HypernetModel/Helpers/DataHelper.h"

Node Model::AddNode(Branch &branch) {
    // добавляем вершину
    Node newNode = Node::GetSimpleElement(GetUniqueId(_hypernet.GetNodes()), p, false,
                                          _hypernet.GetFN().front().GetC().size());
    _hypernet.GetNodes().push_back(newNode);
    // добавляем маршруты
    int firstNode = -1;
    std::vector<Route> newRoutes;
    for(auto &route : branch.GetRoutes()) {
        auto newVector = *route.Ptr;
        for (int i = 0; i < newVector.size() - 1; ++i) {
            if (H::IsIncident(newVector[i], branch) && H::IsIncident(newVector[i + 1], branch)) {
                firstNode = newVector[i];
                newVector.erase(newVector.begin() + i + 1, newVector.end());
                newVector.push_back(newNode.GetId());
                route.Ptr->erase(route.Ptr->begin(), route.Ptr->begin() + i + 1);
                route.Ptr->insert(route.Ptr->begin(), newNode.GetId());
                break;
            }
        }
        auto ptr = std::make_shared<std::vector<int>>(newVector);
        Route newRoute = Route(GetUniqueId(_hypernet.GetF()), ptr);
        // заменяем старый маршрут новым
        for(auto &item : _hypernet.GetFN()) {
            auto routeIt = std::find(item.GetRoutes().begin(), item.GetRoutes().end(), route);
            if (routeIt != item.GetRoutes().end()) {
                for (int i = 0; i < newVector.size() - 1; ++i) {
                    if (H::IsIncident(newVector[i], item) && H::IsIncident(newVector[i + 1], item)) {
                       *routeIt = newRoute;
                        continue;
                    }
                }
            }
        }
        newRoutes.push_back(newRoute);
        _hypernet.GetF().emplace_back(newRoute);
    }
    // добавляем ветвь
    if (firstNode < 0) {
        throw std::runtime_error("First node for new branch not found for model.");
    }
    Branch newBranch = Branch::GetSimpleElement(GetUniqueId(_hypernet.GetFN()), firstNode, newNode.GetId(),
                                                _hypernet.GetFN().front().GetC().size());
    if (branch.GetFirstNode() == firstNode) {
        branch.SetFirstNode(newNode.GetId());
    } else {
        branch.SetSecondNode(newNode.GetId());
    }
    newBranch.SetRoutes(newRoutes);
    _hypernet.GetFN().emplace_back(newBranch);

    return newNode;
}

bool Model::CheckConditions() {
    if (_solution.size() >= MAX_BRANCH_COUNT) {
        return false;
    }

    if (IS_NODES_RELIABLE == 1) {
        Branch branchSum = Branch::GetZero();
        ComputeMENC(branchSum, _hypernet);
        _reliability = branchSum.GetPolynomialValue(p);
        if (_reliability < MIN_MENC_VALUE) {
            return false;
        }
    } else {
        Node nodeSum = Node::GetZero();
        ComputeMENC(nodeSum, _hypernet);
        _reliability = nodeSum.GetPolynomialValue(p);
        if (_reliability < MIN_MENC_VALUE) {
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
    std::cout << "Solution:" << VectorToString(_solution) << std::endl;
    std::cout << "Reliability:" << _reliability << std::endl;
    std::cout << "ObjFunctionValue:" << _objFunctionValue << std::endl << std::endl;
}

void Model::PrintModel(const std::string& header) {
    std::cout << std::string(3, ' ') << header << ":" << std::endl;
    PrintModel();
}