#include <iostream>
using namespace std;

// =========================
// STRUCT DEFINITIONS
// =========================

// Node in adjacency list
struct Node {
    int vertex;
    int weight;
    Node* next;
};

// Graph structure using adjacency list
struct Graph {
    int numVertices;
    Node** adjList;

    void init(int n) {
        numVertices = n;
        adjList = new Node*[n + 1];
        for (int i = 1; i <= n; i++)
            adjList[i] = NULL;
    }

    void addEdge(int u, int v, int w) {
        // Add v to u's list
        Node* temp1 = new Node;
        temp1->vertex = v;
        temp1->weight = w;
        temp1->next = adjList[u];
        adjList[u] = temp1;

        // Add u to v's list (since undirected)
        Node* temp2 = new Node;
        temp2->vertex = u;
        temp2->weight = w;
        temp2->next = adjList[v];
        adjList[v] = temp2;
    }
};

// =========================
// PRIM'S ALGORITHM
// =========================
void primMST(Graph& g) {
    int n = g.numVertices;

    bool* inMST = new bool[n + 1];
    int* key = new int[n + 1];      // Minimum weight edge to connect vertex
    int* parent = new int[n + 1];   // To store MST edges

    for (int i = 1; i <= n; i++) {
        key[i] = 999999;  // infinity
        inMST[i] = false;
        parent[i] = -1;
    }

    key[1] = 0; // Start from vertex 1

    // MST will have (n-1) edges
    for (int count = 1; count <= n - 1; count++) {
        // Pick vertex u not in MST with smallest key value
        int minKey = 999999, u = -1;
        for (int v = 1; v <= n; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1) break; // disconnected graph

        inMST[u] = true;

        // Update key and parent for adjacent vertices
        Node* temp = g.adjList[u];
        while (temp != NULL) {
            int v = temp->vertex;
            int w = temp->weight;
            if (!inMST[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
            }
            temp = temp->next;
        }
    }

    // Print MST
    int totalWeight = 0;
    cout << "Edges in MST:" << endl;
    for (int v = 2; v <= n; v++) {
        if (parent[v] != -1) {
            cout << "(" << parent[v] << ", " << v << ") weight = " << key[v] << endl;
            totalWeight += key[v];
        }
    }
    cout << "Total Weight = " << totalWeight << endl;

    delete[] inMST;
    delete[] key;
    delete[] parent;
}

// =========================
// MAIN FUNCTION
// =========================
int main() {
    int n, m;
    cin >> n >> m;

    Graph g;
    g.init(n);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }

    primMST(g);

    return 0;
}

