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

        // std::cout << "all nodes: " << str(graph.nodes()) << std::endl;

        while ( bfs_root != component.end() ) {
            // std::cout << "ncc=" << ncc << std::endl;
            // std::cout << "current root: " << bfs_root->first << std::endl;
            bfs.run( bfs_root->first );
            // std::cout << str(bfs.preorder) << std::endl;
            for (auto node : bfs.preorder) component[node] = ncc;
            ++ncc;

            bfs_root = std::find_if( component.begin(), component.end(), [](const auto& n) { return n.second == -1; } );
        }

        // --ncc;
    }
};
// Usage:
// auto algorithm = ConnectedComponents<int,Graph>(G); // macierz sąsiedztwa
// auto algorithm = ConnectedComponents<char,Graph<char>>(G); // lista sąsiedztwa
// algorithm.run();
// for (auto& pair : algorithm.component)
//     std::cout << pair.first << " component : " << pair.second << std::endl;
// std::cout << "total components " << algorithm.ncc << std::endl;


#endif //ALGOSY2GRAFY_CONNECTEDCOMPONENTS_HPP
