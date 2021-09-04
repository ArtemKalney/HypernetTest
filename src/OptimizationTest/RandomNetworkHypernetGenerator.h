#pragma once

#include "../HypernetModel/Helpers/RandomHypernetGenerator.h"

class RandomNetworkHypernetGenerator : public RandomHypernetGenerator {
public:
    RandomNetworkHypernetGenerator(std::vector<Branch>& primaryNetwork, std::vector<Node>& nodes)
    {
        srand(seed++);
        _primaryNetwork = primaryNetwork;
        _nodes = nodes;
        int node = rand() % nodes.size();
        std::vector<int> testNodes {node};
        while (nodes.size() / testNodes.size() > NETWORK_NODES_PROPORTION) {
            node = rand() % nodes.size();
            if (std::find(testNodes.begin(), testNodes.end(), node) == testNodes.end()) {
                testNodes.push_back(node);
            }
        }
        // создаём полный граф
        _secondaryNetwork = GetRandomNetwork(testNodes.size(), testNodes.size() * (testNodes.size() - 1) / 2,
                                                 primaryNetwork.size());
        // восстанавливаем нумерацию из testNodes
        for (int i = 0; i < testNodes.size(); ++i) {
            RenumerateNodes(_secondaryNetwork, i, testNodes[i]);
        }
    }

    void RenumerateNodes(std::vector<Branch>& network, const int firstNode, const int secondNode);
};