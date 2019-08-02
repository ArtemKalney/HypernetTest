#pragma once

#include "HypernetBase/Branch.h"

#define IS_TEST 0
#define IS_TEST_CHECK_SPECIFICATIONS 1

#define IS_FULL_ENUMERATION 0

#define IS_TEST_HYPERNET 1
#define IS_TEST_HYPERNET_SIMPLE_MAPPING 1
#define IS_TEST_HYPERNET_DOUBLE_TREE_MAP 1
#define TEST_HYPERNET_TREE_SIZE 34
#define TEST_HYPERNET_FIRST_TREE_ROOT 76
#define TEST_HYPERNET_SECOND_TREE_ROOT 77
#define TEST_HYPERNET_MAPPING_TIME 300000

#define MAX_DIMENSIONAL 3

#define IS_DEBUG 1
#define DEBUG_FIRST_NODE 0
#define DEBUG_SECOND_NODE 1

// Any parameter need previous one
#define ENABLE_SIMPLE_CASE 1
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
#define ENABLE_CHAIN_REDUCTION 1

#define IS_FULL_SEARCH_CHAINS 0
#define IS_REDUCE_RELIABLE_CHAINS 0

extern std::vector<Branch> Bin;
extern int n, m, k;
extern int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced, UnconnectedNodesReduced,
        PairConnectivityCalls, EdgesReduced, ComplexChains, TreeNodeIntersections, UnconnectedTreeNodes;
extern double p, z;
extern std::ifstream input;
extern std::ofstream output;