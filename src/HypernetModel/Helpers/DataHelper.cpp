#include <Globals.h>
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
    int buf;
    input >> buf; n = buf;
    input >> buf; m = buf;
    input >> buf; k = buf;
    // Read all nodes from input.txt
    for (int i = 0; i < n; i++) {
        input >> buf; int nodeNumber = buf - 1;
        double doubleBuf;
        input >> doubleBuf; double value = doubleBuf;
        Node node = Node::GetSimpleElement(nodeNumber, value, false);
        nodes.push_back(node);
        if (!IsUniqueId(nodes, nodeNumber)) {
            throw "GetData: not unique branch id";
        }
    }
    std::vector<std::vector<int>> branchRouteIds;
    // Read all branches from input.txt
    for (int i = 0; i < m; i++) {
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
        branches.push_back(Branch::GetSimpleElement(id, firstNode, secondNode));
        if (!IsUniqueId(branches, id)) {
            throw "GetData: not unique branch id";
        }
    }
    // Read all routes from input.txt
    for (int i = 0; i < k; i++) {
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

void ComputeBinomialCoefficients() {
    Bin.resize(m + 1);
    for(auto &item : Bin) {
        item.resize(m + 1);
        item.front() = 1;
    }
    for (int i = 0; i < Bin.size(); i++) {
        if (i != 0) {
            for (int j = 1; j < m + 1; j++) {
                Bin[i][j] = Bin[i - 1][j - 1] + Bin[i - 1][j];
            }
        }
    }
}

void ErrorHandler(const char *str) {
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Occurred next error:" << std::endl;
    std::cout << str << std::endl;
    std::cout << "--------------------------------" << std::endl;
}