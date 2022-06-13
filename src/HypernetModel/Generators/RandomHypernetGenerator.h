#pragma once

#include "../Hypernet.h"

class RandomHypernetGenerator {
protected:
    std::vector<Branch> _primaryNetwork;
    std::vector<Branch> _secondaryNetwork;
    std::vector<Node> _nodes;
    int _seed = time(0);
    int _maxDistance{};
public:
    RandomHypernetGenerator() = default;

    RandomHypernetGenerator(std::vector<Branch>& primaryNetwork, std::vector<Node> &nodes, int routeSize) :
            _primaryNetwork(primaryNetwork),
            _nodes(nodes)
    {
        _secondaryNetwork = GetRandomNetwork(nodes.size(), routeSize, primaryNetwork.size());
    }

    RandomHypernetGenerator(const int nodeSize, const int branchSize, const int routeSize) {
        _primaryNetwork = GetRandomNetwork(nodeSize, branchSize, branchSize);
        _secondaryNetwork = GetRandomNetwork(nodeSize, routeSize, branchSize);
        for (int i = 0; i < nodeSize; i++) {
            Node node = Node::GetSimpleElement(i, false, branchSize);
            _nodes.push_back(node);
        }
    }

    void SetMaxDistance(const int maxDistance) {
        _maxDistance = maxDistance;
    }

    int GetMaxDistance() const {
        return _maxDistance;
    }

    std::vector<Branch> GetRandomNetwork(int nodesCount, int edgeCount, int vectorSize);

    void BFSWithRoute(std::vector<int> &nodeRote,
                      std::vector<Branch> &branchRoute, const int startNode, const int endNode);

    virtual H GenerateHypernet();
};