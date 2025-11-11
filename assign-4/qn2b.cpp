#include <iostream>
using namespace std;

// =========================
// STRUCT DEFINITIONS
// =========================

// Edge structure
struct Edge {
    int u, v, w;
};

// Disjoint Set Union (Union-Find)
struct DSU {
    int* parent;
    int* rank;
    int n;

    void init(int size) {
        n = size;
        parent = new int[n + 1];
        rank = new int[n + 1];
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int xr = find(x);
        int yr = find(y);
        if (xr == yr) return;
        if (rank[xr] < rank[yr]) parent[xr] = yr;
        else if (rank[xr] > rank[yr]) parent[yr] = xr;
        else {
            parent[yr] = xr;
            rank[xr]++;
        }
    }
};

// =========================
// KRUSKAL'S ALGORITHM
// =========================
void kruskalMST(Edge edges[], int n, int m) {
    // Sort edges by weight (simple bubble sort)
    for (int i = 0; i < m - 1; i++) {
        for (int j = 0; j < m - i - 1; j++) {
            if (edges[j].w > edges[j + 1].w) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    DSU dsu;
    dsu.init(n);

    int totalWeight = 0;
    cout << "Edges in MST:" << endl;

    int edgesUsed = 0;
    for (int i = 0; i < m && edgesUsed < n - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].w;

        if (dsu.find(u) != dsu.find(v)) {
            cout << "(" << u << ", " << v << ") weight = " << w << endl;
            totalWeight += w;
            dsu.unite(u, v);
            edgesUsed++;
        }
    }

    cout << "Total Weight = " << totalWeight << endl;
}

// =========================
// MAIN FUNCTION
// =========================
int main() {
    int n, m;
    cin >> n >> m;

    Edge* edges = new Edge[m];
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    kruskalMST(edges, n, m);

    delete[] edges;
    return 0;
}

