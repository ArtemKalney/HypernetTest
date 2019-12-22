#pragma once

#include "Stdafx.h"
#include "IIdentity.h"

//todo пернести C и методы
class Element {
protected:
    double _value;
    int _power;
    bool _isReliable;
public:
    Element() = default;

    Element(double value, const int &power, bool isReliable) :
            _value(value),
            _power(power),
            _isReliable(isReliable) {}

    Element(const Element &element) :
            _value(element._value),
            _power(element._power),
            _isReliable(element._isReliable) {}

    double GetValue() const {
        return _value;
    }

    void SetValue(const double &value) {
        _value = value;
    }

    void SetPower(const int &power) {
        _power = power;
    }

    int GetPower() const {
        return _power;
    }

    void SetIsReliable(bool isReliable) {
        _isReliable = isReliable;
    }

    bool GetIsReliable() const {
        return _isReliable;
    }
};