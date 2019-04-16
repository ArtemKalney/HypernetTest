#include "Globals.h"
#include "Funcs.h"
#include "DTO.h"

std::ifstream input("input.txt");
std::ofstream output;
int n = 0, m = 0, k = 0;
int ReliableHypernets = 0, UnconnectedHypernets = 0, TwoNodesHypernets = 0, ChainsReduced = 0,
        UnconnectedNodesReduced = 0, PairConnectivityCalls = 0, EdgesReduced = 0, ComplexChains = 0, HelpProcessors = 0;
std::vector<Branch> Bin;
double p = 0.9, z = 0.1;
unsigned long long int TotalBytesTransfer = 0;
int seed = 0;

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
    if (IS_TEST_TIME == 1) {
        n = TEST_HYPERNET_NODES;
        m = TEST_HYPERNET_BRANCHES;
        k = TEST_HYPERNET_EDGES;
        std::cout << "Input graph : H(" << n << ", " << m << ", " << k << ")" << std::endl;
        return;
    }

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
    // Fill all nodes
    for (int i = 0; i < n; i++) {
        Node node = Node(i, false);
        nodes.push_back(node);
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

void LogHypernet(H &H, int &hypernetNumber) {
    output << "-----------------------------> " << hypernetNumber + 1 << std::endl;
    output << H.GetNodes().size() << " " << H.GetFN().size() << " "  << H.GetF().size() << std::endl;
    for(auto &branch : H.GetFN()) {
        output << branch.GetId() + 1 << std::endl;
        output << branch.GetFirstNode() + 1 << " " << branch.GetSecondNode() + 1 << std::endl;
        for (auto &item : branch.GetRoutes()) {
            output << item.GetId() + 1 << " ";
        }
        output << 0 << std::endl;
    }
    for(auto &route : H.GetF()) {
        output << route.GetId() + 1 << std::endl;
        for (auto &item : *route.Ptr) {
            output << item + 1 << " ";
        }
        output << 0 << std::endl;
    }
    output << "$$$" << std::endl;
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

void NormalizeSolution(Branch &branch){
    branch.GetC().resize(m + 1);
    if (branch.GetPower() < m) {
        branch = branch * Bin[m - branch.GetPower()];
    }
}

template <class T>
//typename = std::enable_if<std::is_base_of<T, ISerialize>::value>
void Send(const T &object, int processorNumber){
    std::stringstream ss;
    boost::archive::binary_oarchive oarchive{ss};
    oarchive << object;
    int length = ss.str().size();
    TotalBytesTransfer += length;
    MPI_Send((void *) ss.str().c_str(), length, MPI_BYTE, processorNumber, SEND_RECV_TAG, MPI_COMM_WORLD);
}

template <class T>
T Recv() {
    MPI_Status status;
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    int length;
    MPI_Get_count(&status, MPI_PACKED, &length);
    char data[length + 1];
    MPI_Recv(data, length, MPI_PACKED, MPI_ANY_SOURCE, SEND_RECV_TAG, MPI_COMM_WORLD, &status);
    data[length] = '\0';
    std::stringstream ss;
    ss.write((const char *) data, length);
    boost::archive::binary_iarchive iarchive(ss);
    T object;
    iarchive >> object;
    return object;
}

void SendControl(std::vector<H> &hypernetList, int &size) {
    std::stack<int> freeProcessors;
    for (int i = 1; i < size; ++i) {
        freeProcessors.push(i);
    }

    while (!freeProcessors.empty() && !hypernetList.empty()) {
        DTO data = DTO(hypernetList.back(), Branch::GetBranch(0));
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
                DTO data = DTO(hypernetList.back(), Branch::GetBranch(0));
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

void SendControl(H &H, int &size) {
    std::stack<int> freeProcessors;
    for (int i = 1; i < size; ++i) {
        freeProcessors.push(i);
    }

    DTO data = DTO(H, Branch::GetBranch(0));
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

void ComputeMENC(const H& initialHypernet,  int &size) {
    std::vector<H> hypernetList;
    for (int i = 1; i < n; i++) {
        auto H = initialHypernet;
        if (i != 1) {
            H.RenumerateNodes(i, 1);
        }
        if (H.IsSNconnected()) {
            if (IS_TWO_LEVEL_PARALLELIZATION == 1) {
                hypernetList.push_back(H);
            } else {
                SendControl(H, size);
            }
        }
    }

    if (IS_TWO_LEVEL_PARALLELIZATION == 1) {
        SendControl(hypernetList, size);
    }
}

void ComputeAPC(const H& initialHypernet, int &size) {
    std::vector<H> hypernetList;
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
                if (IS_TWO_LEVEL_PARALLELIZATION == 1) {
                    hypernetList.push_back(H);
                } else {
                    SendControl(H, size);
                }
            }
        }
    }

    if (IS_TWO_LEVEL_PARALLELIZATION == 1) {
        SendControl(hypernetList, size);
    }
}

void ErrorHandler(const char *str) {
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Occurred next error:" << std::endl;
    std::cout << str << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void ComputeHypernet(H &initialHypernet, int &size, int &option) {
    initialHypernet.RemoveEmptyBranches();
    // In the beginning we consider only connected hypernets
    if (!initialHypernet.IsSNconnected()) {
        throw "Unconnected hypernet on input!";
    }
    if (option == 1) {
        ComputeAPC(initialHypernet, size);
    } else if (option == 2) {
        ComputeMENC(initialHypernet, size);
    }
}

void BcastDataByMaster() {
    std::stringstream ss;
    boost::archive::binary_oarchive oarchive{ss};
    oarchive << Bin;
    int length = ss.str().size();
    TotalBytesTransfer += length + 4*sizeof(int);
    MPI_Bcast(&length, 1, MPI_INT, HOST_PROCESSOR, MPI_COMM_WORLD);
    MPI_Bcast((void *) ss.str().c_str(), length, MPI_BYTE, HOST_PROCESSOR, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, HOST_PROCESSOR, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, HOST_PROCESSOR, MPI_COMM_WORLD);
    MPI_Bcast(&k, 1, MPI_INT, HOST_PROCESSOR, MPI_COMM_WORLD);
}

void Master(int size) {
    output.open("output.txt");
    setlocale(LC_ALL, "");

    std::vector<Branch> branches;
    std::vector<Node> nodes;
    std::vector<Route> routes;
    GetData(branches, nodes, routes);

    std::cout << "Press 1 to get APC polynomial" << std::endl;
    std::cout << "Press 2 to get MENC polynomial" << std::endl;
    int option;
    std::cin >> option;
    if (option != 1 && option != 2) {
        std::cout << "Wrong number" << std::endl;
        return;
    }

    ComputeBinomialCoefficients();
    BcastDataByMaster();
    // Create a pseudo-branch F, which we multiply by the end of the calculations
    double startTime, averageTime = 0;
    if (IS_TEST_TIME == 1) {
        seed = time(0);
        for (int i = 0; i < TEST_HYPERNETS; i++) {
            H initialHypernet = GetRandomHypernet();
            if (IS_DEBUG == 1) {
                LogHypernet(initialHypernet, i);
            }
            startTime = MPI_Wtime();
            ComputeHypernet(initialHypernet, size, option);
            double endTime = MPI_Wtime();
            output << "Time = " << endTime - startTime << std::endl;
            averageTime += endTime - startTime;
        }
    } else {
        startTime = MPI_Wtime();
        H initialHypernet = H(std::move(branches), std::move(nodes), std::move(routes));
        ComputeHypernet(initialHypernet, size, option);
    }
    for (int i = 1; i < size; i++) {
        MPI_Send(&i, 0, MPI_INT, i, STOP_TAG, MPI_COMM_WORLD);
    }

    if (IS_TEST_TIME == 1 && TEST_HYPERNETS > 1) {
        averageTime = averageTime / TEST_HYPERNETS;
        std::cout << "Average time = " << averageTime;
        output << "Average time = " << averageTime;
        return;
    }

    MPI_Status status;
    Branch sum;
    for (int i = 1; i < size; i++) {
        Branch branch = Recv<Branch>();
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

    if (option == 1) {
        for (int i = 0; i < sum.GetC().size(); i++) {
            auto sumVector = sum.GetC();
            sumVector[i] = sumVector[i] / Bin[n].GetC()[2];
            sum.SetC(sumVector);
        }
    } else if (option == 2) {
        sum = sum + Branch::GetUnity();
    }
    double searchTime = MPI_Wtime() - startTime;
    std::cout << "Time of programm " << searchTime << " msec" << std::endl;
    std::cout << "PairConnectivityCalls " << PairConnectivityCalls << std::endl;
    std::cout << "Reductions : " << std::endl;
    std::cout << " UnconnectedNodesReduced " << UnconnectedNodesReduced << std::endl;
    std::cout << " EdgesReduced " << EdgesReduced << std::endl;
    std::cout << " ChainsReduced " << ChainsReduced << std::endl;
    if (IS_DEBUG == 1) {
        std::cout << " ComplexChains " << ComplexChains << std::endl;
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
    std::cout << "Solution:" << std::endl;
    if (!sum.IsZero()) {
        NormalizeSolution(sum);
        sum.PrintBranch();
        std::cout << "Value at point " << p << ": " << std::setprecision(15) << sum.GetValue() << std::endl;

        for (auto &item : sum.GetC()) {
            output << std::setprecision(15) << item << " ";
        }
        output << std::endl;
    } else {
        std::cout << "no sum :(" << std::endl;
    }
}

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
    MPI_Bcast(&n, 1, MPI_INT, HOST_PROCESSOR, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, HOST_PROCESSOR, MPI_COMM_WORLD);
    MPI_Bcast(&k, 1, MPI_INT, HOST_PROCESSOR, MPI_COMM_WORLD);
}

void Slaves(int rank) {
    BcastDataBySlaves();

    int value;
    Branch sum;
    MPI_Status status;
    do {
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == SEND_RECV_TAG) {
            DTO data = Recv<DTO>();
            sum = sum + PairConnectivity(data.H, data.Branch);
            MPI_Send(&value, 0, MPI_INT, HOST_PROCESSOR, I_AM_FREE_TAG, MPI_COMM_WORLD);
        }
    } while (status.MPI_TAG != STOP_TAG);

    if (IS_TEST_TIME == 0 || TEST_HYPERNETS == 1) {
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
        MPI_Send(&TotalBytesTransfer, 1, MPI_UNSIGNED_LONG_LONG, HOST_PROCESSOR, TOTAL_BYTES_TRANSFER_TAG, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0)
            std::cerr << "Require at least 2 tasks" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    try {
        rank == 0 ? Master(size) : Slaves(rank);
    } catch (const std::overflow_error &e) {
        std::cout << "throw std::overflow_error (same type rule)" << std::endl << e.what();
    } catch (const std::runtime_error &e) {
        std::cout << "throw std::underflow_error (base class rule)" << std::endl << e.what();
    } catch (const std::exception &e) {
        std::cout << "throw std::logic_error (base class rule)" << std::endl << e.what();
    } catch (const char *str) {
        ErrorHandler(str);
    } catch (...) {
        std::cout << "throw std::string or int or any other unrelated type";
    }
    MPI_Finalize();
    input.close();
    output.close();
    return 0;
}