#include "Globals.h"
#include "Funcs.h"
#include "../HypernetModel/Helpers/DataHelper.h"
#include "../HypernetModel/Hypernet.h"
#include "../HypernetTest/ComputeAPC.h"
#include "../HypernetTest/ComputeMENC.h"
#include "Model .h"
#include "FullEnumerationAlgorithm.h"
#include "GeneticAlgorithm.h"

std::ifstream input;
std::ofstream output;
const double p = 0.9;
int ReliableHypernets = 0, UnconnectedHypernets = 0, TwoNodesHypernets = 0, ChainsReduced = 0,
        UnconnectedNodesReduced = 0, PairConnectivityCalls = 0, EdgesReduced = 0, ComplexChains = 0,
        TreeNodeIntersections = 0, UnconnectedTreeNodes = 0;
std::vector<std::vector<double>> Bin;
int seed = time(0);
int CheckedConditions, UncheckedConditions, GeneticAlgorithmIterations;

void OutputResult(const std::shared_ptr<Model> model, int startTime) {
    output << "Solution:" << VectorToString(model->GetSolution()) << std::endl;
    output << "Reliability:" << model->GetReliability() << std::endl;
    output << "ObjFunctionValue:" << model->GetObjFunctionValue() << std::endl;
    output << "Statistics:" << std::endl;
    output << std::string(3, ' ') << "Time:" << clock() - startTime << std::endl;
    if (IS_FULL_ENUMERATION_ALGORITHM == 1) {
        output << std::string(3, ' ') << "CheckedConditions:" << CheckedConditions << std::endl;
        output << std::string(3, ' ') << "UncheckedConditions:" << UncheckedConditions << std::endl;
    }
    if (IS_GENETIC_ALGORITHM == 1) {
        output << std::string(3, ' ') << "GeneticAlgorithmIterations:" << GeneticAlgorithmIterations << std::endl;
    }
}

int main(int argc, char** argv) {
    input.open("input.txt");
    output.open("output.txt");
    setlocale(LC_ALL, "");
    std::vector<Branch> branches;
    std::vector<Node> nodes;
    std::vector<Route> routes;
    H initialHypernet;
    try {
        GetData(branches, nodes, routes);
    }
    catch (std::exception const &e) {
        HandleException(e);

        return EXIT_FAILURE;
    }
    ComputeBinomialCoefficients(branches.size());
    initialHypernet = GetRandomNetworkHypernet(branches, nodes);
    initialHypernet.RemoveEmptyBranches();
    if (IS_DEBUG == 1) {
        initialHypernet.LogHypernet();
    }
    std::shared_ptr<Model> minModel;
    int startTime = clock();
    try {
        if (IS_FULL_ENUMERATION_ALGORITHM == 1) {
            auto fullEnumerationAlgorithm = new FullEnumerationAlgorithm(initialHypernet);
            minModel = fullEnumerationAlgorithm->GetMinModel();
        } else if (IS_GENETIC_ALGORITHM == 1) {
            auto geneticAlgorithm = new GeneticAlgorithm(initialHypernet);
            minModel = geneticAlgorithm->GetMinModel();
        }
        if (!minModel->GetIsConditionsChecked()) {
            throw std::runtime_error("Solution does not satisfy conditions");
        }
    }
    catch (std::exception const &e) {
        HandleException(e);

        return EXIT_FAILURE;
    }
    catch (const char *str) {
        ErrorHandler(str);

        return EXIT_FAILURE;
    }
    OutputResult(minModel, startTime);
    input.close();
    output.close();
    if (IS_DEBUG != 1) {
        system("pause>>void");
    }
    
    return EXIT_SUCCESS;
}