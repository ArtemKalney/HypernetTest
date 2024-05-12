#include "Globals.h"
#include "../Route.h"
#include "../Node.h"
#include "../Branch.h"
#include "DataHelper.h"

void GetData(std::vector<Branch>& branches, std::vector<Node>& nodes, std::vector<Route>& routes) {
    if (!input.is_open()) {
        throw "GetData: File can not be opened!";
    }
    char str[50];
    input.getline(str, 50);
    std::cout << "Input graph : " << str << std::endl;
    int buf, nodeSize, branchSize, routeSize;
    double doubleBuf;
    input >> buf; nodeSize = buf;
    input >> buf; branchSize = buf;
    input >> buf; routeSize = buf;
    // Read all nodes from input.txt
    for (int i = 0; i < nodeSize; i++) {
        input >> buf; int nodeNumber = buf - 1;
        Node node = Node::GetSimpleElement(nodeNumber, false, branchSize);
        if (AppSettings.InputNodesValues == 1) {
            input >> doubleBuf;
            node.SetValue(doubleBuf);
        }
        nodes.push_back(node);
        if (!IsUniqueId(nodes, nodeNumber)) {
            throw "GetData: not unique branch id";
        }
    }
    std::vector<std::vector<int>> branchRouteIds;
    // Read all branches from input.txt
    for (int i = 0; i < branchSize; i++) {
        input >> buf; int id = buf;
        input >> buf; int firstNode = buf - 1;
        input >> buf; int secondNode = buf - 1;
        std::vector<int> vector;
        input >> buf;
        while (buf != 0) {
            int routeId = buf;
            vector.push_back(routeId);
            input >> buf;
        }
        branchRouteIds.push_back(vector);
        Branch branch = Branch::GetSimpleElement(id, firstNode, secondNode, branchSize);
        if (AppSettings.InputBranchValues == 1) {
            input >> doubleBuf;
            branch.SetValue(doubleBuf);
        }
        if (AppSettings.InputBranchCosts == 1) {
            input >> doubleBuf;
            branch.SetCost(doubleBuf);
        }
        if (AppSettings.InputMaxBranchSaturations == 1) {
            input >> buf;
            branch.SetMaxSaturation(buf);
        }
        branches.push_back(branch);
        if (!IsUniqueId(branches, id)) {
            throw "GetData: not unique branch id";
        }
    }
    // Read all routes from input.txt
    for (int i = 0; i < routeSize; i++) {
        std::vector<int> vector;
        input >> buf; int id = buf;
        input >> buf;
        while (buf != 0) {
            int node = buf - 1;
            vector.push_back(node);
            input >> buf;
        }

        auto ptr = std::make_shared<std::vector<int>>(vector);
        routes.emplace_back(id, ptr);
        if (!IsUniqueId(routes, id)) {
            throw "GetData: not unique route id";
        }
    }
    // Fill branch Routes by ids
    for (int i = 0; i < branches.size(); i++) {
        auto vector = branchRouteIds[i];
        for (auto &routeId : vector) {
            auto it = std::find_if(routes.begin(), routes.end(), [routeId](Route &item) ->
                    bool { return routeId == item.Id; });
            if (it != routes.end()) {
                branches[i].GetRoutes().push_back(routes[it - routes.begin()]);
            }
        }
    }
    // Input should end by $$$
    input >> str;
    if (strcmp(str, "$$$") != 0) {
        throw "GetData: Incorrect entry";
    }
}

void ComputeBinomialCoefficients(const int vectorSize) {
    Bin.resize(vectorSize + 1);
    for(auto &item : Bin) {
        item.resize(vectorSize + 1);
        item.front() = 1;
    }
    for (int i = 0; i < Bin.size(); i++) {
        if (i != 0) {
            for (int j = 1; j < vectorSize + 1; j++) {
                Bin[i][j] = Bin[i - 1][j - 1] + Bin[i - 1][j];
            }
        }
    }
}

double Factorial(int n) {
    return (n == 1 || n == 0) ? 1 : Factorial(n - 1) * n;
}

void ComputeFactorials(const int vectorSize) {
    for(int i=0; i<vectorSize; i++) {
        Factorials.push_back(Factorial(i));
    }
}

void ErrorHandler(const char *str) {
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Occurred next error:" << std::endl;
    std::cout << str << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void HandleException(const std::exception &e) {
    std::cerr << e.what() << std::endl;
    output << e.what() << std::endl;
}

bool DoubleEquals(double a, double b, double epsilon)
{
    return std::abs(a - b) < epsilon;
}

double GetPolynomialValue(const std::vector<double> &vector, int power, double point) {
    double value = 0;
    for (int i = 0; i <= power; i++) {
        value += vector[i] * pow(point, power - i) * pow(1 - point, i);
    }

    return value;
}