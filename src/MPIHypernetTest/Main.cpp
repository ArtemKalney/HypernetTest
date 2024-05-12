#include "Globals.h"
#include "Funcs.h"
#include "DTO.h"
#include "PairConnectivity.h"
#include "../HypernetModel/Helpers/DataHelper.h"
#include "../HypernetModel/Generators/RandomHypernetGenerator.h"
#include "RandomKPHypernetGenerator.h"
#include "../HypernetModel/Helpers/InputParser.h"

// инициализация глобальных переменных
std::ifstream input;
std::ofstream output;
int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced,
        UnconnectedNodesReduced, PairConnectivityCalls, EdgesReduced, ComplexChains, HelpProcessors,
        TreeNodeIntersections, UnconnectedTreeNodes;
int FirstRoot, SecondRoot;
std::vector<std::vector<double>> Bin;
std::vector<int> KpNodesCombination;
const int max_dimensional = 3;
unsigned long long int TotalBytesTransfer;
Settings AppSettings;

void GetDataWithTestNodes(std::vector<Branch>& branches, std::vector<Node>& nodes, std::vector<Route>& routes,
                          std::vector<int>& testNodes) {
    if (!input.is_open()) {
        throw "GetData: File can not be opened!";
    }
    char str[50];
    input.getline(str, 50);
    std::cout << "Input graph : " << str << std::endl;
    int buf, nodeSize, branchSize, routeSize, testNodeSize;
    input >> buf; nodeSize = buf;
    input >> buf; branchSize = buf;
    input >> buf; routeSize = buf;
    input >> buf; testNodeSize = buf;
    // Read all nodes from input.txt
    for (int i = 0; i < nodeSize; i++) {
        input >> buf; int nodeNumber = buf - 1;
        Node node = Node::GetSimpleElement(nodeNumber, false, branchSize);
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
        branches.push_back(Branch::GetSimpleElement(id, firstNode, secondNode, branchSize));
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
    // Read all test nodes from input.txt
    for (int i = 0; i < testNodeSize; i++) {
        input >> buf;
        testNodes.push_back(--buf);
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

// отправления объекта ядру для расчёта
template <class T>
void Send(const T &object, const int processorNumber){
    std::stringstream ss;
    boost::archive::binary_oarchive oarchive{ss};
    oarchive << object;
    int length = ss.str().size();
    TotalBytesTransfer += length;
    MPI_Send((void *) ss.str().c_str(), length, MPI_BYTE, processorNumber, SEND_RECV_TAG, MPI_COMM_WORLD);
}

// получение объекта ядру для расчёта
template <class T>
T Recv(MPI_Status &status) {
    int length;
    MPI_Get_count(&status, MPI_BYTE, &length);
    char data[length + 1];
    MPI_Recv(data, length, MPI_BYTE, status.MPI_SOURCE, SEND_RECV_TAG, MPI_COMM_WORLD, &status);
    data[length] = '\0';
    std::stringstream ss;
    ss.write((const char *) data, length);
    boost::archive::binary_iarchive iarchive(ss);
    T object;
    iarchive >> object;
    return object;
}

// управление исполнением программы используя MPI (уровень 2)
template <class T>
void SendControl(std::vector<H>& hypernetList, const T& pseudoElement, const int size) {
    std::stack<int> freeProcessors;
    for (int i = 1; i < size; ++i) {
        freeProcessors.push(i);
    }

    while (!freeProcessors.empty() && !hypernetList.empty()) {
        auto data = DTO<T>(hypernetList.back(), pseudoElement);
        hypernetList.pop_back();
        Send(data, freeProcessors.top());
        freeProcessors.pop();
    }

    int value;
    MPI_Status status;
    while (freeProcessors.size() != size - 1) {
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == I_AM_FREE_TAG) {
            MPI_Recv(&value, 0, MPI_INT, status.MPI_SOURCE, I_AM_FREE_TAG, MPI_COMM_WORLD, &status);
            if (!hypernetList.empty()) {
                auto data = DTO<T>(hypernetList.back(), pseudoElement);
                hypernetList.pop_back();
                Send(data, status.MPI_SOURCE);
            } else {
                freeProcessors.push(status.MPI_SOURCE);
            }
        } else if (status.MPI_TAG == I_NEED_HELP_TAG) {
            MPI_Recv(&value, 0, MPI_INT, status.MPI_SOURCE, I_NEED_HELP_TAG, MPI_COMM_WORLD, &status);
            int helpProcessor = 0;
            if (freeProcessors.size() > 0) {
                helpProcessor = freeProcessors.top();
                freeProcessors.pop();
            }
            MPI_Send(&helpProcessor, 1, MPI_INT, status.MPI_SOURCE, I_NEED_HELP_TAG, MPI_COMM_WORLD);
        }
    }
}

// управление исполнением программы используя MPI (уровень 1)
template <class T>
void SendControl(H& H, const T& pseudoElement, const int size) {
    std::stack<int> freeProcessors;
    for (int i = 1; i < size; ++i) {
        freeProcessors.push(i);
    }

    auto data = DTO<T>(H, pseudoElement);
    Send(data, freeProcessors.top());
    freeProcessors.pop();

    int value;
    MPI_Status status;
    while (freeProcessors.size() != size - 1) {
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == I_AM_FREE_TAG) {
            MPI_Recv(&value, 0, MPI_INT, status.MPI_SOURCE, I_AM_FREE_TAG, MPI_COMM_WORLD, &status);
            freeProcessors.push(status.MPI_SOURCE);
        } else if (status.MPI_TAG == I_NEED_HELP_TAG) {
            MPI_Recv(&value, 0, MPI_INT, status.MPI_SOURCE, I_NEED_HELP_TAG, MPI_COMM_WORLD, &status);
            int helpProcessor = 0;
            if (freeProcessors.size() > 0) {
                helpProcessor = freeProcessors.top();
                freeProcessors.pop();
            }
            MPI_Send(&helpProcessor, 1, MPI_INT, status.MPI_SOURCE, I_NEED_HELP_TAG, MPI_COMM_WORLD);
        }
    }
}

//todo разобраться с условием HasReliablePath
template <class T>
void PrepareSendHypernet(T& sum, std::vector<H>& hypernetList, H& H, T& pseudoElement, int size) {
    if (!H.IsSNconnected()) {
        return;
    }

    if (AppSettings.IsNodesReliable == 1) {
        pseudoElement = T::GetElement(0, H.GetFN().front().GetC().size());
    } else {
        pseudoElement = T::GetSimpleElement(H.GetFN().front().GetC().size()) *
                T::GetSimpleElement(H.GetFN().front().GetC().size());
        auto it = std::find_if(H.GetNodes().begin(), H.GetNodes().end(), [](Node &item) -> bool { return item == 0; });
        it->SetIsReliable(true);
        it = std::find_if(H.GetNodes().begin(), H.GetNodes().end(), [](Node &item) -> bool { return item == 1; });
        it->SetIsReliable(true);
        if (H.HasReliablePath<Node>()) {
            sum = sum + pseudoElement;

            return;
        }
    }
    if (IS_TWO_LEVEL_PARALLELIZATION == 1) {
        hypernetList.push_back(H);
    } else {
        SendControl<T>(H, pseudoElement, size);
    }
}

// вычисление MENC
template <class T>
void ComputeMENC(T& sum, H& initialHypernet, int size) {
    std::vector<H> hypernetList;
    T pseudoElement;
    for (int i = 1; i < initialHypernet.GetNodes().size(); i++) {
        auto H = initialHypernet;
        if (i != 1) {
            H.RenumerateNodes(i, 1);
        }
        PrepareSendHypernet(sum, hypernetList, H, pseudoElement, size);
    }

    if (IS_TWO_LEVEL_PARALLELIZATION == 1) {
        SendControl<T>(hypernetList, pseudoElement, size);
    }
}

// вычисление ComputeMENCKP
template <class T>
void ComputeMENCKP(T& sum, H& initialHypernet, int size) {
    std::vector<H> hypernetList;
    T pseudoElement;
    for(auto &item : KpNodesCombination) {
        auto H = initialHypernet;
        if (item != 1) {
            H.RenumerateNodes(item, 1);
        }
        for (int i = 1; i < initialHypernet.GetNodes().size(); i++) {
            if (i != 1) {
                H.RenumerateNodes(i, 1);
            }
            PrepareSendHypernet(sum, hypernetList, H, pseudoElement, size);
        }
    }

    if (IS_TWO_LEVEL_PARALLELIZATION == 1) {
        SendControl<T>(hypernetList, pseudoElement, size);
    }
}

// вычисление APC
template <class T>
void ComputeAPC(T& sum, H& initialHypernet, const int size) {
    std::vector<H> hypernetList;
    T pseudoElement;
    for (int i = 0; i < initialHypernet.GetNodes().size(); i++) {
        for (int j = i + 1; j < initialHypernet.GetNodes().size(); j++) {
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
            PrepareSendHypernet(sum, hypernetList, H, pseudoElement, size);
        }
    }

    if (IS_TWO_LEVEL_PARALLELIZATION == 1) {
        SendControl<T>(hypernetList, pseudoElement, size);
    }
}

// вычисление гиперсети
template <class T>
void ComputeHypernet(T& sum, H& initialHypernet, const int size, const int option) {
    initialHypernet.RemoveEmptyBranches();
    if (option == 1) {
        ComputeAPC<T>(sum, initialHypernet, size);
    } else if (option == 2 && IS_OPTIMIZATION == 1) {
        ComputeMENCKP<T>(sum, initialHypernet, size);
    } else if (option == 2) {
        ComputeMENC<T>(sum, initialHypernet, size);
    }
}

// отправка дыннх от процесса мастера испольнителям
void BcastDataByMaster() {
    std::stringstream ss;
    boost::archive::binary_oarchive oarchive{ss};
    oarchive << Bin;
    int length = ss.str().size();
    TotalBytesTransfer += length + 4 * sizeof(int);
    MPI_Bcast(&length, 1, MPI_INT, HOST_PROCESSOR, MPI_COMM_WORLD);
    MPI_Bcast((void *) ss.str().c_str(), length, MPI_BYTE, HOST_PROCESSOR, MPI_COMM_WORLD);
}

// вывод расчётных данных
template <class T>
void PrintSolution(T &solution, double &time) {
    std::cout << "Time of programm " << time << " sec" << std::endl;
    std::cout << "PairConnectivityCalls " << PairConnectivityCalls << std::endl;
    std::cout << "Reductions : " << std::endl;
    std::cout << " UnconnectedNodesReduced " << UnconnectedNodesReduced << std::endl;
    std::cout << " EdgesReduced " << EdgesReduced << std::endl;
    if (AppSettings.IsNodesReliable == 1) {
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
    if (IS_DEBUG == 1) {
        std::cout << "TotalBytesTransfer : " << TotalBytesTransfer << std::endl;
        std::cout << "HelpProcessors : " << HelpProcessors << std::endl;
    }
    PrintSolution(solution);
    for (auto &item : solution.GetC()) {
        output << std::setprecision(14) << item << " ";
    }
    output << std::endl;
}

template <class T>
void PrintSolutionOptimization(T &solution, double &time) {
    if (IS_DEBUG == 1) {
        PrintSolution<T>(solution, time);
    }
    output << FirstRoot + 1 << " " << SecondRoot + 1 << " " << TreeNodeIntersections << " "
           << UnconnectedTreeNodes << " ";
    double value1 = AppSettings.IsNumberComputation == 1 ?
                   solution.GetValue() :
                   solution.GetPolynomialValue(AppSettings.ReliabilityValue);
    double value2 = AppSettings.IsNumberComputation == 1 ?
                    solution.GetValue() :
                    solution.GetPolynomialValue(0.99);
    output << std::setprecision(14) << value1 << " ";
    output << std::setprecision(14) << value2 << " ";
    output << time << std::endl;
    TreeNodeIntersections = 0;
    UnconnectedTreeNodes = 0;
}

// получение решения
template <class T>
void ComputeSolution(T& solution, const int size, const int option, std::vector<Branch>& branches,
                     std::vector<Node>& nodes, std::vector<Route>& routes, double& time) {
    // переменные начала времени расчёта и среднего времени расчёта
    double startTime, averageTime = 0;
    T sum = T::GetZero();
    // выбор тестирования по конфигурации приложения (время, оптимизация, расчёт гиперсети)
    H initialHypernet;
    if (IS_TEST_TIME == 1) {
        for (int i = 0; i < TEST_HYPERNETS; i++) {
            auto randomHypernetGenerator = new RandomHypernetGenerator(TEST_HYPERNET_NODES, TEST_HYPERNET_BRANCHES, TEST_HYPERNET_EDGES);
            initialHypernet = randomHypernetGenerator->GenerateHypernet();
            if (IS_DEBUG == 1) {
                initialHypernet.LogHypernet();
            }
            startTime = MPI_Wtime();
            ComputeHypernet<T>(sum, initialHypernet, size, option);
            double endTime = MPI_Wtime();
            output << "Time = " << endTime - startTime << std::endl;
            averageTime += endTime - startTime;
        }
    } else {
        // выбор метода получения гиперстеи (случайно или из аргументов функции)
        if (IS_OPTIMIZATION == 1) {
            auto randomKPHypernetGenerator = new RandomKPHypernetGenerator(branches, nodes);
            initialHypernet = randomKPHypernetGenerator->GenerateHypernet();
            if (IS_DEBUG == 1) {
                initialHypernet.LogHypernet();
            }
        } else {
            initialHypernet = H(std::move(branches), std::move(nodes), std::move(routes));
        }

        startTime = MPI_Wtime();
        ComputeHypernet<T>(sum, initialHypernet, size, option);
    }
    // запрос значений от процессов испольнителей
    for (int i = 1; i < size; i++) {
        MPI_Send(&i, 0, MPI_INT, i, SEND_SOLUTION_TAG, MPI_COMM_WORLD);
    }
    // сохранение среденего времени 
    if (IS_TEST_TIME == 1) {
        averageTime = averageTime / TEST_HYPERNETS;
        std::cout << "Average time = " << averageTime;
        return;
    }

    MPI_Status status;
    for (int i = 1; i < size; i++) {
        MPI_Probe(MPI_ANY_SOURCE, SEND_RECV_TAG, MPI_COMM_WORLD, &status);
        T branch = Recv<T>(status); // получение значения от процесса исполнителя
        sum = sum + branch;
        int buff;
        MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, RELIABLE_HYPERNETS_COUNT_TAG, MPI_COMM_WORLD, &status);
        ReliableHypernets += buff;
        MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, UNCONNECTED_HYPERNET_COUNT_TAG, MPI_COMM_WORLD, &status);
        UnconnectedHypernets += buff;
        MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, TWO_NODES_HYPERNET_COUNT, MPI_COMM_WORLD, &status);
        TwoNodesHypernets += buff;
        MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, CHAINS_REDUCED_TAG, MPI_COMM_WORLD, &status);
        ChainsReduced += buff;
        MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, UNCONNECTED_NODES_REDUCED_TAG, MPI_COMM_WORLD, &status);
        UnconnectedNodesReduced += buff;
        MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, PAIR_CONNECTIVITY_CALLS_TAG, MPI_COMM_WORLD, &status);
        PairConnectivityCalls += buff;
        MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, EDGES_REDUCED_TAG, MPI_COMM_WORLD, &status);
        EdgesReduced += buff;
        MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, UNSIMPLE_CHAINS_TAG, MPI_COMM_WORLD, &status);
        ComplexChains += buff;
        MPI_Recv(&buff, 1, MPI_INT, MPI_ANY_SOURCE, HELP_PROCESSORS_TAG, MPI_COMM_WORLD, &status);
        HelpProcessors += buff;
        unsigned long long int longBuff;
        MPI_Recv(&longBuff, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, TOTAL_BYTES_TRANSFER_TAG, MPI_COMM_WORLD, &status);
        TotalBytesTransfer += longBuff;
    }
    // обработка полченного значения в завсимости от выбранного критериия 
    if (option == 1) {
        if (AppSettings.IsNumberComputation == 1) {
            sum.SetValue(sum.GetValue() / Bin[initialHypernet.GetNodes().size()][2]);
        } else {
            for (int i = 0; i < sum.GetC().size(); i++) {
                auto sumVector = sum.GetC();
                sumVector[i] = sumVector[i] / Bin[initialHypernet.GetNodes().size()][2];
                sum.SetC(sumVector);
            }
        }
    } else if (option == 2) {
        sum = sum + T::GetUnity();
    }
    time = MPI_Wtime() - startTime;
    solution = solution + sum;
}

