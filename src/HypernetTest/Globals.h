#pragma once

#include "Stdafx.h"
#include "ApplicationSettings.h"

extern std::vector<std::vector<double>> Bin;
extern std::vector<int> KpNodesCombination;
extern int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced, UnconnectedNodesReduced,
        PairConnectivityCalls, EdgesReduced, ComplexChains, TreeNodeIntersections, UnconnectedTreeNodes;
extern const double p;
extern std::ifstream input;
extern std::ofstream output;
extern int seed;
extern int FirstRoot, SecondRoot;
extern std::vector<double> LB, UB;