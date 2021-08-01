#pragma once

#include "../HypernetModel/Hypernet.h"

// определения заголовков для глобальных функций

template <class T>
void Send(const T& object, int processorNumber);
template <class T>
T Recv();
H GetKpRandomHypernet(std::vector<Branch>& primaryNetwork, std::vector<Node>& nodes);