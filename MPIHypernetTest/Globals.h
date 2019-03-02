#pragma once

#include "Branch.h"

#define MAX_DIMENSIONAL 3
#define IS_RELEASE 0
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
// ENABLE_EDGE_REDUCTION must be 1 if enable next parameter
#define ENABLE_CHAIN_REDUCTION 1
#define ENABLE_SIMPLE_CASE 1

#define IS_TWO_LEVEL_PARALLELIZATION 1
#define IS_TEST_TIME 1
#define TEST_GRAPH_COUNT 30
#define TEST_GRAPH_NODE_SIZE 25
#define TEST_GRAPH_EDGE_SIZE 30
#define HOST_PROCESSOR 0
#define STOP_TAG 0
#define I_AM_FREE_TAG 1
#define I_NEED_HELP_TAG 2
#define SEND_RECV_TAG 3
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
extern int n, m, k;
extern int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced, UnconnectedNodesReduced,
        PairConnectivityCalls, EdgesReduced, UnsimpleChains, HelpProcessors;
extern double p, z;
extern unsigned long long int TotalBytesTransfer;