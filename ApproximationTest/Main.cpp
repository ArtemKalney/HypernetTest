#include "Funcs.h"
#include "Globals.h"

std::ifstream input("input.txt");
std::ofstream output;

int n = 0, m = 0, k = 0;
int ReliableHypernetsCount = 0, UnconnectedHypernetsCount = 0, TwoNodesHypernetsCount = 0, ChainsReduced = 0,
        UnconnectedNodesReduced = 0, PairConnectivityCalls = 0, EdgesReduced = 0, UnsimpleChains = 0;
std::vector<Branch> Bin;
std::vector<double> Factorials;
double p = 0.9, z = 0.1;

void NormalizeSolution(Branch &branch){
    branch.GetC().resize(m + 1);
    if (branch.GetPower() < m) {
        branch = branch * Bin[m - branch.GetPower()];
    }
}

void ComputePairConnectivities(Branch& sum, const H& initialHypernet, Branch& pseudoBranch) {
    H H = initialHypernet;
//    H.RenumerateNodes(6, 0);
//    H.RenumerateNodes(7, 1);
    if (H.IsSNconnected()) {
        auto result = ApproximationPairConnectivity(H, pseudoBranch);
        sum = sum + result;
    }
    // All connectivities
//    for (int i = 0; i < n; i++) {
//        for (int j = i + 1; j < n; j++) {
//            auto H = initialHypernet;
//            if (i != 0 || j != 1) {
//                if (i != 0 && j != 1) {
//                    H.RenumerateNodes(i, 0);
//                    H.RenumerateNodes(j, 1);
//                }
//                if (i == 0 && j != 1) {
//                    H.RenumerateNodes(j, 1);
//                }
//                if (i != 0 && j == 1) {
//                    H.RenumerateNodes(i, 0);
//                }
//            }
//            if (H.IsSNconnected()) {
//                auto result = PairConnectivity(H, pseudoBranch);
//                NormalizeSolution(result);
//                for (auto &item : result.GetC()) {
//                    output << std::setprecision(15) << item << " ";
//                }
//                output << std::endl;
//            }
//        }
//    }
}

void ComputeMENC(Branch& sum, const H& initialHypernet, Branch& pseudoBranch) {
    for (int i = 1; i < n; i++) {
        auto H = initialHypernet;
        if (i != 1) {
            H.RenumerateNodes(i, 1);
        }
        if (H.IsSNconnected()) {
            auto result = ApproximationPairConnectivity(H, pseudoBranch);
            sum = sum + result;
        }
    }

    sum = sum + Branch::GetUnity();
}

void ComputeAPC(Branch& sum, const H& initialHypernet, Branch& pseudoBranch) {
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
            if (H.IsSNconnected()) {
                auto result = ApproximationPairConnectivity(H, pseudoBranch);
                sum = sum + result;
            }
        }
    }

    for (int i = 0; i < sum.GetC().size(); i++) {
        auto sumVector = sum.GetC();
        sumVector[i] = sumVector[i] / Bin[n].GetC()[2];
        sum.SetC(sumVector);
    }
}

