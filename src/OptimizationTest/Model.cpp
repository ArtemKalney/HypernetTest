#include "Model .h"
#include "../HypernetTest/ComputeMENC.h"
#include "../HypernetModel/Helpers/DataHelper.h"

Node Model::AddNode(Branch &branch) {
    // добавляем вершину
    Node newNode = Node::GetSimpleElement(GetUniqueId(_hypernet.GetNodes()), p, false,
                                          _hypernet.GetFN().front().GetC().size());
    _hypernet.GetNodes().push_back(newNode);
    // добавляем маршруты
    std::vector<Route> newRoutes;
    for(auto &route : branch.GetRoutes()) {
        auto newVector = *route.Ptr;
        for (int i = 0; i < newVector.size() - 1; ++i) {
            if (H::IsIncident(newVector[i], branch) && H::IsIncident(newVector[i + 1], branch)) {
                if (newVector[i] == branch.GetSecondNode()) {
                    newVector.erase(newVector.begin() + i + 1, newVector.end());
                    newVector.push_back(newNode.GetId());
                    route.Ptr->erase(route.Ptr->begin(), route.Ptr->begin() + i + 1);
                    route.Ptr->insert(route.Ptr->begin(), newNode.GetId());
                } else {
                    route.Ptr->erase(route.Ptr->begin() + i + 1, route.Ptr->end());
                    route.Ptr->push_back(newNode.GetId());
                    newVector.erase(newVector.begin(), newVector.begin() + i + 1);
                    newVector.insert(newVector.begin(), newNode.GetId());
                }

                break;
            }
        }
        auto ptr = std::make_shared<std::vector<int>>(newVector);
        Route newRoute = Route(GetUniqueId(_hypernet.GetF()), ptr);
        // заменяем старый маршрут новым
        for(auto &item : _hypernet.GetFN()) {
            auto routeIt = std::find(item.GetRoutes().begin(), item.GetRoutes().end(), route);
            if (routeIt != item.GetRoutes().end() && H::IsSlightlyIncident(item, newRoute)) {
                *routeIt = newRoute;

                continue;
            }
        }
        newRoutes.push_back(newRoute);
        _hypernet.GetF().emplace_back(newRoute);
    }
    // добавляем ветвь
    Branch newBranch = Branch::GetSimpleElement(GetUniqueId(_hypernet.GetFN()), newNode.GetId(), branch.GetSecondNode(),
                                                _hypernet.GetFN().front().GetC().size());
    branch.SetSecondNode(newNode.GetId());
    newBranch.SetRoutes(newRoutes);
    _hypernet.GetFN().emplace_back(newBranch);

    return newNode;
}

bool Model::CheckConditions() {
    bool isReliable;
    if (IS_NODES_RELIABLE == 1) {
        Branch branchSum = Branch::GetZero();
        isReliable = ComputeMENC(branchSum, _hypernet, MIN_MENC_VALUE + _reliabilityDelta);
        _reliability = branchSum.GetPolynomialValue(p) - _reliabilityDelta;
    } else {
        Node nodeSum = Node::GetZero();
        isReliable = ComputeMENC(nodeSum, _hypernet, MIN_MENC_VALUE + _reliabilityDelta);
        _reliability = nodeSum.GetPolynomialValue(p) - _reliabilityDelta;
    }

    return isReliable;
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