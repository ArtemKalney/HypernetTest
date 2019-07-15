#pragma once

#include "HypernetBase/Hypernet.h"

Branch PairConnectivity(H &H, Branch &pseudoEdge);
void FullEnumeration(const H &H, const std::vector<Branch> &branchList, Branch &sum, std::vector<bool> &branchMask,
                     int &curPos);
void GetData(std::vector<Branch>& branches, std::vector<Node>& nodes, std::vector<Route>& routes);
void ComputeBinomialCoefficients();
void ComputeAPC(Branch& sum, const H& initialHypernet, Branch& pseudoBranch);
void ComputeMENC(Branch& sum, const H& initialHypernet, Branch& pseudoBranch);
H GetRandomHypernet(std::vector<Branch> &primaryNetwork, std::vector<Node> &nodes);
