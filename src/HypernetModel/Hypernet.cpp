#include "Hypernet.h"
#include "Globals.h"

//потребовалось для вершин не иметь привязку к индксу массива вершин
bool H::IsSNconnected() {
    auto it = std::find_if(_nodes.begin(), _nodes.end(), [](Node &item) -> bool { return item == 1; });
    if (it == _nodes.end()) {
        return false;
    }

    for(auto &item : _nodes) {
        item.SetIsVisited(false);
    }
    H::DFS(0, _nodes, GetSN());

    return it->GetIsVisited();
}

void H::RemoveElement(const Branch &branch) {
//    преобразования F
    for(auto &item : _FN) {
        if (item == branch) {
            for(auto &route : item.GetRoutes()) {
                if (!route.Ptr->empty()) {
                    route.Ptr->clear();
                    _F.erase(std::remove_if(_F.begin(), _F.end(), [](Route &route) ->
                            bool { return route.Ptr->empty(); }), _F.end());
                }
            }
        }
    }
//    преобразования FN
    _FN.erase(std::remove_if(_FN.begin(), _FN.end(), [branch](Branch &item) ->
            bool { return branch == item; }), _FN.end());

    RemoveEmptyBranches();
}
// второй способ разрушения вершины
void H::RemoveElement(const Node &node) {
    int id = node.GetId();
    //удаление инцедентных рёбер
    _F.erase(std::remove_if(_F.begin(), _F.end(), [id](Route &item) ->
            bool { return IsIncident(id, item); }), _F.end());
    RemoveEmptyBranches();
}
// перый способ разрушения вершины
void H::RemoveNode(const int& node) {
//    преобразования FN
    RemoveNodeFN(node);
//    преобразования F
    RemoveNodeSN(node);
}

void H::MakeReliableElement(const Branch &branch) {
    auto it = std::find_if(_FN.begin(), _FN.end(), [branch](Branch &item) ->
            bool { return branch == item; });
   _FN[it - _FN.begin()].SetIsReliable(true);
}

void H::MakeReliableElement(const Node &node) {
    auto it = std::find_if(_nodes.begin(), _nodes.end(), [node](Node &item) ->
            bool { return node == item; });
    _nodes[it - _nodes.begin()].SetIsReliable(true);
}
//todo переделать через IsSNconnected
template <>
bool H::HasReliablePath<Branch>() {
    auto HwithRemovedElements = *this;
    for (auto &item : _FN) {
        if (!item.GetIsReliable()) {
            HwithRemovedElements.RemoveElement(item);
        }
    }
    for (auto &item : _nodes) {
        item.SetIsVisited(false);
    }
    H::DFS(0, _nodes, HwithRemovedElements.GetSN());

    return _nodes[1].GetIsVisited();
}
//todo убрать лишнее удаляение вершин которые уже удалили
template <>
bool H::HasReliablePath<Node>() {
    auto HwithRemovedElements = *this;
    for (auto &item : _nodes) {
        if (!item.GetIsReliable()) {
            HwithRemovedElements.RemoveElement(item);
        }
    }

    return HwithRemovedElements.IsSNconnected();
}
//todo переделать через рекурсию
std::vector<bool> H::GetCanDeleteMask(const std::vector<Branch> &SN) {
    std::vector<bool> edgeMask(SN.size(), false);
    std::vector<int> nodePowers = GetNodePowers(SN, _nodes.size());
    // инициализируем edgeMask рёбрами не лежащими в компоненте связности содержащей выдленные вершины, а также
    // рёбра инцидентые висячим вершинам
    for (int i = 0; i < edgeMask.size(); i++) {
        Branch edge = SN[i];
        int firstNode = edge.GetFirstNode(), secondNode = edge.GetSecondNode();
        int penduntNode = nodePowers[firstNode] == 1 ? firstNode : (nodePowers[secondNode] == 1 ? secondNode : -1);
        if (!IsPivotNode(penduntNode) && (!_nodes[firstNode].GetIsVisited() || penduntNode != -1)) {
            edgeMask[i] = true;
        }
    }
    // расширяем маску за счёт висячих деревьев
    bool isChanged;
    do {
        isChanged = false;
        for (int i = 0; i < edgeMask.size(); i++) {
            Branch edge = SN[i];
            int firstNode = edge.GetFirstNode(), secondNode = edge.GetSecondNode();
            // расматриваем толко не выбранные ещё рёбра и в компоненте связности содержащей выделенные вершины
            if (!edgeMask[i] && _nodes[firstNode].GetIsVisited()) {
                int unMaskedEdgesCount = 0, unMaskedEdgeInd = -1;
                if (!IsPivotNode(firstNode)) {
                    for (int j = 0; j < edgeMask.size(); j++) {
                        edge = SN[j];
                        if (!edgeMask[j] && IsIncident(firstNode, edge)) {
                            unMaskedEdgesCount++;
                            unMaskedEdgeInd = j;
                        }
                    }
                }
                if (!IsPivotNode(secondNode) && unMaskedEdgesCount != 1) {
                    unMaskedEdgesCount = 0; unMaskedEdgeInd = -1;
                    for (int j = 0; j < edgeMask.size(); j++) {
                        edge = SN[j];
                        if (!edgeMask[j] && IsIncident(secondNode, edge)) {
                            unMaskedEdgesCount++;
                            unMaskedEdgeInd = j;
                        }
                    }
                }
                if (unMaskedEdgesCount == 1) {
                    isChanged = true;
                    edgeMask[unMaskedEdgeInd] = true;
                }
            }
        }
    } while (isChanged);

    return edgeMask;
}

