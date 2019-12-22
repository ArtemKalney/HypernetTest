#include "Branch.h"
#include "Globals.h"

//todo возможно надо заменить на GetUnity
Branch Branch::GetElement(const int &power) {
    std::vector<double> C(m);
    C.front() = 1;
    std::vector<Route> routes;
    double value = std::pow(p, power);
    return Branch(-1, value, C, routes, power, 0, 0, false);
}

Branch Branch::GetElement(const std::vector<double> &C, const int &power) {
    std::vector<Route> routes;
    double value = std::pow(p, power);
    return Branch(-1, value, C, routes, power, 0, 0, false);
}

Branch Branch::GetSimpleElement() {
    return GetSimpleElement(-1, -1, -1);
}

Branch Branch::GetSimpleElement(const int &id, const int &firstNode, const int &secondNode) {
    std::vector<double> C(m);
    C.front() = 1;
    std::vector<Route> routes;
    double value = p;
    return Branch(id, value, C, routes, 1, firstNode, secondNode, false);
}

Branch Branch::GetZero() {
    std::vector<double> C;
    std::vector<Route> routes;
    double value = 0;
    return Branch(-1, value, C, routes, 0, 0, 0, false);
}

Branch Branch::GetUnity() {
    std::vector<double> C;
    C.push_back(1);
    std::vector<Route> routes;
    double value = 1;
    return Branch(-1, value, C, routes, 0, 0, 0, false);
}

bool Branch::EqualNodes(const Branch& firstBranch, const Branch& secondBranch) {
    auto sameNodes = firstBranch.GetFirstNode() == secondBranch.GetFirstNode() &&
                     firstBranch.GetSecondNode() == secondBranch.GetSecondNode();
    auto reversedNodes = firstBranch.GetFirstNode() == secondBranch.GetSecondNode() &&
                         firstBranch.GetSecondNode() == secondBranch.GetFirstNode();
    return sameNodes || reversedNodes;
}

bool Branch::IsUnacceptableElement() {
    bool isFirstNodeUnacceptable = _firstNode < 0 || _secondNode > n,
            isSecondNodeUnacceptable = _secondNode < 0 || _firstNode > n,
            isPowerUnacceptable = GetPower() < 0 || GetPower() > m;
    return isFirstNodeUnacceptable || isSecondNodeUnacceptable || isPowerUnacceptable;
}

bool Branch::IsZero() {
    return IS_NUMBER_COMPUTATION == 1 ? GetValue() == 0 : GetC().empty();
}

bool Branch::IsUnity() {
    return IS_NUMBER_COMPUTATION == 1 ? GetValue() == 1 : GetC().size() == 1 && GetC().front() == 1;
}

bool Branch::IsSimpleBranch() {
    return GetPower() == 1;
}

void Branch::PrintBranch() {
    if (IS_NUMBER_COMPUTATION == 1) {
        return;
    }
    if (!IsZero()) {
        std::cout << "Branch:" << std::endl;
        for (auto &item : GetC()) {
            std::cout << item << " ";
        }
        std::cout << std::endl << "power=" << GetPower() << std::endl;
    } else {
        std::cout << "empty edge" << std::endl;
    }
}

double Branch::GetPolynomialValue(double point) {
    if (IS_NUMBER_COMPUTATION == 1) {
        return GetValue();
    }

    double value = 0;
    if (IsZero()) {
        return value;
    }
    for (int i = 0; i <= GetPower(); i++) {
        value += GetC()[i] * pow(point, GetPower() - i) * pow(1 - point, i);
    }

    return value;
}

int Branch::GetBranchSaturation() {
    int count = 0;
    for(auto &item : _routes) {
        if (!item.Ptr->empty()) {
            count++;
        }
    }

    return count;
}

bool operator !=(Branch firstBranch, Branch secondBranch) {
    return firstBranch.GetId() != secondBranch.GetId();
}