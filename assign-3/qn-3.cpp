#include <bits/stdc++.h>
using namespace std;

const int t_val = 2;                    // order of B-tree
const int MIN_KEYS = t_val - 1;
const int MAX_KEYS = 2 * t_val - 1;

struct BTreeNode {
    int key[MAX_KEYS];
    int count;                          // number of valid keys
    BTreeNode* children[MAX_KEYS + 1];  // children pointers
    bool leaf;

    BTreeNode(bool isLeaf = true) {
        count = 0;
        leaf = isLeaf;
        for (int i = 0; i < MAX_KEYS + 1; i++)
            children[i] = NULL;
    }
};

// ---------- SEARCH ----------
bool search(BTreeNode* node, int k) {
    if (!node) return false;
    int i = 0;
    while (i < node->count && k > node->key[i]) i++;
    if (i < node->count && node->key[i] == k) return true;
    if (node->leaf) return false;
    return search(node->children[i], k);
}

// ---------- SPLIT ----------
void splitChild(BTreeNode* parent, int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->leaf);
    z->count = t_val - 1;

    // move last (t-1) keys from y to z
    for (int j = 0; j < t_val - 1; j++)
        z->key[j] = y->key[j + t_val];

    // move children if not leaf
    if (!y->leaf) {
        for (int j = 0; j < t_val; j++)
            z->children[j] = y->children[j + t_val];
    }

    y->count = t_val - 1;

    // make room in parent for new child
    for (int j = parent->count; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[i + 1] = z;

    // move keys in parent
    for (int j = parent->count - 1; j >= i; j--)
        parent->key[j + 1] = parent->key[j];

    parent->key[i] = y->key[t_val - 1];
    parent->count++;
}

// ---------- INSERT NON-FULL ----------
void insertNonFull(BTreeNode* node, int k) {
    int i = node->count - 1;

    if (node->leaf) {
        // shift keys to make space
        while (i >= 0 && k < node->key[i]) {
            node->key[i + 1] = node->key[i];
            i--;
        }
        node->key[i + 1] = k;
        node->count++;
    } else {
        while (i >= 0 && k < node->key[i]) i--;
        i++;
        if (node->children[i]->count == MAX_KEYS) {
            splitChild(node, i, node->children[i]);
            if (k > node->key[i]) i++;
        }
        insertNonFull(node->children[i], k);
    }
}

// ---------- INSERT (with duplicate check) ----------
BTreeNode* insert(BTreeNode* root, int k, bool &inserted, bool &duplicate) {
    if (!root) {
        root = new BTreeNode(true);
        root->key[0] = k;
        root->count = 1;
        inserted = true;
        return root;
    }

    if (search(root, k)) {
        duplicate = true;
        return root;
    }

    if (root->count == MAX_KEYS) {
        BTreeNode* s = new BTreeNode(false);
        s->children[0] = root;
        splitChild(s, 0, root);
        int i = 0;
        if (k > s->key[0]) i++;
        insertNonFull(s->children[i], k);
        inserted = true;
        return s;
    } else {
        insertNonFull(root, k);
        inserted = true;
        return root;
    }
}

// ---------- LEVEL ORDER PRINT ----------
void printLevelOrder(BTreeNode* root) {
    if (!root) return;
    queue<BTreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            BTreeNode* temp = q.front();
            q.pop();
            for (int j = 0; j < temp->count; j++) {
                cout << temp->key[j];
                if (!(i == size - 1 && j == temp->count - 1)) cout << " ";
            }
            for (int j = 0; j <= temp->count; j++)
                if (temp->children[j]) q.push(temp->children[j]);
        }
        cout << ""<<endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    BTreeNode* root = NULL;

    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        bool inserted = false, duplicate = false;
        root = insert(root, x, inserted, duplicate);
    }

    int type, val;
    while (cin >> type) {
        if (type == 1) {
            cin >> val;
            if (search(root, val))
                cout << val << " present"<<endl;
            else
                cout << val << " not present"<<endl;
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
            // Deletion not implemented (printing messages only)
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

