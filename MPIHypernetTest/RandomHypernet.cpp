#include "Funcs.h"
#include "Globals.h"

std::vector<Branch> GetRandomNetwork(int &nodesCount, int &edgeCount) {
    std::vector<Branch> network;
    std::vector<int> nodes1;
    for (int i = 0; i < nodesCount; i++) {
        nodes1.push_back(i);
    }
    std::vector<int> nodes2;
    int j = rand() % (nodes1.size() - 1);
    for (int i = 0; i < nodesCount - 1; i++) {
        nodes2.push_back(nodes1[j]);
        nodes1.erase(nodes1.begin() + j);
        int firstPosition = nodes1.size() > 1 ? rand() % (nodes1.size() - 1) : 0;
        int secondPosition = nodes2.size() > 1 ? rand() % (nodes2.size() - 1) : 0;
        j = firstPosition;
        network.push_back(Branch::GetSimpleBranch(network.size(), nodes1[firstPosition], nodes2[secondPosition]));
    }
    for (int i = 0; i < edgeCount - nodesCount + 1; i++) {
        int firstNode = rand() % nodesCount, secondNode = rand() % nodesCount;
        while (firstNode == secondNode)
            secondNode = rand() % nodesCount;
        network.push_back(Branch::GetSimpleBranch(network.size(), firstNode, secondNode));
    }

    return network;
}

std::vector<Branch> GetRandomTree(const int &nodesCount, std::vector<int> &nodes,
                                  const std::vector<int> &forbiddenNodes) {
    std::vector<Branch> tree;
    while (nodes.size() != nodesCount) {
        int node = rand() % (n - 1);
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
    for (int i = 0; i < nodesCount - 1; i++) {
        secondSetNodes.push_back(firstSetNodes[j]);
        firstSetNodes.erase(firstSetNodes.begin() + j);
        int firstPosition = firstSetNodes.size() > 1 ? rand() % (firstSetNodes.size() - 1) : 0;
        int secondPosition = secondSetNodes.size() > 1 ? rand() % (secondSetNodes.size() - 1) : 0;
        j = firstPosition;
        tree.push_back(Branch::GetSimpleBranch(-1, firstSetNodes[firstPosition], secondSetNodes[secondPosition]));
    }

    return tree;
}

void Mapping(const std::vector<Branch> &primaryNetwork, std::vector<std::vector<int>> &nodeRotes,
             std::vector<std::vector<Branch>> &branchRoutes, std::vector<bool> &isVisited, std::vector<int> &nodeRote,
             std::vector<Branch> &branchRoute, int &node, const int &endNode, const int &startTime) {
    isVisited[node] = true;
    if(node == endNode) {
        nodeRotes.push_back(nodeRote);
        branchRoutes.push_back(branchRoute);
    }
    for(auto &branch : primaryNetwork) {
        int searchTime = clock() - startTime;
        if (searchTime >= TEST_HYPERNET_MAPPING_TIME) {
            throw "Timeout";
        }
        if (IS_TEST_HYPERNET_SIMPLE_MAPPING == 1 && !nodeRotes.empty() && !branchRoutes.empty()) {
            return;
        }
        if (H::IsIncident(node, branch)) {
            int incidentNode = branch.GetFirstNode() != node ? branch.GetFirstNode() : branch.GetSecondNode();
            if (!isVisited[incidentNode]) {
                nodeRote.push_back(incidentNode);
                branchRoute.push_back(branch);
                Mapping(primaryNetwork, nodeRotes, branchRoutes, isVisited, nodeRote, branchRoute, incidentNode,
                        endNode, startTime);
                nodeRote.erase(std::find(nodeRote.begin(), nodeRote.end(), incidentNode));
                auto it = std::find_if(branchRoute.begin(), branchRoute.end(), [branch](Branch &item) -> bool {
                    return  item == branch;
                });
                branchRoute.erase(it);
            }
        }
    }

    isVisited[node] = false;
}

void SetMapping(std::vector<Branch> &primaryNetwork, std::vector<Branch> &secondaryNetwork,
                std::vector<Route> &routes) {
    for (auto &edge : secondaryNetwork) {
        int node = edge.GetFirstNode();
        std::vector<bool> isVisited(n, false);
        std::vector<int> nodeRote{node};
        std::vector<Branch> branchRoute;
        std::vector<std::vector<int>> nodeRotes;
        std::vector<std::vector<Branch>> branchRoutes;
        Mapping(primaryNetwork, nodeRotes, branchRoutes, isVisited, nodeRote, branchRoute, node, edge.GetSecondNode(),
                clock());
        int randomIndex = nodeRotes.size() > 1 ? rand() % (nodeRotes.size() - 1) : 0;
        nodeRote = nodeRotes[randomIndex];
        branchRoute = branchRoutes[randomIndex];
        routes.emplace_back(routes.size(), std::make_shared<std::vector<int>>(nodeRote));
        for(auto &item : branchRoute) {
            std::find(primaryNetwork.begin(), primaryNetwork.end(), item) -> GetRoutes().push_back(routes.back());
        }
    }
}

H TryGetRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes) {
    srand(seed++);
    std::vector<int> firstTreeNodes{FirstRoot};
    std::vector<int> forbiddenNodes{SecondRoot};
    auto firstRandomTree = GetRandomTree(TEST_HYPERNET_TREE_SIZE, firstTreeNodes, forbiddenNodes);
    std::vector<Route> routes;
    SetMapping(primaryNetwork, firstRandomTree, routes);
    if (IS_TEST_HYPERNET_DOUBLE_TREE_MAP == 1) {
        std::vector<int> secondTreeNodes{FirstRoot};
        forbiddenNodes.clear();
        auto secondRandomTree = GetRandomTree(TEST_HYPERNET_TREE_SIZE, secondTreeNodes, forbiddenNodes);
        SetMapping(primaryNetwork, secondRandomTree, routes);
        for (auto &node : firstTreeNodes) {
            if (std::find(secondTreeNodes.begin(), secondTreeNodes.end(), node) != secondTreeNodes.end()) {
                TreeNodeIntersections++;
            }
        }
        UnconnectedTreeNodes = n - firstTreeNodes.size() - secondTreeNodes.size() +
                               TreeNodeIntersections;
    }

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}

H GetRandomHypernet() {
    srand(seed++);
    auto primaryNetwork = GetRandomNetwork(n, m);
    auto secondaryNetwork = GetRandomNetwork(n, k);
    std::vector<Route> routes;
    SetMapping(primaryNetwork, secondaryNetwork, routes);
    std::vector<Node> nodes;
    for (int i = 0; i < n; i++) {
        Node node = Node(i, false);
        nodes.push_back(node);
    }

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}

H GetRandomHypernet(std::vector<Branch> &primaryNetwork, std::vector<Node> &nodes) {
    try {
        return TryGetRandomHypernet(primaryNetwork, nodes);
    } catch (const char *str) {
        if (IS_DEBUG == 1) {
            output << str << std::endl;
        }
        TreeNodeIntersections = 0;
        UnconnectedTreeNodes = 0;
        return GetRandomHypernet(primaryNetwork, nodes);
    }
}