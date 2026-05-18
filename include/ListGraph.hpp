//
// Created by Mateusz Miliutin on 05/05/2026.
//

#ifndef ALGOSY2GRAFY_LISTGRAPH_H
#define ALGOSY2GRAFY_LISTGRAPH_H
#include <cassert>
#include <unordered_map>
#include <list>

#include "BaseGraph.hpp"
#include "HelperFunctions.hpp"
#include "GraphExceptions.hpp"


//region ===================== CLASS =====================
template <typename T>
class ListGraph final : public BaseGraph<T> { // wersja5c
    bool directed;
    std::unordered_map<T, std::list<Edge<T> *> > adj_list; // lista sąsiedztwa
    //std::unordered_map<T, std::vector<Edge<T> *> > adj_list; // lista sąsiedztwa

public:
    explicit ListGraph(const bool directed=false) : directed(directed) {}
    ~ListGraph() override { clear(); } // trzeba zwolnić pamięć krawędzi

    //region ===================== GRAPH =====================
    bool is_directed() const override { return directed; }
    int v() const override { return adj_list.size(); } // liczba wierzchołków
    int e() const override { // liczba krawędzi
        int counter = 0;
        for (auto& pair : adj_list)
            counter += pair.second.size();
        return (directed ? counter : counter / 2);
    }
    //endregion

    //region ====================== NODE DEGREE =====================
    int degree(T u) override {
        // assert( !directed ); // Nie rozumiem tego ograniczenia, wg. definicji, które znalazłem, dla nieskierowanego grafu da się liczyć stopinie wierzchołków
        if (directed) return indegree(u) + outdegree(u);
        else return outdegree(u); // Dla nieskierowanych outdegree=indegree więc zwracam out bo jest prostsze obliczeniowo
    }
    int indegree(T u) override {
        int counter = 0;
        for (auto node_container : adj_list) {
            for (Edge<T> *edge : node_container.second) {
                if (edge->target == u) ++counter;
            }
        }
        return counter;
    }
    int outdegree(T u) override { return adj_list[u].size(); }
    //endregion

    //region ====================== NODES =====================
    void add_node(T u) override {
        if (has_node(u)) throw NodeExistsException();
        adj_list[u];
    }
    void del_node(T u) override {
        // Delete all edges that point to node u
        for (auto node : adj_list) {
            try {
                del_edge(node.first, u);
            } catch ( [[maybe_unused]] EdgeDoesntExistException &e ) {} // expected behavior to catch attempts to delete edges that don't exist; controlled environment
        }
        // Delete node u
        adj_list.erase(u);
    }
    bool has_node(T u) const override {
        return adj_list.contains(u);
    }
    T random_node() override {
        auto start = node_begin();
        const int r = rand_int( v() );
        for (int i = 0; i < r; ++i) { ++start; }
        return *start;
    }
    //endregion

    //region ====================== EDGES =====================
    void add_edge(T u, T w, float weight=1.0) override {
        try {
            if (has_edge(u,w)) throw EdgeExistsException();
        }
        catch ( SourceDoesntExistException &e ) {} // Expected exception if target or source doesn't exist
        catch ( TargetDoesntExistException &e ) {}

        if ( u == w ) throw std::runtime_error("Loops are not allowed in simple graphs"); //TODO improve this design
        if ( !has_node(u) ) add_node(u);
        if ( !has_node(w) ) add_node(w);


        adj_list[u].push_back(new Edge<T>(u, w, weight));
        if (!directed) adj_list[w].push_back(new Edge<T>(w, u, weight));

    }
    void add_edge(Edge<T> edge) override { add_edge(edge.source, edge.target, edge.weight); }

    void del_edge(T u, T w) override {

        if (!has_edge(u,w)) throw EdgeDoesntExistException();

        auto it = std::find_if(adj_list.at(u).begin(), adj_list.at(u).end(), [w](Edge<T> *edge) { return edge->target == w; });
        if (it != adj_list.at(u).end()) adj_list[u].erase(it);

        if (!directed) { //TODO try to make this more readable
            it = std::find_if(adj_list[w].begin(), adj_list[w].end(), [u](Edge<T> *edge) { return edge->target == u; });
            if (it != adj_list.at(w).end()) adj_list[w].erase(it);
        }

    }
    void del_edge(Edge<T> edge) override { del_edge(edge.source, edge.target); }

    bool has_edge(T u, T w) const override {
        if (!has_node(u)) throw SourceDoesntExistException();
        if (!has_node(w)) throw TargetDoesntExistException();

        auto it = std::find_if(adj_list.at(u).begin(), adj_list.at(u).end(), [w](Edge<T> *edge) { return edge->target == w; });
        return it != adj_list.at(u).end();

    }
    bool has_edge(Edge<T> edge) const override { return has_edge(edge.source, edge.target); }

    float weight(T u, T w) const override {
        if (has_edge(u,w)) {
            auto it = std::find_if(adj_list.at(u).begin(), adj_list.at(u).end(), [w](Edge<T> *edge) { return edge->target == w; });
            return (*it)->weight;
        }
        else return 0; // z polecenia: waga krawędzi lub 0.0

    }
    float weight(Edge<T> edge) const override { return weight(edge.source, edge.target); }
    //endregion

