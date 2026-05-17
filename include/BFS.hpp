//
// Created by Mateusz Miliutin on 05/05/2026.
//

#ifndef ALGOSY2GRAFY_BFS_H
#define ALGOSY2GRAFY_BFS_H

#include "BaseGraph.hpp"

// bfs.hpp
template <typename T, typename G> // node type, graph type
requires Graph<G, T>
class BFS {
    G& graph;
    std::unordered_map<T, bool> visited;
public:
    std::vector<T> preorder;
    std::vector<T> postorder;
    std::unordered_map<T, T> parent;     // drzewo BFS
    std::unordered_map<T, int> distance; // odległość od źrodła w przeskokach
    std::unordered_map<T, float> price;

    BFS(G& g) : graph(g) {
        for (auto n_it = graph.node_begin();
            n_it != graph.node_end();
            ++n_it) {
            visited[*n_it] = false;
            parent[*n_it] = *n_it; // konwencja, brak rodzica
            distance[*n_it] = -1; // konwencja, -1 oznacza nieskończoność
            price[*n_it] = -1;
            }
    }
    ~BFS() = default; // destruktor
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
    void visit(T s) {
        std::queue<T> queue;
        queue.push(s);

        visited[s] = true;
        parent[s] = s;
        distance[s] = 0;
        price[s] = 0;

        while (!queue.empty()) {
            T node = queue.front();
            queue.pop();

            // std::cout << node << " visited? " + yn(visited[node]) << std::endl;

            preorder.push_back(node);

            for (auto *edge : graph.adjacents(node)) {
                auto [root, neighbour, weight] = *edge;
                assert(root == node);
                if (!visited[edge->target]) {
                    visited[neighbour] = true;
                    parent[neighbour] = node;
                    distance[neighbour] = distance[node] + 1;
                    price[neighbour] = price[node] + weight;

                    queue.push(neighbour);
                }
            }
            postorder.push_back(node);
        }
    }

    void display() {
        int level = 0;
        std::list<T> candidates;

        do {
            candidates.clear();
            std::copy_if(preorder.begin(), preorder.end(), std::back_inserter(candidates), [level, this]( T node ) {return this->distance[node] == level; });
            ++level;
            if (!candidates.empty()) std::cout << level << ": " << str(candidates) << std::endl;
        } while ( !candidates.empty() );
    }
};


#endif //ALGOSY2GRAFY_BFS_H