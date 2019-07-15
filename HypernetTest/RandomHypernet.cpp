#include "Funcs.h"
#include "Globals.h"

std::vector<Branch> GetRandomTree(const int &nodesCount, std::vector<int> &nodes) {
    std::vector<Branch> tree;
    while (nodes.size() != nodesCount) {
        int node = rand() % (n - 1);
        if (std::find(nodes.begin(), nodes.end(), node) == nodes.end()) {
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

void Mapping(const std::vector<Branch> primaryNetwork, std::vector<std::vector<int>> &nodeRotes,
             std::vector<std::vector<Branch>> &branchRoutes, std::vector<bool> &isVisited, std::vector<int> &nodeRote,
             std::vector<Branch> &branchRoute, int &node, const int &endNode) {
    isVisited[node] = true;
    if(node == endNode) {
        nodeRotes.push_back(nodeRote);
        branchRoutes.push_back(branchRoute);
    }
    for(auto &branch : primaryNetwork) {
        if (IS_TEST_HYPERNET_SIMPLE_MAPPING == 1 && !nodeRotes.empty() && !branchRoutes.empty()) {
            return;
        }
        if (H::IsIncident(node, branch)) {
            int incidentNode = branch.GetFirstNode() != node ? branch.GetFirstNode() : branch.GetSecondNode();
            if (!isVisited[incidentNode]) {
                nodeRote.push_back(incidentNode);
                branchRoute.push_back(branch);
                Mapping(primaryNetwork, nodeRotes, branchRoutes, isVisited, nodeRote, branchRoute, incidentNode,
                        endNode);
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

void SetMapping(std::vector<Branch> &primaryNetwork, std::vector<Branch> &secondaryNetwork, std::vector<Route> &routes,
                std::vector<int> &nodes) {
    for (auto &edge : secondaryNetwork) {
        int node = edge.GetFirstNode();
        std::vector<bool> isVisited(n, false);
        std::vector<int> nodeRote{node};
        std::vector<Branch> branchRoute;
        std::vector<std::vector<int>> nodeRotes;
        std::vector<std::vector<Branch>> branchRoutes;
        Mapping(primaryNetwork, nodeRotes, branchRoutes, isVisited, nodeRote, branchRoute, node, edge.GetSecondNode());
        int randomIndex = nodeRotes.size() > 1 ? rand() % (nodeRotes.size() - 1) : 0;
        nodeRote = nodeRotes[randomIndex];
        branchRoute = branchRoutes[randomIndex];

//        for (auto &item : nodeRote) {
//            output << item + 1 << " ";
//        }
//        output << std::endl;

        for (auto &item : nodeRote) {
            if (std::find(nodes.begin(), nodes.end(), item) == nodes.end()) {
                nodes.push_back(item);
            }
        }
        routes.emplace_back(routes.size(), std::make_shared<std::vector<int>>(nodeRote));
        for(auto &item : branchRoute) {
            std::find(primaryNetwork.begin(), primaryNetwork.end(), item) -> GetRoutes().push_back(routes.back());
        }
    }
}

H GetRandomHypernet(std::vector<Branch> &primaryNetwork, std::vector<Node> &nodes) {
    srand(time(0));
    if (IS_TEST_HYPERNET_RANDOM_ROOTS == 1) {
        FirstRoot = rand() % (n - 1);
    }
    std::vector<int> firstTreeNodes{FirstRoot};
    auto firstRandomTree = GetRandomTree(TEST_HYPERNET_TREE_SIZE, firstTreeNodes);
    std::vector<Route> routes;
    std::vector<int> firstTreeMappingNodes;
    SetMapping(primaryNetwork, firstRandomTree, routes, firstTreeMappingNodes);
    FirstTreeMappingNodes = firstTreeMappingNodes.size();
    if (IS_TEST_HYPERNET_DOUBLE_TREE_MAP == 1) {
        if (IS_TEST_HYPERNET_RANDOM_ROOTS == 1) {
            do { SecondRoot = rand() % (n - 1); } while (SecondRoot == FirstRoot);
        }
        std::vector<int> secondTreeNodes{SecondRoot};
        auto secondRandomTree = GetRandomTree(TEST_HYPERNET_TREE_SIZE, secondTreeNodes);
        std::vector<int> secondTreeMappingNodes;
        SetMapping(primaryNetwork, secondRandomTree, routes, secondTreeMappingNodes);
        SecondTreeMappingNodes = secondTreeMappingNodes.size();
        for (auto &node : firstTreeMappingNodes) {
            auto it = std::find(secondTreeMappingNodes.begin(), secondTreeMappingNodes.end(), node);
            if (it != secondTreeMappingNodes.end()) {
                TreeNodeMappingIntersections++;
            }
        }
        for (auto &node : firstTreeNodes) {
            if (std::find(secondTreeNodes.begin(), secondTreeNodes.end(), node) != secondTreeNodes.end()) {
                TreeNodeIntersections++;
            }
        }
    }

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}