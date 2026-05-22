//
// Created by Mateusz M on 09/05/2026.
//

#ifndef ALGOSY2GRAFY_GRAPHEXCEPTIONS_HPP
#define ALGOSY2GRAFY_GRAPHEXCEPTIONS_HPP

#include "BaseGraph.hpp"


class GraphException : public std::runtime_error
{
    public:
    explicit GraphException(const std::string& error_msg) : runtime_error( "Graph Exception: " + error_msg ) {}
};

//region ===================== NODES =====================
class NodeException : public GraphException
{
public:
    explicit NodeException(const std::string& error_msg) : GraphException(error_msg) {}
};

class NodeExistsException : public NodeException
{
    public:
    explicit NodeExistsException() : NodeException("node already exists") {}
};
//endregion

//region ===================== EDGES =====================
class EdgeException : public GraphException
{
public:
    explicit EdgeException(const std::string& error_msg) : GraphException(error_msg) {}
};

class LoopsInSimpleGraph : public EdgeException
{
public:
    explicit LoopsInSimpleGraph() : EdgeException("loop edges are not allowed in a simple graph") {}
};

class EdgeExistsException : public EdgeException
{
public:
    explicit EdgeExistsException() : EdgeException("edge already exists") {}
};


class EdgeDoesntExistException : public EdgeException
{
public:
    explicit EdgeDoesntExistException() : EdgeException("edge doesn't exist") {}
};

class TargetDoesntExistException : public EdgeException
{
public:
    explicit TargetDoesntExistException() : EdgeException("target node doesn't exist") {}
};

class SourceDoesntExistException : public EdgeException
{
public:
    explicit SourceDoesntExistException() : EdgeException("source node doesn't exist") {}
};
//endregion


#endif //ALGOSY2GRAFY_GRAPHEXCEPTIONS_HPP