#include <iostream>
#include <vector>
#include <cstdio>
#include <queue>
#include <limits>

using namespace std;

struct Edge 
{
    int vertex, capacity, flow;

    Edge(int vertex, int capacity) : vertex(vertex), capacity(capacity), flow(0) {}
};

bool breadthFirstSearch(vector<vector<Edge>>& newGraph, vector<int>& parent, int node, int drainage) 
{
    vector<bool> visited(newGraph.size(), false);
    queue<int> vertexQueue;
    int u, vertex, capacity, flow;

    vertexQueue.push(node);
    visited[node] = true;
    parent[node] = -1;

    while (!vertexQueue.empty()) {
        u = vertexQueue.front();
        vertexQueue.pop();

        for (const auto& edge : newGraph[u]) {
            vertex = edge.vertex;
            capacity = edge.capacity;
            flow = edge.flow;

            if (capacity - flow > 0 && !visited[vertex]) {
                vertexQueue.push(vertex);
                parent[vertex] = u;
                visited[vertex] = true;
            }
        }
    }

    return visited[drainage];
}

int maxFlowFind(vector<vector<Edge>>& graph, int source, int drainage) 
{
    vector<vector<Edge>> newGraph(graph);
    vector<int> parent(graph.size());
    int maxFlow = 0, pathFlow, u;

    while (breadthFirstSearch(newGraph, parent, source, drainage)) 
    {
        pathFlow = numeric_limits<int>::max();

        for (int vertex = drainage; vertex != source; vertex = parent[vertex]) 
        {
            u = parent[vertex];
            for (auto& edge : newGraph[u]) 
                if (edge.vertex == vertex) {
                    pathFlow = min(pathFlow, edge.capacity - edge.flow);
                    edge.flow += pathFlow;
                    break;
                }
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

void minCut(vector<vector<Edge>>& graph, int source, vector<bool>& visited) 
{
    visited[source] = true;
    for (const auto& edge : graph[source])
        if (!visited[edge.vertex] && edge.capacity - edge.flow > 0)
            minCut(graph, edge.vertex, visited);
}

int main() 
{
    int numOfVertexes, numOfEdges;
    int source, drainage;
    int u, vertex, capacity;
    int maxFlow;
    FILE* str;

    //------------------------------------------------------------

    freopen_s(&str, "input_graph.txt", "r", stdin);
    cin >> numOfVertexes >> numOfEdges;
    cin >> source >> drainage;

    cout << "Num of vertexes: " << numOfVertexes << "\n";
    cout << "Number of edges: " << numOfEdges << "\n";
    cout << "Path: " << source << " -> " << drainage << "\n\n";

    //------------------------------------------------------------

    numOfVertexes++;
    vector<vector<Edge>> graph(numOfVertexes);

    for (int i = 0; i < numOfEdges; ++i) {
        cin >> u >> vertex >> capacity;
        graph[u - 1].emplace_back(vertex, capacity);

        cout << "Edge " << i + 1 << ": (" << u << ";" << vertex << ") = " << capacity << "\n";
    }

    maxFlow = maxFlowFind(graph, source, drainage);

    vector<bool> visited(numOfVertexes, false);
    minCut(graph, source, visited);

    //------------------------------------------------------------

    cout << "\nMax flow: " << maxFlow << endl;
    cout << "Min cut: ";
    for (int i = 0; i < numOfVertexes; ++i)
        if (visited[i])
            cout << i << " ";

    return 0;
}