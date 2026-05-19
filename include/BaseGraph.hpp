//
// Created by Mateusz M on 04/05/2026.
//

#ifndef ALGOSY2GRAFY_BASEGRAPH_H
#define ALGOSY2GRAFY_BASEGRAPH_H



// basegraph.hpp
#include <fstream>
#include <filesystem> // IDEA change string args to filepaths
#include "Edge.hpp"

//region ===================== CONCEPTS =====================
/**
 * Graph concept allows to ensure compatibility with graph iterators.
 * Unfortunately they cannot be included in base class (especially the postfix ++ operator) and this workaround allows them to be used in templates for the BaseGraph.
 * Usage: template <Graph G> ...
 */
template <typename G>
concept HasNodes = requires(G graph) { graph.nodes(); };
template <typename G, typename T>
concept HasAdjacents = requires(G graph, T node) { graph.adjacents(node); };

template <typename G, typename T>
concept Graph = HasNodes<G> && HasAdjacents<G, T>;
//endregion

//region ===================== CLASS =====================
template <typename T>
class BaseGraph { // abstract class

private:
    virtual std::string nodes_to_json() const = 0;

public:
    BaseGraph() = default;
    virtual ~BaseGraph() = default;

    BaseGraph(const BaseGraph& other) = default;
    BaseGraph& operator=(const BaseGraph& other) = default;

    //region ===================== GRAPH =====================
    virtual bool is_directed() const = 0;
    virtual int v() const = 0; // liczba wierzcholkow
    virtual int e() const = 0; // liczba krawedzi
    //endregion

    //region ===================== NODE DEGREE =====================
    virtual int degree(T u) = 0; // grafy nieskierowane, problem z const dla operator[]
    virtual int indegree(T u) = 0; // grafy skierowane, stopień wejściowy
    virtual int outdegree(T u) = 0; // grafy skierowane, stopień wyjściowy
    //endregion

    //region ===================== NODES =====================
    virtual void add_node(T u) = 0;
    // Ponowne dodanie istniejącego wierzchołka jest ignorowane bez błędu.
    virtual void del_node(T u) = 0;
    // Usunięcie wierzchołka automatycznie usuwa krawędzie wchodzące i wychodzące.
    virtual bool has_node(T u) const = 0;
    virtual T random_node() = 0;
    //endregion

    //region ===================== EDGES =====================
    virtual void add_edge(T u, T w, float weight=1.0) = 0;
    virtual void add_edge(Edge<T> edge) = 0;
    // Jeżeli krawędź (u,w) istnieje w grafie, to próba dodania tej krawędzi
    // powinna być raportowana jako błąd w grafach prostych.
    // Dodanie krawędzi automatycznie dodaje brakujące wierzchołki.
    virtual void del_edge(T u, T w) = 0;
    virtual void del_edge(Edge<T> edge) = 0;
    virtual bool has_edge(T u, T w) const = 0;
    virtual bool has_edge(Edge<T> edge) const = 0;
    virtual float weight(T u, T w) const = 0;
    virtual float weight(Edge<T> edge) const = 0;
    //endregion

    //region ===================== MISC =====================
    virtual void clear() = 0;
    virtual void display() const = 0;
    //endregion

    //region ===================== META =====================
    virtual std::string graph_implementation() const = 0;
    void get_json( const std::string& directory = ".", const std::string &filename = "graph" ) const {
        if ( !std::filesystem::exists(directory) ) std::filesystem::create_directory( directory );
        std::ofstream file(directory + "/" + filename);
        file << R"({"graph": {"implementation": ")" << graph_implementation();
        file <<  R"(", "directed": ")" << (is_directed() ? "True" : "False");
        file << R"(","content": {)";
        file << nodes_to_json();
        file << "}}}" << std::endl;
        file.close();
    }
    //endregion


};
//endregion


#endif //ALGOSY2GRAFY_BASEGRAPH_H