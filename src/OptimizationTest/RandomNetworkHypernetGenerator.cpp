#include "RandomNetworkHypernetGenerator.h"

void RandomNetworkHypernetGenerator::RenumerateNodes(std::vector<Branch>& network, const int firstNode, const int secondNode) {
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