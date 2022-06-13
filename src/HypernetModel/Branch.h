#pragma once

#include "Stdafx.h"
#include "Route.h"
#include "Element.h"
#include "Operators.h"

class Branch : public Element, public IIdentity {
private:
    int _id;
    std::vector<Route> _routes;
    int _firstNode;
    int _secondNode;
    std::vector<double> _C;
    // параметры для оптимизации
    int _maxSaturation = 0;
    double _cost = 1;

    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & _id;
        ar & _value;
        ar & _C;
        ar & _routes;
        ar & _power;
        ar & _firstNode;
        ar & _secondNode;
        ar & _isReliable;
    }
public:
    Branch() = default;

    Branch(const Branch &branch) :
            Element(branch.GetValue(), branch.GetPower(), branch.GetIsReliable()),
            _id(branch._id),
            _routes(branch._routes),
            _firstNode(branch._firstNode),
            _secondNode(branch._secondNode),
            _C(branch._C),
            _maxSaturation(branch._maxSaturation),
            _cost(branch._cost){}

    void SetId(const int &id) {
        _id = id;
    }

    int GetId() const override {
        return _id;
    }

    void SetRoutes(const std::vector<Route> &routes) {
        _routes = routes;
    }

    std::vector<Route> &GetRoutes() {
        return _routes;
    }

    void SetFirstNode(const int &firstNode) {
        _firstNode = firstNode;
    }

    int GetFirstNode() const {
        return _firstNode;
    }

    void SetSecondNode(const int &secondNode) {
        _secondNode = secondNode;
    }

    int GetSecondNode() const {
        return _secondNode;
    }

    void SetC(const std::vector<double> &C) {
        _C = C;
    }

    std::vector<double> &GetC() {
        return _C;
    }

    void SetMaxSaturation(const int maxBranchSaturation) {
        _maxSaturation = maxBranchSaturation;
    }

    int GetMaxSaturation() const {
        return _maxSaturation;
    }

    double SetCost(const double cost) {
        _cost = cost;
    }

    double GetCost() const {
        return _cost;
    }

    //todo уменьшить static функкций
    static Branch GetElement(const std::vector<double> &C, int power);

    static Branch GetElement(int power, int vectorSize);

    static Branch GetSimpleElement(int vectorSize);

    static Branch GetSimpleElement(int id, int firstNode, int secondNode, int vectorSize);

    static Branch GetZero();

    static Branch GetUnity();

    static bool EqualNodes(const Branch &firstBranch, const Branch &secondBranch);

    bool IsZero();

    bool IsUnity();

    bool IsSimpleBranch();

    void PrintBranch();

    double GetPolynomialValue(double point);

    int GetSaturation();
};

bool operator !=(Branch firstBranch, Branch secondBranch);
bool operator <(Branch firstBranch, Branch secondBranch);