#pragma once

#include "HypernetBase/Hypernet.h"

Branch PairConnectivity(H &H, Branch &pseudoEdge);
template <class T>
void Send(const T &object, int processorNumber);
template <class T>
T Recv();
H GetRandomHypernet();
H GetRandomHypernet(std::vector<Branch> &primaryNetwork, std::vector<Node> &nodes);