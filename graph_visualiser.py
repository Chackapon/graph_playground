import matplotlib.pyplot as plt
import networkx as nx
import subprocess

import json
import yaml
import os

nx_layouts = {
    "planar": nx.planar_layout,
    "spring": nx.spring_layout,
    "circular": nx.circular_layout,
    "random": nx.random_layout,
    "shell": nx.shell_layout,
    "spectral": nx.spectral_layout,
    "spiral": nx.spiral_layout,
    "kamada": nx.kamada_kawai_layout,
}


class GraphVisualiser:
    def __init__(self, filename=''):
        self.directed = None
        self.graph = nx.DiGraph()
        self.data = None
        self.implementation = None
        self.filename = None

        if filename != '':
            self.loadGraph(filename)

    def addEdge(self, source, target, weight):
        assert self.graph is not None
        self.graph.add_edge(source, target, weight=weight)

    def addNode(self, source):
        assert self.graph is not None
        self.graph.add_node(source)

    def visualise(self):
        assert self.data is not None
        # print(self.graph.edges)

        try:
            pos = nx_layouts["shell"](self.graph)
        except nx.NetworkXException:
            pos = nx_layouts["shell"](self.graph)

        fig, ax = plt.subplots(figsize=(6, 4))
        # pos = nx.spring_layout(self.graph, k=0.8, iterations=50, scale=2)

        # Handle nodes
        nx.draw_networkx_nodes(self.graph, pos=pos)
        nx.draw_networkx_labels(self.graph, pos=pos)

        # Handle edges
        visited = set()
        one_directional = []
        bidirectional = []
        for (v,u) in self.graph.edges():
            # print()
            # print(self.graph.edges, )
            # print( visited )
            # print( (v,u), (u, v) in self.graph.edges(), (v,u) not in visited )
            # print(bidirectional)
            if (v,u) not in visited:
                if (u, v) in self.graph.edges():
                    # print(self.data)
                    if self.directed and self.data[u][v] != self.data[v][u]:
                        bidirectional.append((v, u))
                        bidirectional.append((u, v))
                        visited.add( (u,v) )
                    else:
                        one_directional.append((v, u))
                else:
                    one_directional.append( (v,u) )
                    pass
                visited.add( (v,u) )

        # one_directional = self.graph.edges

        nx.draw_networkx_edges(self.graph, pos=pos, edgelist=one_directional)
        nx.draw_networkx_edges(self.graph, pos=pos, edgelist=bidirectional, connectionstyle="arc3,rad=0.05")


        # Handle edge labels
        # TODO cleanup
        # edge_labels = nx.get_edge_attributes(self.graph, "weight")
        mono_edge_labels = {
            edge: weight
            for edge, weight in nx.get_edge_attributes(self.graph, "weight").items()
            if edge in one_directional
        }

        visited = set()
        bi_edge_labels = {}
        for bi_edge in bidirectional:
            if bi_edge not in visited and tuple(reversed(bi_edge)) not in visited:
                bi_edge_labels[bi_edge] = f"{self.data[bi_edge[1]][bi_edge[0]]}<->{self.data[bi_edge[0]][bi_edge[1]]}"
                visited.add(bi_edge)
        # quit()
        # bi_edge_labels = {
        #     edge: ""# edge: f"{vu_weight}<->{uv_veight}"
        #     for edge, weight in nx.get_edge_attributes(self.graph, "weight").items()
        #     if edge in bidirectional
        # }

        label_bounding_box = dict(
            facecolor="white",
            edgecolor="none",
            alpha=0.75,
            pad=1
        )
        nx.draw_networkx_edge_labels(self.graph, pos, mono_edge_labels, font_size=8, bbox=label_bounding_box)
        nx.draw_networkx_edge_labels(self.graph, pos, bi_edge_labels, font_size=8, bbox=label_bounding_box) #TODO fix this


        plt.title("Implementation: " + self.implementation, fontsize=20)
        plt.figtext(0.5, 0.07, "Directed: " + str(self.directed), ha="center")
        subprocess.run( ["mkdir", "-p", getConfigData()["IMAGE_DIRECTORY"]] )
        plt.savefig( getConfigData()["IMAGE_DIRECTORY"] + '/' + self.filename + ".png" )
        plt.close()

    def loadGraph(self, filename):

        try:
            with open(filename, 'r') as f:
                self.filename = filename.split('/')[1].split('.')[0]
                data = json.load(f)
                self.data = data['graph']['content']
                self.implementation = data['graph']['implementation']
                self.directed = data['graph']['directed']

                # print(self.data); quit()
                for source in self.data.keys():
                    for target in self.data[source].keys():
                        self.addEdge(source, target, self.data[source][target])
                    if len(self.data[source]) == 0:
                        self.addNode(source)

        except FileNotFoundError:
            print("Graph File Not Found")


def getConfigFile():
    with open("Makefile", "r") as makefile:
        return makefile.readline().split()[-1] # TODO make dynamic

def getConfigData():
    from yaml import SafeLoader, load
    with open(getConfigFile(), "r") as cfg:
        return load(cfg, Loader=SafeLoader)

if __name__ == '__main__':
    config_file = getConfigData()

    for graph_file in os.listdir( config_file["JSON_DIRECTORY"] ):
        if graph_file.endswith( '.' + config_file["JSON_DIRECTORY"] ):
            g = GraphVisualiser( config_file["JSON_DIRECTORY"] + '/' + graph_file )
            g.visualise()

