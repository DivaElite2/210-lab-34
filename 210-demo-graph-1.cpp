#include <iostream>
#include <vector>
using namespace std;

const int SIZE = 7;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {

private:
vector<vector<Pair>> adjList;// encapsulation to keep data private
void BFSRecursive(int current, vector<bool>& visited);


public:
// Graph Constructor
 Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }

//Adding BFS graph 
void BFS(int startVertex) {
    vector<bool> visited(SIZE, false);
    cout<< "BFS starting from vertex" << startVertex<< ":"<<endl;
    BFSRecursive(startVertex, visited);
    cout<<endl;
}
 

};

void Graph::BFSRecursive(int current,vector<bool>& visited ){
    visited [current] = true;
    cout << current << " ";
    for(Pair neighbor: adjList[current]) {
        int neighborVertex = neighbor.first;
        if(!visited[neighborVertex]) {
            BFSRecursive(neighborVertex, visited);
        }

    }
}







int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,12},{0,2,8},{0,3,21},{2,3,6},{2,6,2},{5,6,6},{4,5,9},{2,4,4},{2,5,5}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();
    graph.BFS(0);
    return 0;
}
