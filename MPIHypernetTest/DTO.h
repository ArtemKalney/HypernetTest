#pragma once

#include "HypernetBase/Hypernet.h"
// структура для коммуникациями между процессами
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