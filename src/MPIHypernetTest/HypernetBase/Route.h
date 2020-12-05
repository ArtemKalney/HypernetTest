#pragma once

#include "Stdafx.h"
#include "IIdentity.h"
#include "Globals.h"

struct Route : public IIdentity
{
    int Id;
    std::shared_ptr<std::vector<int>> Ptr;

    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & Id;
        ar & Ptr;
    }

    Route() = default;

    Route(const int &id, std::shared_ptr<std::vector<int>> ptr) :
            Id(id),
            Ptr(std::move(ptr)) {}

    int GetId() const override {
        return Id;
    }
};

inline bool operator ==(Route firstRoute, Route secondRoute)
{
    return firstRoute.Id == secondRoute.Id;
}