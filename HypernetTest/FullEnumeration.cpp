#include "Funcs.h"
#include "Globals.h"

void FullEnumeration(const H &H, const std::vector<Branch> &branchList, Branch &sum, std::vector<bool> &branchMask,
                     int &curPos){
    if (curPos == branchList.size()) {
        auto hypernet = H;
        for (int i = 0; i < branchMask.size(); i++) {
            Branch branch = branchList[i];
            if (!branchMask[i]) {
                hypernet.RemoveBranch(branch);
            } else {
                hypernet.MakeReliableBranch(branch);
            }
        }

        if (hypernet.IsSNconnected()) {
            ReliableHypernets++;
            Branch result = Branch::GetBranch(0);
            for (bool item : branchMask) {
                if (item) {
                    result.SetPower(result.GetPower() + 1);
                } else {
                    result.SetPower(result.GetPower() + 1);
                    result.GetC().insert(result.GetC().begin(), 0);
                    result.GetC().pop_back();
                }
            }

            if (result.GetPower() != branchList.size()) {
                throw "FullEnumeration: strange result power";
            }
            sum = sum + result;
        } else {
            UnconnectedHypernets++;
        }
    }
    else{
        branchMask[curPos] = false;
        int increasedPos = curPos + 1;
        FullEnumeration(H, branchList, sum, branchMask, increasedPos);
        branchMask[curPos] = true;
        FullEnumeration(H, branchList, sum, branchMask, increasedPos);
    }
}