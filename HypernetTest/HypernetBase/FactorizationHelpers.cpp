#include "Hypernet.h"
#include "Globals.h"

template<>
Branch H::GetAllowingElement<Branch>() {
    //    std::sort(FN.begin(), FN.end());
    Branch allowingBranch;
    int maxSaturation = 0;
    for (auto &branch : _FN) {
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

template<>
Node H::GetAllowingElement<Node>() {
    Node allowingNode;
    int maxIncidentEdgesCount = 0;
    for (auto &node : _nodes) {
        if (node.IsReliable) {
            continue;
        }

        int incidentEdgesCount = 0;
        for (auto &item : _F) {
            if (IsIncident(node.NodeNumber, item)) {
                incidentEdgesCount++;
            }
        }
        if (maxIncidentEdgesCount < incidentEdgesCount) {
            allowingNode = node;
            maxIncidentEdgesCount = incidentEdgesCount;
        }
    }

    return allowingNode;
}

Branch H::SimpleCase(const Branch& pseudoBranch) {
    TwoNodesHypernets++;
    if (_FN.size() == 1) {
        return _FN.front().GetIsReliable() ? pseudoBranch : pseudoBranch*_FN.front();
    } else {
        _FN.erase(std::remove_if(_FN.begin(), _FN.end(), [](Branch &item) ->
                bool { return item.GetIsReliable(); }), _FN.end());
        return pseudoBranch*Branch::ParallelReduction(_FN);
    }
}

Node H::SimpleCase(const Node& pseudoBranch) {
    return IsSNconnected() ? pseudoBranch : Node::GetZero();
}