#include "Stdafx.h"
#include "Funcs.h"
#include "Globals.h"

//todo сделать проще - заменяем в FN ссылки соответсвующие ptr на новое ребро
// Chain replacement by edge
void ChainReduction(H& H, std::vector<int> &forbiddenNodes) {
    auto chain = H.GetHomogeneousChain(forbiddenNodes);
    if (chain.empty()) {
        return;
    }

    bool isReliableChain = chain.front().GetIsReliable();
    auto nodesInChain = H.GetNodesInChain(chain);
    auto SNnodepowers = H::GetNodePowers(H.GetSN(), H.GetNodes().size());
    bool isSimpleChain = true;
    for (int i = 1; i < nodesInChain.size() - 1; i++) {
        int nodeNumber = nodesInChain[i];
        if (SNnodepowers[nodeNumber] != 2 && SNnodepowers[nodeNumber] != 0) {
            isSimpleChain = false;
            break;
        }
    }

    if (isSimpleChain) {
        ChainsReduced++;
//        преобразования F при замене цепи ребром
        auto nodesF = nodesInChain;
        for (int i = 1; i < nodesF.size() - 1; i++) {
            int nodeNumber = nodesF[i];
            for (int j = 0; j < H.GetF().size(); j++) {
                Route route = H.GetF()[j];
                auto ptr = route.Ptr;
                if (H::IsSlightlyIncident(nodeNumber, route)) {
                    ptr->erase(std::remove(ptr->begin(), ptr->end(), nodeNumber), ptr->end());
                    // если длина ребра с тановится 1, то удаляем его
                    if (ptr->size() == 1) {
                        ptr->clear();
                        H.GetF().erase(H.GetF().begin() + j--);
                    }
                } else if (H::IsIncident(nodeNumber, route)) {
                    H.GetF().erase(H.GetF().begin() + j--);
                    auto it = std::find_if(H.GetF().begin(), H.GetF().end(),
                                           [nodeNumber](Route &route) ->
                                                   bool { return H::IsIncident(nodeNumber, route); });
                    auto ptrToInsert = it -> Ptr;
                    if (ptrToInsert->front() == nodeNumber && ptr->front() == nodeNumber) {
                        std::reverse(ptrToInsert->begin(), ptrToInsert->end());
                    } else if (ptrToInsert->back() == nodeNumber && ptr->back() == nodeNumber) {
                        std::reverse(ptr->begin(), ptr->end());
                    } else if (ptrToInsert->front() == nodeNumber && ptr->back() == nodeNumber) {
                        std::reverse(ptrToInsert->begin(), ptrToInsert->end());
                        std::reverse(ptr->begin(), ptr->end());
                    }
                    // удаляем вершину из обоих рёбер
                    ptrToInsert->erase(std::remove(ptrToInsert->begin(), ptrToInsert->end(), nodeNumber));
                    ptr->erase(std::remove(ptr->begin(), ptr->end(), nodeNumber));
                    // объединяем рёбра
                    ptrToInsert->insert(ptrToInsert->end(), ptr->begin(), ptr->end());
                    // заменяем в FN ссылки соответсвующие ptr на новое ребро
                    int idToInsert = H.GetF()[it - H.GetF().begin()].Id;
                    for (auto &branch : H.GetFN()) {
                        for (auto &item : branch.GetRoutes()) {
                            if (item == route) {
                                item.Ptr = ptrToInsert;
                                item.Id = idToInsert;
                            }
                        }
                    }
                    // нужно для newBranch
                    for(auto &item : chain.front().GetRoutes()) {
                        if (item == route) {
                            item.Ptr = ptrToInsert;
                            item.Id = idToInsert;
                        }
                    }
                }
            }
            // decrease indexes after delete
            for (auto &item : nodesF) {
                if (nodeNumber < item) {
                    item--;
                }
            }
            for (auto &route : H.GetF()) {
                for (auto &item : *route.Ptr) {
                    if (nodeNumber < item) {
                        item--;
                    }
                }
            }
        }
//        преобразования FN при замене цепи ребром
        auto nodesFN = nodesInChain;
        Branch newBranch;
        if (isReliableChain) {
            newBranch = Branch::GetUnity();
            newBranch.SetIsReliable(true);
        } else {
            newBranch = Branch::GetUnity();
            for (auto &item : chain) {
                newBranch = newBranch * item;
            }
        }
        newBranch.SetRoutes(chain.front().GetRoutes());
        newBranch.SetId(chain.front().GetId());
        for (int i = 1; i < nodesFN.size() - 1; i++) {
            int nodeNumber = nodesFN[i];
            H.RemoveNodeFN(nodeNumber);
            for (auto &item : nodesFN) {
                if (nodeNumber < item) {
                    item--;
                }
            }
        }
        newBranch.SetFirstNode(nodesFN.front());
        newBranch.SetSecondNode(nodesFN.back());
        H.GetFN().push_back(newBranch);

        return ChainReduction(H, forbiddenNodes);
    } else {
        UnsimpleChains++;
        for (auto &item : nodesInChain) {
            if (std::find(forbiddenNodes.begin(), forbiddenNodes.end(), item) == forbiddenNodes.end()) {
                forbiddenNodes.push_back(item);
            }
        }

//        auto routesF = H.GetRoutesF();
//        auto routesFN = H.GetRoutesFN();
        return ChainReduction(H, forbiddenNodes);
    }
}
// Removes the connectivity component, where both pivot nodes do not lie and returns true,
//if the pivot nodes in different components, then returns false
bool BridgeReduction(H& H)
{
    for(auto &item : H.GetNodes()) {
        item.IsVisited = false;
    }
    H::DFS(0, H.GetNodes(), H.GetFN());
    int visitedNodesCount = 0;
    for(auto &item : H.GetNodes()) {
       if (item.IsVisited) {
           visitedNodesCount++;
       }
    }

    if (visitedNodesCount != H.GetNodes().size()) {
        for (int i = 0; i < H.GetNodes().size(); i++) {
            Node node = H.GetNodes()[i];
            if (!node.IsVisited) {
                UnconnectedNodesReduced++;
                H.RemoveNode(i--);
            }
        }

        return !H.GetNodes()[1].IsVisited;
    } else {
        return false;
    }
}

