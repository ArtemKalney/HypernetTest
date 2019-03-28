#pragma once

#include "HypernetBase/Branch.h"

#define IS_DEBUG 1
#define IS_FULL_ENUMERATION 0
#define MAX_DIMENSIONAL 3

#define ENABLE_SIMPLE_CASE 1
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
// ENABLE_EDGE_REDUCTION must be 1 if enable next parameter
#define ENABLE_CHAIN_REDUCTION 1

extern std::vector<Branch> Bin;
extern int n, m, k;
extern int ReliableHypernetsCount, UnconnectedHypernetsCount, TwoNodesHypernetsCount, ChainsReduced,
        UnconnectedNodesReduced, PairConnectivityCalls, EdgesReduced, UnsimpleChains;
extern double p, z;