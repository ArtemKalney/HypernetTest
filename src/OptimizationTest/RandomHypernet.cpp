#include "../HypernetModel/Hypernet.h"
#include "../HypernetModel/Helpers/RandomHypernetHelper.h"

void RenumerateNodes(std::vector<Branch>& network, const int firstNode, const int secondNode) {
    if (firstNode == secondNode) {
        return;
    }

    for (auto &item : network) {
        if (item.GetFirstNode() == firstNode || item.GetFirstNode() == secondNode) {
            item.SetFirstNode(item.GetFirstNode() == firstNode ? secondNode : firstNode);
        }
        if (item.GetSecondNode() == firstNode || item.GetSecondNode() == secondNode) {
            item.SetSecondNode(item.GetSecondNode() == firstNode ? secondNode : firstNode);
        }
    }
}

H GetRandomNetworkHypernet(std::vector<Branch>& primaryNetwork, std::vector<Node>& nodes) {
    srand(seed++);
    int node = rand() % nodes.size();
    std::vector<int> testNodes {node};
    while (nodes.size() / testNodes.size() > NETWORK_NODES_PROPORTION) {
        node = rand() % nodes.size();
        if (std::find(testNodes.begin(), testNodes.end(), node) == testNodes.end()) {
            testNodes.push_back(node);
        }
    }
    // создаём полный граф
    auto secondaryNetwork = GetRandomNetwork(testNodes.size(), testNodes.size() * (testNodes.size() - 1) / 2,
                                             primaryNetwork.size());
    // восстанавливаем нумерацию из testNodes
    for (int i = 0; i < testNodes.size(); ++i) {
        RenumerateNodes(secondaryNetwork, i, testNodes[i]);
    }
    std::vector<Route> routes;
    SetMapping(primaryNetwork, secondaryNetwork, routes, nodes.size());

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}