#pragma once

#include "../HypernetModel/Route.h"
#include "../HypernetModel/Node.h"
#include "../HypernetModel/Branch.h"

void GetData(std::vector<Branch>& branches, std::vector<Node>& nodes, std::vector<Route>& routes);
void ComputeBinomialCoefficients();
void ErrorHandler(const char *str);