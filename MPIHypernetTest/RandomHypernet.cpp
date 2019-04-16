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

void Mapping(const std::vector<Branch> primaryNetwork, std::vector<std::vector<int>> &nodeRotes,
              std::vector<std::vector<Branch>> &branchRoutes, std::vector<bool> &isVisited, std::vector<int> &nodeRote,
              std::vector<Branch> &branchRoute, int &node, const int &endNode) {
    isVisited[node] = true;
    if(node == endNode) {
        nodeRotes.push_back(nodeRote);
        branchRoutes.push_back(branchRoute);
    }
    for(auto &branch : primaryNetwork) {
        if (H::IsIncident(node, branch)) {
            int incidentNode = branch.GetFirstNode() != node ? branch.GetFirstNode() : branch.GetSecondNode();
            if (!isVisited[incidentNode]) {
                nodeRote.push_back(incidentNode);
                branchRoute.push_back(branch);
                Mapping(primaryNetwork, nodeRotes, branchRoutes, isVisited, nodeRote, branchRoute, incidentNode, endNode);
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

H GetRandomHypernet() {
    srand(seed++);
    auto primaryNetwork = GetRandomNetwork(n, m);
    auto secondaryNetwork = GetRandomNetwork(n, k);

    std::vector<Route> routes;
    for (auto &branch : secondaryNetwork) {
        int node = branch.GetFirstNode();
        std::vector<bool> isVisited(n, false);
        std::vector<int> nodeRote{node};
        std::vector<Branch> branchRoute;
        std::vector<std::vector<int>> nodeRotes;
        std::vector<std::vector<Branch>> branchRoutes;
        Mapping(primaryNetwork, nodeRotes, branchRoutes, isVisited, nodeRote, branchRoute, node, branch.GetSecondNode());
        int randomIndex = nodeRotes.size() > 1 ? rand() % (nodeRotes.size() - 1) : 0;
        nodeRote = nodeRotes[randomIndex];
        branchRoute = branchRoutes[randomIndex];
        routes.emplace_back(routes.size(), std::make_shared<std::vector<int>>(nodeRote));
        for(auto &item : branchRoute) {
            auto it = std::find_if(primaryNetwork.begin(), primaryNetwork.end(), [item](Branch branch) -> bool {
                return branch == item;
            });
            primaryNetwork[it - primaryNetwork.begin()].GetRoutes().push_back(routes.back());
        }
    }

    std::vector<Node> nodes;
    for (int i = 0; i < n; i++) {
        Node node = Node(i, false);
        nodes.push_back(node);
    }

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}