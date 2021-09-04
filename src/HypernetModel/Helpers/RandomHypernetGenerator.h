#pragma once

#include "../Hypernet.h"

class RandomHypernetGenerator {
protected:
    std::vector<Branch> _primaryNetwork;
    std::vector<Branch> _secondaryNetwork;
    std::vector<Node> _nodes;
public:
    RandomHypernetGenerator() = default;

    RandomHypernetGenerator(std::vector<Branch>& primaryNetwork, std::vector<Node> &nodes, int routeSize) :
            _primaryNetwork(primaryNetwork),
            _nodes(nodes)
    {
        srand(seed++);
        _secondaryNetwork = GetRandomNetwork(nodes.size(), routeSize, primaryNetwork.size());
    }

    RandomHypernetGenerator(const int nodeSize, const int branchSize, const int routeSize) {
        srand(seed++);
        _primaryNetwork = GetRandomNetwork(nodeSize, branchSize, branchSize);
        _secondaryNetwork = GetRandomNetwork(nodeSize, routeSize, branchSize);
        for (int i = 0; i < nodeSize; i++) {
            Node node = Node::GetSimpleElement(i, p, false, branchSize);
            _nodes.push_back(node);
        }
    }

    std::vector<Branch> GetRandomNetwork(int nodesCount, int edgeCount, int vectorSize);

    void SetMapping(std::vector<Branch> &primaryNetwork, std::vector<Branch> &secondaryNetwork, std::vector<Route> &routes,
                    int nodeSize);

    void BFSWithRoutes(const std::vector<Branch>& primaryNetwork, std::vector<int>& nodeRote,
                       std::vector<Branch>& branchRoute, const int startNode, const int endNode, const int nodeSize);

    virtual H GenerateHypernet();
};