#pragma once

#include "Stdafx.h"

struct Route {
    int Id;
    std::shared_ptr<std::vector<int>> Ptr;

    Route(const int &id, std::shared_ptr<std::vector<int>> ptr) :
            Id(id),
            Ptr(std::move(ptr)) {}
};

bool operator ==(Route firstRoute, Route secondRoute);