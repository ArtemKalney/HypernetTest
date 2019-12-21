#pragma once

#include "HypernetBase/Hypernet.h"

void GetData(std::vector<Branch>& branches, std::vector<Node>& nodes, std::vector<Route>& routes);
void ComputeBinomialCoefficients();
H GetRandomHypernet(std::vector<Branch> &primaryNetwork, std::vector<Node> &nodes);
template <class T>
T FullEnumeration(H &H);
template <class T>
T PairConnectivity(H &H);