std::vector<int> GetNodesInChain(const std::vector<Branch>& chain) {
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

bool IsSimpleChain(H &hypernet, std::vector<Branch> &chain, std::vector<int> &nodesInChain) {
    auto nodePowers = hypernet.GetNodePowers(hypernet.GetSN(), hypernet.GetNodes().size());
    for (int i = 1; i < nodesInChain.size() - 1; i++) {
        int nodeNumber = nodesInChain[i];
        if (nodePowers[nodeNumber] != 2 && nodePowers[nodeNumber] != 0) {
            return false;
        }
    }
    for (auto &branch : chain) {
        auto copyChain = chain;
        auto copyF = hypernet.GetF();
        for (auto &route : copyF) {
            auto vec = *route.Ptr;
            auto ptr = std::make_shared<std::vector<int>>(vec);
            route.Ptr = ptr;
            for (auto &copyBranch : copyChain) {
                for (auto &item : copyBranch.GetRoutes()) {
                    if (item == route) {
                        item.Ptr = ptr;
                    }
                }
            }
        }
        H copyH = H(copyChain, hypernet.GetNodes(), copyF);
        copyH.RemoveElement(branch);
        auto nodePowersCopyH = copyH.GetNodePowers(copyH.GetSN(), copyH.GetNodes().size());
        RemovePenduntRoutesInChain(copyH, nodesInChain, nodePowersCopyH);
        for (auto &item : copyChain) {
            if (item.GetSaturation() != 0) {
                return false;
            }
        }
    }

    return true;
}

bool IsExtensionBranch(Branch &item, H &H, const std::vector<Branch> &chain, const int &firstNode,
                       const Branch &firstBranch, const int &secondNode, bool isReliableChain) {
    bool isHomogeneousChain = isReliableChain == item.GetIsReliable();
    bool isIncidentBranch = item != firstBranch && H::IsIncident(firstNode, item);
    int incidentNode = item.GetFirstNode() == firstNode ? item.GetSecondNode() : item.GetFirstNode();
    return isIncidentBranch && incidentNode != secondNode && isHomogeneousChain;
}

std::vector<Branch> H::GetSimpleChain(std::vector<int> &forbiddenNodes) {
    auto nodePowers = H::GetNodePowers(_FN, _nodes.size());
    // Initializing chain by branch
    auto it = std::find_if(_FN.begin(), _FN.end(), [this, nodePowers, forbiddenNodes](Branch &branch) -> bool {
        int firstNode = branch.GetFirstNode(), secondNode = branch.GetSecondNode();
        bool isParallelBranch = std::find_if(_FN.begin(), _FN.end(), [branch](Branch &item) -> bool {
            return Branch::EqualNodes(item, branch) && item != branch;
        }) != _FN.end();
        bool isForbiddenNodes =
                std::find(forbiddenNodes.begin(), forbiddenNodes.end(), firstNode) != forbiddenNodes.end() &&
                std::find(forbiddenNodes.begin(), forbiddenNodes.end(), secondNode) != forbiddenNodes.end();
        bool isBaseBranch = !isForbiddenNodes && !isParallelBranch && (nodePowers[firstNode] == 2 ||
                nodePowers[secondNode] == 2);
        return IS_REDUCE_RELIABLE_CHAINS == 1 ? isBaseBranch : isBaseBranch && !branch.GetIsReliable();
    });
    std::vector<Branch> chain;
    bool isReliableChain;
    if (it != _FN.end()) {
        chain.push_back(*it);
        isReliableChain = it->GetIsReliable();
    } else {
        return chain;
    }
    // Chain extensions in both directions
    int leftNode = chain.front().GetFirstNode(), rightNode = chain.front().GetSecondNode();
    Branch leftBranch = chain.front(), rightBranch = chain.front();
    while (nodePowers[leftNode] == 2 && !IsPivotNode(leftNode)) {
        it = std::find_if(_FN.begin(), _FN.end(), [this, chain, leftNode, leftBranch, rightNode, isReliableChain]
                (Branch &item) {
            return IsExtensionBranch(item, *this, chain, leftNode, leftBranch, rightNode, isReliableChain);
        });
        if (it == _FN.end()) {
            break;
        }
        if (IS_FULL_SEARCH_CHAINS == 1) {
            auto newChain = chain;
            newChain.insert(newChain.begin(), *it);
            auto nodesInChain = GetNodesInChain(newChain);
            if (!IsSimpleChain(*this, newChain, nodesInChain)) {
                break;
            }
        }
        leftBranch = *it;
        leftNode = leftBranch.GetFirstNode() == leftNode ? leftBranch.GetSecondNode() :
                   leftBranch.GetFirstNode();
        chain.insert(chain.begin(), leftBranch);
    }
    while (nodePowers[rightNode] == 2 && !IsPivotNode(rightNode)) {
        it = std::find_if(_FN.begin(), _FN.end(), [this, chain, rightNode, rightBranch, leftNode, isReliableChain]
                (Branch &item) {
            return IsExtensionBranch(item, *this, chain, rightNode, rightBranch, leftNode, isReliableChain);
        });
        if (it == _FN.end()) {
            break;
        }
        if (IS_FULL_SEARCH_CHAINS == 1) {
            auto newChain = chain;
            newChain.push_back(*it);
            auto nodesInChain = GetNodesInChain(newChain);
            if (!IsSimpleChain(*this, newChain, nodesInChain)) {
                break;
            }
        }
        rightBranch = *it;
        rightNode = rightBranch.GetFirstNode() == rightNode ? rightBranch.GetSecondNode() :
                    rightBranch.GetFirstNode();
        chain.push_back(rightBranch);
    }

    if (chain.size() == 1) {
        if (std::find(forbiddenNodes.begin(), forbiddenNodes.end(), leftNode) == forbiddenNodes.end()) {
            forbiddenNodes.push_back(leftNode);
        }
        if (std::find(forbiddenNodes.begin(), forbiddenNodes.end(), rightNode) == forbiddenNodes.end()) {
            forbiddenNodes.push_back(rightNode);
        }
        return GetSimpleChain(forbiddenNodes);
    }
    if (IS_FULL_SEARCH_CHAINS == 0) {
        auto nodesInChain = GetNodesInChain(chain);
        if (!IsSimpleChain(*this, chain, nodesInChain)) {
            ComplexChains++;
            for (auto &item : nodesInChain) {
                if (std::find(forbiddenNodes.begin(), forbiddenNodes.end(), item) == forbiddenNodes.end()) {
                    forbiddenNodes.push_back(item);
                }
            }
            return GetSimpleChain(forbiddenNodes);
        }
    }
    return chain;
}
//todo сделать отдельную модель для ребра (не использовать Branch)
std::vector<Branch> H::GetSN() {
    std::vector<Branch> graph;
    for (auto &route : _F) {
        Branch newEdge = Branch::GetUnity();
        newEdge.SetFirstNode(route.Ptr->front());
        newEdge.SetSecondNode(route.Ptr->back());
        newEdge.SetId(route.Id);
        graph.push_back(std::move(newEdge));
    }

    return graph;
}

template <class T>
void H::DFS(const int& node, std::vector<Node>& nodes, const std::vector<T>& graph) {
    auto it = std::find_if(nodes.begin(), nodes.end(), [node](Node &item) -> bool { return item == node; });
    nodes[it - nodes.begin()].SetIsVisited(true);
    for(auto &item : graph) {
        if (item.GetFirstNode() == node || item.GetSecondNode() == node) {
            int incidentNode = item.GetFirstNode();
            if (item.GetFirstNode() == node) {
                incidentNode = item.GetSecondNode();
            }

            it = std::find_if(nodes.begin(), nodes.end(), [incidentNode](Node &node) ->
                    bool { return node == incidentNode; });
            bool visitedNode = nodes[it - nodes.begin()].GetIsVisited();
            if (!visitedNode) {
                DFS(incidentNode, nodes, graph);
            }
        }
    }
}

std::vector<int> H::GetNodePowers(const std::vector<Branch>& graph, const int& size) {
    std::vector<int> nodePowers(size);
    for (int i = 0; i < nodePowers.size(); i++) {
        for (auto &item : graph) {
            if (IsIncident(i, item)) {
                nodePowers[i]++;
            }
        }
    }

    return nodePowers;
}

void H::RemoveEmptyBranches() {
    for (int i=0; i<_FN.size(); i++) {
        if (_FN[i].GetSaturation() == 0) {
            _FN.erase(_FN.begin() + i--);
        }
    }
}

void H::RenumerateNodes(const int& firstNode, const int& secondNode) {
    if (firstNode == secondNode) {
        return;
    }
    // transforms FN with RenumerateNodes
    for (auto &item : _FN) {
        if (item.GetFirstNode() == firstNode || item.GetFirstNode() == secondNode) {
            item.SetFirstNode(item.GetFirstNode() == firstNode ? secondNode : firstNode);
        }
        if (item.GetSecondNode() == firstNode || item.GetSecondNode() == secondNode) {
            item.SetSecondNode(item.GetSecondNode() == firstNode ? secondNode : firstNode);
        }
    }
    // transforms F with RenumerateNodes
    for (auto &route : _F) {
        for (auto &item : *route.Ptr) {
            if (item == firstNode) {
                item = secondNode;
            } else if (item == secondNode) {
                item = firstNode;
            }
        }
    }
}

void H::RemoveNodeFN(const int& node) {
    for (int i = 0; i < _FN.size(); i++) {
        Branch branch = _FN[i];
        if (IsIncident(node, branch)) {
            _FN.erase(_FN.begin() + i--);
        } else {
            int firstNode = branch.GetFirstNode(), secondNode = branch.GetSecondNode();
            if (firstNode > node) {
                _FN[i].SetFirstNode(--firstNode);
            }
            if (secondNode > node) {
                _FN[i].SetSecondNode(--secondNode);
            }
        }
    }
    // пробразования nodes
    _nodes.erase(std::remove_if(_nodes.begin(), _nodes.end(), [node](Node &item) ->
            bool { return item == node; }), _nodes.end());
    for (auto &item : _nodes) {
        if (item > node) {
            item.SetId(item.GetId() - 1);
        }
    }
}

void H::RemoveNodeSN(const int& node) {
    for (int i = 0; i < _F.size(); i++) {
        bool deletedRow = false;
        for(auto &item : *_F[i].Ptr) {
//            удаляемая вершина оказалась в ребре
            if (item == node) {
                _F.erase(_F.begin() + i--);
                deletedRow = true;
                break;
            }
        }
//        decrease nodes after delete
        if (!deletedRow) {
            for (auto &item : *_F[i].Ptr) {
                if (item > node) {
                    item--;
                }
            }
        }
    }

    RemoveEmptyBranches();
}

bool H::IsSlightlyIncident(const int &node, const Route &route) {
    bool containNode = std::find(route.Ptr->begin(), route.Ptr->end(), node) != route.Ptr->end();
    return route.Ptr->front() != node && route.Ptr->back() != node && containNode;
}

bool H::IsIncident(const int &node, const Route &route) {
    return route.Ptr->front() == node || route.Ptr->back() == node;
}

bool H::IsIncident(const int &node, const Branch &branch) {
    return branch.GetFirstNode() == node || branch.GetSecondNode() == node;
}

bool H::IsSlightlyIncident(const Branch &branch, const Route &route) {
    for (int i = 0; i < route.Ptr->size() - 1; ++i) {
        if (IsIncident(route.Ptr->at(i), branch) && IsIncident(route.Ptr->at(i + 1), branch)) {
            return true;
        }
    }

    return false;
}

bool H::IsPivotNode(const int &node) {
    return node == 0 || node == 1;
}
//print
void H::PrintHypernet() {
    std::cout << "FN:" << std::endl;
    for(auto &item : _FN) {
        std::cout << "(" << item.GetFirstNode() << "," << item.GetSecondNode() << ")" << std::endl;
        item.PrintBranch();
    }
    std::cout << "F:" << std::endl;
    for (auto &route : _F) {
        for (auto &item : *route.Ptr) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}

void H::LogHypernet() {
    output << "Hypernet(" << _nodes.size() << ", " << _FN.size() << ", " << _F.size() << ")" << std::endl;
    output << _nodes.size() << " " << _FN.size() << " "  << _F.size() << std::endl;
    for(auto &node : _nodes) {
        output << node.GetId() << std::endl;
        if (OUTPUT_NODE_VALUES == 1) {
            output << node.GetValue() << std::endl;
        }
    }
    for(auto &branch : _FN) {
        output << branch.GetId() << std::endl;
        output << branch.GetFirstNode() << " " << branch.GetSecondNode() << std::endl;
        output << VectorToString(branch.GetRoutes(), " ") << " " << 0 << std::endl;
        if (OUTPUT_BRANCH_VALUES == 1) {
            output << branch.GetValue() << std::endl;
        }
        if (OUTPUT_BRANCH_COSTS == 1) {
            output << branch.GetCost() << std::endl;
        }
        if (OUTPUT_MAX_BRANCH_SATURATIONS == 1) {
            output << branch.GetMaxSaturation() << std::endl;
        }
    }
    for(auto &route : _F) {
        output << route.GetId() << std::endl;
        for (auto &item : *route.Ptr) {
            output << item << " ";
        }
        output << 0 << std::endl;
    }
    output << "$$$" << std::endl;
}

void H::RenumerateNodesForGen(const int& firstNode, const int& secondNode) {
    if (firstNode == secondNode) {
        return;
    }
    RenumerateNodes(firstNode, secondNode);
    for(auto &item : _FN) {
        int node1 = item.GetFirstNode(), node2 = item.GetSecondNode();
        if (node1 == firstNode) {
            item.SetFirstNode(secondNode);
        } else if (node1 == secondNode) {
            item.SetFirstNode(firstNode);
        }
        if (node2 == firstNode) {
            item.SetSecondNode(secondNode);
        } else if (node2 == secondNode) {
            item.SetSecondNode(firstNode);
        }
    }
}

bool H::IsValidHypernet() {
    for(auto &branch : _FN) {
        for(auto &item : branch.GetRoutes()) {
            if (!IsSlightlyIncident(branch, item)) {
                return false;
            }
        }
    }

    return true;
}

// to debug
std::vector<std::vector<int>> H::GetRoutesF(){
    std::vector<std::vector<int>> edges;
    for(auto &item : _F) {
        if(item.Ptr != nullptr) {
            edges.push_back(*item.Ptr);
        }
    }
    return edges;
}

std::vector<std::vector<std::vector<int>>> H::GetRoutesFN(){
    std::vector<std::vector<std::vector<int>>> edgesByBranches;
    for(auto &branch : _FN) {
        std::vector<std::vector<int>> edges;
        for(auto &item : branch.GetRoutes()) {
            if(item.Ptr != nullptr) {
                edges.push_back(*item.Ptr);
            }
        }
        edgesByBranches.push_back(edges);
    }
    return edgesByBranches;
}