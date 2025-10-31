#include <iostream>
using namespace std;

struct Node{
    int val;
    Node* left;
    Node* right;
    Node* parent;
    Node(int v){val = v; left = right = parent = nullptr;}
};
struct BST{
    Node* root;
    BST(){root = nullptr;}
    Node* insert(Node* curr, int v){
        if(curr == nullptr) return new Node(v);
        if(v < curr->val){
            Node* temp = insert(curr->left, v);
            curr->left = temp;
            temp->parent = curr;
     }else{
            Node* temp = insert(curr->right, v);
            curr->right = temp;
            temp->parent = curr;
      }
        return curr;
  }
    void insertVal(int v){root = insert(root, v);}
    Node* find(Node* curr, int v){
        if(curr == nullptr) return nullptr;
        if(curr->val == v) return curr;
        if(v < curr->val) return find(curr->left, v);
        return find(curr->right, v);
  }
    Node* findVal(int v){return find(root, v);}
};
int pathToRoot(Node* node, Node* path[], int &len){
    len = 0;
    while(node != nullptr){
        path[len++] = node;
        node = node->parent;
  } return len;
}
Node* findLCA(Node* u, Node* v){
    Node* pathU[100];
    Node* pathV[100];
    int lenU, lenV;
    pathToRoot(u, pathU, lenU);
    pathToRoot(v, pathV, lenV);
    int i = lenU-1, j = lenV-1;
    Node* lca = nullptr;
    while(i>=0 && j>=0 && pathU[i]==pathV[j]){
        lca = pathU[i];
        i--; j--;
  }
   return lca;
}

int buildPath(Node* u, Node* v, Node* path[]){
    Node* lca = findLCA(u,v);
    int len = 0;
    Node* temp = u;
    while(temp != lca){
        path[len++] = temp;
        temp = temp->parent;
  }
    path[len++] = lca;
    int stack[100]; 
    int top = -1;
    temp = v;
    while(temp != lca){
        stack[++top] = temp->val; 
        temp = temp->parent;
  }
    while(top >= 0){
        Node* n = new Node(stack[top--]);
        path[len++] = n;
  }

    return len;
}
int main(){
    int n;
    cin >> n;
    BST bst;
    for(int i=0;i<n;i++){
        int v; cin >> v;
        bst.insertVal(v);
  }
    int q; cin >> q;
    for(int i=0;i<q;i++){
        int uVal, vVal, k;
        cin >> uVal >> vVal >> k;
        Node* uNode = bst.findVal(uVal);
        Node* vNode = bst.findVal(vVal);
        if(uNode==nullptr || vNode==nullptr){cout << -1 << endl; continue;}
        Node* path[100];
        int len = buildPath(uNode, vNode, path);
        if(k <= 0 || k > len) cout << -1 << endl;
        else cout << path[k-1]->val << endl;
  }
    return 0;
}

