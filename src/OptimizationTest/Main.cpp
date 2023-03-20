#include "Globals.h"
#include "Funcs.h"
#include "../HypernetModel/Helpers/DataHelper.h"
#include "../HypernetTest/ComputeAPC.h"
#include "Model .h"
#include "FullEnumerationAlgorithm.h"
#include "SimulatedAnnealingAlgorithm.h"
#include "../HypernetModel/Helpers/InputParser.h"

std::ifstream input;
std::ofstream output;
const int max_dimensional = 3;
int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced,
        UnconnectedNodesReduced, PairConnectivityCalls, EdgesReduced, ComplexChains,
        TreeNodeIntersections, UnconnectedTreeNodes;
std::vector<std::vector<double>> Bin;
int CheckedConditions, UncheckedConditions;
Settings AppSettings;

void OutputResult(const std::shared_ptr<Model> model, int startTime) {
    std::cout << "Solution:" << VectorToString(model->GetSolution()) << std::endl;
    std::cout << "Reliability:" << model->GetReliability() << std::endl;
    std::cout << "ObjFunctionValue:" << model->GetObjFunctionValue() << std::endl;
    std::cout << "Statistics:" << std::endl;
    std::cout << std::string(3, ' ') << "Time:" << clock() - startTime << std::endl;
    std::cout << std::string(3, ' ') << "CheckedConditions:" << CheckedConditions << std::endl;
    std::cout << std::string(3, ' ') << "UncheckedConditions:" << UncheckedConditions << std::endl;

    output << VectorToString(model->GetSolution()) << std::endl;
}

void SetGlobals(int argc, char** argv) {
    ReliableHypernets = 0;
    UnconnectedHypernets = 0;
    TwoNodesHypernets = 0;
    ChainsReduced = 0;
    UnconnectedNodesReduced = 0;
    PairConnectivityCalls = 0;
    EdgesReduced = 0;
    ComplexChains = 0,
    TreeNodeIntersections = 0;
    UnconnectedTreeNodes = 0;

    AppSettings.IsNodesReliable = IS_NODES_RELIABLE;
    AppSettings.IsNumberComputation = IS_NUMBER_COMPUTATION;
    AppSettings.InputNodesValues = INPUT_NODE_VALUES;
    AppSettings.InputMaxBranchSaturations = INPUT_MAX_BRANCH_SATURATIONS;

    InputParser inputParser(argc, argv);
    std::string str;
    str = inputParser.getCmdOption("-iBranchCosts");
    AppSettings.InputBranchCosts = !str.empty() ? std::stoi(str) : INPUT_BRANCH_COSTS;

    str = inputParser.getCmdOption("-iBranchValues");
    AppSettings.InputBranchValues = !str.empty() ? std::stoi(str) : INPUT_BRANCH_VALUES;

    str = inputParser.getCmdOption("-r");
    AppSettings.MinMENCValue = !str.empty() ? std::stod(str) : MIN_MENC_VALUE;

    str = inputParser.getCmdOption("-max");
    AppSettings.TMax = !str.empty() ? std::stod(str) : T_MAX;

    str = inputParser.getCmdOption("-min");
    AppSettings.TMin = !str.empty() ? std::stod(str) : T_MIN;

    str = inputParser.getCmdOption("-n");
    AppSettings.InitialBranchCount = !str.empty() ? std::stoi(str) : INITIAL_BRANCH_COUNT;

    str = inputParser.getCmdOption("-p");
    AppSettings.ReliabilityValue = !str.empty() ? std::stod(str) : RELIABILITY_VALUE;

    str = inputParser.getCmdOption("-input");
    input.open(!str.empty() ? str : "input.txt");
    str = inputParser.getCmdOption("-output");
    output.open(!str.empty() ? str : "output.txt");
}

int main(int argc, char** argv) {
    SetGlobals(argc, argv);
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
        } else {
            auto simulatedAnnealingAlgorithm = new SimulatedAnnealingAlgorithm(initialHypernet);
            minModel = simulatedAnnealingAlgorithm->GetMinModel();
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
    
    return EXIT_SUCCESS;
}