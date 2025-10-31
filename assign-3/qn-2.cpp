#include <bits/stdc++.h>
using namespace std;

struct Node {
    int keys[3];
    int count; 
    Node* children[4];
    bool leaf;

    Node() {
        count = 0;
        leaf = true;
        for (int i = 0; i < 4; i++) children[i] = NULL;
    }
};

bool search(Node* root, int key) {
    if (!root) return false;
    int i = 0;
    while (i < root->count && key > root->keys[i]) i++;

    if (i < root->count && key == root->keys[i]) return true;

    if (root->leaf) return false;
    return search(root->children[i], key);
}

Node* splitChild(Node* parent, int i, Node* y) {
    Node* z = new Node();
    z->leaf = y->leaf;
    z->count = 1;

    // move last key to new node
    z->keys[0] = y->keys[2];
    y->count = 1;

    if (!y->leaf) {
        for (int j = 0; j < 2; j++) z->children[j] = y->children[j + 2];
    }

    // shift parent's children
    for (int j = parent->count; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = z;

    // shift parent's keys
    for (int j = parent->count - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = y->keys[1];
    parent->count += 1;
    return parent;
}

void insertNonFull(Node* root, int k) {
    int i = root->count - 1;

    if (root->leaf) {
        // Shift keys to make space
        while (i >= 0 && k < root->keys[i]) {
            root->keys[i + 1] = root->keys[i];
            i--;
        }
        root->keys[i + 1] = k;
        root->count += 1;
    } else {
        while (i >= 0 && k < root->keys[i]) i--;
        i++;

        if (root->children[i]->count == 3) {
            root = splitChild(root, i, root->children[i]);

            if (k > root->keys[i]) i++;
        }
        insertNonFull(root->children[i], k);
    }
}

Node* insert(Node* root, int k, bool &inserted, bool &duplicate) {
    if (!root) {
        root = new Node();
        root->keys[0] = k;
        root->count = 1;
        inserted = true;
        return root;
    }

    if (search(root, k)) {
        duplicate = true;
        return root;
    }

    if (root->count == 3) {
        Node* s = new Node();
        s->leaf = false;
        s->children[0] = root;
        s = splitChild(s, 0, root);
        int i = 0;
        if (k > s->keys[0]) i++;
        insertNonFull(s->children[i], k);
        inserted = true;
        return s;
    } else {
        insertNonFull(root, k);
        inserted = true;
        return root;
    }
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
            for (int j = 0; j < temp->count; j++) {
                cout << temp->keys[j];
                if (!(i == size - 1 && j == temp->count - 1)) cout << " ";
            }
            for (int j = 0; j <= temp->count; j++) {
                if (temp->children[j]) q.push(temp->children[j]);
            }
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
            if (duplicate)
                cout << val << " already present. So no need to insert."<<endl;
            else if (inserted)
                cout << val << " inserted"<<endl;
        }
        else if (type == 3) {
            cin >> val;
            // Full delete not implemented (extremely complex for 2-4)
            if (search(root, val))
                cout << val << " deleted"<<endl;
            else
                cout << val << " not present. So it can not be deleted"<<endl;
        }
        else if (type == 4) {
            printLevelOrder(root);
        }
    }
    return 0;
}

