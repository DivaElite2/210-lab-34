#include <iostream>
#include <climits>
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
    // NEW: Minimum Spanning Tree for Transportation Network
    void buildOptimalNetwork() {
        vector<int> key(SIZE, INT_MAX);
        vector<int> parent(SIZE, -1);
        vector<bool> inMST(SIZE, false);
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        // Start from Central Station
        key[0] = 0;
        pq.push(make_pair(0, 0));

        cout << "=== OPTIMAL NETWORK INFRASTRUCTURE ===" << endl;
        cout << "Building most efficient connection network:" << endl << endl;

        while (!pq.empty()) {
            int current = pq.top().second;
            pq.pop();

            inMST[current] = true;

            for (Pair neighbor : adjList[current]) {
                int neighborVertex = neighbor.first;
                int weight = neighbor.second;

                if (!inMST[neighborVertex] && weight < key[neighborVertex]) {
                    key[neighborVertex] = weight;
                    parent[neighborVertex] = current;
                    pq.push(make_pair(weight, neighborVertex));
                }
            }
        }
              // Display the optimal network
        cout << "🏗️  MINIMUM SPANNING TREE CONNECTIONS:" << endl;
        int totalCost = 0;
        int connectionCount = 0;
        
        for (int i = 1; i < SIZE; i++) {
            if (parent[i] != -1 && stationNames.find(i) != stationNames.end() && 
                stationNames.find(parent[i]) != stationNames.end()) {
                connectionCount++;
                cout << "   " << connectionCount << ". " << stationNames[parent[i]] 
                     << " ←[" << key[i] << "min]→ " << stationNames[i] << endl;
                totalCost += key[i];
            }
        }
        
        cout << endl << "📊 NETWORK SUMMARY:" << endl;
        cout << "   • Total Connections: " << connectionCount << endl;
        cout << "   • Total Infrastructure Cost: " << totalCost << " minutes" << endl;
        cout << "   • Average Connection Cost: " << (totalCost / (double)connectionCount) << " minutes" << endl;
        cout << "   • All stations connected with minimum total travel time" << endl << endl;
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

// Menu Functions
void displayMenu() {
    cout << "\n=========================================" << endl;
    cout << "   🚇 CITY TRANSPORTATION PLANNER 🚇" << endl;
    cout << "=========================================" << endl;
    cout << "1. View Transportation Network Map" << endl;
    cout << "2. Find Fastest Exploration Route (DFS)" << endl;
    cout << "3. Find Optimal Station Coverage (BFS)" << endl;
    cout << "4. Calculate Shortest Path Between Stations" << endl;
    cout << "5. Build Optimal Network Infrastructure (MST)" << endl;
    cout << "6. Run All Analyses" << endl;
    cout << "7. Exit" << endl;
    cout << "=========================================" << endl;
    cout << "Enter your choice (1-7): ";
}

void runAllAnalyses(Graph& transportSystem) {
    cout << "\n🚀 RUNNING COMPREHENSIVE TRANSPORTATION ANALYSIS..." << endl;
    transportSystem.printTransportMap();
    transportSystem.findFastestRoutes(0);
    transportSystem.findOptimalCoverage(0);
    transportSystem.findShortestPath(0, 10);
    transportSystem.buildOptimalNetwork();
}

int getStationInput(const string& prompt, Graph& transportSystem) {
    cout << "\nAvailable Stations:" << endl;
    for (int i = 0; i < SIZE; i++) {
        if (transportSystem.stationNames.find(i) != transportSystem.stationNames.end()) {
            cout << i << ". " << transportSystem.stationNames[i] << endl;
        }
    }
    
    int station;
    cout << prompt;
    cin >> station;
    
    while (station < 0 || station >= SIZE || 
           transportSystem.stationNames.find(station) == transportSystem.stationNames.end()) {
        cout << "Invalid station! Please enter a valid station number: ";
        cin >> station;
    }
    
    return station;
}


int main() {
    // Transportation network edges: (station1, station2, travel_time_minutes)
    vector<Edge> edges = {
        {0,2,15}, {2,6,5}, {5,6,12}, {4,5,7},
        {2,4,10}, {2,5,8}, {0,7,20}, {7,8,3},
        {8,9,14}, {4,10,6}, {10,11,9}, {6,12,18}, {11,12,4}
    };

    Graph transportSystem(edges);
    int choice;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                transportSystem.printTransportMap();
                break;
                
            case 2: {
                int start = getStationInput("Enter starting station number: ", transportSystem);
                transportSystem.findFastestRoutes(start);
                break;
            }
                
            case 3: {
                int start = getStationInput("Enter starting station number: ", transportSystem);
                transportSystem.findOptimalCoverage(start);
                break;
            }
                
            case 4: {
                int start = getStationInput("Enter starting station number: ", transportSystem);
                int end = getStationInput("Enter destination station number: ", transportSystem);
                transportSystem.findShortestPath(start, end);
                break;
            }
                
            case 5:
                transportSystem.buildOptimalNetwork();
                break;
                
            case 6:
                runAllAnalyses(transportSystem);
                break;
                
            case 7:
                cout << "\nThank you for using City Transportation Planner! Goodbye! 🚇" << endl;
                break;
                
            default:
                cout << "Invalid choice! Please enter 1-7." << endl;
        }
        
        if (choice != 7) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
        
    } while (choice != 7);

    // Application Interface
    cout << "🚇 CITY TRANSPORTATION PLANNER 🚇" << endl;
    cout << "==================================" << endl;
    
    transportSystem.printTransportMap();
    transportSystem.findFastestRoutes(0);      // DFS for exploration
    transportSystem.findOptimalCoverage(0);    // BFS for coverage
    transportSystem.calculateTravelTime(0, 7); // Specific route query

    return 0;
}