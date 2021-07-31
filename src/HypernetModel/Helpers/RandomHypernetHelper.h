#pragma once

#include "../Hypernet.h"

H GetRandomHypernet();
void SetMapping(std::vector<Branch> &primaryNetwork, std::vector<Branch> &secondaryNetwork, std::vector<Route> &routes);
std::vector<Branch> GetRandomNetwork(int &nodesCount, int &edgeCount);
H GetRandomHypernet(std::vector<Branch>& primaryNetwork, std::vector<Node> &nodes);