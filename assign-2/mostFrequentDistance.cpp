#include <iostream>
#include <string>
using namespace std;
struct Tree{
    string nodes[1000]; 
    int size;
    Tree(){size = 0;}
    bool valid(int idx){return idx <= size && nodes[idx] != "" && nodes[idx] != "NULL";}
};

bool buildTree(string inorder[], string preorder[], int inStart, int inEnd, int preStart, int preEnd,
               string nodes[], int pos){
    if(inStart > inEnd) return true;
    if(preStart > preEnd) return false;
    string root = preorder[preStart];
    int rootIndex = -1;
    for(int i=inStart;i<=inEnd;i++){
        if(inorder[i]==root){rootIndex=i; break;}
  }
    if(rootIndex==-1) return false;
    nodes[pos] = root;
    int leftSize = rootIndex - inStart;
    int rightSize = inEnd - rootIndex;
    if(leftSize>0){
        if(!buildTree(inorder, preorder, inStart, rootIndex-1, preStart+1, preStart+leftSize, nodes, 2*pos))
            return false;
  }else nodes[2*pos] = "NULL";
    if(rightSize>0){
        if(!buildTree(inorder, preorder, rootIndex+1, inEnd, preStart+leftSize+1, preEnd, nodes, 2*pos+1))
            return false;
  }else nodes[2*pos+1] = "NULL";
    return true;
}

void computeDepths(Tree &t, int depths[], int idx, int d){
    if(!t.valid(idx)) return;
    depths[idx] = d;
    computeDepths(t, depths, 2*idx, d+1);
    computeDepths(t, depths, 2*idx+1, d+1);
}
int findLCA(int u, int v){
    while(u != v){
        if(u>v) u/=2;
        else v/=2;
  }
    return u;
}

int main(){
    int n;
    cin >> n;
    string preorder[1000], inorder[1000];
    for(int i=0;i<n;i++) cin >> preorder[i];
    for(int i=0;i<n;i++) cin >> inorder[i];
    Tree t;
    for(int i=0;i<1000;i++) t.nodes[i] = "";
    t.size = 0;
    bool ok = buildTree(inorder, preorder, 0, n-1, 0, n-1, t.nodes, 1);
    if(!ok){cout << "ERROR"<<endl; return 0;}
    int maxIdx=1;
    for(int i=1;i<1000;i++){
        if(t.valid(i)) maxIdx=i;
  }
    t.size=maxIdx;
    int depths[1000];
    for(int i=0;i<1000;i++) depths[i]=-1;
    computeDepths(t, depths, 1, 0);
    int distCount[1000] ={0}; 
    for(int i=1;i<=t.size;i++){
        if(!t.valid(i)) continue;
        for(int j=i+1;j<=t.size;j++){
            if(!t.valid(j)) continue;
            int lca = findLCA(i,j);
            int dist = depths[i] + depths[j] - 2*depths[lca];
            distCount[dist]++;
      }
  }
    int maxFreq=0, ans=1;
    for(int i=1;i<1000;i++){
        if(distCount[i]>maxFreq){
            maxFreq=distCount[i];
            ans=i;
      }
  }
    cout << ans << endl;
    return 0;
}

