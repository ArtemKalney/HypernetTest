#pragma once

#include "Globals.h"

template <class T>
T operator *(T firstElement, T secondElement) {
    T result = T::GetZero();
    if (IS_NUMBER_COMPUTATION == 1) {
        result.SetValue(firstElement.GetValue() * secondElement.GetValue());
        return result;
    }
    if (firstElement.IsZero() || secondElement.IsZero()) {
        return result;
    }
    if (firstElement.IsUnity()) {
        return secondElement;
    }
    if (secondElement.IsUnity()) {
        return firstElement;
    }

    result.GetC().resize(firstElement.GetC().size());
    result.SetPower(firstElement.GetPower() + secondElement.GetPower());
    for (int i = 0; i < firstElement.GetC().size(); i++) {
        for (int j = 0; j < secondElement.GetC().size(); j++) {
            if (firstElement.GetC()[i] != 0 && secondElement.GetC()[j] != 0) {
                if (i + j >= result.GetC().size()) {
                    throw "Branch operator *: vector is out of size";
                }
                result.GetC()[i + j] += firstElement.GetC()[i] * secondElement.GetC()[j];
            }
        }
    }
    result.SetIsReliable(false);
    return result;
}

template <class T>
T operator +(T firstElement, T secondElement) {
    T result = T::GetZero();
    if (IS_NUMBER_COMPUTATION == 1) {
        result.SetValue(firstElement.GetValue() + secondElement.GetValue());
        return result;
    }
    if (firstElement.IsZero() && secondElement.IsZero()) {
        return result;
    }
    if (!firstElement.IsZero() && secondElement.IsZero()) {
        return firstElement;
    }
    if (firstElement.IsZero() && !secondElement.IsZero()) {
        return secondElement;
    }
    if (firstElement.IsUnity() && secondElement.IsUnity()) {
        throw "Branch operator +: unity + unity";
    }

    result.GetC().resize(firstElement.GetC().size());
    // Multiply by a unit of the required degree, so that the degrees x and y coincide
    if (firstElement.GetPower() != secondElement.GetPower()) {
        int power = abs(firstElement.GetPower() - secondElement.GetPower());
        T I = T::GetElement(Bin[power], power);
        if (firstElement.GetPower() < secondElement.GetPower()) {
            firstElement = firstElement * I;
        } else {
            secondElement = secondElement * I;
        }
    }

    result.SetPower(firstElement.GetPower());
    for (int i = 0; i<result.GetC().size(); i++) {
        result.GetC()[i] = firstElement.GetC()[i] + secondElement.GetC()[i];
    }
    result.SetIsReliable(false);
    return result;
}

template <class T>
T operator -(T firstElement, T secondElement) {
    T result = T::GetZero();
    if (IS_NUMBER_COMPUTATION == 1) {
        result.SetValue(firstElement.GetValue() - secondElement.GetValue());
        return result;
    }
    if (firstElement.IsZero() && secondElement.IsZero()) {
        return result;
    }
    if (!firstElement.IsZero() && secondElement.IsZero()) {
        return firstElement;
    }
    if (firstElement.IsZero() && !secondElement.IsZero()) {
        return secondElement;
    }
    if (firstElement.IsUnity() && secondElement.IsUnity()) {
        throw "Branch operator -: unity - unity";
    }

    result.GetC().resize(firstElement.GetC().size());
    // Multiply by a unit of the required degree, so that the degrees x and y coincide
    if (firstElement.GetPower() != secondElement.GetPower()) {
        int power = abs(firstElement.GetPower() - secondElement.GetPower());
        T I = T::GetElement(Bin[power], power);
        if (firstElement.GetPower() < secondElement.GetPower()) {
            firstElement = firstElement * I;
        }
        if (secondElement.GetPower() < firstElement.GetPower()) {
            secondElement = secondElement * I;
        }
    }

    result.SetPower(firstElement.GetPower());
    for (int i = 0; i<result.GetC().size(); i++) {
        result.GetC()[i] = firstElement.GetC()[i] - secondElement.GetC()[i];
    }
    result.SetIsReliable(false);
    return result;
}

template <class T>
T operator ~(T element) {
    T result = T::GetZero();
    if (IS_NUMBER_COMPUTATION == 1) {
        result.SetValue(1 - element.GetValue());
        return result;
    }
    if (element.IsZero()) {
        return T::GetUnity();
    }
    if (element.IsUnity()) {
        return result;
    }

    result.GetC().resize(element.GetC().size());
    // Subtract from unity the same degree as x
    result.SetPower(element.GetPower());
    T I = T::GetElement(Bin[result.GetPower()], result.GetPower());
    for (int i = 0; i<result.GetC().size(); i++) {
        result.GetC()[i] = I.GetC()[i] - element.GetC()[i];
    }
    result.SetIsReliable(false);
    return result;
}