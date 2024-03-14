#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

struct Edge 
{
    int vertex;
    int flow;
    int capacity;
};

int maxFlowFind(vector<vector<Edge>>& graph, int source, int drainage) 
{
    vector<vector<Edge>> newGraph(graph);
    int maxFlow = 0, pathFlow, u;


    while (true)
    {
        vector<int> parent(graph.size(), -1);
        queue<int> q;
        q.push(source);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (Edge& edge : newGraph[u])
                if (parent[edge.vertex] == -1 && edge.capacity > 0) {
                    q.push(edge.vertex);
                    parent[edge.vertex] = u;
                }
        }

        if (parent[drainage] == -1) {
            break;
        }

        pathFlow = INT_MAX;
        for (int vertex = drainage; vertex != source; vertex = parent[vertex])
        {
            u = parent[vertex];
            for (Edge& edge : newGraph[u])
                if (edge.vertex == vertex) {
                    pathFlow = min(pathFlow, edge.capacity);
                    break;
                }
        }

        maxFlow += pathFlow;

        for (int v = drainage; v != source; v = parent[v]) {
            int u = parent[v];
            for (Edge& edge : newGraph[u])
                if (edge.vertex == v) {
                    edge.capacity -= pathFlow;
                    for (Edge& revEdge : newGraph[v])
                        if (revEdge.vertex == u) {
                            revEdge.capacity += pathFlow;
                            break;
                        }
                    break;
                }
        }
    }

    return maxFlow;
}

int main() 
{
    int numOfVertexes, numOfEdges;
    int source, drainage;
    int u, vertex, capacity;
    int maxFlow;
    ifstream file;
    file.open("input_graph.txt");

    //------------------------------------------------------------

    file >> numOfVertexes >> numOfEdges;
    file >> source >> drainage;

    cout << "Num of vertexes: " << numOfVertexes << "\n";
    cout << "Number of edges: " << numOfEdges << "\n";
    cout << "Path: " << source + 1 << " -> " << drainage + 1 << "\n\n";

    //------------------------------------------------------------

    vector<vector<Edge>> graph(numOfVertexes);

    cout << "Number\tEdge\tCapacity\n";
    for (int i = 0; i < numOfEdges; i++) {
        file >> u >> vertex >> capacity;
        graph[u - 1].push_back({ vertex - 1, capacity, capacity });

        cout << i + 1 << "\t(" << u << "," << vertex << ")\t" << capacity << "\n";
    }

    maxFlow = maxFlowFind(graph, source, drainage);

    //------------------------------------------------------------

    cout << "\nMax flow: " << maxFlow << "\n";

    return 0;
}