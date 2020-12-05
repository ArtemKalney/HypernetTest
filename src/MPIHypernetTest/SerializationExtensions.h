#pragma once

//friend class boost::serialization::access;
//template<typename Archive>
//void serialize(Archive &ar, const unsigned version) {
//    ar & _id;
//    ar & _value;
//    ar & _C;
//    ar & _routes;
//    ar & _power;
//    ar & _firstNode;
//    ar & _secondNode;
//    ar & _isReliable;
//}

//friend class boost::serialization::access;
//template<typename Archive>
//void serialize(Archive &ar, const unsigned version) {
//    ar & _id;
//    ar & _value;
//    ar & _C;
//    ar & _power;
//    ar & _isReliable;
//}
//
//friend class boost::serialization::access;
//template<typename Archive>
//void serialize(Archive &ar, const unsigned version) {
//    ar & _FN;
//    ar & _nodes;
//    ar & _F;
//}
//
//friend class boost::serialization::access;
//template<typename Archive>
//void serialize(Archive &ar, const unsigned version) {
//    ar & Id;
//    ar & Ptr;
//}

//todo make extensions of HypernetBase classes

//class ISerialize
//{
//};
//
//struct SerializeRoute : Route, ISerialize {
//    SerializeRoute() : Route() {}
//
////    SerializeRoute(const int &id, std::shared_ptr<std::vector<int>> ptr) : Route(id, ptr) {}
//
//    friend class boost::serialization::access;
//    template<typename Archive>
//    void serialize(Archive &ar, const unsigned version) {
//        ar & Id;
//        ar & Ptr;
//    }
//};
//
//struct SerializeNode : Node, ISerialize {
//    SerializeNode() : Node() {}
//
////    SerializeNode(const int& nodeNumber, bool isVisited) : Node(nodeNumber, isVisited) {}
//
//    friend class boost::serialization::access;
//    template<typename Archive>
//    void serialize(Archive &ar, const unsigned version) {
//        ar & NodeNumber;
//        ar & IsVisited;
//    }
//};
//
//class Branch : public Branch, ISerialize {
//private:
//    friend class boost::serialization::access;
//    template<typename Archive>
//    void serialize(Archive &ar, const unsigned version) {
//        ar & _id;
//        ar & _C;
//        ar & _power;
//        ar & _firstNode;
//        ar & _secondNode;
//        ar & _isReliable;
//        std::vector<SerializeRoute> serializeRoutes;
//        ar & serializeRoutes;
//        std::vector<Route> routes(serializeRoutes.begin(), serializeRoutes.end());
//       _routes = routes;
//    }
//public:
//    Branch() : Branch() {}
//
////    SerializeBranch(const int& id, const std::vector<double> C, std::vector<SerializeRoute>& SerializeRoutes,
////                    const int& power, const int& firstNode, const int& secondNode, bool isReliable) :
////            SerializeBranch(id, std::move(C), power, firstNode, secondNode, isReliable)
////    {
////        std::vector<Route> routes(SerializeRoutes.begin(), SerializeRoutes.end());
////        _routes = routes;
////    }
//
//    Branch(const Branch& branch) : Branch(branch) {}
//};
//
//class H : public H, ISerialize {
//private:
//    friend class boost::serialization::access;
//    template<typename Archive>
//    void serialize(Archive &ar, const unsigned version) {
////        ar & _FN;
////        ar & _nodes;
////        ar & _F;
//        std::vector<Branch> SerializeBranches;
//        ar & SerializeBranches;
//        std::vector<Branch> Branches(SerializeBranches.begin(), SerializeBranches.end());
//        _FN = Branches;
//        std::vector<SerializeRoute> SerializeRoutes;
//        ar & SerializeRoutes;
//        std::vector<Route> Routes(SerializeRoutes.begin(), SerializeRoutes.end());
//        _F = Routes;
//        std::vector<SerializeNode> SerializeNodes;
//        ar & SerializeNodes;
//        std::vector<Node> Nodes(SerializeNodes.begin(), SerializeNodes.end());
//        _nodes = Nodes;
//    }
//public:
//    H() : H() {}
//
////    SerializeH(std::vector<SerializeBranch> SerializeFN, std::vector<SerializeNode> SerializeNodes,
////               std::vector<SerializeRoute> SerializeF)
////    {
////        std::vector<SerializeBranch> FN(SerializeFN.begin(), SerializeFN.end());
////        _FN = std::move(FN);
////        std::vector<Route> F(SerializeF.begin(), SerializeF.end());
////        _F = std::move(F);
////        std::vector<Node> nodes(SerializeNodes.begin(), SerializeNodes.end());
////        _nodes = std::move(nodes);
////    }
//
//    H(const H &h) : H(h) {}
//};