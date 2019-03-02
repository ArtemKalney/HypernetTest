#pragma once

#include "Branch.h"
#include "Hypernet.h"

Branch PairConnectivity(H &H, Branch &pseudoEdge);
template <class T>
void Send(T &object, int processorNumber);
template <class T>
T Recv();