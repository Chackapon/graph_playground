//
// Created by Mateusz M on 19/05/2026.
//

#ifndef ALGOSY2GRAFY_GRAPHSEARCHALGORITHM_HPP
#define ALGOSY2GRAFY_GRAPHSEARCHALGORITHM_HPP

#include "BaseGraph.hpp"

enum search_mode {
    PREORDER, POSTORDER
};

template <typename T, typename G> // node type, graph type
requires Graph<G, T>
class GraphSearchAlgorithm {
};


#endif //ALGOSY2GRAFY_GRAPHSEARCHALGORITHM_HPP