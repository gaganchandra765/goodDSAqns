#include <bits/stdc++.h>
using namespace std;

struct Node{
    int keys[3];
    int count;
    Node* children[4];
    bool leaf;  
    Node(){
        count = 0;
        leaf = true;
        for (int i = 0; i < 4; i++) children[i] = NULL;
    }
};

bool search(Node* root, int key){
    if (!root) return false;
    int i = 0;
    while (i < root->count && key > root->keys[i]) i++;
    if (i < root->count && key == root->keys[i]) return true;
    if (root->leaf) return false;
    return search(root->children[i], key);
}

void splitChild(Node* parent, int idx, Node* child){
    Node* newNode = new Node();
    newNode->leaf = child->leaf;
    newNode->count = 1;
    newNode->keys[0] = child->keys[2];
    if (!child->leaf){
        newNode->children[0] = child->children[2];
        newNode->children[1] = child->children[3];
    }
    child->count = 1;
    for (int j = parent->count; j >= idx + 1; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[idx + 1] = newNode;
    for (int j = parent->count - 1; j >= idx; j--)
        parent->keys[j + 1] = parent->keys[j];
    parent->keys[idx] = child->keys[1];
    parent->count++;
}

void insertNonFull(Node* node, int key){
    int i = node->count - 1;
    if (node->leaf){
        while (i >= 0 && key < node->keys[i]){
            node->keys[i + 1] = node->keys[i];
            i--;
    }
        node->keys[i + 1] = key;
        node->count++;
    } else{
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        if (node->children[i]->count == 3){
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i]) i++;
        }
        insertNonFull(node->children[i], key);
    }
}

Node* insert(Node* root, int key, bool& inserted, bool& duplicate){
    if (search(root, key)){
        duplicate = true;
        return root;
    }
    if (!root){
        root = new Node();
        root->leaf = true;
        root->keys[0] = key;
        root->count = 1;
        inserted = true;
        return root;
    }
    if (root->count == 3){
        Node* newRoot = new Node();
        newRoot->leaf = false;
        newRoot->children[0] = root;
        splitChild(newRoot, 0, root);
        int i = 0;
        if (key > newRoot->keys[0]) i++;
        insertNonFull(newRoot->children[i], key);
        inserted = true;
        return newRoot;
    } else{
        insertNonFull(root, key);
        inserted = true;
        return root;
    }
}

void merge(Node* parent, int idx){
    Node* left = parent->children[idx];
    Node* right = parent->children[idx + 1];
    left->keys[left->count] = parent->keys[idx];
    left->count++;
    for (int j = 0; j < right->count; j++)
        left->keys[left->count + j] = right->keys[j];
    left->count += right->count;
    if (!left->leaf){
        for (int j = 0; j <= right->count; j++)
            left->children[left->count + j - 1] = right->children[j];
    }
    for (int j = idx + 1; j < parent->count; j++)
        parent->keys[j - 1] = parent->keys[j];
    for (int j = idx + 2; j <= parent->count; j++)
        parent->children[j - 1] = parent->children[j];
    parent->count--;
    delete right;
}

void borrowFromLeft(Node* parent, int idx){
    Node* child = parent->children[idx];
    Node* sibling = parent->children[idx - 1];

    for (int j = child->count; j > 0; j--)
        child->keys[j] = child->keys[j - 1];
    if (!child->leaf){
        for (int j = child->count + 1; j > 0; j--)
            child->children[j] = child->children[j - 1];
    }

    child->keys[0] = parent->keys[idx - 1];
    child->count++;

    parent->keys[idx - 1] = sibling->keys[sibling->count - 1];
    sibling->count--;

    if (!child->leaf)
        child->children[0] = sibling->children[sibling->count + 1];
}

void borrowFromRight(Node* parent, int idx){
    Node* child = parent->children[idx];
    Node* sibling = parent->children[idx + 1];

    child->keys[child->count] = parent->keys[idx];
    child->count++;

    parent->keys[idx] = sibling->keys[0];
    sibling->count--;

    for (int j = 0; j < sibling->count; j++)
        sibling->keys[j] = sibling->keys[j + 1];

    if (!child->leaf){
        child->children[child->count] = sibling->children[0];
        for (int j = 0; j <= sibling->count; j++)
            sibling->children[j] = sibling->children[j + 1];
    }
}

