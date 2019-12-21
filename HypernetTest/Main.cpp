#include "Globals.h"
#include "Funcs.h"
#include <gtest/gtest.h>
#include "ComputeMENC.h"
#include "ComputeAPC.h"

std::ifstream input;
std::ofstream output;
int n = 0, m = 0, k = 0;
int ReliableHypernets = 0, UnconnectedHypernets = 0, TwoNodesHypernets = 0, ChainsReduced = 0,
        UnconnectedNodesReduced = 0, PairConnectivityCalls = 0, EdgesReduced = 0, ComplexChains = 0,
        TreeNodeIntersections = 0, UnconnectedTreeNodes = 0;
std::vector<Branch> Bin;
std::vector<int> KpNodesCombination{0, 6, 12, 18, 24};
const double p = 0.9;

template <class T>
bool IsUniqueId(const T &items, const int &id) {
    int count = 0;
    for(auto &item : items) {
        if (item.GetId() == id) {
            count++;
        }
    }
    return count < 2;
}

void GetData(std::vector<Branch>& branches, std::vector<Node>& nodes, std::vector<Route>& routes) {
    if (!input.is_open()) {
        throw "GetData: File can not be opened!";
    }
    char str[50];
    input.getline(str, 50);
    if (IS_TEST == 0) {
        std::cout << "Input graph : " << str << std::endl;
    }
    int buf;
    input >> buf; n = buf;
    input >> buf; m = buf;
    input >> buf; k = buf;
    // Read all nodes from input.txt
    for (int i = 0; i < n; i++) {
        input >> buf; int nodeNumber = buf - 1;
        double doubleBuf;
        input >> doubleBuf; double value = doubleBuf;
        Node node = Node::GetSimpleBranch(nodeNumber, value, false);
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
        branches.push_back(Branch::GetSimpleBranch(id, firstNode, secondNode));
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
    Bin.resize(m + 1, Branch::GetBranch(m + 1, 0));
    for (int i = 0; i < Bin.size(); i++) {
        Bin[i].SetPower(i);
        if (i != 0) {
            for (int j = 1; j < m + 1; j++) {
                Bin[i].GetC()[j] = Bin[i - 1].GetC()[j - 1] + Bin[i - 1].GetC()[j];
            }
        }
    }
}
//todo not implemented
void NormalizeSolution(Node &node){
}

void NormalizeSolution(Branch &branch){
    branch.GetC().resize(m + 1);
    if (branch.GetPower() < m) {
        branch = branch * Bin[m - branch.GetPower()];
    }
}

template <class T>
void ComputePairConnectivities(T& sum, const H& initialHypernet) {
    // All connectivities
    if (IS_DEBUG != 1) {
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                auto H = initialHypernet;
                if (i != 0 || j != 1) {
                    if (i != 0 && j != 1) {
                        H.RenumerateNodes(i, 0);
                        H.RenumerateNodes(j, 1);
                    }
                    if (i == 0 && j != 1) {
                        H.RenumerateNodes(j, 1);
                    }
                    if (i != 0 && j == 1) {
                        H.RenumerateNodes(i, 0);
                    }
                }
                T result;
                if (IS_FULL_ENUMERATION != 1) {
                    result = PairConnectivity<T>(H);
                } else if (IS_FULL_ENUMERATION == 1) {
                    sum = sum + FullEnumeration<T>(H);
                }
                NormalizeSolution(result);
                for (auto &item : result.GetC()) {
                    output << std::setprecision(14) << item << " ";
                }
                output << std::endl;
            }
        }
    } else if (IS_FULL_ENUMERATION != 1) {
        H H = initialHypernet;
        H.RenumerateNodes(DEBUG_FIRST_NODE, 0);
        H.RenumerateNodes(DEBUG_SECOND_NODE, 1);
        auto result = PairConnectivity<T>(H);
        sum = sum + result;
    } else if (IS_FULL_ENUMERATION == 1) {
        auto H = initialHypernet;
        H.RenumerateNodesForGen(DEBUG_FIRST_NODE, 0);
        H.RenumerateNodesForGen(DEBUG_SECOND_NODE, 1);
        sum = sum + FullEnumeration<T>(H);
    }
}

