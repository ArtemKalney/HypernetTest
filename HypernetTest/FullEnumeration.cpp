#include "Funcs.h"
#include "Globals.h"

template <class T>
void RecursiveFullEnumeration(const H &H, T &sum, const std::vector<T> &elements, std::vector<bool> &mask, int &curPos) {
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
            T result = T::GetBranch(0);
            for (bool item : mask) {
                if (item) {
                    result = result * T::GetSimpleBranch();
                } else {
                    result = result * ~T::GetSimpleBranch();
                }
            }

            sum = sum + result;
        } else {
            UnconnectedHypernets++;
        }
    } else {
        mask[curPos] = false;
        int increasedPos = curPos + 1;
        RecursiveFullEnumeration(H, sum, elements, mask, increasedPos);
        mask[curPos] = true;
        RecursiveFullEnumeration(H, sum, elements, mask, increasedPos);
    }
}

template <>
Branch FullEnumeration<Branch>(H &H) {
    int startPos = 0;
    auto FN = H.GetFN();
    Branch sum = Branch::GetZero();
    std::vector<bool> branchMask(FN.size(), false);
    RecursiveFullEnumeration(H, sum, FN, branchMask, startPos);
    return sum;
}

template <>
Node FullEnumeration<Node>(H &H) {
    int startPos = 0;
    auto nodes = H.GetNodes();
    std::vector<bool> nodeMask(nodes.size(), false);
    Node sum = Node::GetZero();
    RecursiveFullEnumeration(H, sum, nodes, nodeMask, startPos);
    return sum;
}