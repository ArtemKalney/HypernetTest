#pragma once

//todo разобраться почему тесты ломаются если убрать
class IIdentity
{
public:
    virtual ~IIdentity() = default;
    virtual int GetId() const = 0;

    static bool IdentityCompare(const IIdentity &a, const IIdentity &b)
    {
        return a.GetId() < b.GetId();
    }
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
std::string VectorToString(std::vector<T>& vector, std::string delimiter = ",") {
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
bool VectorEqual(std::vector<T>& vector1, std::vector<T>& vector2) {
    if (vector1.size() != vector2.size()) {
        return false;
    }

    std::sort(vector1.begin(), vector1.end());
    std::sort(vector2.begin(), vector2.end());

    return std::equal(vector1.begin(), vector1.end(), vector2.begin());
}