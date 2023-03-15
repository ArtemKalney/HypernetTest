#include "Branch.h"
#include "Globals.h"

//todo возможно надо заменить на GetUnity
Branch Branch::GetElement(const int power, const int vectorSize) {
    Branch branch = Branch();

    std::vector<double> C(vectorSize);
    C.front() = 1;
    branch.SetC(C);

    std::vector<Route> routes;
    branch.SetRoutes(routes);

    branch.SetId(-1);
    branch.SetValue(std::pow(p, power));
    branch.SetPower(power);
    branch.SetFirstNode(0);
    branch.SetSecondNode(0);
    branch.SetIsReliable(false);

    return branch;
}

Branch Branch::GetElement(const std::vector<double>& C, const int power) {
    Branch branch = Branch();

    std::vector<Route> routes;
    branch.SetRoutes(routes);

    branch.SetId(-1);
    branch.SetValue(std::pow(p, power));
    branch.SetC(C);
    branch.SetPower(power);
    branch.SetFirstNode(0);
    branch.SetSecondNode(0);
    branch.SetIsReliable(false);

    return branch;
}

Branch Branch::GetSimpleElement(const int vectorSize) {
    return GetSimpleElement(-1, -1, -1, vectorSize);
}

Branch Branch::GetSimpleElement(const int id, const int firstNode, const int secondNode, const int vectorSize) {
    Branch branch = Branch();

    std::vector<double> C(vectorSize);
    C.front() = 1;
    branch.SetC(C);

    std::vector<Route> routes;
    branch.SetRoutes(routes);

    branch.SetId(id);
    branch.SetValue(p);
    branch.SetPower(1);
    branch.SetFirstNode(firstNode);
    branch.SetSecondNode(secondNode);
    branch.SetIsReliable(false);

    return branch;
}

Branch Branch::GetZero() {
    Branch branch = Branch();

    std::vector<double> C;
    branch.SetC(C);

    std::vector<Route> routes;
    branch.SetRoutes(routes);

    branch.SetId(-1);
    branch.SetValue(0);
    branch.SetPower(0);
    branch.SetFirstNode(0);
    branch.SetSecondNode(0);
    branch.SetIsReliable(false);

    return branch;
}

Branch Branch::GetUnity() {
    Branch branch = Branch();

    std::vector<double> C;
    C.push_back(1);
    branch.SetC(C);

    std::vector<Route> routes;
    branch.SetRoutes(routes);

    branch.SetId(-1);
    branch.SetValue(1);
    branch.SetPower(0);
    branch.SetFirstNode(0);
    branch.SetSecondNode(0);
    branch.SetIsReliable(false);

    return branch;
}

bool Branch::EqualNodes(const Branch& firstBranch, const Branch& secondBranch) {
    auto sameNodes = firstBranch.GetFirstNode() == secondBranch.GetFirstNode() &&
                     firstBranch.GetSecondNode() == secondBranch.GetSecondNode();
    auto reversedNodes = firstBranch.GetFirstNode() == secondBranch.GetSecondNode() &&
                         firstBranch.GetSecondNode() == secondBranch.GetFirstNode();
    return sameNodes || reversedNodes;
}

bool Branch::IsZero() {
    return AppSettings.IsNumberComputation == 1 ? GetValue() == 0 : GetC().empty();
}

bool Branch::IsUnity() {
    return AppSettings.IsNumberComputation == 1 ? GetValue() == 1 : GetC().size() == 1 && GetC().front() == 1;
}

bool Branch::IsSimpleBranch() {
    return GetPower() == 1;
}

void Branch::PrintBranch() {
    if (AppSettings.IsNumberComputation == 1) {
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
    if (AppSettings.IsNumberComputation == 1) {
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

int Branch::GetSaturation() {
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

bool operator <(Branch firstBranch, Branch secondBranch) {
    return firstBranch.GetId() < secondBranch.GetId();
}