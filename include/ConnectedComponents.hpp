//
// Created by Mateusz M on 21/05/2026.
//

#ifndef ALGOSY2GRAFY_CONNECTEDCOMPONENTS_HPP
#define ALGOSY2GRAFY_CONNECTEDCOMPONENTS_HPP

#include <unordered_map>
#include <cassert>

#include "BaseGraph.hpp"
#include "BFS.hpp"



template <typename T, typename G> // node type, graph type
requires Graph<G, T>
class ConnectedComponents {
    G& graph;
public:
    std::unordered_map<T, int> component; // a table for component numbers
    int ncc; // the number of connected components
    explicit ConnectedComponents(G& g) : graph(g), ncc(0) {
        assert( !graph.is_directed() );
        for ( auto n_it : graph.nodes() ) {
            component[n_it] = -1; // składowa nieustalona
        }
    }
    ~ConnectedComponents() = default; // destruktor
    void run() {
        ncc = 0;
        auto bfs_root = std::find_if( component.begin(), component.end(), [](const auto& n) { return n.second == -1; } );
        BFS<T, G> bfs(graph);

        while ( bfs_root != component.end() ) {
            bfs.run( bfs_root->first );
            for (auto node : bfs.preorder) component[node] = ncc;
            ++ncc;

            bfs_root = std::find_if( component.begin(), component.end(), [](const auto& n) { return n.second == -1; } );
        }
    }
};

#endif //ALGOSY2GRAFY_CONNECTEDCOMPONENTS_HPP
