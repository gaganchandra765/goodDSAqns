#include <iostream>
using namespace std;

// =========================
// STRUCT DEFINITIONS
// =========================

// Node for adjacency list
struct Node {
    int vertex;
    int weight;
    Node* next;
};

// Graph representation
struct Graph {
    int numVertices;
    Node** adjList;

    void init(int n) {
        numVertices = n;
        adjList = new Node*[n + 1];
        for (int i = 1; i <= n; i++)
            adjList[i] = NULL;
    }

    // Add directed edge u -> v with weight w
    void addEdge(int u, int v, int w) {
        Node* temp = new Node;
        temp->vertex = v;
        temp->weight = w;
        temp->next = adjList[u];
        adjList[u] = temp;
    }
};

// =========================
// MIN HEAP IMPLEMENTATION
// =========================
struct HeapNode {
    int vertex;
    int dist;
};

struct MinHeap {
    int size;
    int capacity;
    HeapNode* arr;
    int* pos; // to track positions for decrease-key

    void init(int cap) {
        capacity = cap;
        size = 0;
        arr = new HeapNode[cap + 1];
        pos = new int[cap + 1];
    }

    void swapNodes(int i, int j) {
        HeapNode temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
        pos[arr[i].vertex] = i;
        pos[arr[j].vertex] = j;
    }

    void heapifyDown(int i) {
        int left = 2 * i, right = 2 * i + 1, smallest = i;
        if (left <= size && arr[left].dist < arr[smallest].dist) smallest = left;
        if (right <= size && arr[right].dist < arr[smallest].dist) smallest = right;
        if (smallest != i) {
            swapNodes(i, smallest);
            heapifyDown(smallest);
        }
    }

    void heapifyUp(int i) {
        int parent = i / 2;
        if (i > 1 && arr[i].dist < arr[parent].dist) {
            swapNodes(i, parent);
            heapifyUp(parent);
        }
    }

    void insert(int v, int dist) {
        size++;
        arr[size].vertex = v;
        arr[size].dist = dist;
        pos[v] = size;
        heapifyUp(size);
    }

    bool isEmpty() {
        return size == 0;
    }

    HeapNode extractMin() {
        HeapNode root = arr[1];
        arr[1] = arr[size];
        pos[arr[1].vertex] = 1;
        size--;
        heapifyDown(1);
        return root;
    }

    void decreaseKey(int v, int newDist) {
        int i = pos[v];
        arr[i].dist = newDist;
        heapifyUp(i);
    }

    bool isInHeap(int v) {
        return pos[v] <= size && pos[v] >= 1;
    }
};

// =========================
// PATH PRINTING FUNCTION
// =========================
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

// =========================
// DIJKSTRA'S ALGORITHM
// =========================
void dijkstra(Graph& g, int s) {
    int n = g.numVertices;

    int* dist = new int[n + 1];
    int* parent = new int[n + 1];
    bool* inHeap = new bool[n + 1];

    for (int i = 1; i <= n; i++) {
        dist[i] = 999999;
        parent[i] = -1;
        inHeap[i] = true;
    }

    MinHeap heap;
    heap.init(n);

    // Initialize heap
    for (int v = 1; v <= n; v++) {
        heap.insert(v, dist[v]);
    }

    // Distance to source = 0
    dist[s] = 0;
    heap.decreaseKey(s, 0);

    while (!heap.isEmpty()) {
        HeapNode minNode = heap.extractMin();
        int u = minNode.vertex;
        inHeap[u] = false;

        Node* temp = g.adjList[u];
        while (temp != NULL) {
            int v = temp->vertex;
            int w = temp->weight;

            if (inHeap[v] && dist[u] != 999999 && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                heap.decreaseKey(v, dist[v]);
            }
            temp = temp->next;
        }
    }

    // Output
    for (int v = 1; v <= n; v++) {
        cout << "Vertex " << v << ": ";
        if (dist[v] == 999999) {
            cout << "Length = -1, Path = Unreachable" << endl;
        } else {
            cout << "Length = " << dist[v] << ", Path = ";
            printPath(parent, v, s);
            cout << endl;
        }
    }

    delete[] dist;
    delete[] parent;
    delete[] inHeap;
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

    int s;
    cin >> s;

    dijkstra(g, s);

    return 0;
}

