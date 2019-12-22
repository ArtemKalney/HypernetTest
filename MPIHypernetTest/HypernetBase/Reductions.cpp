#include "Hypernet.h"
#include "Globals.h"

// Chain replacement by branch
void H::ChainReduction() {
    std::vector<int> forbiddenNodes;
    auto chain = GetSimpleChain(forbiddenNodes);
    if (chain.empty()) {
        return;
    }
    ChainsReduced++;
    Branch newBranch;
    if (chain.front().GetIsReliable()) {
        newBranch = Branch::GetUnity();
        newBranch.SetIsReliable(true);
    } else {
        newBranch = Branch::GetUnity();
        for (auto &item : chain) {
            newBranch = newBranch * item;
        }
    }
    newBranch.SetId(chain.front().GetId());
    auto nodesInChain = GetNodesInChain(chain);
    auto nodesFN = nodesInChain;
    for (int i = 1; i < nodesFN.size() - 1; i++) {
        int nodeNumber = nodesFN[i];
        RemoveNodeFN(nodeNumber);
        for (auto &item : nodesFN) {
            if (nodeNumber < item) {
                item--;
            }
        }
    }
    newBranch.SetFirstNode(nodesFN.front());
    newBranch.SetSecondNode(nodesFN.back());

    std::vector<Route> newBranchRoutes(chain.front().GetRoutes());
    auto nodesF = nodesInChain;
    for (int i = 1; i < nodesF.size() - 1; i++) {
        int node = nodesF[i];
        for (int j = 0; j < _F.size(); j++) {
            Route route = _F[j];
            auto ptr = route.Ptr;
            if (IsIncident(node, route)) {
                _F.erase(_F.begin() + j--);
                auto it = std::find_if(_F.begin(), _F.end(), [node](Route &route) ->
                        bool { return IsIncident(node, route); });
                if(it == _F.end()) {
                    throw "ChainReduction: no incident route";
                }
                auto ptrToInsert = it->Ptr;
                if (ptrToInsert->front() == node && ptr->front() == node) {
                    std::reverse(ptrToInsert->begin(), ptrToInsert->end());
                } else if (ptrToInsert->back() == node && ptr->back() == node) {
                    std::reverse(ptr->begin(), ptr->end());
                } else if (ptrToInsert->front() == node && ptr->back() == node) {
                    std::reverse(ptrToInsert->begin(), ptrToInsert->end());
                    std::reverse(ptr->begin(), ptr->end());
                }
                ptrToInsert->insert(ptrToInsert->end(), ptr->begin(), ptr->end());
                // удаляем вершину из обоих рёбер
                ptrToInsert->erase(std::remove(ptrToInsert->begin(), ptrToInsert->end(), node), ptrToInsert->end());
                // удаляем петли, так как они бессмысленны и приводят к ошибкам
                bool isLoop = ptrToInsert->front() == ptrToInsert->back();
                if (isLoop) {
                    ptr->clear();
                    ptrToInsert->clear();
                    _F.erase(it);
                }
                if (!isLoop) {
                    int idToInsert = _F[it - _F.begin()].Id;
                    // заменяем в FN ссылки соответсвующие ptr на новое ребро
                    for (auto &branch : _FN) {
                        for (auto &item : branch.GetRoutes()) {
                            if (item == route) {
                                item.Ptr = ptrToInsert;
                                item.Id = idToInsert;
                            }
                        }
                    }
                    it = std::find(newBranchRoutes.begin(), newBranchRoutes.end(), route);
                    if (it != newBranchRoutes.end()) {
                        it->Ptr = ptrToInsert;
                        it->Id = idToInsert;
                    }
                }
            } else if (IsSlightlyIncident(node, route)) {
                ptr->erase(std::remove(ptr->begin(), ptr->end(), node), ptr->end());
            }
        }
        // decrease indexes after delete
        for (auto &item : nodesF) {
            if (node < item) {
                item--;
            }
        }
        for (auto &route : _F) {
            for (auto &item : *route.Ptr) {
                if (node < item) {
                    item--;
                }
            }
        }
    }
    newBranch.SetRoutes(newBranchRoutes);

    _FN.push_back(newBranch);

    return ChainReduction();
}
// Removes the connectivity component, where both pivot nodes do not lie and returns true,
//if the pivot nodes in different components, then returns false
//if return true we return 0 in pairconnectivity
bool H::BridgeReduction() {
    for(auto &item : _nodes) {
        item.SetIsVisited(false);
    }
    DFS(0, _nodes, _FN);
    int visitedNodesCount = 0;
    for(auto &item : _nodes) {
        if (item.GetIsVisited()) {
            visitedNodesCount++;
        }
    }

    if (visitedNodesCount != _nodes.size()) {
        for (int i = 0; i < _nodes.size(); i++) {
            Node node = _nodes[i];
            if (!node.GetIsVisited()) {
                if (node.IsPivotNode()) {
                    throw "BridgeReduction: delete pivot node";
                }
                UnconnectedNodesReduced++;
                RemoveNode(i--);
            }
        }

        return !_nodes[1].GetIsVisited();
    } else {
        return false;
    }
}

