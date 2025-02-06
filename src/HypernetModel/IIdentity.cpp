#include "IIdentity.h"
#include "Branch.h"
#include "Node.h"

template <>
bool operator ==<Branch>(const Branch& a, const Branch& b) {
    return a.GetId() == b.GetId();
}

template <>
bool operator ==<Node>(const Node& a, const Node& b) {
    return a.GetId() == b.GetId();
}

template <>
bool operator ==<Route>(const Route& a, const Route& b) {
    return a.GetId() == b.GetId();
}