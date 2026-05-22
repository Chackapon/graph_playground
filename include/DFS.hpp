//
// Created by Mateusz M on 18/05/2026.
//

#ifndef ALGOSY2GRAFY_DFS_HPP
#define ALGOSY2GRAFY_DFS_HPP

#include <unordered_map>
#include <vector>

#include "BaseGraph.hpp"
#include "GraphSearchAlgorithm.hpp"
#include "HelperFunctions.hpp"




// dfs.hpp
template <typename T, typename G> // node type, graph type
requires Graph<G, T>
class DFS final : public GraphSearchAlgorithm<T, G> {
public:
    std::unordered_map<T, T> parent; // drzewo DFS
    explicit DFS(G& g) : GraphSearchAlgorithm<T, G>(g) {
        for ( auto n_it : this->graph.nodes() ) {
            this->visited[n_it] = false;
            parent[n_it] = n_it; // konwencja, brak rodzica
        }
    }

    ~DFS() override = default; // destruktor

    void visit(T u, const int lvl = 0) override {
        if ( !this->visited[u] ) {
            this->visited[u] = true;
            this->preorder.push_back(u);

            for ( auto *edge : this->graph.adjacents( u ) ) {
                auto [root, neighbour, weight] = *edge;
                visit( neighbour, lvl+1 );
            }
            this->postorder.push_back(u);
        }
    }

    void display(const search_mode mode = PREORDER) override {
        std::vector<T> traversal_list;
        if (mode == PREORDER) traversal_list = this->preorder;
        else if (mode == POSTORDER) traversal_list = this->postorder;
        else throw std::invalid_argument("Invalid search mode");
        std::cout << str( traversal_list ) << std::endl;
    }
};
// Usage:
// auto algorithm = DFS<int,Graph>(graph); // macierz sąsiedztwa
// auto algorithm = DFS<char,Graph<char>>(graph); // lista sąsiedztwa
// algorithm.run(start_node);
// for (auto& node : algorithm.preorder) { ... }
// for (auto& node : algorithm.postorder) { ... }


#endif //ALGOSY2GRAFY_DFS_HPP