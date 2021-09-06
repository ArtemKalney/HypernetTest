#include "../Hypernet.h"
#include "Globals.h"

template<>
Branch H::GetAllowingElement<Branch>() {
    //    std::sort(FN.begin(), FN.end());
    Branch allowingBranch;
    int maxSaturation = 0;
    for (auto &branch : _FN) {
        if (!branch.GetIsReliable()) {
            int saturation = branch.GetSaturation();
            if (maxSaturation < saturation) {
                allowingBranch = branch;
                maxSaturation = saturation;
            }
        }
    }
    bool isFirstNodeUnacceptable = allowingBranch.GetFirstNode() < 0,
            isSecondNodeUnacceptable = allowingBranch.GetSecondNode() < 0,
            isPowerUnacceptable = allowingBranch.GetPower() < 0;
    if (isFirstNodeUnacceptable || isSecondNodeUnacceptable || isPowerUnacceptable) {
        throw "PairConnectivity: unacceptable allowingElement";
    }

    return allowingBranch;
}

template<>
Node H::GetAllowingElement<Node>() {
    Node allowingNode;
    int maxIncidentEdgesCount = 0;
    for (auto &node : _nodes) {
        if (node.GetIsReliable()) {
            continue;
        }

        int incidentEdgesCount = 0;
        for (auto &item : _F) {
            if (IsIncident(node.GetId(), item)) {
                incidentEdgesCount++;
            }
        }
        if (maxIncidentEdgesCount < incidentEdgesCount) {
            allowingNode = node;
            maxIncidentEdgesCount = incidentEdgesCount;
        }
    }
    bool isNodeUnacceptable = allowingNode.GetId() < 0, isPowerUnacceptable = allowingNode.GetPower() < 0;
    if (isNodeUnacceptable || isPowerUnacceptable) {
        throw "PairConnectivity: unacceptable allowingElement";
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
        return pseudoBranch*ParallelReduction();
    }
}

Node H::SimpleCase(const Node& pseudoBranch) {
    return IsSNconnected() ? pseudoBranch : Node::GetZero();
}