#pragma once

#include "Globals.h"
#include "Funcs.h"
#include "../HypernetModel/Helpers/DataHelper.h"

template <class T>
void ProcessSum(T& sum, H& initialHypernet) {
    if (AppSettings.IsNumberComputation == 1) {
        sum.SetValue(sum.GetValue() / Bin[initialHypernet.GetNodes().size()][2]);
    } else {
        for (int i = 0; i < sum.GetC().size(); i++) {
            auto sumVector = sum.GetC();
            sumVector[i] = sumVector[i] / Bin[initialHypernet.GetNodes().size()][2];
            sum.SetC(sumVector);
        }
    }
}

template <class T>
void ComputeAPC(T& sum, H& initialHypernet) {
    for (int i = 0; i < initialHypernet.GetNodes().size(); i++) {
        for (int j = i + 1; j < initialHypernet.GetNodes().size(); j++) {
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
    ProcessSum(sum, initialHypernet);
}

template <class T>
bool ComputeAPC(T& sum, H& initialHypernet, double requiredValue) {
    double prevLowerBound, prevUpperBound;
    prevLowerBound = 0;
    prevUpperBound = Bin[initialHypernet.GetNodes().size()][2];
    for (int i = 0; i < initialHypernet.GetNodes().size(); i++) {
        for (int j = i + 1; j < initialHypernet.GetNodes().size(); j++) {
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
                double value = AppSettings.IsNumberComputation == 1 ?
                        result.GetValue() :
                        result.GetPolynomialValue(AppSettings.ReliabilityValue);
                double lowerBound = prevLowerBound + value,
                        upperBound = prevUpperBound + value - 1;
                if (lowerBound/Bin[initialHypernet.GetNodes().size()][2] > requiredValue)
                {
                    ProcessSum(sum, initialHypernet);

                    return true;
                } else if (upperBound/Bin[initialHypernet.GetNodes().size()][2] < requiredValue) {
                    ProcessSum(sum, initialHypernet);

                    return false;
                }

                prevLowerBound = lowerBound;
                prevUpperBound = upperBound;
            }
        }
    }
    ProcessSum(sum, initialHypernet);
    double value = AppSettings.IsNumberComputation == 1 ?
                   sum.GetValue() :
                   sum.GetPolynomialValue(AppSettings.ReliabilityValue);

    return DoubleEquals(value, requiredValue) || value > requiredValue;
}
