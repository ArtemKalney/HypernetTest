#pragma once

/// Режим дебага
#define IS_DEBUG 0

/// параметры для модели
#define MIN_MENC_VALUE 7.8

/// параметры для алгоритма полного перебора
#define IS_FULL_ENUMERATION_ALGORITHM 0

/// параметры для алгоритма имитации отжига
#define IS_SIMULATED_ANNEALING_ALGORITHM 1
#define T_MAX 10
#define T_MIN 0.01
#define MAX_ITERATION_COUNT 10 // в режиме дебага
#define INITIAL_BRANCH_COUNT 2 // экспертная оценка для начального решения

/// Определяет какие элементы надёжны в сети (ветви или вершины)
#define IS_NODES_RELIABLE 1

/// Определяет что считается (полином или число)
#define IS_NUMBER_COMPUTATION 1

/// Каждая редукция требует чтобы были включены предыдущие
#define ENABLE_SIMPLE_CASE 1
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
#define ENABLE_CHAIN_REDUCTION 1
/// Экспериментальные показатели для CHAIN_REDUCTION
#define IS_FULL_SEARCH_CHAINS 0
#define IS_REDUCE_RELIABLE_CHAINS 0

/// Параметры ввода
#define INPUT_NODE_VALUES 1
#define INPUT_BRANCH_VALUES 0
#define INPUT_BRANCH_COSTS 0
#define INPUT_MAX_BRANCH_SATURATIONS 0

/// Параметры вывода
#define OUTPUT_NODE_VALUES 0
#define OUTPUT_BRANCH_VALUES 0
#define OUTPUT_BRANCH_COSTS 0
#define OUTPUT_MAX_BRANCH_SATURATIONS 0
#define OUTPUT_SYNC_NODES 0