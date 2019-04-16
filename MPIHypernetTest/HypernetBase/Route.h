#pragma once

#include "Stdafx.h"
#include "IIdentity.h"

struct Route : public IIdentity
{
    int Id;
    std::shared_ptr<std::vector<int>> Ptr;

    Route() = default;

    Route(const int &id, std::shared_ptr<std::vector<int>> ptr) :
            Id(id),
            Ptr(std::move(ptr)) {}

    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & Id;
        ar & Ptr;
    }

    int GetId() const override {
        return Id;
    }
};

inline bool operator ==(Route firstRoute, Route secondRoute)
{
    return firstRoute.Id == secondRoute.Id;
}