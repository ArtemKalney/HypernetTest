#pragma once

#include "HypernetBase/Branch.h"
// определения глобальных переменных и конфигурации приложения
#define IS_TEST_TIME 0
#define TEST_HYPERNETS 50
#define TEST_HYPERNET_NODES 25
#define TEST_HYPERNET_BRANCHES 30
#define TEST_HYPERNET_EDGES 30

#define IS_TEST_HYPERNET 1
#define IS_TEST_HYPERNET_SIMPLE_MAPPING 1
#define IS_TEST_HYPERNET_DOUBLE_TREE_MAP 1
#define TEST_HYPERNET_TREE_SIZE 28
#define TEST_HYPERNET_MAPPING_TIME 300000

#define IS_NUMBER_COMPUTATION 0

#define IS_DEBUG 1
#define IS_TWO_LEVEL_PARALLELIZATION 1
#define MAX_DIMENSIONAL 3

// Any parameter need previous ones
#define ENABLE_SIMPLE_CASE 1
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
#define ENABLE_CHAIN_REDUCTION 1

#define IS_FULL_SEARCH_CHAINS 0
#define IS_REDUCE_RELIABLE_CHAINS 0

#define HOST_PROCESSOR 0
#define STOP_TAG 0
#define I_AM_FREE_TAG 1
#define I_NEED_HELP_TAG 2
#define SEND_RECV_TAG 3
#define SEND_SOLUTION_TAG 4
#define RELIABLE_HYPERNETS_COUNT_TAG 5
#define UNCONNECTED_HYPERNET_COUNT_TAG 6
#define TWO_NODES_HYPERNET_COUNT 7
#define CHAINS_REDUCED_TAG 8
#define UNCONNECTED_NODES_REDUCED_TAG 9
#define PAIR_CONNECTIVITY_CALLS_TAG 10
#define EDGES_REDUCED_TAG 11
#define UNSIMPLE_CHAINS_TAG 12
#define HELP_PROCESSORS_TAG 13
#define TOTAL_BYTES_TRANSFER_TAG 14

extern std::vector<Branch> Bin;
extern int n, m, k, l;
extern int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced, UnconnectedNodesReduced,
        PairConnectivityCalls, EdgesReduced, ComplexChains, HelpProcessors, TreeNodeIntersections, UnconnectedTreeNodes;
extern int FirstRoot, SecondRoot;
extern const double p, z;
extern unsigned long long int TotalBytesTransfer;
extern int seed;
extern std::ifstream input;
extern std::ofstream output;