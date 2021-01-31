#pragma once

#include "Stdafx.h"
#include "ApplicationSettings.h"

extern std::vector<std::vector<double>> Bin;
extern std::vector<int> KpNodesCombination;
extern int n, m, k, l;
extern int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced, UnconnectedNodesReduced,
        PairConnectivityCalls, EdgesReduced, ComplexChains, HelpProcessors, TreeNodeIntersections, UnconnectedTreeNodes;
extern int FirstRoot, SecondRoot;
extern const double p;
extern unsigned long long int TotalBytesTransfer;
extern int seed;
extern std::ifstream input;
extern std::ofstream output;