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
bool H::BridgeReduction() {
    for(auto &item : _nodes) {
        item.IsVisited = false;
    }
    DFS(0, _nodes, _FN);
    int visitedNodesCount = 0;
    for(auto &item : _nodes) {
        if (item.IsVisited) {
            visitedNodesCount++;
        }
    }

    if (visitedNodesCount != _nodes.size()) {
        for (int i = 0; i < _nodes.size(); i++) {
            Node node = _nodes[i];
            if (!node.IsVisited) {
                UnconnectedNodesReduced++;
                RemoveNode(i--);
            }
        }

        return !_nodes[1].IsVisited;
    } else {
        return false;
    }
}

void H::EdgeReduction() {
    auto SN = GetSN();
    for (auto &item : _nodes) {
        item.IsVisited = false;
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