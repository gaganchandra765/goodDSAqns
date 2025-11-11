#include <bits/stdc++.h>
using namespace std;

struct Node{
    int data;
    int height;
    Node *left, *right;
    Node(int v){ data = v; height = 1; left = right = NULL;}
};

int Height(Node* n){ return n ? n->height : 0;}
int Balance(Node* n){ return n ? Height(n->left) - Height(n->right) : 0;}

Node* rightRotate(Node* y){
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max(Height(y->left), Height(y->right)) + 1;
    x->height = max(Height(x->left), Height(x->right)) + 1;
    return x;
}

Node* leftRotate(Node* x){
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max(Height(x->left), Height(x->right)) + 1;
    y->height = max(Height(y->left), Height(y->right)) + 1;
    return y;
}

bool search(Node* root, int key){
    if (!root) return false;
    if (root->data == key) return true;
    if (key < root->data) return search(root->left, key);
    return search(root->right, key);
}

Node* insert(Node* root, int key, bool& inserted, bool& duplicate){
    if (!root){ inserted = true; return new Node(key);}

    if (key < root->data)
        root->left = insert(root->left, key, inserted, duplicate);
    else if (key > root->data)
        root->right = insert(root->right, key, inserted, duplicate);
    else{ duplicate = true; return root;}      // duplicates

    root->height = 1 + max(Height(root->left), Height(root->right));
    int bal = Balance(root);

    // LL
    if (bal > 1 && key < root->left->data) return rightRotate(root);
    // RR
    if (bal < -1 && key > root->right->data) return leftRotate(root);
    // LR
    if (bal > 1 && key > root->left->data){
        root->left = leftRotate(root->left);
        return rightRotate(root);
 }
    // RL
    if (bal < -1 && key < root->right->data){
        root->right = rightRotate(root->right);
        return leftRotate(root);
 }
    return root;
}

Node* minNode(Node* n){
    while (n && n->left) n = n->left;
    return n;
}

Node* deleteNode(Node* root, int key, bool& deleted, bool& notPresent){
    if (!root){ notPresent = true; return root;}

    if (key < root->data)
        root->left = deleteNode(root->left, key, deleted, notPresent);
    else if (key > root->data)
        root->right = deleteNode(root->right, key, deleted, notPresent);
    else{
        deleted = true;
        if (!root->left || !root->right){               // 0 or 1 child
            Node* temp = root->left ? root->left : root->right;
            delete root;
            return temp;
     } else{                                       
              Node* temp = minNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data, deleted, notPresent);
     }
 }

    if (!root) return root;
    root->height = 1 + max(Height(root->left), Height(root->right));
    int bal = Balance(root);
    // LL
    if (bal > 1 && Balance(root->left) >= 0) return rightRotate(root);
    // LR
    if (bal > 1 && Balance(root->left) < 0){
        root->left = leftRotate(root->left);
        return rightRotate(root);
 }    // RR
    if (bal < -1 && Balance(root->right) <= 0) return leftRotate(root);
    // RL
    if (bal < -1 && Balance(root->right) > 0){
        root->right = rightRotate(root->right);
        return leftRotate(root);
 }
    return root;
}

void printLevelOrder(Node* root){
    if (!root) return;
    queue<Node*> q;
    q.push(root);
    while (!q.empty()){
        int sz = q.size();
        for (int i = 0; i < sz; ++i){
            Node* cur = q.front(); q.pop();
            cout << cur->data;
            if (i < sz - 1) cout << " ";
            if (cur->left)  q.push(cur->left);
            if (cur->right) q.push(cur->right);
     }
        cout << endl;
 }
}

int main(){
    int n;
    if (!(cin >> n)) return 0;

    Node* root = NULL;
    for (int i = 0; i < n; ++i){
        int x; cin >> x;
        bool ins = false, dup = false;
        root = insert(root, x, ins, dup);
 }
int type;
    while (cin >> type){
        if (type == 1){                     
            int val; cin >> val;
            if (search(root, val))
                cout << val << " present" << endl;
            else
                cout << val << " not present" << endl;
        }
        else if (type == 2){               
            int val; cin >> val;
            bool inserted = false, duplicate = false;
            root = insert(root, val, inserted, duplicate);
            if (duplicate)
                cout << val << " already present. So no need to insert." << endl;
            else
                cout << val << " inserted" << endl;
     }
        else if (type == 3){              
            int val; cin >> val;
            bool deleted = false, notPresent = false;
            root = deleteNode(root, val, deleted, notPresent);
            if (notPresent)
                cout << val << " not present. So it can not be deleted" << endl;
            else
                cout << val << " deleted" << endl;
     }
        else if (type == 4){               
            printLevelOrder(root);
     }
 }
    return 0;
}
