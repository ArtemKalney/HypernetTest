#pragma once

#include "Stdafx.h"
#include "ApplicationSettings.h"
#include "../HypernetModel/Helpers/Settings.h"

extern std::vector<std::vector<double>> Bin;
extern int ReliableHypernets, UnconnectedHypernets, TwoNodesHypernets, ChainsReduced, UnconnectedNodesReduced,
        PairConnectivityCalls, EdgesReduced, ComplexChains, TreeNodeIntersections, UnconnectedTreeNodes;
extern const double p;
extern const int max_dimensional;
extern std::ifstream input;
extern std::ofstream output;
extern int CheckedConditions, UncheckedConditions;
extern Settings AppSettings;