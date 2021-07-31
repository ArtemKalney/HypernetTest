#pragma once

//todo разобраться почему тесты ломаются если убрать
class IIdentity
{
public:
    virtual ~IIdentity() = default;
    virtual int GetId() const = 0;
};

template <class T>
bool operator ==(T firstElement, T secondElement) {
    return firstElement.GetId() == secondElement.GetId();
}

template <class T>
bool operator ==(T element, int id) {
    return element.GetId() == id;
}

template <class T>
bool operator >(const T& element, const int id) {
    return element.GetId() > id;
}

template <class T>
std::string VectorToString(std::vector<T> &vector, std::string delimiter) {
    std::string str = "";
    for(auto &item : vector) {
        str += std::to_string(item.GetId());
        if (item == vector.back()) {
            continue;
        } else {
            str += delimiter;
        }
    }

    return str;
}

template <class T>
std::string VectorToString(std::vector<T> &vector) {
    return VectorToString(vector, ",");
}