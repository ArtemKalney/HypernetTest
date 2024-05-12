#pragma once

#include "Globals.h"
#include "Funcs.h"
#include "../HypernetModel/Helpers/DataHelper.h"

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

template <class T>
bool ComputeMENC(T& sum, H& initialHypernet, double requiredValue) {
    double prevLowerBound, prevUpperBound;
    prevLowerBound = 1;
    prevUpperBound = initialHypernet.GetNodes().size();
    for (int i = 1; i < initialHypernet.GetNodes().size(); i++) {
        auto H = initialHypernet;
        if (i != 1) {
            H.RenumerateNodes(i, 1);
        }
        if (H.IsSNconnected()) {
            auto result = PairConnectivity<T>(H);
            sum = sum + result;
            double value = AppSettings.IsNumberComputation == 1 ?
                           result.GetValue() :
                           result.GetPolynomialValue(AppSettings.ReliabilityValue);
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
    sum = sum + T::GetUnity();
    double value = AppSettings.IsNumberComputation == 1 ?
                   sum.GetValue() :
                   sum.GetPolynomialValue(AppSettings.ReliabilityValue);

    return DoubleEquals(value, requiredValue) || value > requiredValue;
}