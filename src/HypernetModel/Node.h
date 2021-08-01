#pragma once

#include "Element.h"
#include "IIdentity.h"
#include "Globals.h"

//todo убрать IsVisited
class Node : public Element, public IIdentity
{
private:
    int _id;
    bool _isVisited;
    std::vector<double> _C;

    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & _id;
        ar & _value;
        ar & _C;
        ar & _power;
        ar & _isReliable;
    }
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

    static Node GetElement(const std::vector<double>& C, int power);

    static Node GetSimpleElement(int id, double value, bool isVisited, int vectorSize);

    static Node GetSimpleElement(int vectorSize);

    static Node GetZero();

    static Node GetUnity();

    static Node GetElement(int power, int vectorSize);

    bool IsZero() const;

    bool IsUnity();

    bool IsPivotNode();

    double GetPolynomialValue(double point);
};