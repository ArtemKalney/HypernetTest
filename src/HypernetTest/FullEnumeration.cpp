#include "Funcs.h"
#include "Globals.h"

template <class T>
void RecursiveFullEnumeration(const H& H, T& sum, const std::vector<T>& elements, std::vector<bool>& mask, int curPos,
                              const int vectorSize) {
    if (curPos == elements.size()) {
        auto hypernet = H;
        for (int i = 0; i < mask.size(); i++) {
            T element = elements[i];
            if (!mask[i]) {
                hypernet.RemoveElement(element);
            } else {
                hypernet.MakeReliableElement(element);
            }
        }

        if (hypernet.IsSNconnected()) {
            ReliableHypernets++;
            T result = T::GetElement(0, vectorSize);
            for (bool item : mask) {
                if (item) {
                    result = result * T::GetSimpleElement(vectorSize);
                } else {
                    result = result * ~T::GetSimpleElement(vectorSize);
                }
            }

            sum = sum + result;
        } else {
            UnconnectedHypernets++;
        }
    } else {
        mask[curPos] = false;
        int increasedPos = curPos + 1;
        RecursiveFullEnumeration(H, sum, elements, mask, increasedPos, vectorSize);
        mask[curPos] = true;
        RecursiveFullEnumeration(H, sum, elements, mask, increasedPos, vectorSize);
    }
}

template <>
Branch FullEnumeration<Branch>(H &H) {
    int startPos = 0;
    auto FN = H.GetFN();
    Branch sum = Branch::GetZero();
    std::vector<bool> branchMask(FN.size(), false);
    RecursiveFullEnumeration(H, sum, FN, branchMask, startPos, H.GetFN().front().GetC().size());
    return sum;
}

template <>
Node FullEnumeration<Node>(H &H) {
    int startPos = 0;
    auto nodes = H.GetNodes();
    std::vector<bool> nodeMask(nodes.size(), false);
    Node sum = Node::GetZero();
    RecursiveFullEnumeration(H, sum, nodes, nodeMask, startPos, H.GetFN().front().GetC().size());
    return sum;
}