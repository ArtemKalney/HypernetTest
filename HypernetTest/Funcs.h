#pragma once

#include "Branch.h"
#include "Hypernet.h"

Branch PairConnectivity(H &H, Branch &pseudoEdge);
// debug
void GenCombinations(const H& H, const std::vector<Branch>& branchList, Branch& sum, std::vector<bool>& branchMask,
                     int& curPos);
