#include "Globals.h"
#include "Funcs.h"
#include "../HypernetModel/Helpers/DataHelper.h"
#include "../HypernetTest/ComputeAPC.h"
#include "Model .h"
#include "FullEnumerationAlgorithm.h"
#include "SimulatedAnnealingAlgorithm.h"

std::ifstream input;
std::ofstream output;
const double p = 0.9;
const int max_dimensional = 3;
int ReliableHypernets = 0, UnconnectedHypernets = 0, TwoNodesHypernets = 0, ChainsReduced = 0,
        UnconnectedNodesReduced = 0, PairConnectivityCalls = 0, EdgesReduced = 0, ComplexChains = 0,
        TreeNodeIntersections = 0, UnconnectedTreeNodes = 0;
std::vector<std::vector<double>> Bin;
int seed = time(0);
int CheckedConditions, UncheckedConditions;

void OutputResult(const std::shared_ptr<Model> model, int startTime) {
    output << "Solution:" << VectorToString(model->GetSolution()) << std::endl;
    output << "Reliability:" << model->GetReliability() << std::endl;
    output << "ObjFunctionValue:" << model->GetObjFunctionValue() << std::endl;
    output << "Statistics:" << std::endl;
    output << std::string(3, ' ') << "Time:" << clock() - startTime << std::endl;
    output << std::string(3, ' ') << "CheckedConditions:" << CheckedConditions << std::endl;
    output << std::string(3, ' ') << "UncheckedConditions:" << UncheckedConditions << std::endl;
}

int main(int argc, char** argv) {
    input.open("input.txt");
    output.open("output.txt");
    setlocale(LC_ALL, "");

    try {
        std::vector<Branch> branches;
        std::vector<Node> nodes;
        std::vector<Route> routes;
        GetData(branches, nodes, routes);
        ComputeBinomialCoefficients(branches.size());
        H initialHypernet;
        initialHypernet = H(std::move(branches), std::move(nodes), std::move(routes));
        initialHypernet.RemoveEmptyBranches();
        std::shared_ptr<Model> minModel;
        int startTime = clock();
        if (IS_FULL_ENUMERATION_ALGORITHM == 1) {
            auto fullEnumerationAlgorithm = new FullEnumerationAlgorithm(initialHypernet);
            minModel = fullEnumerationAlgorithm->GetMinModel();
        } else if (IS_SIMULATED_ANNEALING_ALGORITHM == 1) {
            auto simulatedAnnealingAlgorithm = new SimulatedAnnealingAlgorithm(initialHypernet);
            minModel = simulatedAnnealingAlgorithm->GetMinModel();
        } else {
            std::vector<Branch> solution;
            auto it = std::find_if(initialHypernet.GetFN().begin(), initialHypernet.GetFN().end(), [](Branch &item) ->
                    bool { return item == 8; });
            solution.push_back(*it);
            it = std::find_if(initialHypernet.GetFN().begin(), initialHypernet.GetFN().end(), [](Branch &item) ->
                    bool { return item == 9; });
            solution.push_back(*it);

            auto model = new Model(initialHypernet, solution);
            model->CheckConditions();
            minModel = std::make_shared<Model>(*model);
        }
        OutputResult(minModel, startTime);
    }
    catch (std::exception const &e) {
        HandleException(e);

        return EXIT_FAILURE;
    }
    catch (const char *str) {
        ErrorHandler(str);

        return EXIT_FAILURE;
    }

    input.close();
    output.close();
    if (IS_DEBUG != 1) {
        system("pause>>void");
    }
    
    return EXIT_SUCCESS;
}