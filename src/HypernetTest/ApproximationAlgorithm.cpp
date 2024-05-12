#include "Funcs.h"

template <class T>
T CommonApproximationAlgorithm(H &H, const std::vector<T>& elements) {
    int size = elements.size();
    std::vector<double> C;
    std::vector<double> elementCounts;
    elementCounts.push_back(1);

    for(int i=0; i < size - 1; i++) {
        std::vector<T> connectedElements;
        for(auto &item : elements) {
            auto HWithRemovedElement = H;
            HWithRemovedElement.RemoveElement(item);
            if (HWithRemovedElement.IsSNconnected()) {
                connectedElements.push_back(item);
            }
        }

        elementCounts.push_back(connectedElements.size());
        if (connectedElements.empty()) {
            break;
        }
        int index = connectedElements.size() == 1 ? 0 : rand() % (connectedElements.size() - 1);
        H.RemoveElement(connectedElements[index], false);
    }

    if (elementCounts.size() < size) {
        elementCounts.resize(size);
    }
    for(int i=0; i < size; i++) {
        double multiplication = 1;
        for(int j=0; j<=i; j++) {
            multiplication *= elementCounts[j];
        }
        C.push_back(multiplication/Factorials[i]);
    }

    return T::GetElement(C, size);
}

template <>
Branch ApproximationAlgorithm<Branch>(H &H) {
    auto FN = H.GetFN();

    return CommonApproximationAlgorithm<Branch>(H, FN);
}

template <>
Node ApproximationAlgorithm<Node>(H &H) {
    auto nodes = H.GetNodes();

    return CommonApproximationAlgorithm<Node>(H, nodes);
}