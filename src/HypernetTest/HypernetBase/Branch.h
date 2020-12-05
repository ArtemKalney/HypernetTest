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
public:
    Branch() = default;

    Branch(const int &id, double& value, std::vector<double> C, std::vector<Route> &routes, const int &power,
           const int &firstNode,
           const int &secondNode, bool isReliable) :
            Element(value, power, isReliable),
            _id(id),
            _routes(std::move(routes)),
            _firstNode(firstNode),
            _secondNode(secondNode),
            _C(std::move(C)) {}

    Branch(const Branch &branch) :
            Element(branch.GetValue(), branch.GetPower(), branch.GetIsReliable()),
            _id(branch._id),
            _routes(branch._routes),
            _firstNode(branch._firstNode),
            _secondNode(branch._secondNode),
            _C(branch._C) {}

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

    //todo уменьшить static функкций
    static Branch GetElement(const std::vector<double> &C, const int &power);

    static Branch GetElement(const int &power);

    static Branch GetSimpleElement();

    static Branch GetSimpleElement(const int &id, const int &firstNode, const int &secondNode);

    static Branch GetZero();

    static Branch GetUnity();

    static bool EqualNodes(const Branch &firstBranch, const Branch &secondBranch);

    bool IsUnacceptableElement();

    bool IsZero();

    bool IsUnity();

    bool IsSimpleBranch();

    void PrintBranch();

    double GetPolynomialValue(double point);

    int GetBranchSaturation();
};

bool operator !=(Branch firstBranch, Branch secondBranch);