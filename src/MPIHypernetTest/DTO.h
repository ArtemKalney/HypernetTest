#pragma once

#include "HypernetBase/Hypernet.h"

// структура для коммуникациями между процессами
template<class T>
struct DTO {
    DTO(H &H, const T &branch) :
            H(H),
            Element(branch) {}

    H H;
    T Element;

    DTO() = default;

    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & H;
        ar & Element;
    }
};