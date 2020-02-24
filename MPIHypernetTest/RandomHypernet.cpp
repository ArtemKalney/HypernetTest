#include "Funcs.h"

//todo compile in separate library
//todo GetRandomNetwork через GetRandomTree

// генератор случайных сетей для построения случайной гиперсети
std::vector<Branch> GetRandomNetwork(int &nodesCount, int &edgeCount) {
    std::vector<Branch> network;
    std::vector<int> nodes1; // набор вершин 1
    for (int i = 0; i < nodesCount; i++) {
        nodes1.push_back(i);
    }
    std::vector<int> nodes2; // набор вершин 2
    int j = rand() % (nodes1.size() - 1);
    // цикл построения случайного дерева, гарантирует связность сети
    for (int i = 0; i < nodesCount - 1; i++) {
        nodes2.push_back(nodes1[j]);
        nodes1.erase(nodes1.begin() + j);
        int firstPosition = nodes1.size() > 1 ? rand() % (nodes1.size() - 1) : 0;
        int secondPosition = nodes2.size() > 1 ? rand() % (nodes2.size() - 1) : 0;
        j = firstPosition;
        network.push_back(Branch::GetSimpleElement(network.size(), nodes1[firstPosition], nodes2[secondPosition]));
    }
    // расстановка оставшихся рёбер
    for (int i = 0; i < edgeCount - nodesCount + 1; i++) {
        int firstNode = rand() % nodesCount, secondNode = rand() % nodesCount;
        while (firstNode == secondNode)
            secondNode = rand() % nodesCount;
        network.push_back(Branch::GetSimpleElement(network.size(), firstNode, secondNode));
    }

    return network;
}

// получение случайного дерева заданного размера с запретами использования конкретных вершин
std::vector<Branch> GetRandomTree(const int &nodesRequired, std::vector<int> &nodes,
                                  const std::vector<int> &forbiddenNodes) {
    if (nodesRequired > n) {
        throw "GetRandomTree: nodesRequired > n";
    }
    std::vector<Branch> tree;
    while (nodes.size() != nodesRequired) {
        int node = rand() % n;
        std::vector<int> newForbiddenNodes;
        newForbiddenNodes.reserve(nodes.size() + forbiddenNodes.size());
        newForbiddenNodes.insert(newForbiddenNodes.end(), nodes.begin(), nodes.end());
        newForbiddenNodes.insert(newForbiddenNodes.end(), forbiddenNodes.begin(), forbiddenNodes.end());
        if (std::find(newForbiddenNodes.begin(), newForbiddenNodes.end(), node) == newForbiddenNodes.end()) {
            nodes.push_back(node);
        }
    }
    std::vector<int> firstSetNodes = nodes;
    std::vector<int> secondSetNodes;
    int j = rand() % (firstSetNodes.size() - 1);
    for (int i = 0; i < nodesRequired - 1; i++) {
        secondSetNodes.push_back(firstSetNodes[j]);
        firstSetNodes.erase(firstSetNodes.begin() + j);
        int firstPosition = firstSetNodes.size() > 1 ? rand() % (firstSetNodes.size() - 1) : 0;
        int secondPosition = secondSetNodes.size() > 1 ? rand() % (secondSetNodes.size() - 1) : 0;
        j = firstPosition;
        tree.push_back(Branch::GetSimpleElement(-1, firstSetNodes[firstPosition], secondSetNodes[secondPosition]));
    }

    return tree;
}

// укладка ребра в первичную сеть
void BFSWithRoutes(const std::vector<Branch> &primaryNetwork, std::vector<int> &nodeRote,
                   std::vector<Branch> &branchRoute, int &startNode, const int &endNode) {
    std::vector<bool> isVisited(n, false);
    std::stack<int> queue;
    queue.push(startNode);
    isVisited[startNode] = true;
    std::vector<std::vector<int>> nodeRotes(n);
    nodeRotes[startNode].push_back(startNode);
    std::vector<std::vector<Branch>> branchRoutes(n);

    while(!queue.empty()) {
        int node = queue.top();
        queue.pop();
        if (node == endNode) {
            nodeRote = nodeRotes[endNode];
            branchRoute = branchRoutes[endNode];
            return;
        }

        for(auto &branch : primaryNetwork) {
            if (H::IsIncident(node, branch)) {
                int incidentNode = branch.GetFirstNode() != node ? branch.GetFirstNode() : branch.GetSecondNode();
                if (!isVisited[incidentNode]) {
                    queue.push(incidentNode);
                    isVisited[incidentNode] = true;
                    nodeRotes[incidentNode] = nodeRotes[node];
                    nodeRotes[incidentNode].push_back(incidentNode);
                    branchRoutes[incidentNode] = branchRoutes[node];
                    branchRoutes[incidentNode].push_back(branch);
                }
            }
        }
    }
}

