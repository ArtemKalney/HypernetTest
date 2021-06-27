#include "Globals.h"
#include "Funcs.h"
#include <gtest/gtest.h>
#include "ComputeMENC.h"
#include "ComputeAPC.h"
#include "../HypernetModel/Helpers/DataHelper.h"

std::ifstream input;
std::ofstream output;
int n = 0, m = 0, k = 0;
int ReliableHypernets = 0, UnconnectedHypernets = 0, TwoNodesHypernets = 0, ChainsReduced = 0,
        UnconnectedNodesReduced = 0, PairConnectivityCalls = 0, EdgesReduced = 0, ComplexChains = 0,
        TreeNodeIntersections = 0, UnconnectedTreeNodes = 0;
std::vector<std::vector<double>> Bin;
std::vector<int> KpNodesCombination;
const double p = 0.9;
int seed = time(0);
int FirstRoot, SecondRoot;

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

int main(int argc, char** argv) {
    if (IS_TEST == 1) {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }

    input.open("input.txt");
    output.open("output.txt");
    setlocale(LC_ALL, "");
    try {
        std::vector<Branch> branches;
        std::vector<Node> nodes;
        std::vector<Route> routes;
        GetData(branches, nodes, routes);
        int option;
        if (IS_RANDOM_HYPERNET != 1) {
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
        if (IS_RANDOM_HYPERNET == 1) {
            if (IS_RANDOM_AOSH == 1) {
                FirstRoot = RANDOM_FIRST_TREE_ROOT;
                SecondRoot = RANDOM_SECOND_TREE_ROOT;
                initialHypernet = GetAoshRandomHypernet(branches, nodes);
                output << "FirstRoot " << RANDOM_FIRST_TREE_ROOT + 1 << std::endl;
                output << "SecondRoot " << RANDOM_SECOND_TREE_ROOT + 1 << std::endl;
                output << "TreeNodeIntersections " << TreeNodeIntersections << std::endl;
                output << "UnconnectedTreeNodes " << UnconnectedTreeNodes << std::endl;
            } else if (IS_RANDOM_KP == 1) {
                KpNodesCombination = {0, 6, 12, 18, 24};
                initialHypernet = GetKpRandomHypernet(branches, nodes);
            } else {
                k = n + RANDOM_ADDITIONAL_EDGES;
                if (IS_RANDOM_SN == 1) {
                    initialHypernet = GetRandomHypernet(branches, nodes);
                } else {
                    initialHypernet = GetRandomHypernet();
                }
            }
            initialHypernet.LogHypernet();
            return 0;
        } else {
            initialHypernet = H(std::move(branches), std::move(nodes), std::move(routes));
        }
        initialHypernet.RemoveEmptyBranches();
        ComputeBinomialCoefficients();
        Branch branchSum = Branch::GetZero();
        Node nodeSum = Node::GetZero();
        int startTime = clock();
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

    input.close();
    output.close();
    if (IS_DEBUG != 1) {
        system("pause>>void");
    }
    return 0;
}