void H::EdgeReduction() {
    auto SN = GetSN();
    for (auto &item : _nodes) {
        item.SetIsVisited(false);
    }
    DFS(0, _nodes, SN);
    auto canDeleteMask = GetCanDeleteMask(SN);
    for (int i = 0; i < canDeleteMask.size(); i++) {
        if (canDeleteMask[i]) {
            EdgesReduced++;
            Branch edge = SN[i];
            _F.erase(std::remove_if(_F.begin(), _F.end(), [edge](Route &route) -> bool {
                bool isRemoved = route.Id == edge.GetId();
                if (isRemoved) {
                    route.Ptr->clear();
                }
                return isRemoved;
            }), _F.end());
        }
    }

    RemoveEmptyBranches();
}

Branch H::ParallelReduction() {
    int size = _FN.size();
    _FN.erase(std::remove_if(_FN.begin(), _FN.end(), [](Branch &item) ->
            bool { return item.IsSimpleBranch(); }), _FN.end());
    int simpleBranchesCount = size - _FN.size();
    Branch parallelBranch = Branch::GetElement(Bin[simpleBranchesCount], simpleBranchesCount);
    parallelBranch.GetC()[simpleBranchesCount] = 0;
    Branch result = parallelBranch;
    for (auto &item : _FN) {
        result = item + result - item *result;
    }
    return result;
}

template<>
bool H::Reductions<Branch>(Branch &pseudoElement, Branch &returnValue) {
    if (ENABLE_BRIDGE_REDUCTION == 1 && BridgeReduction()) {
        returnValue = Branch::GetZero();
        return true;
    }
    if (ENABLE_SIMPLE_CASE == 1 && _nodes.size() < MAX_DIMENSIONAL) {
        returnValue = SimpleCase(pseudoElement);
        return true;
    }

    if (ENABLE_EDGE_REDUCTION == 1) {
        EdgeReduction();
        if (ENABLE_BRIDGE_REDUCTION == 1 && BridgeReduction()) {
            returnValue = Branch::GetZero();
            return true;
        }
        if (ENABLE_SIMPLE_CASE == 1 && _nodes.size() < MAX_DIMENSIONAL) {
            returnValue = SimpleCase(pseudoElement);
            return true;
        }
    }
    if (ENABLE_CHAIN_REDUCTION == 1) {
        ChainReduction();
        if (ENABLE_BRIDGE_REDUCTION == 1 && BridgeReduction()) {
            returnValue = Branch::GetZero();
            return true;
        }
        if (ENABLE_SIMPLE_CASE == 1 && _nodes.size() < MAX_DIMENSIONAL) {
            returnValue = SimpleCase(pseudoElement);
            return true;
        }
    }

    return false;
}

template<>
bool H::Reductions<Node>(Node &pseudoElement, Node &returnValue) {
    if (ENABLE_BRIDGE_REDUCTION == 1 && BridgeReduction()) {
        returnValue = Node::GetZero();
        return true;
    }
    if (ENABLE_SIMPLE_CASE == 1 && _nodes.size() < MAX_DIMENSIONAL) {
        returnValue = SimpleCase(pseudoElement);
        return true;
    }

    if (ENABLE_EDGE_REDUCTION == 1) {
        EdgeReduction();

        auto routesFn = GetRoutesFN();
        auto routesF = GetRoutesF();

        if (ENABLE_BRIDGE_REDUCTION == 1 && BridgeReduction()) {
            returnValue = Node::GetZero();
            return true;
        }
        if (ENABLE_SIMPLE_CASE == 1 && _nodes.size() < MAX_DIMENSIONAL) {
            returnValue = SimpleCase(pseudoElement);
            return true;
        }
    }
    return false;
}