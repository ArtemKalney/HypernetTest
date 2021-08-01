#pragma once

#include "Globals.h"
#include "Funcs.h"

template <class T>
void ComputeMENC(T& sum, H& initialHypernet) {
    for (int i = 1; i < initialHypernet.GetNodes().size(); i++) {
        if (IS_FULL_ENUMERATION != 1) {
            auto H = initialHypernet;
            if (i != 1) {
                H.RenumerateNodes(i, 1);
            }
            if (H.IsSNconnected()) {
                auto result = PairConnectivity<T>(H);
                sum = sum + result;
            }
        } else if (IS_FULL_ENUMERATION == 1) {
            auto H = initialHypernet;
            if (i != 1) {
                H.RenumerateNodesForGen(i, 1);
            }
            sum = sum + FullEnumeration<T>(H);
        }
        if (IS_DEBUG == 1) {
            output << "+R1" << "," << i + 1 << std::endl;
        }
    }

    sum = sum + T::GetUnity();
}