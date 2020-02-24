#pragma once

#include "HypernetBase/Hypernet.h"

void GetData(std::vector<Branch>& branches, std::vector<Node>& nodes, std::vector<Route>& routes);
void ComputeBinomialCoefficients();
H GetAoshRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes);
H GetKpRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes);
H GetRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes);
H GetRandomHypernet();
template <class T>
T FullEnumeration(H &H);
template <class T>
T PairConnectivity(H &H);