void deleteFromLeaf(Node* node, int key){
    int idx = 0;
    while (idx < node->count && node->keys[idx] != key) idx++;
    for (int j = idx; j < node->count - 1; j++)
        node->keys[j] = node->keys[j + 1];
    node->count--;
}

// Delete from non-leaf (using predecessor/successor) 
void deleteFromNonLeaf(Node* node, int idx);

// Main delete function (must come before deleteFromNonLeaf)
void deleteKey(Node* node, int key){
    int idx = 0;
    while (idx < node->count && key > node->keys[idx]) idx++;
    if (idx < node->count && key == node->keys[idx]){
        if (node->leaf)
            deleteFromLeaf(node, key);
        else
            deleteFromNonLeaf(node, idx);
    } else{
            if (node->leaf) return;
        Node* child = node->children[idx];
        bool last = (idx == node->count);

        if (child->count == 1){
            if (idx > 0 && node->children[idx - 1]->count >= 2)
                borrowFromLeft(node, idx);
            else if (!last && node->children[idx + 1]->count >= 2)
                borrowFromRight(node, idx);
            else{
                if (last) idx--;
                merge(node, idx);
            }
            }

        if (last && idx > node->count) idx--;
        deleteKey(node->children[idx], key);
    }
}

// Now define deleteFromNonLeaf (after deleteKey) 
void deleteFromNonLeaf(Node* node, int idx){
    int key = node->keys[idx];

    if (node->children[idx]->count >= 2){
        Node* pred = node->children[idx];
        while (!pred->leaf) pred = pred->children[pred->count];
        int predKey = pred->keys[pred->count - 1];
        node->keys[idx] = predKey;
        deleteKey(node->children[idx], predKey);
    }
    else if (node->children[idx + 1]->count >= 2){
        Node* succ = node->children[idx + 1];
        while (!succ->leaf) succ = succ->children[0];
        int succKey = succ->keys[0];
        node->keys[idx] = succKey;
        deleteKey(node->children[idx + 1], succKey);
    }
    else{
        merge(node, idx);
        deleteKey(node->children[idx], key);
    }
}
// final delete wrapper fxn
Node* deleteNode(Node* root, int key, bool& deleted, bool& notPresent){
    if (!root){
        notPresent = true;
        return root;
    }

    if (!search(root, key)){
        notPresent = true;
        return root;
    }

    if (root->count == 1 && !root->leaf){
        Node* left = root->children[0];
        Node* right = root->children[1];
        if (left->count == 1 && right->count == 1){
            merge(root, 0);
            delete root;
            root = left;
        }
    }

    deleteKey(root, key);
    deleted = true;

    if (root->count == 0 && !root->leaf){
        Node* temp = root;
        root = root->children[0];
        delete temp;
    }

    return root;
}

void printLevelOrder(Node* root){
    if (!root) return;
    queue<Node*> q;
    q.push(root);
    while (!q.empty()){
        int sz = q.size();
        bool firstInLine = true;
        for (int i = 0; i < sz; i++){
            Node* cur = q.front(); q.pop();
            for (int j = 0; j < cur->count; j++){
                if (!firstInLine) cout << " ";
                cout << cur->keys[j];
                firstInLine = false;
            }
            for (int j = 0; j <= cur->count; j++){
                if (cur->children[j]) q.push(cur->children[j]);
            }
        }
        cout << endl;
    }
}

int main(){
    int n;
    if (!(cin >> n)) return 0;
    Node* root = NULL;
    for (int i = 0; i < n; i++){
        int x; cin >> x;
        bool ins = false, dup = false;
        root = insert(root, x, ins, dup);
    }
    int type;
    while (cin >> type){
        if (type == 1){
            int val; cin >> val;
            cout << val << (search(root, val) ? " present" : " not present") << endl;
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
