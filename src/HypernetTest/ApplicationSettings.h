#pragma once

// Тесты, на тестирование не влиияет IS_NODES_RELIABLE
#define IS_TEST 0
#define IS_TEST_CHECK_SPECIFICATIONS 1
// Определяет какие элементы надёжны в сети (ветви или вершины)
#define IS_NODES_RELIABLE 1
// Определяет что считается (полином или число)
#define IS_NUMBER_COMPUTATION 0
// Режим полного перебора
#define IS_FULL_ENUMERATION 0
// Каждая редукция требует чтобы были включены предыдущие
#define ENABLE_SIMPLE_CASE 1
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
#define ENABLE_CHAIN_REDUCTION 1
// Максимальная размерность гиперсети для факторизации
#define MAX_DIMENSIONAL 3
// Режим дебага, нумерация вершин с 0
#define IS_DEBUG 1
#define DEBUG_FIRST_NODE 0
#define DEBUG_SECOND_NODE 1
// Экспериментальные показатели для CHAIN_REDUCTION
#define IS_FULL_SEARCH_CHAINS 0
#define IS_REDUCE_RELIABLE_CHAINS 0
// Генерация случайной гиперсети (без вычисления надёжности)
#define IS_RANDOM_HYPERNET 0
#define RANDOM_ADDITIONAL_EDGES 5
// параметры для кумулятивных оценок
#define IS_CUMULATIVE_MODE 1
#define REQUIRED_VALUE 12