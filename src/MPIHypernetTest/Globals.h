#pragma once

#include "Stdafx.h"
#include "ApplicationSettings.h"
#include "../HypernetModel/Helpers/Settings.h"

extern std::vector<std::vector<double>> Bin;
extern std::vector<int> KpNodesCombination;
extern int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced, UnconnectedNodesReduced,
        PairConnectivityCalls, EdgesReduced, ComplexChains, HelpProcessors, TreeNodeIntersections, UnconnectedTreeNodes;
extern int FirstRoot, SecondRoot;
extern const double p;
extern const int max_dimensional;
extern unsigned long long int TotalBytesTransfer;
extern std::ifstream input;
extern std::ofstream output;
extern Settings AppSettings;