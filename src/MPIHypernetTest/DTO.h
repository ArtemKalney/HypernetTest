#pragma once

#include "../HypernetModel/Hypernet.h"

// структура для коммуникациями между процессами
template<class T>
struct DTO {
    DTO(H &H, const T &branch) :
            Hypernet(H),
            Element(branch) {}

    H Hypernet;
    T Element;

    DTO() = default;

    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & Hypernet;
        ar & Element;
    }
};