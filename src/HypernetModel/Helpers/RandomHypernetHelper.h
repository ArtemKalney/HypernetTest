#pragma once

#include "../Hypernet.h"

H GetRandomHypernet(int nodeSize, int branchSize, int routeSize);
void SetMapping(std::vector<Branch> &primaryNetwork, std::vector<Branch> &secondaryNetwork, std::vector<Route> &routes,
                int nodeSize);
std::vector<Branch> GetRandomNetwork(int nodesCount, int edgeCount, int vectorSize);
H GetRandomHypernet(std::vector<Branch>& primaryNetwork, std::vector<Node> &nodes, int routeSize);