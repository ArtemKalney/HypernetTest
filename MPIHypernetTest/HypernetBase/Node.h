#pragma once

struct Node {
    int NodeNumber;
    bool IsVisited;

    Node() = default;

    Node(const int& nodeNumber, bool isVisited) :
            NodeNumber(nodeNumber),
            IsVisited(isVisited) {}

    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & NodeNumber;
        ar & IsVisited;
    }
};