// получение укладки ребра в первичную сеть
void SetMapping(std::vector<Branch> &primaryNetwork, std::vector<Branch> &secondaryNetwork, std::vector<Route> &routes) {
    for (auto &edge : secondaryNetwork) {
        int node = edge.GetFirstNode();
        std::vector<int> nodeRote{node};
        std::vector<Branch> branchRoute;
        BFSWithRoutes(primaryNetwork, nodeRote, branchRoute, node, edge.GetSecondNode());
        routes.emplace_back(routes.size(), std::make_shared<std::vector<int>>(nodeRote));
        for(auto &item : branchRoute) {
            std::find(primaryNetwork.begin(), primaryNetwork.end(), item) -> GetRoutes().push_back(routes.back());
        }
    }
}

// получение случайной гиперсети необходимой для работы алгоритма оптимизации (возможен timeout)
H GetAoshRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes) {
    srand(seed++);
    std::vector<int> firstTreeNodes{FirstRoot};
    std::vector<int> forbiddenNodes{SecondRoot};
    auto firstRandomTree = GetRandomTree(RANDOM_TREE_SIZE, firstTreeNodes, forbiddenNodes);
    std::vector<Route> routes;
    SetMapping(primaryNetwork, firstRandomTree, routes);
    std::vector<int> secondTreeNodes{FirstRoot};
    forbiddenNodes.clear();
    auto secondRandomTree = GetRandomTree(RANDOM_TREE_SIZE, secondTreeNodes, forbiddenNodes);
    SetMapping(primaryNetwork, secondRandomTree, routes);
    for (auto &node : firstTreeNodes) {
        if (std::find(secondTreeNodes.begin(), secondTreeNodes.end(), node) != secondTreeNodes.end()) {
            TreeNodeIntersections++;
        }
    }
    UnconnectedTreeNodes = n - firstTreeNodes.size() - secondTreeNodes.size() +
                           TreeNodeIntersections;

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}

H GetKpRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes) {
    srand(seed++);
    std::vector<Route> routes;
    std::vector<int> forbiddenNodes = KpNodesCombination;
    for (auto &root : KpNodesCombination) {
        std::vector<int> treeNodes{root};
        int nodesCount = RANDOM_TREE_SIZE;
        if (n - forbiddenNodes.size() + 1 < RANDOM_TREE_SIZE) {
            nodesCount = n - forbiddenNodes.size() + 1;
        }
        auto randomTree = GetRandomTree(nodesCount, treeNodes, forbiddenNodes);
        for(auto &item : treeNodes) {
            if (std::find(forbiddenNodes.begin(), forbiddenNodes.end(), item) == forbiddenNodes.end()) {
                forbiddenNodes.push_back(item);
            }
        }
        SetMapping(primaryNetwork, randomTree, routes);
    }

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}

// получение случайной гиперсети
H GetRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes) {
    srand(seed++);
    auto secondaryNetwork = GetRandomNetwork(n, k);
    std::vector<Route> routes;
    SetMapping(primaryNetwork, secondaryNetwork, routes);

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}

// получение случайной гиперсети
H GetRandomHypernet() {
    srand(seed++);
    auto primaryNetwork = GetRandomNetwork(n, m);
    auto secondaryNetwork = GetRandomNetwork(n, k);
    std::vector<Route> routes;
    SetMapping(primaryNetwork, secondaryNetwork, routes);
    std::vector<Node> nodes;
    for (int i = 0; i < n; i++) {
        Node node = Node::GetSimpleElement(i, p, false);
        nodes.push_back(node);
    }

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}