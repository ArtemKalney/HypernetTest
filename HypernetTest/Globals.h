#pragma once

#include "HypernetBase/Branch.h"

#define IS_TEST 0
#define IS_TEST_CHECK_SPECIFICATIONS 1

#define IS_TEST_HYPERNET_SIMPLE_MAPPING 1

#define IS_OPTIMIZATION 1
#define IS_OPTIMIZATION_AOSH 0
#define OPTIMIZATION_TREE_SIZE 8// if IS_OPTIMIZATION_AOSH != 1 must be <= 20
#define OPTIMIZATION_KP_COUNT 0
#define OPTIMIZATION_FIRST_TREE_ROOT 0
#define OPTIMIZATION_SECOND_TREE_ROOT 1
#define OPTIMIZATION_MAPPING_TIME 300000

#define IS_FULL_ENUMERATION 0

#define IS_NUMBER_COMPUTATION 0

#define MAX_DIMENSIONAL 3

#define IS_DEBUG 1
#define DEBUG_FIRST_NODE 0
#define DEBUG_SECOND_NODE 1

// Any parameter need previous ones
#define ENABLE_SIMPLE_CASE 1
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
#define ENABLE_CHAIN_REDUCTION 1

#define IS_FULL_SEARCH_CHAINS 0
#define IS_REDUCE_RELIABLE_CHAINS 0

extern std::vector<Branch> Bin;
extern std::vector<int> KpNodesCombination;
extern int n, m, k;
extern int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced, UnconnectedNodesReduced,
        PairConnectivityCalls, EdgesReduced, ComplexChains, TreeNodeIntersections, UnconnectedTreeNodes;
extern const double p;
extern std::ifstream input;
extern std::ofstream output;