//
// Created by Mateusz M on 18/05/2026.
//

#ifndef ALGOSY2GRAFY_DFS_HPP
#define ALGOSY2GRAFY_DFS_HPP

#include <unordered_map>
#include <vector>
#include <list>

#include "BaseGraph.hpp"
#include "GraphSearchAlgorithm.hpp"




// dfs.hpp
template <typename T, typename G> // node type, graph type
requires Graph<G, T>
class DFS : GraphSearchAlgorithm<T, G> {
    G& graph;
    std::unordered_map<T, bool> visited;
public:
    std::vector<T> preorder;
    std::vector<T> postorder;
    std::unordered_map<T, T> parent; // drzewo DFS
    explicit DFS(G& g) : graph(g) {
        for (auto n_it = graph.node_begin();
            n_it != graph.node_end();
            ++n_it) {
            visited[*n_it] = false;
            parent[*n_it] = *n_it; // konwencja, brak rodzica
            }
    }
    ~DFS() = default; // destruktor
    void run(T u) { // badamy jedną składową spójną
        visit(u);
    }
    void run() {
        for (auto n_it = graph.node_begin();
            n_it != graph.node_end();
            ++n_it) {
            if (!visited[*n_it])
                visit(*n_it);
            }
    }
    void visit(T u, int lvl = 0) {
        if ( !visited[u] ) {
            // std::cout << u << ": " << lvl << std::endl;
            visited[u] = true;
            preorder.push_back(u);

            for ( auto *edge : graph.adjacents( u ) ) {
                auto [root, neighbour, weight] = *edge;
                visit( neighbour, lvl+1 );
            }
            postorder.push_back(u);
        }
    }

    // void display(const search_mode mode = PREORDER) {
    //     std::vector<T> traversal_list;
    //     if (mode == PREORDER) traversal_list = this->preorder;
    //     else if (mode == POSTORDER) traversal_list = this->postorder;
    //     else throw std::invalid_argument("Invalid search mode");
    //
    //     int level = 0;
    //     std::list<T> candidates;
    //
    //     std::cout << str( traversal_list ) << std::endl;
    //
    //     // do {
    //     //     candidates.clear();
    //     //     //std::copy_if(traversal_list.begin(), traversal_list.end(), std::back_inserter(candidates), [level, this]( T node ) {return this->distance[node] == level; });
    //     //     ++level;
    //     //     if (!candidates.empty()) std::cout << level << ": " << str(candidates) << std::endl;
    //     // } while ( !candidates.empty() );
    // }
};
// Usage:
// auto algorithm = DFS<int,Graph>(graph); // macierz sąsiedztwa
// auto algorithm = DFS<char,Graph<char>>(graph); // lista sąsiedztwa
// algorithm.run(start_node);
// for (auto& node : algorithm.preorder) { ... }
// for (auto& node : algorithm.postorder) { ... }


#endif //ALGOSY2GRAFY_DFS_HPP