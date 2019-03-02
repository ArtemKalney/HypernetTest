#pragma once

#include "Hypernet.h"

struct DTO {
    DTO(const H &H, const Branch &branch) :
            H(H),
            Branch(branch) {}

    H H;
    Branch Branch;

    DTO() = default;

    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & H;
        ar & Branch;
    }
};