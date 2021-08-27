#pragma once

#include "Globals.h"
#include "Funcs.h"

template <class T>
bool ComputeMENC(T& sum, H& initialHypernet, double requiredValue = 1) {
    double prevLowerBound, prevUpperBound;
    if (IS_CUMULATIVE_MODE == 1) {
        prevLowerBound = 1;
        prevUpperBound = initialHypernet.GetNodes().size();
        if (IS_DEBUG == 1) {
            LB.push_back(prevLowerBound);
            UB.push_back(prevUpperBound);
        }
    }
    for (int i = 1; i < initialHypernet.GetNodes().size(); i++) {
        if (IS_FULL_ENUMERATION != 1) {
            auto H = initialHypernet;
            if (i != 1) {
                H.RenumerateNodes(i, 1);
            }
            if (H.IsSNconnected()) {
                auto result = PairConnectivity<T>(H);
                sum = sum + result;
                if (IS_CUMULATIVE_MODE == 1) {
                    double value = IS_NUMBER_COMPUTATION == 1 ? result.GetValue() : result.GetPolynomialValue(p);
                    double lowerBound = prevLowerBound + value,
                            upperBound = prevUpperBound + value - 1;
                    if (IS_DEBUG == 1) {
                        LB.push_back(lowerBound);
                        UB.push_back(upperBound);
                    }
                    if (lowerBound > requiredValue || upperBound < requiredValue) {
                        return false;
                    }

                    prevLowerBound = lowerBound;
                    prevUpperBound = upperBound;
                }
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

    return true;
}