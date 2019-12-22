#pragma once

// определения глобальных переменных и конфигурации приложения

// Определяет какие элементы надёжны в сети (ветви или вершины)
#define IS_NODES_RELIABLE 0
// Определяет что считается (полином или число)
#define IS_NUMBER_COMPUTATION 1
// Работа с тестированием на время, имеет приоритет перед оптимизацией
#define IS_TEST_TIME 1
#define TEST_HYPERNETS 10
#define TEST_HYPERNET_NODES 25
#define TEST_HYPERNET_BRANCHES 30
#define TEST_HYPERNET_EDGES 30
// Работа с оптимизацией
#define IS_OPTIMIZATION 0
#define IS_OPTIMIZATION_AOSH 0
#define OPTIMIZATION_TREE_SIZE 11 // if IS_OPTIMIZATION_AOSH != 1 must be <= 20
#define OPTIMIZATION_KP_COUNT 7
#define OPTIMIZATION_MAPPING_TIME 300000
// Режим генерации укладки рёбер при генерации случайной гиперсети
#define IS_RANDOM_HYPERNET_SIMPLE_MAPPING 1
// Режим дебага
#define IS_DEBUG 1
// Включение двухуровневой парраллелизации
#define IS_TWO_LEVEL_PARALLELIZATION 1
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
// константы для MPI
#define HOST_PROCESSOR 0
#define STOP_TAG 0
#define I_AM_FREE_TAG 1
#define I_NEED_HELP_TAG 2
#define SEND_RECV_TAG 3
#define SEND_SOLUTION_TAG 4
#define RELIABLE_HYPERNETS_COUNT_TAG 5
#define UNCONNECTED_HYPERNET_COUNT_TAG 6
#define TWO_NODES_HYPERNET_COUNT 7
#define CHAINS_REDUCED_TAG 8
#define UNCONNECTED_NODES_REDUCED_TAG 9
#define PAIR_CONNECTIVITY_CALLS_TAG 10
#define EDGES_REDUCED_TAG 11
#define UNSIMPLE_CHAINS_TAG 12
#define HELP_PROCESSORS_TAG 13
#define TOTAL_BYTES_TRANSFER_TAG 14