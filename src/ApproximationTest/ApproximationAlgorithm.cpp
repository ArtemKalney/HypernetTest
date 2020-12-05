#include "Stdafx.h"
#include "Funcs.h"
#include "Globals.h"

Branch ApproximationAlgorithm(H &H) {
    int FNsize = H.GetFN().size();
    std::vector<double> C;
    std::vector<double> branchCounts;
    branchCounts.push_back(1);

    for(int i=0; i<FNsize - 1; i++) {
        std::vector<Branch> branches;
        for(auto &item : H.GetFN()) {
            auto HwithRemovedBranch = H;
            HwithRemovedBranch.RemoveBranch(item);
            if (HwithRemovedBranch.IsSNconnected()) {
                branches.push_back(item);
            }
        }

        branchCounts.push_back(branches.size());
        if (branches.size() == 0) {
            break;
        }
        int index = branches.size() == 1 ? 0 : rand() % (branches.size() - 1);
        H.RemoveBranchForApproximation(branches[index]);
    }

    if (branchCounts.size() < FNsize) {
        branchCounts.resize(FNsize);
    }
    for(int i=0; i<FNsize; i++) {
        double multiplication = 1;
        for(int j=0; j<=i; j++) {
            multiplication *= branchCounts[j];
        }
        C.push_back(multiplication/Factorials[i]);
    }

    Branch result = Branch::GetBranch(FNsize);
    result.SetC(C);
    return result;
}