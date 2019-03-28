#include "Hypernet.h"
#include "Globals.h"

std::vector<int> GetNodesInChain(H &H, const std::vector<Branch>& chain) {
    std::vector<int> nodesInChain;
    for(int i=0; i<chain.size() - 1; i++) {
        Branch currentBranch = chain[i], nextBranch = chain[i + 1];
        int commonNode;
        if (currentBranch.GetFirstNode() == nextBranch.GetFirstNode()) {
            commonNode = currentBranch.GetFirstNode();
        } else if (currentBranch.GetFirstNode() == nextBranch.GetSecondNode()) {
            commonNode = currentBranch.GetFirstNode();
        } else if (currentBranch.GetSecondNode() == nextBranch.GetFirstNode()) {
            commonNode = currentBranch.GetSecondNode();
        } else if (currentBranch.GetSecondNode() == nextBranch.GetSecondNode()) {
            commonNode = currentBranch.GetSecondNode();
        } else {
            throw "GetNodesInChain: no commonNode ";
        }
        if (currentBranch == chain.front()) {
            int firstNode = commonNode == currentBranch.GetFirstNode() ? currentBranch.GetSecondNode() :
                            currentBranch.GetFirstNode();
            nodesInChain.push_back(firstNode);
        }
        nodesInChain.push_back(commonNode);
        if (nextBranch == chain.back()) {
            int lastNode = commonNode == nextBranch.GetFirstNode() ? nextBranch.GetSecondNode() :
                           nextBranch.GetFirstNode();
            nodesInChain.push_back(lastNode);
        }
    }

    return nodesInChain;
}

void RemovePenduntRoutesInChain(H &hypernet, std::vector<int> &nodesInChain, std::vector<int> &nodePowers) {
    auto nodesIt = std::find_if(nodesInChain.begin() + 1, nodesInChain.end() - 1, [nodePowers](int &item) -> bool {
        return nodePowers[item] == 1;
    });
    if (nodesIt != nodesInChain.end() - 1) {
        auto it = std::find_if(hypernet.GetF().begin(), hypernet.GetF().end(), [nodesIt](Route &item) -> bool {
            return H::IsIncident(*nodesIt, item);
        });
        if (it == hypernet.GetF().end()) {
            throw "RemovePenduntRoutesInChain: no incident route";
        }

        auto ptr = it->Ptr;
        int firstNode = ptr->front(), secondNode = ptr->back();
        nodePowers[firstNode]--;
        nodePowers[secondNode]--;
        ptr->clear();
        hypernet.GetF().erase(it);

        RemovePenduntRoutesInChain(hypernet, nodesInChain, nodePowers);
    }
}
//todo более эффективно организовать проверку на корректность
bool IsSimpleChain(H &hypernet, std::vector<Branch> &chain, std::vector<int> &nodesInChain) {
    bool isCorrect = true;
    for (auto &branch : chain) {
        auto copy = hypernet;
        std::vector<Branch> copyChain;
        for (auto &item : chain) {
            auto it = std::find(copy.GetFN().begin(), copy.GetFN().end(), item);
            copyChain.push_back(*it);
        }
        copy.RemoveBranch(branch);
        auto nodePowers = copy.GetNodePowers(copy.GetSN(), copy.GetNodes().size());
        RemovePenduntRoutesInChain(copy, nodesInChain, nodePowers);

        for (auto &item : copyChain) {
            if (item.GetBranchSaturation() != 0) {
                isCorrect = false;
                break;
            }
        }
        if (!isCorrect) {
            break;
        }
    }

    bool canHandle = true;
    auto SN = hypernet.GetSN();
    auto SNnodepowers = hypernet.GetNodePowers(SN, hypernet.GetNodes().size());
    for (int i = 1; i < nodesInChain.size() - 1; i++) {
        int nodeNumber = nodesInChain[i];
        if (SNnodepowers[nodeNumber] != 2 && SNnodepowers[nodeNumber] != 0) {
            canHandle = false;
            break;
        }
    }

    return canHandle && isCorrect;
}
//todo сделать проще следющее - заменяем в FN ссылки соответсвующие ptr на новое ребро
//todo проверить на сколько нужно вообще замаричавться с isReliableChain
//todo лучше сначала проверять на инцидентность
//todo бывает ли ptr->size() == 1?
//todo перенести проверку на IsSimpleChain в GetHomogeneousChain
//todo возможно нужно заменить RemoveNodeFN на RemoveNode, чтобы упростить логику
// Chain replacement by branch
void H::ChainReduction(std::vector<int> &forbiddenNodes) {
    auto chain = GetHomogeneousChain(forbiddenNodes);
    if (chain.empty()) {
        return;
    }

    bool isReliableChain = chain.front().GetIsReliable();
    auto nodesInChain = GetNodesInChain(*this, chain);
    if (!IsSimpleChain(*this, chain, nodesInChain)) {
        UnsimpleChains++;
        for (auto &item : nodesInChain) {
            if (std::find(forbiddenNodes.begin(), forbiddenNodes.end(), item) == forbiddenNodes.end()) {
                forbiddenNodes.push_back(item);
            }
        }

        return ChainReduction(forbiddenNodes);
    }

    ChainsReduced++;
    Branch newBranch;
    if (isReliableChain) {
        newBranch = Branch::GetUnity();
        newBranch.SetIsReliable(true);
    } else {
        newBranch = Branch::GetUnity();
        for (auto &item : chain) {
            newBranch = newBranch * item;
        }
    }
    newBranch.SetId(chain.front().GetId());
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
            if (IsSlightlyIncident(node, route)) {
                ptr->erase(std::remove(ptr->begin(), ptr->end(), node), ptr->end());
                // если длина ребра с тановится 1, то удаляем его
                if (ptr->size() == 1) {
                    ptr->clear();
                    _F.erase(_F.begin() + j--);
                }
            } else if (IsIncident(node, route)) {
                _F.erase(_F.begin() + j--);
                auto it = std::find_if(_F.begin(), _F.end(), [node](Route &route) ->
                        bool { return IsIncident(node, route); });
                if(it == _F.end()) {
                    throw "ChainReduction: no incident route";
                }
                auto ptrToInsert = it->Ptr;
                int idToInsert = _F[it - _F.begin()].Id;

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
                // заменяем в FN ссылки соответсвующие ptr на новое ребро
                for (auto &branch : _FN) {
                    for (auto &item : branch.GetRoutes()) {
                        if (item == route) {
                            item.Ptr = ptrToInsert;
                            item.Id = idToInsert;
                        }
                    }
                }
                for (auto &item : newBranchRoutes) {
                    if (item == route) {
                        item.Ptr = ptrToInsert;
                        item.Id = idToInsert;
                        break;
                    }
                }
                // удаляем петли, так как они бессмысленны и приводят к ошибкам
                if (ptrToInsert->front() == ptrToInsert->back()) {
                    ptrToInsert->clear();
                    _F.erase(it);
                }
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

    return ChainReduction(forbiddenNodes);
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
    for(auto &item : _nodes) {
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