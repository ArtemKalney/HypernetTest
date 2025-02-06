#include "Globals.h"
#include "Funcs.h"

template <class T>
T RecursivePairConnectivity(H &H, T &pseudoElement, int deep) {
    if (IS_APPROXIMATION == 1 /*&& H.GetFN().size() < MAX_APPROXIMATION_DIMENSIONAL*/ &&
        deep >= MIN_APPROXIMATION_DEEP) {
        return pseudoElement*ApproximationAlgorithm<T>(H);
    }

    PairConnectivityCalls++;
    deep++;

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
            return RecursivePairConnectivity(HwithReliableElement, pseudoElement1, deep);
        }
    } else {
        if (HwithReliableElement.HasReliablePath<T>()) {
            ReliableHypernets++;

            return pseudoElement1 * T::GetUnity() + RecursivePairConnectivity(HwithRemovedElement, pseudoElement2, deep);
        } else {
            return RecursivePairConnectivity(HwithReliableElement, pseudoElement1, deep) +
                    RecursivePairConnectivity(HwithRemovedElement, pseudoElement2, deep);
        }
    }
}

template <>
Branch PairConnectivity(H &H) {
    if (!H.IsSNconnected()) {
        Branch::GetZero();
    }

    Branch pseudoBranch = Branch::GetElement(0, H.GetFN().front().GetC().size());

    return RecursivePairConnectivity(H, pseudoBranch, 0);
}

template <>
Node PairConnectivity(H &H) {
    //препологаем что уже проверли редукцию по выделенным вершинам
    auto it = std::find_if(H.GetNodes().begin(), H.GetNodes().end(),
                           [](Node &item) -> bool { return item.Equals(0); });
    it->SetIsReliable(true);
    it = std::find_if(H.GetNodes().begin(), H.GetNodes().end(),
                      [](Node &item) -> bool { return item.Equals(1); });
    it->SetIsReliable(true);
    if (!H.IsSNconnected()) {
        return Node::GetZero();
    }

    Node pseudoNode = Node::GetSimpleElement(H.GetFN().front().GetC().size()) *
                      Node::GetSimpleElement(H.GetFN().front().GetC().size());

    if (H.HasReliablePath<Node>()) {
        return pseudoNode;
    }

    return RecursivePairConnectivity(H, pseudoNode, 0);
}