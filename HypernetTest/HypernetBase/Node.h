#pragma once

#include "Stdafx.h"
//todo сделать классом
//todo убрать IsVisited
struct Node {
    int NodeNumber;
    bool IsVisited;
    double Value;
    std::vector<double> C;
    int Power;
    bool IsReliable;

    Node() = default;

    Node(const int& nodeNumber, double value, std::vector<double> C, const int &power, bool isVisited, bool isReliable) :
            NodeNumber(nodeNumber),
            IsVisited(isVisited),
            Value(value),
            C(std::move(C)),
            Power(power),
            IsReliable(isReliable) {}

    Node(const Node& node) :
            NodeNumber(node.NodeNumber),
            IsVisited(node.IsVisited),
            Value(node.Value),
            C(node.C),
            Power(node.Power),
            IsReliable(node.IsReliable) {}

    void SetValue(const double& value)
    {
        Value = value;
    }

    double GetValue() const {
        return Value;
    }

    std::vector<double>& GetC()
    {
        return C;
    }

    void SetC(std::vector<double>& C)
    {
        C = C;
    }

    int GetId() const {
        return NodeNumber;
    }

    bool GetIsReliable() const
    {
        return IsReliable;
    }

    static Node GetSimpleBranch(const int &nodeNumber, double value, bool isVisited);
    static Node GetSimpleBranch();
    static Node GetZero();
    static Node GetUnity();
    static Node GetBranch(const int &power);
    bool IsUnacceptableElement();
    bool IsZero() const;
    bool IsPivotNode();
    double GetPolynomialValue(double point);
};

Node operator *(Node firstNode, Node secondNode);
Node operator +(Node firstNode, Node secondNode);
Node operator -(Node firstNode, Node secondNode);
Node operator ~(Node node);
bool operator ==(Node firstBranch, Node secondBranch);
bool operator !=(Node firstBranch, Node secondBranch);