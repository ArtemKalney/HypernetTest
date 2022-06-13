#include "Funcs.h"
#include "RandomHypernetGenerator.h"

// генератор случайных сетей для построения случайной гиперсети
std::vector<Branch> RandomHypernetGenerator::GetRandomNetwork(int nodesCount, int edgeCount, int vectorSize) {
    srand(_seed++);
    std::vector<Branch> network;
    std::vector<int> nodes1; // набор вершин 1
    for (int i = 0; i < nodesCount; i++) {
        nodes1.push_back(i);
    }
    std::vector<int> nodes2; // набор вершин 2
    int j = rand() % (nodes1.size() - 1);
    // цикл построения случайного дерева, гарантирует связность сети
    for (int i = 0; i < nodesCount - 1; i++) {
        nodes2.push_back(nodes1[j]);
        nodes1.erase(nodes1.begin() + j);
        int firstPosition = nodes1.size() > 1 ? rand() % (nodes1.size() - 1) : 0;
        int secondPosition = nodes2.size() > 1 ? rand() % (nodes2.size() - 1) : 0;
        j = firstPosition;
        network.push_back(Branch::GetSimpleElement(network.size(), nodes1[firstPosition], nodes2[secondPosition],
                                                   vectorSize));
    }
    // расстановка оставшихся рёбер
    for (int i = 0; i < edgeCount - nodesCount + 1; i++) {
        int firstNode = rand() % nodesCount, secondNode = rand() % nodesCount;
        while (firstNode == secondNode)
            secondNode = rand() % nodesCount;
        network.push_back(Branch::GetSimpleElement(network.size(), firstNode, secondNode, vectorSize));
    }

    return network;
}

// укладка ребра в первичную сеть
void RandomHypernetGenerator::BFSWithRoute(std::vector<int> &nodeRote, std::vector<Branch> &branchRoute,
                                           const int startNode, const int endNode) {
    std::vector<bool> isVisited(_nodes.size(), false);
    std::stack<int> queue;
    queue.push(startNode);
    isVisited[startNode] = true;
    std::vector<std::vector<int>> nodeRotes(_nodes.size());
    nodeRotes[startNode].push_back(startNode);
    std::vector<std::vector<Branch>> branchRoutes(_nodes.size());
    while(!queue.empty()) {
        int node = queue.top();
        queue.pop();
        if (node == endNode) {
            nodeRote = nodeRotes[endNode];
            branchRoute = branchRoutes[endNode];

            return;
        }

        for(auto &branch : _primaryNetwork) {
            // учитываем максимальную ёмкость ветви, что может приводить к тому что не все вершины будут посещены и
            // путь может быть не найден
            if (H::IsIncident(node, branch) && (branch.GetMaxSaturation() < 1 || branch.GetSaturation() < branch.GetMaxSaturation())) {
                int incidentNode = branch.GetFirstNode() != node ? branch.GetFirstNode() : branch.GetSecondNode();
                if (!isVisited[incidentNode]) {
                    queue.push(incidentNode);
                    isVisited[incidentNode] = true;
                    nodeRotes[incidentNode] = nodeRotes[node];
                    nodeRotes[incidentNode].push_back(incidentNode);
                    branchRoutes[incidentNode] = branchRoutes[node];
                    branchRoutes[incidentNode].push_back(branch);
                }
            }
        }
    }
}

H RandomHypernetGenerator::GenerateHypernet() {
    std::vector<Route> routes;
    for (auto &edge : _secondaryNetwork) {
        int node = edge.GetFirstNode();
        std::vector<int> nodeRote{node};
        std::vector<Branch> branchRoute;
        BFSWithRoute(nodeRote, branchRoute, node, edge.GetSecondNode());
        // если не нашли маршрут или нашли слишком длинный маршрут, то заново запускаем генерацию
        if (branchRoute.empty() || (_maxDistance > 0 && branchRoute.size() > _maxDistance)) {
            routes.clear();
            for (auto &item : _primaryNetwork) {
               item.GetRoutes().clear();
            }
            _secondaryNetwork = GetRandomNetwork(_nodes.size(), _secondaryNetwork.size(), _primaryNetwork.size());

            return GenerateHypernet();
        }

        routes.emplace_back(routes.size(), std::make_shared<std::vector<int>>(nodeRote));
        for(auto &item : branchRoute) {
            std::find(_primaryNetwork.begin(), _primaryNetwork.end(), item)->GetRoutes().push_back(routes.back());
        }
    }

    return H(std::move(_primaryNetwork), std::move(_nodes), std::move(routes));
}