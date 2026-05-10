import matplotlib.pyplot as plt
import networkx as nx
import json
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
            pos = nx_layouts["planar"](self.graph)
        except nx.NetworkXException:
            pos = nx_layouts["shell"](self.graph)
        # pos = nx.spring_layout(self.graph, k=0.8, iterations=50, scale=2)

        # Handle nodes
        nx.draw_networkx_nodes(self.graph, pos=pos)
        nx.draw_networkx_labels(self.graph, pos=pos)

        # Handle edges
        one_directional = []
        bidirectional = []
        # for (v,u) in self.graph.edges():
        #     if (u, v) in self.graph.edges():
        #         if self.directed:
        #             bidirectional.append((v, u))
        #         else:
        #             one_directional.append((v, u))
        #     else:
        #         one_directional.append( (v,u) )

        one_directional = self.graph.edges

        nx.draw_networkx_edges(self.graph, pos=pos, edgelist=one_directional)
        nx.draw_networkx_edges(self.graph, pos=pos, edgelist=bidirectional, connectionstyle="arc3,rad=0.15")


        edge_labels = nx.get_edge_attributes(self.graph, "weight")
        nx.draw_networkx_edge_labels(self.graph, pos, edge_labels)


        plt.title("Implementation: " + self.implementation, fontsize=20)
        plt.figtext(0.5, 0.07, "Directed: " + str(self.directed), ha="center")
        plt.savefig("img/" + self.filename + ".png")
        plt.close()

    def loadGraph(self, filename):

        try:
            with open(filename, 'r') as f:
                self.filename = filename.split('/')[1].split('.')[0]
                data = json.load(f)
                self.data = data['graph']['content']
                self.implementation = data['graph']['implementation']
                self.directed = data['graph']['directed']

                for source in self.data.keys():
                    for target in self.data[source]:
                        # print([source, target['target'], target['weight']])
                        self.addEdge(source, target['target'], target['weight'])
                    if len(self.data[source]) == 0:
                        self.addNode(source)

        except FileNotFoundError:
            print("Graph File Not Found")




if __name__ == '__main__':


    for graph_file in os.listdir('json'):
        if graph_file.endswith(".json"):
            g = GraphVisualiser("json/" + graph_file)
            g.visualise()

