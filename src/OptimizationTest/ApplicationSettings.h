#pragma once

// Определяет какие элементы надёжны в сети (ветви или вершины)
#define IS_NODES_RELIABLE 1
// Определяет что считается (полином или число)
#define IS_NUMBER_COMPUTATION 1
// Режим дебага
#define IS_DEBUG 1
// Максимальная размерность гиперсети для факторизации
#define MAX_DIMENSIONAL 3
// Каждая редукция требует чтобы были включены предыдущие
#define ENABLE_SIMPLE_CASE 1
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
#define ENABLE_CHAIN_REDUCTION 1
// Экспериментальные показатели для CHAIN_REDUCTION
#define IS_FULL_SEARCH_CHAINS 0
#define IS_REDUCE_RELIABLE_CHAINS 0
// параметры для генерации гиперсети
#define IS_GENERATE_NETWORK 0
#define NETWORK_NODES_PROPORTION 5
// параметры для модели
#define MIN_MENC_VALUE 4
#define MAX_BRANCH_COUNT 3
// параметры для алгоритма полного перебора
#define IS_FULL_ENUMERATION_ALGORITHM 0
// параметры для генетического алгоритма
#define IS_GENETIC_ALGORITHM 0
#define USE_MAX_POPULATION_CAPACITY 0
#define MAX_INITIAL_POPULATION_COUNT 1
#define GENETIC_ALGORITHM_LOCAL_DISTANCE 2
#define REQUIRED_ROUTE_DISTANCE 1257
// параметры для алгоритма имитации отжига
#define IS_SIMULATED_ANNEALING_ALGORITHM 1
#define T_MAX 10
#define T_MIN 0.00001
#define MAX_ITERATION_COUNT 10
#define SIMULATED_ANNEALING_LOCAL_DISTANCE 1