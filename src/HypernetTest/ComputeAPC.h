#pragma once

#include "Globals.h"
#include "Funcs.h"

template <class T>
void ComputeAPC(T& sum, const H& initialHypernet) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (IS_FULL_ENUMERATION != 1) {
                auto H = initialHypernet;
                if (i != 0 || j != 1) {
                    if (i != 0 && j != 1) {
                        H.RenumerateNodes(i, 0);
                        H.RenumerateNodes(j, 1);
                    }
                    if (i == 0 && j != 1) {
                        H.RenumerateNodes(j, 1);
                    }
                    if (i != 0 && j == 1) {
                        H.RenumerateNodes(i, 0);
                    }
                }
                if (H.IsSNconnected()) {
                    auto result = PairConnectivity<T>(H);
                    sum = sum + result;
                }
            } else if (IS_FULL_ENUMERATION == 1) {
                auto H = initialHypernet;
                if (i != 0 || j != 1) {
                    if (i != 0 && j != 1) {
                        H.RenumerateNodesForGen(i, 0);
                        H.RenumerateNodesForGen(j, 1);
                    }
                    if (i == 0 && j != 1) {
                        H.RenumerateNodesForGen(j, 1);
                    }
                    if (i != 0 && j == 1) {
                        H.RenumerateNodesForGen(i, 0);
                    }
                }
                auto result = FullEnumeration<T>(H);
                sum = sum + result;
            }
            if (IS_DEBUG == 1) {
                output << "+R" << i + 1 << "," << j + 1 << std::endl;
            }
        }
    }

    if (IS_NUMBER_COMPUTATION == 1) {
        sum.SetValue(sum.GetValue() / Bin[n][2]);
        return;
    }
    for (int i = 0; i < sum.GetC().size(); i++) {
        auto sumVector = sum.GetC();
        sumVector[i] = sumVector[i] / Bin[n][2];
        sum.SetC(sumVector);
    }
}
