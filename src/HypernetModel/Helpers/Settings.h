#pragma once

struct Settings{
    /// Определяет какие элементы надёжны в сети (ветви или вершины)
    int IsNodesReliable;

    /// Определяет что считается (полином или число)
    int IsNumberComputation;

    /// Параметры ввода
    int InputNodesValues;
    int InputBranchValues;
    int InputBranchCosts;
    int InputMaxBranchSaturations;

    /// параметры для алгоритма имитации отжига
    double MinMENCValue;
    double TMax;
    double TMin;
    int InitialBranchCount;

    /// Определяет дефолтное значение надёжности
    double ReliabilityValue;
};