void SetGlobals(int argc, char** argv, int rank) {
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

    TotalBytesTransfer = 0;

    AppSettings.InputBranchCosts = INPUT_BRANCH_COSTS;
    AppSettings.InputMaxBranchSaturations = INPUT_MAX_BRANCH_SATURATIONS;

    InputParser inputParser(argc, argv);
    std::string str;
    str = inputParser.getCmdOption("-nodes");
    AppSettings.IsNodesReliable = !str.empty() ? std::stoi(str) : IS_NODES_RELIABLE;

    str = inputParser.getCmdOption("-number");
    AppSettings.IsNumberComputation = !str.empty() ? std::stoi(str) : IS_NUMBER_COMPUTATION;

    str = inputParser.getCmdOption("-iBranchValues");
    AppSettings.InputBranchValues = !str.empty() ? std::stoi(str) : INPUT_BRANCH_VALUES;

    str = inputParser.getCmdOption("-iNodesValues");
    AppSettings.InputNodesValues = !str.empty() ? std::stoi(str) : INPUT_NODE_VALUES;

    str = inputParser.getCmdOption("-p");
    AppSettings.ReliabilityValue = !str.empty() ? std::stod(str) : RELIABILITY_VALUE;

    if (rank == 0) {
        str = inputParser.getCmdOption("-input");
        input.open(!str.empty() ? str : "input.txt");
        str = inputParser.getCmdOption("-output");
        output.open(!str.empty() ? str : "output.txt");
    }
}