    //region ===================== MISC =====================
    void clear() override {
        for (auto& pair : adj_list) {
            for (const Edge<T>* edge : pair.second) delete edge;
            pair.second.clear();
        }
        adj_list.clear();
    }
    void display() const override {
        std::cout << "Displaying graph at " << this << ":" << std::endl;
        for (auto& pair : adj_list) {
            if (!pair.second.empty()) {
                    std::cout << "\t * Node \"" << pair.first << "\" has following edges:"<< std::endl;
                for (auto& edge : pair.second) {
                    std::cout << "\t\t• \"" << edge->target << "\" (weight=" << edge->weight << ")" << std::endl;
                }
            }else {
                std::cout << "\t * Node \"" << pair.first << "\" has no edges."<< std::endl;
            }
        }
    }
    //endregion

    //region ===================== META =====================
    std::string graph_implementation() const override { return "ListGraph"; }
    // TODO redo using nodes() and adjacents()
    std::string nodes_to_json() const override {
        std::string result = "";
        for (auto root : adj_list) {
            result += "\"" + std::to_string(root.first) + "\":{";
            for (const auto &neighbor : root.second) {
                if (neighbor != nullptr) {
                    result += "\"" + std::to_string(neighbor->target) + "\":" + std::to_string(neighbor->weight) + ",";
                }
            }
            if (!result.empty() && result.back() == ',') result.pop_back();
            result += "},";
        }
        if (!result.empty() && result.back() == ',') result.pop_back();
        return result;
    }
    //endregion

    //region ===================== ITERATORS =====================
        //region ===================== NODES =====================
            //region ===================== ITERATOR =====================
            class NodeIterator final {
                ListGraph* graph;
                int node_idx;
            public:
                explicit NodeIterator(ListGraph* graph, const int idx = 0) : graph(graph), node_idx(idx) {}
                ~NodeIterator() = default;
                NodeIterator& operator=(const NodeIterator& other) = default;

                T operator*() const {
                    auto it = graph->adj_list.begin();
                    for (int i = 0; i < node_idx; ++i) ++it;
                    auto val = (*it).first;
                    return val;
                }

                NodeIterator& operator++() {
                    ++node_idx;
                    return *this;
                }
                NodeIterator operator++(int) {
                    NodeIterator temp = *this;
                    ++node_idx;
                    return temp;
                }

                bool operator==(const NodeIterator& other) const {
                    return graph == other.graph && node_idx == other.node_idx;
                }
                bool operator!=(const NodeIterator& other) const {
                    return graph != other.graph || node_idx != other.node_idx;
                };
                void increment() { ++node_idx; }

            };
            NodeIterator node_begin() { return NodeIterator(this, 0); } // lub bez zera
            NodeIterator node_end() { return NodeIterator(this, v()); }
            //endregion
            //region ===================== RANGE STRUCT =====================
            struct Nodes {
                ListGraph<T>* graph;
                explicit Nodes(ListGraph<T>* graph): graph(graph) {};
                NodeIterator begin() const { return graph->node_begin(); }
                NodeIterator end() const { return graph->node_end(); }
            };
            Nodes nodes() { return Nodes(this); }
            //endregion
        //endregion
        //region ===================== ADJACENT NODES =====================
            //region ===================== ITERATOR =====================
            class AdjacentIterator final {
                ListGraph<T>* graph;
                T source;
                std::list< Edge<T>* > source_edges;
                int edge_idx;
                public:
                explicit AdjacentIterator(ListGraph<T>* graph, T node, const int idx = 0) : graph(graph), source(node), edge_idx(idx) {
                    //std::unordered_map<T, std::list<Edge<T> *> > adj_list; // lista sąsiedztwa
                    auto it = graph->adj_list.find( source );
                    assert( it != graph->adj_list.end() );
                    source_edges = it->second;
                }
                ~AdjacentIterator() = default;
                AdjacentIterator(const AdjacentIterator& other) = default;
                // AdjacentIterator operator=(const NodeIterator& other) = default;

                Edge<T>* operator*() const {
                    auto it = source_edges.begin();
                    for (int i = 0; i < edge_idx; ++i) ++it;
                    return *it;
                }

                bool operator==(const AdjacentIterator& other) const {
                    return graph == other.graph && edge_idx == other.edge_idx;
                }
                bool operator!=(const AdjacentIterator& other) const {
                    return graph != other.graph || edge_idx != other.edge_idx;
                }

                AdjacentIterator& operator++() {
                    ++edge_idx;
                    return *this;
                }
                AdjacentIterator operator++(int) {
                    AdjacentIterator temp = *this;
                    ++edge_idx;
                    return temp;
                }
            };
            AdjacentIterator adj_begin( int node ) { return AdjacentIterator(this, node); }
            AdjacentIterator adj_end( int node ) { return AdjacentIterator(this, node, this->adj_list.find(node)->second.size()); } //TODO improve this maybe
            //endregion
            //region ===================== RANGE STRUCT =====================
            struct Adjacents {
                ListGraph<T>* graph;
                T source;
                explicit Adjacents(ListGraph<T>* graph, T node) : graph(graph), source(node) {};
                AdjacentIterator begin() const { return graph->adj_begin(source); }
                AdjacentIterator end() const { return graph->adj_end(source); }
            };
            Adjacents adjacents( T node ) { return Adjacents(this, node); }
            //endregion
        //endregion
    //endregion

};
//endregion


#endif //ALGOSY2GRAFY_LISTGRAPH_H