#pragma once

#include "../HypernetModel/Generators/RandomHypernetGenerator.h"

class RandomKPHypernetGenerator : public RandomHypernetGenerator {
public:
    RandomKPHypernetGenerator(std::vector<Branch>& primaryNetwork, std::vector<Node>& nodes) {
        _primaryNetwork = primaryNetwork;
        _nodes = nodes;
        std::vector<int> forbiddenNodes = KpNodesCombination;
        for (auto &root : KpNodesCombination) {
            std::vector<int> treeNodes{root};
            int nodesCount = RANDOM_TREE_SIZE;
            if (nodes.size() - forbiddenNodes.size() + 1 < RANDOM_TREE_SIZE) {
                nodesCount = nodes.size() - forbiddenNodes.size() + 1;
            }
            auto randomTree = GetRandomTree(nodesCount, treeNodes, forbiddenNodes, nodes.size(), primaryNetwork.size());
            for(auto &item : treeNodes) {
                if (std::find(forbiddenNodes.begin(), forbiddenNodes.end(), item) == forbiddenNodes.end()) {
                    forbiddenNodes.push_back(item);
                }
            }
            _secondaryNetwork.insert(_secondaryNetwork.end(), randomTree.begin(), randomTree.end() );
        }
    }

    std::vector<Branch> GetRandomTree(const int nodesRequired, std::vector<int>& nodes,
                                      const std::vector<int>& forbiddenNodes, const int nodeSize, const int vectorSize);
};