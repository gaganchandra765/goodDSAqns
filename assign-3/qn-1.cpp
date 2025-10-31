#include <bits/stdc++.h>
using namespace std;

struct Node {
    int data;
    int height;
    Node *left;
    Node *right;
    
    Node(int val) {
        data = val;
        height = 1;
        left = right = NULL;
    }
};

int height(Node* n) {
    return n ? n->height : 0;
}

int getBalance(Node* n) {
    if (!n) return 0;
    return height(n->left) - height(n->right);
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

bool search(Node* root, int key) {
    if (!root) return false;
    if (root->data == key) return true;
    else if (key < root->data) return search(root->left, key);
    else return search(root->right, key);
}

Node* insert(Node* root, int key, bool &inserted, bool &duplicate) {
    if (!root) {
        inserted = true;
        return new Node(key);
    }

    if (key < root->data)
        root->left = insert(root->left, key, inserted, duplicate);
    else if (key > root->data)
        root->right = insert(root->right, key, inserted, duplicate);
    else {
        duplicate = true;
        return root; // duplicate key
    }
    root->height = 1 + max(height(root->left), height(root->right));
    
    int balance = getBalance(root);
    
    // LL
    if (balance > 1 && key < root->left->data)
        return rightRotate(root);

    // RR
    if (balance < -1 && key > root->right->data)
        return leftRotate(root);

    // LR
    if (balance > 1 && key > root->left->data) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RL
    if (balance < -1 && key < root->right->data) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int key, bool &deleted, bool &notPresent) {
    if (!root) {
        notPresent = true;
        return root;
    }

    if (key < root->data)
        root->left = deleteNode(root->left, key, deleted, notPresent);
    else if (key > root->data)
        root->right = deleteNode(root->right, key, deleted, notPresent);
    else {
        deleted = true;

        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;

            if (!temp) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data, deleted, notPresent);
        }
    }

    if (!root) return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    // LL
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // LR
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // RL
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void printLevelOrder(Node* root) {
    if (!root) return;
    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            Node* temp = q.front();
            q.pop();
            cout << temp->data;
            if (i != size - 1) cout << " ";
            if (temp->left) q.push(temp->left);
            if (temp->right) q.push(temp->right);
        }
        cout << ""<<endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        bool inserted = false, duplicate = false;
        root = insert(root, x, inserted, duplicate);
    }

    int type, val;
    while (cin >> type) {
        if (type == 1) {
            cin >> val;
            if (search(root, val)) cout << val << " present"<<endl;
            else cout << val << " not present"<<endl;
        }
        else if (type == 2) {
            cin >> val;
            bool inserted = false, duplicate = false;
            root = insert(root, val, inserted, duplicate);
            if (duplicate) cout << val << " already present. So no need to insert."<<endl;
            else if (inserted) cout << val << " inserted"<<endl;
        }
        else if (type == 3) {
            cin >> val;
            bool deleted = false, notPresent = false;
            root = deleteNode(root, val, deleted, notPresent);
            if (notPresent) cout << val << " not present. So it can not be deleted"<<endl;
            else if (deleted) cout << val << " deleted"<<endl;
        }
        else if (type == 4) {
            printLevelOrder(root);
        }
    }

    return 0;
}