// инициализация работы процесса мастера
template <class T>
void Master(int size) {
    setlocale(LC_ALL, "");

    std::vector<Branch> branches;
    std::vector<Node> nodes;
    std::vector<Route> routes;
    std::vector<int> testNodes;
    if (IS_TEST_TIME == 1) {
        std::cout << "Input graph : H(" << TEST_HYPERNET_NODES << ", " << TEST_HYPERNET_BRANCHES << ", " <<
                  TEST_HYPERNET_EDGES << ")" << std::endl;
    } else {
        if (IS_OPTIMIZATION == 1) {
            GetDataWithTestNodes(branches, nodes, routes, testNodes);
        } else {
            GetData(branches, nodes, routes);
        }
    }
    std::cout << "Press 1 to get APC polynomial" << std::endl;
    std::cout << "Press 2 to get MENC polynomial" << std::endl;
    int option;
    std::cin >> option;
    if (option != 1 && option != 2) {
        std::cout << "Wrong number" << std::endl;

        return;
    }
    ComputeBinomialCoefficients(IS_TEST_TIME == 1 ? TEST_HYPERNET_BRANCHES : branches.size());
    BcastDataByMaster();
    double time;
    T solution = T::GetZero();
    if (IS_OPTIMIZATION != 1) {
        ComputeSolution<T>(solution, size, option, branches, nodes, routes, time);
        if (IS_TEST_TIME != 1) {
            PrintSolution(solution, time);
        }
    } else {
        std::vector<std::vector<int>> combinations;
        std::vector<int> combination;
        ComputeCombinations(testNodes, combinations, combination, 0, OPTIMIZATION_KP_COUNT);
        for(auto &item : combinations) {
            KpNodesCombination = item;
            ComputeSolution<T>(solution, size, option, branches, nodes, routes, time);
            PrintSolutionOptimization(solution, time);
        }
    }
    for (int i = 1; i < size; i++) {
        MPI_Send(&i, 0, MPI_INT, i, STOP_TAG, MPI_COMM_WORLD);
    }
}

