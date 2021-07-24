#include "../HypernetModel/Hypernet.h"
#include "../HypernetModel/Helpers/RandomHypernetHelper.h"

H GetRandomNetworkHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes) {
    srand(seed++);
    int node = rand() % nodes.size();
    std::vector<int> testNodes {node};
    while (nodes.size() / testNodes.size() > NETWORK_NODES_PROPORTION) {
        node = rand() % nodes.size();
        if (std::find(testNodes.begin(), testNodes.end(), node) == testNodes.end()) {
            testNodes.push_back(node);
        }
    }
    int size = testNodes.size();
    k = size * (size - 1) / 2;

    auto secondaryNetwork = GetRandomNetwork(size, k);
    std::vector<Route> routes;
    SetMapping(primaryNetwork, secondaryNetwork, routes);

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}