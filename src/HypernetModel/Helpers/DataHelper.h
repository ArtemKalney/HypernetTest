#pragma once

#include <Globals.h>
#include "../Route.h"
#include "../Node.h"
#include "../Branch.h"
#include "../Hypernet.h"

void ComputeBinomialCoefficients(int vectorSize);
void ErrorHandler(const char *str);
void GetData(std::vector<Branch>& branches, std::vector<Node>& nodes, std::vector<Route>& routes);
void HandleException(const std::exception &e);

template <class T>
void ComputeCombinations(const std::vector<T> &vector, std::vector<std::vector<T>> &combinations,
                         std::vector<T> &combination, int offset, int k) {
    if (k == 0) {
        combinations.push_back(combination);
        return;
    }

    for (int i = offset; i <= vector.size() - k; ++i) {
        combination.push_back(vector[i]);
        ComputeCombinations(vector, combinations, combination, i + 1, k - 1);
        combination.pop_back();
    }
}

template <class T>
void NormalizeSolution(T &element){
    int vectorSize = element.GetC().size();
    element.GetC().resize(vectorSize + 1);
    if (element.GetPower() < vectorSize) {
        int power = vectorSize - element.GetPower();
        T unity = T::GetElement(Bin[power], power);
        element = element * unity;
    }
}

template <class T>
void PrintSolution(T& sum) {
    if (!sum.IsZero()) {
        if (AppSettings.IsNumberComputation == 0) {
            NormalizeSolution(sum);
            std::cout << "Value at point " << AppSettings.ReliabilityValue << ": " << std::setprecision(14) << sum.GetPolynomialValue(AppSettings.ReliabilityValue) << std::endl;
        }
        if (AppSettings.IsNumberComputation == 1) {
            output << std::setprecision(14) << sum.GetPolynomialValue(AppSettings.ReliabilityValue) << " ";
        }
        else {
            for (auto &item : sum.GetC()) {
                output << std::setprecision(14) << item << " ";
            }
        }
        output << std::endl;
    } else {
        std::cout << "unconnected hypernet" << std::endl;
    }
}

template <class T>
bool IsUniqueId(const std::vector<T>& items, const int id) {
    int count = 0;
    for(auto &item : items) {
        if (item.GetId() == id) {
            count++;
        }
    }

    return count < 2;
}

template <class T>
int GetUniqueId(const std::vector<T>& items) {
    return std::max_element(items.begin(), items.end(), IIdentity::IdentityCompare)->GetId() + 1;
}

bool DoubleEquals(double a, double b, double epsilon = 0.0000000000001);