#include "Node.h"
#include "Globals.h"

Node Node::GetElement(const int power, const int vectorSize) {
    Node node = Node();

    std::vector<double> C(vectorSize);
    C.front() = 1;
    node.SetC(C);

    node.SetId(-1);
    node.SetValue(std::pow(AppSettings.ReliabilityValue, power));
    node.SetPower(power);
    node.SetIsVisited(false);
    node.SetIsReliable(false);

    return node;
}

Node Node::GetElement(std::vector<double>& C, const int power) {
    Node node = Node();

    node.SetId(-1);
    node.SetValue(std::pow(AppSettings.ReliabilityValue, power));
    node.SetC(C);
    node.SetPower(power);
    node.SetIsVisited(false);
    node.SetIsReliable(false);

    return node;
}

Node Node::GetSimpleElement(const int id, bool isVisited, const int vectorSize) {
    Node node = Node();

    std::vector<double> C(vectorSize);
    C.front() = 1;
    node.SetC(C);

    node.SetId(id);
    node.SetValue(AppSettings.ReliabilityValue);
    node.SetPower(1);
    node.SetIsVisited(isVisited);
    node.SetIsReliable(false);

    return node;
}

Node Node::GetSimpleElement(const int vectorSize) {
    return GetSimpleElement(-1, false, vectorSize);
}

Node Node::GetZero() {
    Node node = Node();

    std::vector<double> C;
    node.SetC(C);

    node.SetId(-1);
    node.SetValue(0);
    node.SetPower(0);
    node.SetIsVisited(false);
    node.SetIsReliable(false);

    return node;
}

Node Node::GetUnity() {
    Node node = Node();

    std::vector<double> C;
    C.push_back(1);
    node.SetC(C);

    node.SetId(-1);
    node.SetValue(1);
    node.SetPower(0);
    node.SetIsVisited(false);
    node.SetIsReliable(false);

    return node;
}

bool Node::IsUnity() {
    return AppSettings.IsNumberComputation == 1 ? GetValue() == 1 : GetC().size() == 1 && GetC().front() == 1;
}

bool Node::IsZero() const {
    return AppSettings.IsNumberComputation == 1 ? GetValue() == 0 : _C.empty();
}

bool Node::IsPivotNode() {
    return _id == 0 || _id == 1;
}

double Node::GetPolynomialValue(double point) {
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