//
// Created by Mateusz M on 19/05/2026.
//

#ifndef ALGOSY2GRAFY_TARJAN_HPP
#define ALGOSY2GRAFY_TARJAN_HPP

#include <vector>

#include "DFS.hpp"
#include "Edge.hpp"

template <typename T, typename G> // node type, graph type
std::vector<T> topsort_kahn(G& graph) {
    std::vector<T> result;

    DFS<T, G> dfs( graph );
    dfs.run();

    std::reverse_copy(dfs.postorder.begin(), dfs.postorder.end(), back_inserter(result));
    return result;
}



#endif //ALGOSY2GRAFY_TARJAN_HPP
