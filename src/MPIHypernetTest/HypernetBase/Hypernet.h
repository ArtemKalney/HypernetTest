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

    //todo уменьшить static функкций
    //todo DFS отдельно
    //todo метод ISFNConnected для BridgeReduction
    template <class T>
    void DFS(const int& node, std::vector<Node>& nodes, const std::vector<T>& graph);

    static std::vector<int> GetNodePowers(const std::vector<Branch> &graph, const int &size);

    static bool IsPivotNode(const int &node);

    static bool IsSlightlyIncident(const int &node, const Route &route);

    static bool IsIncident(const int &node, const Route &route);

    static bool IsIncident(const int &node, const Branch &branch);

    bool IsSNconnected();

    template<class T>
    bool HasReliablePath();

    std::vector<Branch> GetSimpleChain(std::vector<int> &forbiddenNodes);

    void RemoveElement(const Branch &branch);

    void RemoveElement(const Node &branch);

    void RemoveNode(const int &node);

    void RemoveNodeFN(const int &node);

    void RemoveNodeSN(const int &node);

    void MakeReliableElement(const Branch &branch);

    void MakeReliableElement(const Node &node);

    void RemoveEmptyBranches();

    void RenumerateNodes(const int &firstNode, const int &secondNode);

    std::vector<Branch> GetSN();

    std::vector<bool> GetCanDeleteMask(const std::vector<Branch> &SN);

    //factorization helper
    Branch SimpleCase(const Branch& pseudoBranch);

    Node SimpleCase(const Node& pseudoBranch);

    template<class T>
    T GetAllowingElement();

    //reductions
    void ChainReduction();

    bool BridgeReduction();

    void EdgeReduction();

    template<class T>
    bool Reductions(T &pseudoElement, T &returnValue);

    Branch ParallelReduction();

    //print
    void PrintHypernet();

    void LogHypernet();

    // debug
    void RenumerateNodesForGen(const int &firstNode, const int &secondNode);

    std::vector<std::vector<int>> GetRoutesF();

    std::vector<std::vector<std::vector<int>>> GetRoutesFN();
};
//todo разобраться
std::vector<int> GetNodesInChain(const std::vector<Branch>& chain);
bool IsSimpleChain(H &hypernet, std::vector<Branch> &chain, std::vector<int> &nodesInChain);
void RemovePenduntRoutesInChain(H &hypernet, std::vector<int> &nodesInChain, std::vector<int> &nodePowers);
bool IsExtensionBranch(Branch &item, H &H, const std::vector<Branch> &chain, const int &firstNode,
                       const Branch &firstBranch, const int &secondNode, bool isReliableChain);