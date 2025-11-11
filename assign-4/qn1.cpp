#include <iostream>
using namespace std;

// =========================
// STRUCT DEFINITIONS
// =========================

// Node in adjacency list
struct Node {
    int vertex;          // Connected vertex
    Node* next;          // Pointer to next node
};

// Queue node (for BFS)
struct QueueNode {
    int data;
    QueueNode* next;
};

// Simple Queue implementation using linked list
struct Queue {
    QueueNode* front;
    QueueNode* rear;

    // Initialize empty queue
    void init() {
        front = rear = NULL;
    }

    // Enqueue operation
    void enqueue(int x) {
        QueueNode* temp = new QueueNode;
        temp->data = x;
        temp->next = NULL;
        if (rear == NULL) {
            front = rear = temp;
        } else {
            rear->next = temp;
            rear = temp;
        }
    }

    // Dequeue operation
    int dequeue() {
        if (front == NULL) return -1;  // empty queue
        int val = front->data;
        QueueNode* temp = front;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete temp;
        return val;
    }

    // Check if queue is empty
    bool isEmpty() {
        return (front == NULL);
    }
};

// Graph structure using adjacency list
struct Graph {
    int numVertices;
    Node** adjList;   // array of adjacency lists

    // Initialize graph with n vertices
    void init(int n) {
        numVertices = n;
        adjList = new Node*[n + 1]; // 1-indexed
        for (int i = 1; i <= n; i++) {
            adjList[i] = NULL;
        }
    }

    // Add undirected edge u-v
    void addEdge(int u, int v) {
        // Add v to u’s list
        Node* temp1 = new Node;
        temp1->vertex = v;
        temp1->next = adjList[u];
        adjList[u] = temp1;

        // Add u to v’s list (since undirected)
        Node* temp2 = new Node;
        temp2->vertex = u;
        temp2->next = adjList[v];
        adjList[v] = temp2;
    }
};

// Function to print path from source to vertex v
void printPath(int parent[], int v, int s) {
    if (v == s) {
        cout << s;
        return;
    }
    if (parent[v] == -1) {
        cout << "Unreachable";
        return;
    }
    printPath(parent, parent[v], s);
    cout << " " << v;
}

// BFS algorithm
void BFS(Graph& g, int s) {
    int n = g.numVertices;

    // Arrays for visited, distance, and parent tracking
    bool* visited = new bool[n + 1];
    int* dist = new int[n + 1];
    int* parent = new int[n + 1];

    // Initialize arrays
    for (int i = 1; i <= n; i++) {
        visited[i] = false;
        dist[i] = -1;
        parent[i] = -1;
    }

    Queue q;
    q.init();

    // Start BFS from source s
    visited[s] = true;
    dist[s] = 0;
    q.enqueue(s);

    while (!q.isEmpty()) {
        int u = q.dequeue();

        Node* temp = g.adjList[u];
        while (temp != NULL) {
            int v = temp->vertex;
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.enqueue(v);
            }
            temp = temp->next;
        }
    }

    // Print results
    for (int v = 1; v <= n; v++) {
        cout << "Vertex " << v << ": ";
        if (dist[v] == -1) {
            cout << "Length = -1, Path = Unreachable" << endl;
        } else {
            cout << "Length = " << dist[v] << ", Path = ";
            printPath(parent, v, s);
            cout << endl;
        }
    }

    delete[] visited;
    delete[] dist;
    delete[] parent;
}

// =========================
// MAIN FUNCTION
// =========================
int main() {
    int n, m;
    cin >> n; // number of vertices
    cin >> m; // number of edges

    Graph g;
    g.init(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int s;
    cin >> s; // source vertex

    BFS(g, s);

    return 0;
}

