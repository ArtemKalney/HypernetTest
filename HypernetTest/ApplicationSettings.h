#pragma once

// Тесты, на тестирование не влиияет IS_NODES_RELIABLE
#define IS_TEST 0
#define IS_TEST_CHECK_SPECIFICATIONS 1
// Определяет какие элементы надёжны в сети (ветви или вершины)
#define IS_NODES_RELIABLE 0
// Определяет что считается (полином или число)
#define IS_NUMBER_COMPUTATION 1
// Режим полного перебора
#define IS_FULL_ENUMERATION 0
// Каждая редукция требует чтобы были включены предыдущие
#define ENABLE_SIMPLE_CASE 1
#define ENABLE_BRIDGE_REDUCTION 1
#define ENABLE_EDGE_REDUCTION 1
#define ENABLE_CHAIN_REDUCTION 1
// Максимальная размерность гиперсети для факторизации
#define MAX_DIMENSIONAL 3
// Режим генерации укладки рёбер при генерации случайной гиперсети
#define IS_RANDOM_HYPERNET_SIMPLE_MAPPING 1
// Режим дебага, нумерация вершин с 0
#define IS_DEBUG 1
#define DEBUG_FIRST_NODE 15
#define DEBUG_SECOND_NODE 24
// Экспериментальные показатели для CHAIN_REDUCTION
#define IS_FULL_SEARCH_CHAINS 0
#define IS_REDUCE_RELIABLE_CHAINS 0
// Работа с оптимизацией
#define IS_OPTIMIZATION 0
#define IS_OPTIMIZATION_AOSH 0
#define OPTIMIZATION_TREE_SIZE 8
#define OPTIMIZATION_FIRST_TREE_ROOT 0
#define OPTIMIZATION_SECOND_TREE_ROOT 1
#define OPTIMIZATION_MAPPING_TIME 300000