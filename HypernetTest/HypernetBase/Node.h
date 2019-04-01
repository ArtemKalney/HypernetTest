#pragma once

struct Node {
    int NodeNumber;
    bool IsVisited;

    Node() = default;

    Node(const int& nodeNumber, bool isVisited) :
            NodeNumber(nodeNumber),
            IsVisited(isVisited) {}
};