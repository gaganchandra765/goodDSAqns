#include <bits/stdc++.h>
using namespace std;

int singleRotations = 0;
int doubleRotations = 0;

struct node{
    int data;
    int height;
    node* left;
    node* right;
    node(int val) : data(val), height(1), left(NULL), right(NULL){}
};

int height(node* n){ return n ? n->height : 0;}
int getBalance(node* n){ return n ? height(n->left) - height(n->right) : 0;}
// Rotations
node* rightRotate(node* y, bool countSingle = true){
    node* x = y->left;
    node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    if (countSingle) singleRotations++;
    return x;
}

node* leftRotate(node* x, bool countSingle = true){
    node* y = x->right;
    node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    if (countSingle) singleRotations++;
    return y;
}

  // Insertion
node* insert(node* root, int key){
    if (!root) return new node(key);
    if (key < root->data)
        root->left = insert(root->left, key);
    else if (key > root->data)
        root->right = insert(root->right, key);
    else
        return root; 
        
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);
    // LL
    if (balance > 1 && key < root->left->data)
        return rightRotate(root);
    // RR
    if (balance < -1 && key > root->right->data)
        return leftRotate(root);
    // LR
    if (balance > 1 && key > root->left->data){
        root->left = leftRotate(root->left, false);
        doubleRotations++;
        return rightRotate(root, false);
   }
    // RL
    if (balance <-1 && key < root->right->data){
        root->right = rightRotate(root->right, false);
        doubleRotations++;
        return leftRotate(root, false);
   }

    return root;
}
// min node 
node* minValueNode(node* n){
    while (n->left) n = n->left;
    return n;
}

// deletion
node* deleteNode(node* root, int key){
    if (!root) return root;
    if (key < root->data)
      root->left = deleteNode(root->left, key);
    else if (key > root->data)
        root->right = deleteNode(root->right, key);
    else{
        if (!root->left || !root->right){
            node* temp = root->left ? root->left : root->right;
            delete root;
            return temp;  // return child (or NULL)
       } else{
            node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data);
       }
   }
    if (!root) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    // LL
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // LR
    if (balance > 1 && getBalance(root->left) < 0){
        root->left = leftRotate(root->left, false);
        doubleRotations++;
        return rightRotate(root, false);
   }

    // RR
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // RL
    if (balance < -1 && getBalance(root->right) > 0){
        root->right = rightRotate(root->right, false);
        doubleRotations++;
        return leftRotate(root, false);
   }

    return root;
    }


node* search(node* root, int key){
    if (!root || root->data == key) return root;
    if (key < root->data)
        return search(root->left, key);
    return search(root->right, key);
}

// Inorder trav
void inorder(node* root){
    if (!root) return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

int main(){
    int N;
    if (!(cin >> N)) return 0;

    node* root = NULL;
    for (int i = 0; i < N; i++){
        char op;
        int x;
        cin >> op >> x;
        if (op == 'I')
            root = insert(root, x);
        else if (op == 'D')
            root = deleteNode(root, x);
   }
    cout << "Inorder: ";
    inorder(root);
    cout << endl;
    cout << "Single rotations: " << singleRotations << endl;
    cout << "Double rotations: " << doubleRotations << endl;


    return 0;
}
