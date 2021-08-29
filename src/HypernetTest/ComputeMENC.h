#pragma once

#include "Globals.h"
#include "Funcs.h"

template <class T>
bool ComputeMENC(T& sum, H& initialHypernet, double requiredValue = 1) {
    double prevLowerBound, prevUpperBound;
    if (IS_CUMULATIVE_MODE == 1) {
        prevLowerBound = 1;
        prevUpperBound = initialHypernet.GetNodes().size();
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
                    double value = result.GetPolynomialValue(p);
                    double lowerBound = prevLowerBound + value,
                            upperBound = prevUpperBound + value - 1;
                    if (lowerBound > requiredValue) {
                        sum = sum + T::GetUnity();

                        return true;
                    } else if (upperBound < requiredValue) {
                        sum = sum + T::GetUnity();

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
    if (IS_CUMULATIVE_MODE == 1) {
        auto value = sum.GetPolynomialValue(p);

        return DoubleEquals(value, requiredValue) || value > requiredValue;
    }

    return true;
}