void ErrorHandler(const char *str) {
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Occurred next error:" << std::endl;
    std::cout << str << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

template <class T>
void PrintSolution(T& branchSum) {
    if (!branchSum.IsZero()) {
        NormalizeSolution(branchSum);
        std::cout << "Value at point " << p << ": " << std::setprecision(14) << branchSum.GetPolynomialValue(p) << std::endl;
        for (auto &item : branchSum.GetC()) {
            output << std::setprecision(14) << item << " ";
        }
        output << std::endl;
    } else {
        std::cout << "unconnected hypernet" << std::endl;
    }
}

int main(int argc, char** argv) {
    if (IS_TEST == 1) {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }

    input.open("input.txt");
    output.open("output.txt");
    setlocale(LC_ALL, "");

    std::vector<Branch> branches;
    std::vector<Node> nodes;
    std::vector<Route> routes;
    try {
        GetData(branches, nodes, routes);
    } catch (const char *str) {
        ErrorHandler(str);
        if (IS_DEBUG != 1) {
            system("pause>>void");
        }
        return 0;
    }

    int option;
    if (IS_OPTIMIZATION != 1) {
        std::cout << "Press 1 to get APC polynomial" << std::endl;
        std::cout << "Press 2 to get MENC polynomial" << std::endl;
        std::cout << "Press 3 to get pairwise connectivities" << std::endl;
        std::cin >> option;
        if (option != 1 && option != 2 && option != 3) {
            std::cout << "Wrong number" << std::endl;
            if (IS_DEBUG != 1) {
                system("pause>>void");
            }
            return 0;
        }
    }
    // Create an initialHypernet
    H initialHypernet;
    if (IS_OPTIMIZATION == 1) {
        initialHypernet = GetRandomHypernet(branches, nodes);
        initialHypernet.LogHypernet();
        output << "FirstRoot " << OPTIMIZATION_FIRST_TREE_ROOT + 1 << std::endl;
        output << "SecondRoot " << OPTIMIZATION_SECOND_TREE_ROOT + 1 << std::endl;
        output << "TreeNodeIntersections " << TreeNodeIntersections << std::endl;
        output << "UnconnectedTreeNodes " << UnconnectedTreeNodes << std::endl;
        return 0;
    } else {
        initialHypernet = H(std::move(branches), std::move(nodes), std::move(routes));
    }
    initialHypernet.RemoveEmptyBranches();

    ComputeBinomialCoefficients();
    Branch branchSum;
    Node nodeSum;
    int startTime = clock();
    try {
        if (option == 1) {
            if (IS_NODES_RELIABLE == 1) {
                ComputeAPC(branchSum, initialHypernet);
            } else {
                ComputeAPC(nodeSum, initialHypernet);
            }
        }
        if (option == 2) {
            if (IS_NODES_RELIABLE == 1) {
                ComputeMENC(branchSum, initialHypernet);
            } else {
                ComputeMENC(nodeSum, initialHypernet);
            }
        }
        if (option == 3) {
            if (IS_NODES_RELIABLE == 1) {
                ComputePairConnectivities(branchSum, initialHypernet);
            } else {
                ComputePairConnectivities(nodeSum, initialHypernet);
            }
            if (IS_DEBUG != 1) {
                return 0;
            }
        }

    } catch (const std::overflow_error &e) {
        std::cout << "throw std::overflow_error" << std::endl << e.what();
    } catch (const std::runtime_error &e) {
        std::cout << "throw std::underflow_error" << std::endl << e.what();
    } catch (const std::exception &e) {
        std::cout << "throw std::logic_error" << std::endl << e.what();
    } catch (const char *str) {
        ErrorHandler(str);
    } catch (...) {
        std::cout << "throw std::string or int or any other unrelated type";
    }

    int searchTime = clock() - startTime;
    std::cout << "Time of programm " << searchTime << " msec" << std::endl;
    if (IS_FULL_ENUMERATION != 1) {
        std::cout << "PairConnectivityCalls " << PairConnectivityCalls << std::endl;
        std::cout << "Reductions : " << std::endl;
        std::cout << " UnconnectedNodesReduced " << UnconnectedNodesReduced << std::endl;
        std::cout << " EdgesReduced " << EdgesReduced << std::endl;
        if (IS_NODES_RELIABLE == 1) {
            std::cout << " ChainsReduced " << ChainsReduced << std::endl;
            if (IS_DEBUG == 1) {
                std::cout << " ComplexChains " << ComplexChains << std::endl;
            }
        }
    }
    std::cout << "Were ends of recursion : " << ReliableHypernets + UnconnectedHypernets +
                                                TwoNodesHypernets << std::endl;
    std::cout << " ReliableHypernets " << ReliableHypernets << std::endl;
    std::cout << " UnconnectedHypernets " << UnconnectedHypernets << std::endl;
    if (IS_FULL_ENUMERATION != 1) {
        std::cout << " TwoNodesHypernets " << TwoNodesHypernets << std::endl;
    }
    if (IS_NODES_RELIABLE == 1) {
        PrintSolution(branchSum);
    } else {
        PrintSolution(nodeSum);
    }

    input.close();
    output.close();
    if (IS_DEBUG != 1) {
        system("pause>>void");
    }
    return 0;
}