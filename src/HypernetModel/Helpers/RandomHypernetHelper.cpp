#include "Funcs.h"
#include "RandomHypernetHelper.h"

// генератор случайных сетей для построения случайной гиперсети
std::vector<Branch> GetRandomNetwork(int nodesCount, int edgeCount, int vectorSize) {
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
        network.push_back(Branch::GetSimpleElement(network.size(), nodes1[firstPosition], nodes2[secondPosition],
                                                   vectorSize));
    }
    // расстановка оставшихся рёбер
    for (int i = 0; i < edgeCount - nodesCount + 1; i++) {
        int firstNode = rand() % nodesCount, secondNode = rand() % nodesCount;
        while (firstNode == secondNode)
            secondNode = rand() % nodesCount;
        network.push_back(Branch::GetSimpleElement(network.size(), firstNode, secondNode, vectorSize));
    }

    return network;
}

// укладка ребра в первичную сеть
void BFSWithRoutes(const std::vector<Branch>& primaryNetwork, std::vector<int>& nodeRote,
                   std::vector<Branch>& branchRoute, const int startNode, const int endNode, const int nodeSize) {
    std::vector<bool> isVisited(nodeSize, false);
    std::stack<int> queue;
    queue.push(startNode);
    isVisited[startNode] = true;
    std::vector<std::vector<int>> nodeRotes(nodeSize);
    nodeRotes[startNode].push_back(startNode);
    std::vector<std::vector<Branch>> branchRoutes(nodeSize);
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
void SetMapping(std::vector<Branch> &primaryNetwork, std::vector<Branch> &secondaryNetwork, std::vector<Route> &routes,
                const int nodeSize) {
    for (auto &edge : secondaryNetwork) {
        int node = edge.GetFirstNode();
        std::vector<int> nodeRote{node};
        std::vector<Branch> branchRoute;
        BFSWithRoutes(primaryNetwork, nodeRote, branchRoute, node, edge.GetSecondNode(), nodeSize);
        routes.emplace_back(routes.size(), std::make_shared<std::vector<int>>(nodeRote));
        for(auto &item : branchRoute) {
            std::find(primaryNetwork.begin(), primaryNetwork.end(), item) -> GetRoutes().push_back(routes.back());
        }
    }
}

// получение случайной гиперсети
H GetRandomHypernet(std::vector<Branch>& primaryNetwork, std::vector<Node> &nodes, const int routeSize) {
    srand(seed++);
    auto secondaryNetwork = GetRandomNetwork(nodes.size(), routeSize, primaryNetwork.size());
    std::vector<Route> routes;
    SetMapping(primaryNetwork, secondaryNetwork, routes, nodes.size());

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}

// получение случайной гиперсети
H GetRandomHypernet(const int nodeSize, const int branchSize, const int routeSize) {
    srand(seed++);
    auto primaryNetwork = GetRandomNetwork(nodeSize, branchSize, branchSize);
    auto secondaryNetwork = GetRandomNetwork(nodeSize, routeSize, branchSize);
    std::vector<Route> routes;
    SetMapping(primaryNetwork, secondaryNetwork, routes, nodeSize);
    std::vector<Node> nodes;
    for (int i = 0; i < nodeSize; i++) {
        Node node = Node::GetSimpleElement(i, p, false, branchSize);
        nodes.push_back(node);
    }

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}