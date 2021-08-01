#include "Node.h"
#include "Globals.h"

Node Node::GetElement(const int power, const int vectorSize) {
    std::vector<double> C(vectorSize);
    C.front() = 1;
    double value = std::pow(p, power);
    return Node(-1, value, C, power, false, false);
}

Node Node::GetElement(const std::vector<double>& C, const int power) {
    double value = std::pow(p, power);
    return Node(-1, value, C, power, false, false);
}

Node Node::GetSimpleElement(const int id, double value, bool isVisited, const int vectorSize) {
    std::vector<double> C(vectorSize);
    C.front() = 1;
    return Node(id, value, C, 1, isVisited, false);
}

Node Node::GetSimpleElement(const int vectorSize) {
    return GetSimpleElement(-1, p, false, vectorSize);
}

Node Node::GetZero() {
    std::vector<double> C;
    double value = 0;
    return Node(-1, value, C, 0, false, false);
}

Node Node::GetUnity() {
    std::vector<double> C;
    C.push_back(1);
    double value = 1;
    return Node(-1, value, C, 0, false, false);
}

bool Node::IsUnity() {
    return IS_NUMBER_COMPUTATION == 1 ? GetValue() == 1 : GetC().size() == 1 && GetC().front() == 1;
}

bool Node::IsZero() const {
    return IS_NUMBER_COMPUTATION == 1 ? GetValue() == 0 : _C.empty();
}

bool Node::IsPivotNode() {
    return _id == 0 || _id == 1;
}

double Node::GetPolynomialValue(double point) {
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