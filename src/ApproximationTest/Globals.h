#pragma once

#include "Branch.h"
#include "Hypernet.h"

#define MAX_DIMENSIONAL 3
#define MAX_APPROXIMATION_DIMENSIONAL 2

extern std::vector<Branch> Bin;
extern std::vector<double> Factorials;
extern int n, m, k;
extern int ReliableHypernetsCount, UnconnectedHypernetsCount, TwoNodesHypernetsCount, ChainsReduced,
        UnconnectedNodesReduced, PairConnectivityCalls, EdgesReduced, UnsimpleChains;
extern double p, z;