void EdgeReduction(H& H) {
    auto SN = H.GetSN();
    H::DFS(0, H.GetNodes(), SN);
    auto canDeleteMask = H.GetCanDeleteMask(SN);

    for (int i = 0; i < canDeleteMask.size(); i++) {
        if (canDeleteMask[i]) {
            EdgesReduced++;
            Branch edge = SN[i];
            H.GetF().erase(std::remove_if(H.GetF().begin(), H.GetF().end(), [edge](Route &route) -> bool {
                bool isRemoved = route.Id == edge.GetId();
                if (isRemoved) {
                    route.Ptr->clear();
                }
                return isRemoved;
            }), H.GetF().end());
        }
    }

    H.RemoveEmptyBranches();
}
// Obtain a allowing Branch of max saturation
Branch GetAllowingBranch(H& H)
{
    auto FN = H.GetFN();
//    std::sort(FN.begin(), FN.end());
    Branch allowingBranch;
    int maxSaturation = 0;
    for(auto &branch : FN) {
        if (!branch.GetIsReliable()) {
            int saturation = H::GetBranchSaturation(branch);
            if (maxSaturation < saturation) {
                allowingBranch = branch;
                maxSaturation = saturation;
            }
        }
    }

    return allowingBranch;
}

Branch SimpleCase (std::vector<Branch>& FN, const Branch& pseudoBranch) {
    TwoNodesHypernetsCount++;
    if (FN.size() == 1) {
        return FN.front().GetIsReliable() ? pseudoBranch : pseudoBranch * FN.front();
    } else {
        FN.erase(std::remove_if(FN.begin(), FN.end(), [](Branch &item) ->
                bool { return item.GetIsReliable(); }), FN.end());
        return Branch::ParallelReduction(FN);
    }
}

Branch PairConnectivity(H &H, Branch &pseudoBranch) {
    PairConnectivityCalls++;

    if(ENABLE_BRIDGE_REDUCTION == 1 && BridgeReduction(H)) {
        return Branch::GetZero();
    }
    if(ENABLE_SIMPLE_CASE == 1 && H.GetNodes().size() < MAX_DIMENSIONAL) {
        return SimpleCase(H.GetFN(), pseudoBranch);
    }

    if(ENABLE_EDGE_REDUCTION == 1) {
        EdgeReduction(H);
        if (ENABLE_BRIDGE_REDUCTION == 1 && BridgeReduction(H)) {
            return Branch::GetZero();
        }
        if (ENABLE_SIMPLE_CASE == 1 && H.GetNodes().size() < MAX_DIMENSIONAL) {
            return SimpleCase(H.GetFN(), pseudoBranch);
        }
    }

    if(ENABLE_CHAIN_REDUCTION == 1) {
        std::vector<int> forbiddenNodes;
        ChainReduction(H, forbiddenNodes);
        if (ENABLE_BRIDGE_REDUCTION == 1 && BridgeReduction(H)) {
            return Branch::GetZero();
        }
        if (ENABLE_SIMPLE_CASE == 1 && H.GetNodes().size() < MAX_DIMENSIONAL) {
            return SimpleCase(H.GetFN(), pseudoBranch);
        }
    }

    Branch allowingBranch = GetAllowingBranch(H);
    if (Branch::IsUnacceptableBranch(allowingBranch)) {
        throw "PairConnectivity: strange allowingBranch";
    }

    Branch pseudoBranch1, pseudoBranch2;
    if (allowingBranch.IsSimpleBranch()) {
        pseudoBranch1 = pseudoBranch;
        pseudoBranch1.SetPower(pseudoBranch1.GetPower() + 1);
        pseudoBranch2 = pseudoBranch;
        pseudoBranch2.SetPower(pseudoBranch2.GetPower() + 1);
        pseudoBranch2.GetC().insert(pseudoBranch2.GetC().begin(), 0);
        pseudoBranch2.GetC().pop_back();
    } else {
        pseudoBranch1 = pseudoBranch * allowingBranch;
        pseudoBranch2 = pseudoBranch * ~allowingBranch;
    }

    auto HwithReliableBranch = H, HwithRemovedBranch = H;
    HwithReliableBranch.MakeReliableBranch(allowingBranch);
    HwithRemovedBranch.RemoveBranch(allowingBranch);

    if (!HwithRemovedBranch.IsSNconnected()) {
        UnconnectedHypernetsCount++;
        if (HwithReliableBranch.HasReliablePath()) {
            ReliableHypernetsCount++;
            return pseudoBranch1 * Branch::GetUnity();
        } else {
            return PairConnectivity(HwithReliableBranch, pseudoBranch1);
        }
    } else {
        if (HwithReliableBranch.HasReliablePath()) {
            ReliableHypernetsCount++;
            return pseudoBranch1 * Branch::GetUnity() + PairConnectivity(HwithRemovedBranch, pseudoBranch2);
        } else {
            return PairConnectivity(HwithReliableBranch, pseudoBranch1) +
                   PairConnectivity(HwithRemovedBranch, pseudoBranch2);
        }
    }
}