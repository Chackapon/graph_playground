//
// Created by Mateusz M on 04/05/2026.
//

#ifndef ALGOSY2GRAFY_EDGE_H
#define ALGOSY2GRAFY_EDGE_H

#include <format>
#include <iostream>

// edge.hpp
template <typename T>
class Edge {
public:
    T source;
    T target;
    float weight;
    Edge(T s, T t, const float w=1.0) : source(s), target(t), weight(w) {}
    ~Edge() = default;   // destruktor
    Edge(const Edge& edge) : source(edge.source), target(edge.target),
        weight(edge.weight) {}   // copy-constructor
    // Operator negacji bitowej, ale tu pasuje.
    Edge operator~() const { return Edge(target, source, weight); }
    Edge& operator=(const Edge& other) = default; // return *this;
    friend std::ostream& operator<<<>(std::ostream& os, const Edge<T>& edge);
    bool operator==(const Edge& edge) {
        return source == edge.source && target == edge.target;
    }
    bool operator!=(const Edge& edge) {
        return source != edge.source || target != edge.target;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Edge<T>& edge) {
    os << std::format("Edge({}->{}:{})", edge.source, edge.target, edge.weight);
    return os;
}


#endif //ALGOSY2GRAFY_EDGE_H
