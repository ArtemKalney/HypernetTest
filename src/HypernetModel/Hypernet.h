#pragma once

#include "Branch.h"
#include "Node.h"

class H {
private:
    std::vector<Branch> _FN;
    std::vector<Node> _nodes;
    std::vector<Route> _F;

    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & _FN;
        ar & _nodes;
        ar & _F;
    }

    // region методы распознования

    bool IsSimpleChain(std::vector<Branch> &chain, std::vector<int> &nodesInChain);

    static bool IsExtensionBranch(Branch &item, const std::vector<Branch> &chain, const int &firstNode,
                                  const Branch &firstBranch, const int &secondNode, bool isReliableChain);

    // endregion

    // region методы изменения

    void RemovePenduntRoutesInChain(std::vector<int> &nodesInChain, std::vector<int> &nodePowers);

    // endregion
public:
    H() = default;

    H(std::vector<Branch> FN, std::vector<Node> nodes, std::vector<Route> F) :
            _FN(std::move(FN)),
            _nodes(std::move(nodes)),
            _F(std::move(F)) {}

    H(const H &H) :
            _FN(H._FN),
            _nodes(H._nodes),
            _F(H._F) {
        // создаём новые вектора
        for (auto &route : _F) {
            auto vec = *route.Ptr;
            auto ptr = std::make_shared<std::vector<int>>(vec);
            route.Ptr = ptr;
            for (auto &branch : _FN) {
                for (auto &item : branch.GetRoutes()) {
                    if (item == route) {
                        item.Ptr = ptr;
                    }
                }
            }
        }
    }

    void SetFN(const std::vector<Branch> &FN) {
        _FN = FN;
    }

    std::vector<Branch> &GetFN() {
        return _FN;
    }

    void SetNodes(const std::vector<Node> &nodes) {
        _nodes = nodes;
    }

    std::vector<Node> &GetNodes() {
        return _nodes;
    }

    void SetF(const std::vector<Route> &F) {
        _F = F;
    }

    std::vector<Route> &GetF() {
        return _F;
    }

    //todo уменьшить количество static функкций

    // region методы распознования

    static bool IsPivotNode(const int &node);

    static bool IsSlightlyIncident(const int &node, const Route &route);

    static bool IsIncident(const int &node, const Route &route);

    static bool IsIncident(const int &node, const Branch &branch);

    static bool IsSlightlyIncident(const Branch &branch, const Route &route);

    bool IsSNconnected();

    template<class T>
    bool HasReliablePath();

    bool IsValidHypernet();

    // endregion

    // region методы получения

    static std::vector<int> GetNodePowers(const std::vector<Branch> &graph, const int &size);

    std::vector<Branch> GetSimpleChain(std::vector<int> &forbiddenNodes);

    std::vector<Branch> GetSN();

    std::vector<bool> GetCanDeleteMask(const std::vector<Branch> &SN);

    // endregion

    // region методы изменения

    void RemoveElement(const Branch &branch, bool reduct = true);

    void RemoveElement(const Node &branch, bool reduct = true);

    void RemoveNode(const int &node);

    void RemoveNodeFN(const int &node);

    void RemoveNodeSN(const int &node);

    void MakeReliableElement(const Branch &branch);

    void MakeReliableElement(const Node &node);

    void RemoveEmptyBranches();

    void RenumerateNodes(const int &firstNode, const int &secondNode);

    void RenumerateNodesForGen(const int &firstNode, const int &secondNode);

    // endregion

    // region методы хэлперы для факторизации

    Branch SimpleCase(const Branch& pseudoBranch);

    Node SimpleCase(const Node& pseudoBranch);

    template<class T>
    T GetAllowingElement();

    // endregion

    // region методы хэлперы для редукции

    void ChainReduction();

    bool BridgeReduction();

    void EdgeReduction();

    template<class T>
    bool Reductions(T &pseudoElement, T &returnValue);

    Branch ParallelReduction();

    // endregion

    // region методы для вывода

    void PrintHypernet();

    void LogHypernet();

    // endregion

    // region методы для отладки

    std::vector<std::vector<int>> GetRoutesF();

    std::vector<std::vector<std::vector<int>>> GetRoutesFN();

    // endregion
};

// region вспомогательные методы

void DFS(const int& node, std::vector<Node>& nodes, const std::vector<Branch>& graph);

std::vector<int> GetNodesInChain(const std::vector<Branch>& chain);

// endregion