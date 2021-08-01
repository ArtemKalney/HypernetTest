#pragma once

#include "Funcs.h"
#include "DTO.h"
#include "Globals.h"

template <class T>
T PairConnectivity(H &H, T &pseudoElement) {
    PairConnectivityCalls++;
    T returnValue;
    bool hasReturnValue = H.Reductions<T>(pseudoElement, returnValue);
    if (hasReturnValue) {
        return returnValue;
    }

    T allowingElement = H.GetAllowingElement<T>();
    T pseudoElement1, pseudoElement2;
    pseudoElement1 = pseudoElement * allowingElement;
    pseudoElement2 = pseudoElement * ~allowingElement;
    auto HwithReliableElement = H, HwithRemovedElement = H;
    HwithReliableElement.MakeReliableElement(allowingElement);
    HwithRemovedElement.RemoveElement(allowingElement);
    if (!HwithRemovedElement.IsSNconnected()) {
        UnconnectedHypernets++;
        if (HwithReliableElement.HasReliablePath<T>()) {
            ReliableHypernets++;
            return pseudoElement1 * T::GetUnity();
        } else {
            return PairConnectivity(HwithReliableElement, pseudoElement1);
        }
    } else {
        if (HwithReliableElement.HasReliablePath<T>()) {
            ReliableHypernets++;
            return pseudoElement1 * T::GetUnity() + PairConnectivity(HwithRemovedElement, pseudoElement2);
        } else {
            // назначение исполняющих процессов (запрос помощи процессом)
            int value, helpProcessor = 0;
            if (HwithReliableElement.GetNodes().size() >= MAX_DIMENSIONAL) {
                MPI_Status status;
                MPI_Send(&value, 0, MPI_INT, HOST_PROCESSOR, I_NEED_HELP_TAG, MPI_COMM_WORLD);
                MPI_Recv(&helpProcessor, 1, MPI_INT, HOST_PROCESSOR, I_NEED_HELP_TAG, MPI_COMM_WORLD, &status);

                if (helpProcessor != 0) {
                    HelpProcessors++;
                    auto data = DTO<T>(HwithReliableElement, pseudoElement1);
                    Send(data, helpProcessor);
                    return PairConnectivity(HwithRemovedElement, pseudoElement2);
                }
            }

            return PairConnectivity(HwithReliableElement, pseudoElement1) +
                    PairConnectivity(HwithRemovedElement, pseudoElement2);
        }
    }
}
