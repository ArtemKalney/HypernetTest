#pragma once

#include "Element.h"
#include "IIdentity.h"

//todo убрать IsVisited
class Node : public Element, public IIdentity
{
private:
    int _id;
    bool _isVisited;
    std::vector<double> _C;
public:
    Node() = default;

    Node(const int &id, double& value, std::vector<double> C, const int &power, bool isVisited, bool isReliable) :
            Element(value, power, isReliable),
            _id(id),
            _isVisited(isVisited),
            _C(std::move(C)) {}

    Node(const Node &node) :
            Element(node.GetValue(), node.GetPower(), node.GetIsReliable()),
            _id(node._id),
            _isVisited(node._isVisited),
            _C(node._C) {}

    std::vector<double> &GetC() {
        return _C;
    }

    void SetC(std::vector<double> &C) {
        _C = C;
    }

    int GetId() const override {
        return _id;
    }

    void SetId(const int &id) {
        _id = id;
    }

    void SetIsVisited(bool isReliable)
    {
        _isVisited = isReliable;
    }

    bool GetIsVisited() const
    {
        return _isVisited;
    }

    static Node GetElement(const std::vector<double> &C, const int &power);

    static Node GetSimpleElement(const int &id, double value, bool isVisited);

    static Node GetSimpleElement();

    static Node GetZero();

    static Node GetUnity();

    static Node GetElement(const int &power);

    bool IsUnacceptableElement();

    bool IsZero() const;

    bool IsUnity();

    bool IsPivotNode();

    double GetPolynomialValue(double point);
};