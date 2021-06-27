#pragma once

#include "../HypernetModel/Hypernet.h"

// определения заголовков для глобальных функций

template <class T>
void Send(const T &object, int processorNumber);
template <class T>
T Recv();
H GetRandomHypernet();
H GetAoshRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes);
H GetKpRandomHypernet(std::vector<Branch> primaryNetwork, std::vector<Node> &nodes);