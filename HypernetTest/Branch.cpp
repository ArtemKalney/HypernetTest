#include "Branch.h"
#include "Globals.h"

Branch Branch::GetBranch(const int& power) {
    std::vector<double> C(m);
    C.front() = 1;
    std::vector<Route> routes;
    return Branch(-1, C, routes, power, 0, 0, false);
}

Branch Branch::GetBranch(const int& vectorSize, const int& power) {
    std::vector<double> C(vectorSize);
    C.front() = 1;
    std::vector<Route> routes;
    return Branch(-1, C, routes, power, 0, 0, false);
}

Branch Branch::GetSimpleBranch(const int& id, const int& firstNode, const int& secondNode) {
    std::vector<double> C(m);
    C.front() = 1;
    std::vector<Route> routes;
    return Branch(id, C, routes, 1, firstNode, secondNode, false);
}

Branch Branch::GetZero() {
    std::vector<double> C;
    std::vector<Route> routes;
    return Branch(-1, C, routes, 0, 0, 0, false);
}

Branch Branch::GetUnity() {
    std::vector<double> C;
    C.push_back(1);
    std::vector<Route> routes;
    return Branch(-1, C, routes, 0, 0, 0, false);
}

bool Branch::EqualNodes(const Branch& firstBranch, const Branch& secondBranch) {
    auto sameNodes = firstBranch.GetFirstNode() == secondBranch.GetFirstNode() &&
                     firstBranch.GetSecondNode() == secondBranch.GetSecondNode();
    auto reversedNodes = firstBranch.GetFirstNode() == secondBranch.GetSecondNode() &&
                         firstBranch.GetSecondNode() == secondBranch.GetFirstNode();
    return sameNodes || reversedNodes;
}

bool Branch::IsUnacceptableBranch(Branch &branch) {
    int firstNode = branch.GetFirstNode(), secondNode = branch.GetSecondNode();

    bool isFirstNodeUnacceptable = firstNode < 0 || firstNode > n,
            isSecondNodeUnacceptable = secondNode < 0 || secondNode > n;
    return isFirstNodeUnacceptable || isSecondNodeUnacceptable;
}

bool Branch::IsZero() const {
    return _C.empty();
}

bool Branch::IsUnity() {
    return _C.size() == 1 && _C.front() == 1;
}

bool Branch::IsSimpleBranch() {
    return _power == 1;
}

void Branch::PrintBranch() {
    if (!IsZero()) {
        std::cout << "Branch:" << std::endl;
        for (auto &item : _C) {
            std::cout << item << " ";
        }
        std::cout << std::endl << "power=" << _power << std::endl;
    } else {
        std::cout << "empty edge" << std::endl;
    }
}

double Branch::GetValue() {
    double value = 0;
    if (IsZero()) {
        return value;
    }
    for (int i = 0; i <= _power; i++) {
        value += _C[i] * pow(p, _power - i) * pow(z, i);
    }

    return value;
}

Branch Branch::ParallelReduction(std::vector<Branch> &branches) {
    int size = branches.size();
    branches.erase(std::remove_if(branches.begin(), branches.end(), [](Branch &item) ->
            bool { return item.IsSimpleBranch(); }), branches.end());
    int simpleBranchesCount = size - branches.size();
    Branch parallelBranch = Bin[simpleBranchesCount];
    parallelBranch.GetC()[simpleBranchesCount] = 0;
    Branch result = parallelBranch;
    for (auto &item : branches) {
        result = item + result - item *result;
    }
    return result;
}

bool Branch::operator <(const Branch& branch) const {
    return std::min(_firstNode, _secondNode) < std::min(branch._firstNode, branch._secondNode);
}

