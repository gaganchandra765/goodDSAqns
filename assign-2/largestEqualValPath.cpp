#include <iostream>
using namespace std;

struct Edge{
    int to;
    int next;
};

struct Graph{
    Edge edges[2000];
    int head[1001];
    int edgeCount;
    Graph(){
        for(int i=0;i<1001;i++) head[i] = -1;
        edgeCount = 0;
   }
    void addEdge(int u,int v){
        edges[edgeCount].to = v;
        edges[edgeCount].next = head[u];
        head[u] = edgeCount++;
   }
};

int n;
int values[1001];
Graph g;
int ans = 1;
int dfs(int u, int parent){
    int longest1 = 0, longest2 = 0; 
    for(int e = g.head[u]; e != -1; e = g.edges[e].next){
        int v = g.edges[e].to;
        if(v == parent) continue;
        int childLen = dfs(v, u);
        if(values[v] == values[u]){
            if(childLen > longest1){
                longest2 = longest1;
                longest1 = childLen;
           }else if(childLen > longest2){
                longest2 = childLen;
           }}}
    int candidate = 1 + longest1 + longest2;
    if(candidate > ans) ans = candidate;
    return 1 + longest1;
}

int main(){
    cin >> n;
    for(int i=1;i<=n;i++) cin >> values[i];
    for(int i=0;i<n-1;i++){
        int u,v;
        cin >> u >> v;
        g.addEdge(u,v);
        g.addEdge(v,u);
   }
    dfs(1,-1);
    cout<<ans<<endl;
    return 0;
}

