#pragma once

#include "HypernetBase/Hypernet.h"

Branch PairConnectivity(H &H, Branch &pseudoEdge);
H GetRandomHypernet();
// debug
void FullEnumeration(const H &H, const std::vector<Branch> &branchList, Branch &sum, std::vector<bool> &branchMask,
                     int &curPos);