// полчение дыннх от процесса мастера
void BcastDataBySlaves() {
    int length;
    MPI_Bcast(&length, 1, MPI_INT, HOST_PROCESSOR, MPI_COMM_WORLD);
    char data[length + 1];
    MPI_Bcast(data, length, MPI_BYTE, HOST_PROCESSOR, MPI_COMM_WORLD);
    data[length] = '\0';
    std::stringstream ss;
    ss.write((const char *) data, length);
    boost::archive::binary_iarchive iarchive(ss);
    iarchive >> Bin;
}

// инициализация работы процессов исполнителей
template <class T>
void Slaves() {
    BcastDataBySlaves();
    int value;
    T sum;
    MPI_Status status;
    do {
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == SEND_RECV_TAG) {
            auto data = Recv<DTO<T>>(status);
            sum = sum + PairConnectivity(data.Hypernet, data.Element);
            MPI_Send(&value, 0, MPI_INT, HOST_PROCESSOR, I_AM_FREE_TAG, MPI_COMM_WORLD);
        }
        if (status.MPI_TAG == SEND_SOLUTION_TAG) {
            MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, SEND_SOLUTION_TAG, MPI_COMM_WORLD, &status);
            Send(sum, HOST_PROCESSOR);
            MPI_Send(&ReliableHypernets, 1, MPI_INT, HOST_PROCESSOR, RELIABLE_HYPERNETS_COUNT_TAG, MPI_COMM_WORLD);
            MPI_Send(&UnconnectedHypernets, 1, MPI_INT, HOST_PROCESSOR, UNCONNECTED_HYPERNET_COUNT_TAG, MPI_COMM_WORLD);
            MPI_Send(&TwoNodesHypernets, 1, MPI_INT, HOST_PROCESSOR, TWO_NODES_HYPERNET_COUNT, MPI_COMM_WORLD);
            MPI_Send(&ChainsReduced, 1, MPI_INT, HOST_PROCESSOR, CHAINS_REDUCED_TAG, MPI_COMM_WORLD);
            MPI_Send(&UnconnectedNodesReduced, 1, MPI_INT, HOST_PROCESSOR, UNCONNECTED_NODES_REDUCED_TAG, MPI_COMM_WORLD);
            MPI_Send(&PairConnectivityCalls, 1, MPI_INT, HOST_PROCESSOR, PAIR_CONNECTIVITY_CALLS_TAG, MPI_COMM_WORLD);
            MPI_Send(&EdgesReduced, 1, MPI_INT, HOST_PROCESSOR, EDGES_REDUCED_TAG, MPI_COMM_WORLD);
            MPI_Send(&ComplexChains, 1, MPI_INT, HOST_PROCESSOR, UNSIMPLE_CHAINS_TAG, MPI_COMM_WORLD);
            MPI_Send(&HelpProcessors, 1, MPI_INT, HOST_PROCESSOR, HELP_PROCESSORS_TAG, MPI_COMM_WORLD);
            MPI_Send(&TotalBytesTransfer, 1, MPI_UNSIGNED_LONG_LONG, HOST_PROCESSOR, TOTAL_BYTES_TRANSFER_TAG,
                     MPI_COMM_WORLD);
            sum = T::GetZero();
            ReliableHypernets = 0;
            UnconnectedHypernets = 0;
            TwoNodesHypernets = 0;
            ChainsReduced = 0;
            UnconnectedNodesReduced = 0;
            PairConnectivityCalls = 0;
            EdgesReduced = 0;
            ComplexChains = 0;
            HelpProcessors = 0;
            TotalBytesTransfer = 0;
        }
    } while (status.MPI_TAG != STOP_TAG);
}

int main(int argc, char **argv) {
    int rank, size;
    // инциализация MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0)
            std::cerr << "Require at least 2 tasks" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    // обработчик ошибок
    try {
        SetGlobals(argc, argv, rank);

        if (rank == 0) {
            if (AppSettings.IsNodesReliable == 1) {
                Master<Branch>(size);
            } else {
                Master<Node>(size);
            }
        } else {
            if (AppSettings.IsNodesReliable == 1) {
                Slaves<Branch>();
            } else {
                Slaves<Node>();
            }
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
    MPI_Finalize(); // окончание работы MPI
    input.close();
    output.close();
    return EXIT_SUCCESS;
}