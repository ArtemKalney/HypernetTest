#include "Globals.h"
#include "Funcs.h"
#include "../HypernetModel/Helpers/DataHelper.h"
#include "../HypernetModel/Hypernet.h"
#include "../HypernetTest/ComputeAPC.h"
#include "../HypernetTest/ComputeMENC.h"

std::ifstream input;
std::ofstream output;
const double p = 0.9;
int n = 0, m = 0, k = 0;
int ReliableHypernets = 0, UnconnectedHypernets = 0, TwoNodesHypernets = 0, ChainsReduced = 0,
        UnconnectedNodesReduced = 0, PairConnectivityCalls = 0, EdgesReduced = 0, ComplexChains = 0,
        TreeNodeIntersections = 0, UnconnectedTreeNodes = 0;
std::vector<std::vector<double>> Bin;
int seed = time(0);

int main(int argc, char** argv) {
    input.open("input.txt");
    output.open("output.txt");
    setlocale(LC_ALL, "");
    try {
        std::vector<Branch> branches;
        std::vector<Node> nodes;
        std::vector<Route> routes;
        GetData(branches, nodes, routes);
        // Create an initialHypernet
        H initialHypernet;
        initialHypernet = GetRandomNetworkHypernet(branches, nodes);
        initialHypernet.RemoveEmptyBranches();
        ComputeBinomialCoefficients();
        Branch branchSum = Branch::GetZero();
        Node nodeSum = Node::GetZero();
        int startTime = clock();
        if (IS_NODES_RELIABLE == 1) {
            ComputeMENC(branchSum, initialHypernet);
        } else {
            ComputeMENC(nodeSum, initialHypernet);
        }
        int searchTime = clock() - startTime;
        std::cout << "Time of programm " << searchTime << " msec" << std::endl;
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
        std::cout << "Were ends of recursion : " << ReliableHypernets + UnconnectedHypernets +
                                                    TwoNodesHypernets << std::endl;
        std::cout << " ReliableHypernets " << ReliableHypernets << std::endl;
        std::cout << " UnconnectedHypernets " << UnconnectedHypernets << std::endl;
        std::cout << " TwoNodesHypernets " << TwoNodesHypernets << std::endl;
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