void GetData (std::vector<Branch>& branches,  std::vector<Node>& nodes, std::vector<Route>& edges) {
    int buf;
    input >> buf;n = buf;
    input >> buf;m = buf;
    input >> buf;k = buf;
    // Read all branches from input.txt
    for (int i = 0; i < m; i++) {
        input >> buf;
        int firstNode = buf - 1;
        input >> buf;
        int secondNode = buf - 1;
        branches.push_back(Branch::GetSimpleBranch(i, firstNode, secondNode));
    }
    // Fill all nodes
    for (int i = 0; i < n; i++) {
        Node node = Node(i, false);
        nodes.push_back(node);
    }
    // Read all edges from input.txt
    for (int i = 0; i < k; i++) {
        std::vector<int> vector;
        input >> buf;
        while (buf != 0) {
            int node = buf - 1;
            vector.push_back(node);
            input >> buf;
        }

        auto ptr = std::make_shared<std::vector<int>>(vector);
        Route route = Route(i, ptr);
        for (int j = 0; j < vector.size() - 1; j++) {
            auto it = std::find_if(branches.begin(), branches.end(), [vector, j](Branch &branch) ->
                    bool { return Branch::EqualNodes(branch, vector[j], vector[j + 1]); });
            branches[it - branches.begin()].GetRoutes().push_back(route);
        }
        edges.push_back(route);
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

double Factorial(int n) {
    return (n == 1 || n == 0) ? 1 : Factorial(n - 1) * n;
}

void ComputeFactorials() {
    for(int i=0; i<m; i++) {
        Factorials.push_back(Factorial(i));
    }
}

int main() {
    output.open("output.txt");
    setlocale(LC_ALL, "");
    if (!input.is_open()) {
        std::cout << "File can not be opened!\n";
        system("pause>>void");
        return 0;
    }
    char str[50];
    input.getline(str, 50);
    std::cout << "Input graph : " << str << std::endl;
    std::vector<Branch> branches;
    std::vector<Node> nodes;
    std::vector<Route> edges;
    GetData(branches, nodes, edges);
    // Input should end by $$$
    input >> str;
    if (strcmp(str, "$$$") != 0) {
        std::cout << "Incorrect entry" << std::endl;
        system("pause>>void");
        return 0;
    }
    std::cout << "Press 1 to get APC polynomial" << std::endl;
    std::cout << "Press 2 to get MENC polynomial" << std::endl;
    std::cout << "Press 3 to get pairwise connectivities" << std::endl;
    int option;
    std::cin >> option;
    if (option != 1 && option != 2 && option != 3) {
        std::cout << "Wrong number" << std::endl;
        system("pause>>void");
        return 0;
    }

    ComputeBinomialCoefficients();
    ComputeFactorials();
    // Create an initialHypernet
    H initialHypernet = H(std::move(branches), std::move(nodes), std::move(edges));
    initialHypernet.RemoveEmptyBranches();
    // In the beginning we consider only connected hypernets
    if (!initialHypernet.IsSNconnected()) {
        std::cout << "Unconnected hypernet on input!" << std::endl;
        return 0;
    }
    // Create a pseudo-branch F, which we multiply by the end of the calculations
    Branch pseudoBranch = Branch::GetBranch(0);
    unsigned int startTime = clock();
    // Randomized generator of random numbers
    srand(time(0));
    Branch sum;

    try {
        if (option == 1) {
            ComputeAPC(sum, initialHypernet, pseudoBranch);
        }
        if (option == 2) {
            ComputeMENC(sum, initialHypernet, pseudoBranch);
        }
        if (option == 3) {
            ComputePairConnectivities(sum, initialHypernet, pseudoBranch);
//            return 0;
        }
    }
    catch (const char *str) {
        std::cout << "--------------------------------" << std::endl;
        std::cout << "Occurred next error:" << std::endl;
        std::cout << str << std::endl;
        std::cout << "--------------------------------" << std::endl;
    }

    unsigned int endTime = clock();
    int searchTime = endTime - startTime;
    std::cout << "Time of programm " << searchTime << " msec" << std::endl;
    std::cout << "PairConnectivityCalls " << PairConnectivityCalls << std::endl;
    std::cout << "Reductions : " << std::endl;
    std::cout << " UnconnectedNodesReduced " << UnconnectedNodesReduced << std::endl;
    std::cout << " EdgesReduced " << EdgesReduced << std::endl;
    std::cout << " ChainsReduced " << ChainsReduced << std::endl;
//    std::cout << " UnsimpleChains " << UnsimpleChains << std::endl;
    std::cout << "Were ends of recursion : " << ReliableHypernetsCount + UnconnectedHypernetsCount +
                                                TwoNodesHypernetsCount << std::endl;
    std::cout << " ReliableHypernetsCount " << ReliableHypernetsCount << std::endl;
    std::cout << " UnconnectedHypernetsCount " << UnconnectedHypernetsCount << std::endl;
    std::cout << " TwoNodesHypernetsCount " << TwoNodesHypernetsCount << std::endl;
    std::cout << "Solution:" << std::endl;
    if (!sum.IsZero()) {
        NormalizeSolution(sum);
        sum.PrintBranch();
        std::cout << "Value at point " << p << ": " << std::setprecision(15) << sum.GetValue() << std::endl;

        output << "Solution:" << std::endl;
        for (auto &item : sum.GetC()) {
            output << std::setprecision(15) << item << " ";
        }
        output << std::endl;
    } else {
        std::cout << "no sum :(" << std::endl;
    }

    input.close();
    output.close();

//    system("pause>>void");
    return 0;
}