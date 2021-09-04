#include "RandomKPHypernetGenerator.h"

// получение случайного дерева заданного размера с запретами использования конкретных вершин
std::vector<Branch> RandomKPHypernetGenerator::GetRandomTree(const int nodesRequired, std::vector<int>& nodes,
                                  const std::vector<int>& forbiddenNodes, const int nodeSize, const int vectorSize) {
    if (nodesRequired > nodeSize) {
        throw "GetRandomTree: nodesRequired > nodeSize";
    }
    std::vector<Branch> tree;
    while (nodes.size() != nodesRequired) {
        int node = rand() % nodeSize;
        std::vector<int> newForbiddenNodes;
        newForbiddenNodes.reserve(nodes.size() + forbiddenNodes.size());
        newForbiddenNodes.insert(newForbiddenNodes.end(), nodes.begin(), nodes.end());
        newForbiddenNodes.insert(newForbiddenNodes.end(), forbiddenNodes.begin(), forbiddenNodes.end());
        if (std::find(newForbiddenNodes.begin(), newForbiddenNodes.end(), node) == newForbiddenNodes.end()) {
            nodes.push_back(node);
        }
    }
    std::vector<int> firstSetNodes = nodes;
    std::vector<int> secondSetNodes;
    int j = rand() % (firstSetNodes.size() - 1);
    for (int i = 0; i < nodesRequired - 1; i++) {
        secondSetNodes.push_back(firstSetNodes[j]);
        firstSetNodes.erase(firstSetNodes.begin() + j);
        int firstPosition = firstSetNodes.size() > 1 ? rand() % (firstSetNodes.size() - 1) : 0;
        int secondPosition = secondSetNodes.size() > 1 ? rand() % (secondSetNodes.size() - 1) : 0;
        j = firstPosition;
        tree.push_back(Branch::GetSimpleElement(-1, firstSetNodes[firstPosition], secondSetNodes[secondPosition],
                                                vectorSize));
    }

    return tree;
}