#include <iostream>
#include <vector>
#include <queue>
#include <map>
using namespace std;

const int SIZE = 13;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;

class Graph {
public:
    vector<vector<Pair>> adjList;
    map<int, string> stationNames;

    Graph(vector<Edge> const &edges) {
        adjList.resize(SIZE);
        
        // Initialize station names
        stationNames[0] = "Central Station";
        stationNames[2] = "University";
        stationNames[4] = "Shopping Mall";
        stationNames[5] = "Sports Complex";
        stationNames[6] = "Airport";
        stationNames[7] = "Hospital";
        stationNames[8] = "Park";
        stationNames[9] = "Theater";
        stationNames[10] = "Business District";
        stationNames[11] = "Residential Area";
        stationNames[12] = "Convention Center";

        for (auto &edge: edges) {
            adjList[edge.src].push_back(make_pair(edge.dest, edge.weight));
            adjList[edge.dest].push_back(make_pair(edge.src, edge.weight));
        }
    }

    void printTransportMap() {
        cout << "=== CITY TRANSPORTATION NETWORK ===" << endl;
        cout << "Station Connections (Travel Time in minutes):" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            if (stationNames.find(i) != stationNames.end()) {
                cout << stationNames[i] << " --> ";
                for (Pair v : adjList[i])
                    cout << "(" << stationNames[v.first] << ", " << v.second << "min) ";
                cout << endl;
            }
        }
        cout << endl;
    }

    void findFastestRoutes(int startStation) {
        vector<bool> visited(SIZE, false);
        cout << "=== FASTEST EXPLORATION ROUTE (DFS) ===" << endl;
        cout << "Starting from " << stationNames[startStation] << ":" << endl;
        DFSRecursive(startStation, visited);
        cout << endl << endl;
    }

    void findOptimalCoverage(int startStation) {
        vector<bool> visited(SIZE, false);
        cout << "=== OPTIMAL STATION COVERAGE (BFS) ===" << endl;
        cout << "Starting from " << stationNames[startStation] << ":" << endl;
        BFS(startStation, visited);
        cout << endl;
    }

    void calculateTravelTime(int start, int end) {
        cout << "=== DIRECT TRAVEL TIME ===" << endl;
        for (Pair neighbor : adjList[start]) {
            if (neighbor.first == end) {
                cout << stationNames[start] << " to " << stationNames[end] 
                     << ": " << neighbor.second << " minutes" << endl;
                return;
            }
        }
        cout << "No direct connection found." << endl;
    }

      void findShortestPath(int startStation, int endStation) {
        vector<int> dist(SIZE, INT_MAX);
        vector<int> parent(SIZE, -1);
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        dist[startStation] = 0;
        pq.push(make_pair(0, startStation));

        cout << "=== SHORTEST PATH CALCULATION ===" << endl;
        cout << "Finding fastest route from " << stationNames[startStation] 
             << " to " << stationNames[endStation] << endl;

        while (!pq.empty()) {
            int current = pq.top().second;
            int currentDist = pq.top().first;
            pq.pop();

            if (current == endStation) break;

            for (Pair neighbor : adjList[current]) {
                int neighborVertex = neighbor.first;
                int weight = neighbor.second;
                int newDist = currentDist + weight;

                if (newDist < dist[neighborVertex]) {
                    dist[neighborVertex] = newDist;
                    parent[neighborVertex] = current;
                    pq.push(make_pair(newDist, neighborVertex));
                }
            }
        }
        // Print the shortest path
        if (dist[endStation] == INT_MAX) {
            cout << "No path exists between these stations!" << endl;
        } else {
            cout << "🚀 FASTEST ROUTE: " << dist[endStation] << " minutes" << endl;
            cout << "📍 PATH: ";
            printPath(parent, endStation);
            cout << endl;
        }
        cout << endl;
    }

private:
    void DFSRecursive(int current, vector<bool>& visited) {
        visited[current] = true;
        cout << stationNames[current] << " → ";
        
        for (int i = adjList[current].size() - 1; i >= 0; i--) {
            int neighborVertex = adjList[current][i].first;
            if (!visited[neighborVertex]) {
                DFSRecursive(neighborVertex, visited);
            }
        }
    }

    void BFS(int startVertex, vector<bool>& visited) {
        queue<int> q;
        visited[startVertex] = true;
        q.push(startVertex);
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            cout << stationNames[current] << " → ";
            
            for (Pair neighbor : adjList[current]) {
                int neighborVertex = neighbor.first;
                if (!visited[neighborVertex]) {
                    visited[neighborVertex] = true;
                    q.push(neighborVertex);
                }
            }
        }
    }

    void printPath(vector<int>& parent, int station) {
        if (parent[station] != -1) {
            printPath(parent, parent[station]);
        }
        cout << stationNames[station];
        if (station != parent.size() - 1) cout << " → ";
    }

};

int main() {
    // Transportation network edges: (station1, station2, travel_time_minutes)
    vector<Edge> edges = {
        {0,2,15}, {2,6,5}, {5,6,12}, {4,5,7},
        {2,4,10}, {2,5,8}, {0,7,20}, {7,8,3},
        {8,9,14}, {4,10,6}, {10,11,9}, {6,12,18}, {11,12,4}
    };

    Graph transportSystem(edges);

    // Application Interface
    cout << "🚇 CITY TRANSPORTATION PLANNER 🚇" << endl;
    cout << "==================================" << endl;
    
    transportSystem.printTransportMap();
    transportSystem.findFastestRoutes(0);      // DFS for exploration
    transportSystem.findOptimalCoverage(0);    // BFS for coverage
    transportSystem.calculateTravelTime(0, 7); // Specific route query

    return 0;
}