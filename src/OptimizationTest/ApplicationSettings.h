#pragma once

// Определяет какие элементы надёжны в сети (ветви или вершины)
#define IS_NODES_RELIABLE 0
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
// пропорция стевых вершин
#define NETWORK_NODES_PROPRTION 5