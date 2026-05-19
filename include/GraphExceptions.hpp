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


class NodeExistsException : public GraphException
{
    public:
    explicit NodeExistsException() : GraphException("node already exists") {}
};


class EdgeExistsException : public GraphException
{
public:
    explicit EdgeExistsException() : GraphException("edge already exists") {}
};


class EdgeDoesntExistException : public GraphException
{
public:
    explicit EdgeDoesntExistException() : GraphException("edge doesn't exist") {}
};

class TargetDoesntExistException : public GraphException
{
public:
    explicit TargetDoesntExistException() : GraphException("target node doesn't exist") {}
};

class SourceDoesntExistException : public GraphException
{
public:
    explicit SourceDoesntExistException() : GraphException("source node doesn't exist") {}
};


#endif //ALGOSY2GRAFY_GRAPHEXCEPTIONS_HPP