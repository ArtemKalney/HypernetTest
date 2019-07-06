#pragma once

#include "Branch.h"
#include "Hypernet.h"

Branch PairConnectivity(H &H, Branch &pseudoBranch);
Branch CumulativePairConnectivity(H &H, Branch &pseudoEdge);
