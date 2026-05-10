//
// Created by Mateusz Miliutin on 04/05/2026.
//

#ifndef ALGOSY2GRAFY_GRAPH_H
#define ALGOSY2GRAFY_GRAPH_H
#include <cassert>
#include <vector>

#include "BaseGraph.hpp"
#include "HelperFunctions.hpp"
#include "GraphExceptions.hpp"


// graph.hpp
class MatrixGraph final : public BaseGraph<int> { // wersja 7x
    bool directed;
    // Adjacency matrix to store graph edges.
    std::vector<std::vector<Edge<int> *> > adj_matrix;

    std::string nodes_to_json() const override {
        std::string result;
        for (int v = 0; v < adj_matrix.size(); ++v) {
            if (has_node(v)) {
                result += "\"" + std::to_string(v) + "\":[";

                for (const auto &neighbor : adj_matrix[v]) {
                    if (neighbor != nullptr  && neighbor->weight != 0) {
                        result += R"({"target": ")" + std::to_string(neighbor->target) + R"(", "weight":)" + std::to_string(neighbor->weight) + "},";
                    }
                }
                if (result.back() == ',') result.pop_back();

                result += "],";
            }
        }
        if (result.back() == ',') result.pop_back();
        return result;
    }
public:
    std::string graph_implementation() const override { return "MatrixGraph"; }
    explicit MatrixGraph(const int n, const bool directed=false) : directed(directed) {
        adj_matrix = std::vector< std::vector<Edge<int> *> >(
            n, std::vector<Edge<int> *>(n, nullptr));
    }
    ~MatrixGraph() override { clear(); } // trzeba zwolnić pamięć krawędzi

    bool is_directed() const override { return directed; }

    int v() const override {
        int counter = 0;
        for (const auto& row : adj_matrix) {
            for (const auto& edge : row) {
                if (edge != nullptr) {
                    counter++;
                    break;
                }
            }
        }
        return counter;
    }
    int e() const override {
        int edges_counter = 0;
        for (const auto &row : adj_matrix) {
            for (const auto &edge : row) {
                if ( edge != nullptr && edge->weight != 0 ) edges_counter++;
            }
        }
        return edges_counter;
    }

    int degree(const int u) override {
        if (directed) return indegree(u) + outdegree(u);
        else return outdegree(u);
    } // stopień wierzchołka
    int indegree(const int u) override {
        int counter = 0;
        for (auto & row : adj_matrix) {
            if (row[u] != nullptr && row[u]->weight != 0) ++counter;
        }
        return counter;
    }
    int outdegree(const int u) override {
        int counter = 0;
        for (const auto edge : adj_matrix.at(u)) {
            if (edge != nullptr && edge->weight != 0) ++counter;
        }
        return counter;
    }
    void add_node(const int u) override {
        if (has_node(u)) throw std::runtime_error("Node already exists");
        for (int i = 0; i < adj_matrix.at(u).size(); i++) adj_matrix.at(u)[i] = new Edge<int>(u, 0, 0);
    }
    void del_node(const int u) override { // TODO clean up try catches
        for (int i = 0; i < adj_matrix.size(); i++) {
            try {
                try {
                    std::cout << "trying to delete edge " << u << "->" << i << std::endl;
                    del_edge(u, i);

                } catch (EdgeDoesntExistException &e) {}
            } catch (TargetDoesntExistException &et) { } // Expected as I'm iterating through all possible edges
            delete adj_matrix.at(u).at(i);
            adj_matrix.at(u).at(i) = nullptr;
        }

    }
    bool has_node(const int u) const override {
        for (int i =0; i < adj_matrix.size(); i++) {
            if ( adj_matrix.at(u).at(i) != nullptr ) return true;
        }
        return false;
    }


    void add_edge(const int u, const int w, const float weight=1.0) override {
        try {
            if (has_edge(u,w)) throw EdgeExistsException();
        }
        catch ( SourceDoesntExistException &e ) {} // Expected exception if target or source doesn't exist
        catch ( TargetDoesntExistException &e ) {}

        if ( u == w ) throw std::runtime_error("Loops are not allowed in simple graphs"); //TODO improve this design
        if ( !has_node(u) ) add_node(u);
        if ( !has_node(w) ) add_node(w);

        adj_matrix.at(u).at(w)->target = w;
        adj_matrix.at(u).at(w)->weight = weight;
        // if (!directed) adj_matrix[w][u] = new Edge<int>(w, u, weight);
        if (!directed) {
            adj_matrix.at(w).at(u)->target = u;
            adj_matrix.at(w).at(u)->weight = weight;
        }

    } // dodanie krawędzi (u,w)
    void add_edge(const Edge<int> edge) override {
        add_edge(edge.source, edge.target, edge.weight);
    }



    void del_edge(const int u, const int w) override {
        if (!has_edge(u,w)) throw EdgeDoesntExistException();

        delete adj_matrix.at(u).at(w);
        adj_matrix.at(u).at(w) = nullptr;
        if (!directed) {
            delete adj_matrix.at(w).at(u);
            adj_matrix.at(w).at(u) = nullptr;
        }
    }
    void del_edge(const Edge<int> edge) override {
        del_edge(edge.source, edge.target);
    }


    bool has_edge(const int u, const int w) const override {
        // if (!has_node(u)) throw SourceDoesntExistException(); //TODO z jakiegos powodu to lamie cala logike; naprawic potem jesli bedzie mi sie chcialo
        // if (!has_node(w)) throw TargetDoesntExistException();
        return adj_matrix.at(u).at(w) != nullptr && adj_matrix.at(u).at(w)->weight != 0;
    }
    bool has_edge(const Edge<int> edge) const override {
        return has_edge(edge.source, edge.target);
    }



    float weight(int u, int w) const override {
        if (!has_edge(u,w)) return 0;
        return adj_matrix.at(u).at(w)->weight;
    } // waga krawędzi lub 0.0
    float weight(Edge<int> edge) const override {
        return weight(edge.source, edge.target);
    }


    void clear() override {
        for (auto &row : adj_matrix) {
            for (auto &edge : row) {
                delete edge;
            }
            row.clear();
        }
        adj_matrix.clear();
    } // usunięcie wszystkich krawędzi O(n^2)
    void display() const override { //TODO unify with how ListGraph displays nodes with no edges
        std::cout << "Displaying graph at " << this << ":" << std::endl;

        for (int i = 0; i < adj_matrix.size(); i++) {
            bool empty_flag = true;
            std::cout << "\t * Node \"" << i << "\" has following edges:"<< std::endl;
            for ( const auto &edge : adj_matrix.at(i) ) {
                if (edge != nullptr && edge->weight != 0) {
                    empty_flag = false;
                    std::cout << "\t\t• \"" << edge->target << "\" (weight=" << edge->weight << ")" << std::endl;
                    // std::cout << "\t* " << edge->source << " -> " << edge->target << " (weight=" << edge->weight << ")" << std::endl;
                }
            }
            if (empty_flag) std::cout << "\t\t• None"<< std::endl;
        }

    }
};


#endif //ALGOSY2GRAFY_GRAPH_H