Branch operator *(Branch firstBranch, Branch secondBranch)
{
    Branch result = Branch::GetZero();
    if (!firstBranch.IsZero() && !secondBranch.IsZero()) {
        if (firstBranch.IsUnity()) {
            return secondBranch;
        }
        if (secondBranch.IsUnity()) {
            return firstBranch;
        }

        result.GetC().resize(firstBranch.GetC().size());
        result.SetPower(firstBranch.GetPower() + secondBranch.GetPower());
        for (int i = 0; i < firstBranch.GetC().size(); i++) {
            for (int j = 0; j < secondBranch.GetC().size(); j++) {
                if (firstBranch.GetC()[i] != 0 && secondBranch.GetC()[j] != 0) {
                    if (i + j >= result.GetC().size()) {
                        throw "Branch operator *: vector is out of size";
                    }
                    result.GetC()[i + j] += firstBranch.GetC()[i] * secondBranch.GetC()[j];
                }
            }
        }
        result.SetIsReliable(false);
    }

    return result;
}

Branch operator +(Branch firstBranch, Branch secondBranch)
{
    Branch result = Branch::GetZero();
    if (!firstBranch.IsZero() && !secondBranch.IsZero()) {
        if (firstBranch.IsUnity() && secondBranch.IsUnity()) {
            throw "Branch operator +: unity + unity";
        }

        result.GetC().resize(firstBranch.GetC().size());
        // Multiply by a unit of the required degree, so that the degrees x and y coincide
        if (firstBranch.GetPower() != secondBranch.GetPower()) {
            Branch I = Bin[abs(firstBranch.GetPower() - secondBranch.GetPower())];
            if (firstBranch.GetPower() < secondBranch.GetPower()) {
                firstBranch = firstBranch * I;
            } else {
                secondBranch = secondBranch * I;
            }
        }

        result.SetPower(firstBranch.GetPower());
        for (int i = 0; i<result.GetC().size(); i++) {
            result.GetC()[i] = firstBranch.GetC()[i] + secondBranch.GetC()[i];
        }
        result.SetIsReliable(false);
    } else if (!firstBranch.IsZero() && secondBranch.IsZero()) {
        return firstBranch;
    } else if (firstBranch.IsZero() && !secondBranch.IsZero()) {
        return secondBranch;
    }

    return result;
}

Branch operator -(Branch firstBranch, Branch secondBranch)
{
    Branch result = Branch::GetZero();
    if (!firstBranch.IsZero() && !secondBranch.IsZero()) {
        if (firstBranch.IsUnity() && secondBranch.IsUnity()) {
            throw "Branch operator -: unity - unity";
        }

        result.GetC().resize(firstBranch.GetC().size());
        // Multiply by a unit of the required degree, so that the degrees x and y coincide
        if (firstBranch.GetPower() != secondBranch.GetPower()) {
            Branch I = Bin[abs(firstBranch.GetPower() - secondBranch.GetPower())];
            if (firstBranch.GetPower() < secondBranch.GetPower()) {
                firstBranch = firstBranch * I;
            }
            if (secondBranch.GetPower() < firstBranch.GetPower()) {
                secondBranch = secondBranch * I;
            }
        }

        result.SetPower(firstBranch.GetPower());
        for (int i = 0; i<result.GetC().size(); i++) {
            result.GetC()[i] = firstBranch.GetC()[i] - secondBranch.GetC()[i];
        }
        result.SetIsReliable(false);
    } else if (!firstBranch.IsZero() && secondBranch.IsZero()) {
        return firstBranch;
    } else if (firstBranch.IsZero() && !secondBranch.IsZero()) {
        return secondBranch;
    }

    return result;
}

Branch operator ~(Branch branch)
{
    Branch result = Branch::GetZero();
    if (!branch.IsZero()) {
        if (branch.IsUnity()) {
            return result;
        }
        result.GetC().resize(branch.GetC().size());
        // Subtract from unity the same degree as x
        result.SetPower(branch.GetPower());
        Branch I = Bin[result.GetPower()];
        for (int i = 0; i<result.GetC().size(); i++) {
            result.GetC()[i] = I.GetC()[i] - branch.GetC()[i];
        }
        result.SetIsReliable(false);
    } else {
        return Branch::GetUnity();
    }

    return result;
}

bool operator ==(Branch firstBranch, Branch secondBranch)
{
    return firstBranch.GetId() == secondBranch.GetId();
}

bool operator !=(Branch firstBranch, Branch secondBranch)
{
    return firstBranch.GetId() != secondBranch.GetId();
}