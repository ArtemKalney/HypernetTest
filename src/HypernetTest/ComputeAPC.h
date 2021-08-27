#pragma once

#include "Globals.h"
#include "Funcs.h"

template <class T>
bool ComputeAPC(T& sum, H& initialHypernet, double requiredValue = 0) {
    double prevLowerBound, prevUpperBound;
    if (IS_CUMULATIVE_MODE == 1) {
        prevLowerBound = 0;
        prevUpperBound = Bin[initialHypernet.GetNodes().size()][2];
        if (IS_DEBUG == 1) {
            LB.push_back(prevLowerBound);
            UB.push_back(prevUpperBound);
        }
    }
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
                    if (IS_CUMULATIVE_MODE == 1) {
                        double value = IS_NUMBER_COMPUTATION == 1 ? result.GetValue() : result.GetPolynomialValue(p);
                        double lowerBound = prevLowerBound + value,
                                upperBound = prevUpperBound + value - 1;
                        if (IS_DEBUG == 1) {
                            LB.push_back(lowerBound);
                            UB.push_back(upperBound);
                        }
                        if (lowerBound/Bin[initialHypernet.GetNodes().size()][2] > requiredValue ||
                            upperBound/Bin[initialHypernet.GetNodes().size()][2] < requiredValue) {
                            for(int k=0; k<LB.size(); k++){
                                LB[k] = LB[k]/Bin[initialHypernet.GetNodes().size()][2];
                                UB[k] = UB[k]/Bin[initialHypernet.GetNodes().size()][2];
                            }

                            return false;
                        }

                        prevLowerBound = lowerBound;
                        prevUpperBound = upperBound;
                    }
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
    if (IS_CUMULATIVE_MODE == 1) {
        for(int i=0; i<LB.size(); i++){
            LB[i] = LB[i]/Bin[initialHypernet.GetNodes().size()][2];
            UB[i] = UB[i]/Bin[initialHypernet.GetNodes().size()][2];
        }
    }
    if (IS_NUMBER_COMPUTATION == 1) {
        sum.SetValue(sum.GetValue() / Bin[initialHypernet.GetNodes().size()][2]);

        return true;
    }

    for (int i = 0; i < sum.GetC().size(); i++) {
        auto sumVector = sum.GetC();
        sumVector[i] = sumVector[i] / Bin[initialHypernet.GetNodes().size()][2];
        sum.SetC(sumVector);
    }

    return true;
}
