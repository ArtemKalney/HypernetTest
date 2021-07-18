#include "../HypernetModel/Hypernet.h"
#include "../HypernetModel/Helpers/RandomHypernetHelper.h"

H GetRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes, std::vector<int>& testNodes) {
    srand(seed++);
    int size = testNodes.size();
    k = size *  (size - 1) / 2;

    auto secondaryNetwork = GetRandomNetwork(size, k);
    std::vector<Route> routes;
    SetMapping(primaryNetwork, secondaryNetwork, routes);

    return H(std::move(primaryNetwork), std::move(nodes), std::move(routes));
}