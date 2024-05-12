#pragma once

// todo вынести в отдельный проект
/// Тесты, на тестирование не влиияет IS_NODES_RELIABLE
#define IS_TEST 0
#define IS_TEST_CHECK_SPECIFICATIONS 1

// todo вынести в отдельный проект
/// Генерация случайной гиперсети (без вычисления надёжности)
#define IS_RANDOM_HYPERNET_GENERATION 0
#define RANDOM_ADDITIONAL_EDGES 0
#define MAX_BRANCH_SATURATION 2
#define MAX_DISTANCE 12

/// Режим полного перебора
#define IS_FULL_ENUMERATION 0

/// Режим дебага, нумерация вершин с 0
#define IS_DEBUG 0
#define DEBUG_FIRST_NODE 0
#define DEBUG_SECOND_NODE 1

/// Режим приближенного решения
#define IS_APPROXIMATION 0
#define MAX_APPROXIMATION_DIMENSIONAL 10

/// Определяет какие элементы надёжны в сети (ветви или вершины)
#define IS_NODES_RELIABLE 1

/// Определяет что считается (полином или число)
#define IS_NUMBER_COMPUTATION 1

/// Определяет дефолтное значение надёжности
#define RELIABILITY_VALUE 0.9

/// Каждая редукция требует чтобы были включены предыдущие
#define ENABLE_SIMPLE_CASE 1
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
#define ENABLE_CHAIN_REDUCTION 1
/// Экспериментальные показатели для CHAIN_REDUCTION
#define IS_FULL_SEARCH_CHAINS 0
#define IS_REDUCE_RELIABLE_CHAINS 0

/// Параметры ввода
#define INPUT_NODE_VALUES 0
#define INPUT_BRANCH_VALUES 0
#define INPUT_BRANCH_COSTS 0
#define INPUT_MAX_BRANCH_SATURATIONS 0

/// Параметры вывода
#define OUTPUT_NODE_VALUES 0
#define OUTPUT_BRANCH_VALUES 0
#define OUTPUT_BRANCH_COSTS 0
#define OUTPUT_MAX_BRANCH_SATURATIONS 0
#define OUTPUT_SYNC_NODES 0