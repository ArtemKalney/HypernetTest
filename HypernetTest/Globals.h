#pragma once

#include "HypernetBase/Stdafx.h"
#include "ApplicationSettings.h"

extern std::vector<std::vector<double>> Bin;
extern std::vector<int> KpNodesCombination;
extern int n, m, k;
extern int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced, UnconnectedNodesReduced,
        PairConnectivityCalls, EdgesReduced, ComplexChains, TreeNodeIntersections, UnconnectedTreeNodes;
extern const double p;
extern std::ifstream input;
extern std::ofstream output;