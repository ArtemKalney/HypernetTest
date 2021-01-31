#include "Globals.h"
#include "Funcs.h"

void LogData() {
    time_t rawTime;
    time(&rawTime);
    output << ctime(&rawTime);
    output << PairConnectivityCalls << " ";
    output << UnconnectedNodesReduced << " ";
    output << EdgesReduced << " ";
    output << ChainsReduced << " ";
    output << ComplexChains << " ";
    output << ReliableHypernets << " ";
    output << UnconnectedHypernets << " ";
    output << TwoNodesHypernets << std::endl;
}

template <class T>
T RecursivePairConnectivity(H &H, T &pseudoElement) {
    PairConnectivityCalls++;
    if (IS_DEBUG == 1) {
        LogData();
    }
    T returnValue;
    bool hasReturnValue = H.Reductions<T>(pseudoElement, returnValue);
    if (hasReturnValue) {
        return returnValue;
    }

    T allowingElement = H.GetAllowingElement<T>();
    if (allowingElement.IsUnacceptableElement()) {
        throw "PairConnectivity: unacceptable allowingElement";
    }
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
            return RecursivePairConnectivity(HwithReliableElement, pseudoElement1);
        }
    } else {
        if (HwithReliableElement.HasReliablePath<T>()) {
            ReliableHypernets++;
            return pseudoElement1 * T::GetUnity() + RecursivePairConnectivity(HwithRemovedElement, pseudoElement2);
        } else {
            return RecursivePairConnectivity(HwithReliableElement, pseudoElement1) +
                    RecursivePairConnectivity(HwithRemovedElement, pseudoElement2);
        }
    }
}

template <>
Branch PairConnectivity(H &H) {
    Branch pseudoBranch = Branch::GetElement(0);
    if (!H.IsSNconnected()) {
        Branch::GetZero();
    }

    return RecursivePairConnectivity(H, pseudoBranch);
}

template <>
Node PairConnectivity(H &H) {
    //препологаем что уже проверли редукцию по выделенным вершинам
    Node pseudoNode = Node::GetSimpleElement() * Node::GetSimpleElement();
    auto it = std::find_if(H.GetNodes().begin(), H.GetNodes().end(),
                           [](Node &item) -> bool { return item == 0; });
    it->SetIsReliable(true);
    it = std::find_if(H.GetNodes().begin(), H.GetNodes().end(),
                      [](Node &item) -> bool { return item == 1; });
    it->SetIsReliable(true);
    if (!H.IsSNconnected()) {
        return Node::GetZero();
    }

    if (H.HasReliablePath<Node>()) {
        return pseudoNode;
    }

    return RecursivePairConnectivity(H, pseudoNode);
}