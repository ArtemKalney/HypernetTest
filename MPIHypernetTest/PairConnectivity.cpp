#include "Funcs.h"
#include "DTO.h"
#include "Globals.h"

// получение разрешай ветви с максимальной насыщенностью
Branch GetAllowingBranch(H& H) {
    auto FN = H.GetFN();
//    std::sort(FN.begin(), FN.end());
    Branch allowingBranch;
    int maxSaturation = 0;
    // цикл поиска ветви
    for(auto &branch : FN) { 
        if (!branch.GetIsReliable()) {
            int saturation = branch.GetBranchSaturation();
            if (maxSaturation < saturation) {
                allowingBranch = branch;
                maxSaturation = saturation;
            }
        }
    }

    return allowingBranch;
}
// явный расчёт гиперсети
Branch SimpleCase (std::vector<Branch>& FN, const Branch& pseudoBranch) {
    TwoNodesHypernets++;
    if (FN.size() == 1) {
        return FN.front().GetIsReliable() ? pseudoBranch : pseudoBranch*FN.front();
    } else {
        // убираем надёжные ветви, они уже учтены
        FN.erase(std::remove_if(FN.begin(), FN.end(), [](Branch &item) ->
                bool { return item.GetIsReliable(); }), FN.end());
        return pseudoBranch*Branch::ParallelReduction(FN);
    }
}
// расчёт парной связности гиперсети
Branch PairConnectivity(H &H, Branch &pseudoBranch) {
    PairConnectivityCalls++;
    // конфигурация метода
    if(ENABLE_BRIDGE_REDUCTION == 1 && H.BridgeReduction()) {
        return Branch::GetZero();
    }
    if(ENABLE_SIMPLE_CASE == 1 && H.GetNodes().size() < MAX_DIMENSIONAL) {
        return SimpleCase(H.GetFN(), pseudoBranch);
    }

    if(ENABLE_EDGE_REDUCTION == 1) {
        H.EdgeReduction();
        if (ENABLE_BRIDGE_REDUCTION == 1 && H.BridgeReduction()) {
            return Branch::GetZero();
        }
        if (ENABLE_SIMPLE_CASE == 1 && H.GetNodes().size() < MAX_DIMENSIONAL) {
            return SimpleCase(H.GetFN(), pseudoBranch);
        }
    }

    if(ENABLE_CHAIN_REDUCTION == 1) {
        H.ChainReduction();
        if (ENABLE_BRIDGE_REDUCTION == 1 && H.BridgeReduction()) {
            return Branch::GetZero();
        }
        if (ENABLE_SIMPLE_CASE == 1 && H.GetNodes().size() < MAX_DIMENSIONAL) {
            return SimpleCase(H.GetFN(), pseudoBranch);
        }
    }
    // получение разрешающей ветви
    Branch allowingBranch = GetAllowingBranch(H);
    if (allowingBranch.IsUnacceptableBranch()) {
        throw "PairConnectivity: unacceptable allowingBranch";
    }
    // вычисленние множителей для метода факторизации 
    Branch pseudoBranch1, pseudoBranch2;
    pseudoBranch1 = pseudoBranch * allowingBranch;
    pseudoBranch2 = pseudoBranch * ~allowingBranch;
    // вычисление гиперсетей для метода факторизации 
    auto HwithReliableBranch = H, HwithRemovedBranch = H;
    HwithReliableBranch.MakeReliableBranch(allowingBranch);
    HwithRemovedBranch.RemoveBranch(allowingBranch);
    // расчёт парной связности
    if (!HwithRemovedBranch.IsSNconnected()) {
        UnconnectedHypernets++;
        if (HwithReliableBranch.HasReliablePath()) {
            ReliableHypernets++;
            return pseudoBranch1 * Branch::GetUnity();
        } else {
            return PairConnectivity(HwithReliableBranch, pseudoBranch1);
        }
    } else {
        if (HwithReliableBranch.HasReliablePath()) {
            ReliableHypernets++;
            return pseudoBranch1 * Branch::GetUnity() + PairConnectivity(HwithRemovedBranch, pseudoBranch2);
        } else {
            // назначение исполняющих процессов (запрос помощи процессом)
            int value, helpProcessor = 0;
            if (HwithReliableBranch.GetNodes().size() >= MAX_DIMENSIONAL) {
                MPI_Status status;
                MPI_Send(&value, 0, MPI_INT, HOST_PROCESSOR, I_NEED_HELP_TAG, MPI_COMM_WORLD);
                MPI_Recv(&helpProcessor, 1, MPI_INT, HOST_PROCESSOR, I_NEED_HELP_TAG, MPI_COMM_WORLD, &status);

                if (helpProcessor != 0) {
                    HelpProcessors++;
                    DTO data = DTO(HwithReliableBranch, pseudoBranch1);
                    Send(data, helpProcessor);
                    return PairConnectivity(HwithRemovedBranch, pseudoBranch2);
                }
            }

            return PairConnectivity(HwithReliableBranch, pseudoBranch1) +
                   PairConnectivity(HwithRemovedBranch, pseudoBranch2);
        }
    }
}