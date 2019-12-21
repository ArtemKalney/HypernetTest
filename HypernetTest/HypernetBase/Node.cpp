//#pragma once

#include "Node.h"
#include "Globals.h"

Node Node::GetBranch(const int &power) {
    std::vector<double> C(m);
    C.front() = 1;
    std::vector<Route> routes;
    return Node(-1, std::pow(p, power), C, power, false, false);
}

Node Node::GetSimpleBranch(const int &nodeNumber, double value, bool isVisited) {
    std::vector<double> C(m);
    C.front() = 1;
    return Node(nodeNumber, value, C, 0, isVisited, false);
}

Node Node::GetSimpleBranch() {
    return GetSimpleBranch(-1, p, false);
}

Node Node::GetZero() {
    std::vector<double> C;
    return Node(-1, 0, C, 0, false, false);
}

Node Node::GetUnity() {
    std::vector<double> C;
    C.push_back(1);
    return Node(-1, 1, C, 0, false, false);
}

bool Node::IsUnacceptableElement() {
    bool isNodeUnacceptable = NodeNumber < 0 || NodeNumber > n, isPowerUnacceptable = Power < 0 || Power > m;
    return isNodeUnacceptable || isPowerUnacceptable;
}

bool Node::IsZero() const {
    return IS_NUMBER_COMPUTATION == 1 ? Value == 0 : C.empty();
}

bool Node::IsPivotNode() {
    return NodeNumber == 0 || NodeNumber == 1;
}

double Node::GetPolynomialValue(double point) {
    if (IS_NUMBER_COMPUTATION == 1) {
        return Value;
    }

    throw "Not Implemented";
}

Node operator *(Node firstNode, Node secondNode) {
    Node result = Node::GetZero();
    if (IS_NUMBER_COMPUTATION == 1) {
        result.Value = firstNode.Value * secondNode.Value;
        return result;
    }
    throw "Not Implemented";
}

Node operator +(Node firstNode, Node secondNode) {
    Node result = Node::GetZero();
    if (IS_NUMBER_COMPUTATION == 1) {
        result.Value = firstNode.Value + secondNode.Value;
        return result;
    }
    throw "Not Implemented";
}

Node operator -(Node firstNode, Node secondNode) {
    Node result = Node::GetZero();
    if (IS_NUMBER_COMPUTATION == 1) {
        result.Value = firstNode.Value - secondNode.Value;
        return result;
    }
    throw "Not Implemented";
}

Node operator ~(Node node) {
    Node result = Node::GetZero();
    if (IS_NUMBER_COMPUTATION == 1) {
        result.Value = 1 - node.Value;
        return result;
    }
    throw "Not Implemented";
}

bool operator ==(Node firstBranch, Node secondBranch) {
    return firstBranch.NodeNumber == secondBranch.NodeNumber;
}

bool operator !=(Node firstBranch, Node secondBranch) {
    return firstBranch.NodeNumber != secondBranch.NodeNumber;
}