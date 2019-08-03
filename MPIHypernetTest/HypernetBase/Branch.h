#pragma once

#include "Stdafx.h"
#include "Route.h"
#include "IIdentity.h"

class Branch : public IIdentity
{
private:
    int _id;
    double _value;
    std::vector<double> _C;
    std::vector<Route>  _routes;
    int _power;
    int _firstNode;
    int _secondNode;
    bool _isReliable;

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

    Branch(const int& id, double value, std::vector<double> C, std::vector<Route>& routes, const int& power, const int& firstNode,
           const int& secondNode, bool isReliable) :
            _id(id),
            _value(value),
            _C(std::move(C)),
            _routes(std::move(routes)),
            _power(power),
            _firstNode(firstNode),
            _secondNode(secondNode),
            _isReliable(isReliable) {}

    Branch(const Branch& branch) :
            _id(branch._id),
            _value(branch._value),
            _C(branch._C),
            _routes(branch._routes),
            _power(branch._power),
            _firstNode(branch._firstNode),
            _secondNode(branch._secondNode),
            _isReliable(branch._isReliable) {}

    void SetId(const int& id)
    {
        _id = id;
    }

    int GetId() const override {
        return _id;
    }

    void SetValue(const double& value)
    {
        _value = value;
    }

    void SetC(const std::vector<double>& C)
    {
        _C = C;
    }

    std::vector<double>& GetC()
    {
        return _C;
    }

    void SetRoutes(const std::vector<Route> &routes)
    {
        _routes = routes;
    }

    std::vector<Route>& GetRoutes()
    {
        return _routes;
    }

    void SetPower(const int& power)
    {
        _power = power;
    }

    int GetPower() const
    {
        return _power;
    }

    void SetFirstNode(const int& firstNode)
    {
        _firstNode = firstNode;
    }

    int GetFirstNode() const
    {
        return _firstNode;
    }

    void SetSecondNode(const int& secondNode)
    {
        _secondNode = secondNode;
    }

    int GetSecondNode() const
    {
        return _secondNode;
    }

    void SetIsReliable(bool isReliable)
    {
        _isReliable = isReliable;
    }

    bool GetIsReliable() const
    {
        return _isReliable;
    }
    //todo уменьшить static функкций
    static Branch GetBranch(const int& vectorSize, const int& power);
    static Branch GetBranch(const int& power);
    static Branch GetSimpleBranch();
    static Branch GetSimpleBranch(const int& id, const int& firstNode, const int& secondNode);
    static Branch GetZero();
    static Branch GetUnity();
    static bool EqualNodes(const Branch& firstBranch, const Branch& secondBranch);
    static Branch ParallelReduction(std::vector<Branch> &branches);
    bool IsUnacceptableBranch();
    bool IsZero() const;
    bool IsUnity();
    bool IsSimpleBranch();
    void PrintBranch();
    bool operator <(const Branch& branch) const;
    double GetValue();
    int GetBranchSaturation();
};

Branch operator *(Branch firstBranch, Branch secondBranch);
Branch operator +(Branch firstBranch, Branch secondBranch);
Branch operator -(Branch firstBranch, Branch secondBranch);
Branch operator ~(Branch branch);
bool operator ==(Branch firstBranch, Branch secondBranch);
bool operator !=(Branch firstBranch, Branch secondBranch);