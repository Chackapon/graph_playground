//
// Created by Mateusz M on 19/05/2026.
//
#include <iostream>

#include "../include/BaseGraph.hpp"
#include "../include/ReportMaker.hpp"
#include "../include/HelperFunctions.hpp"
#include "../include/ListGraph.hpp"
#include "../include/Tarjan.hpp"
#include "../include/GraphTestUtil.hpp"

constexpr int N = 10;



int main () {
    rand_init();

    const auto graph = new ListGraph<int>(true);
    const auto r = new ReportMaker("report", "topological_sorting"); // TODO either report should need extension in name or json doesnt
    r->header("Topological sorting using DFS");

    r->run_test("Generating a random DAG graph");
    try_generate_random_nodes(N, graph );
    r->log("Generated a graph with "+str(graph->v())+" nodes");
    r->log("Generating edges");
    generate_dag(graph);
    r->sublog("Generated " + str(graph->e()) + " edges");
    r->log("Displaying graph");
    r->silent_suberror("display() uses std::cout and can't be displayed in a log");
    graph->display();
    r->silent_log("Generating a json file");
    graph->get_json("json", "tarjan.json");

    r->run_test("Running Tarjan topological sorting");
    r->log("Running the sorting algorithm");
    auto sorted_nodes = topsort_kahn<int, ListGraph<int>>( *graph );
    r->sublog("Sorting result: " + str(sorted_nodes) );
    r->sublog("Is this a valid topological sorting of this graph? " + yn(is_topologically_sorted(graph, sorted_nodes) ) );

    r->title_line("END");

    delete graph;
}