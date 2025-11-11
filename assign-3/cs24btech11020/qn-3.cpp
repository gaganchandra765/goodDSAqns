#include <bits/stdc++.h>
using namespace std;

const int t = 2;                  
const int MAX = 2 * t - 1;           
const int MIN = t - 1;             

// B-tree node
struct BTreeNode{
    int key[MAX];                   
    int count;                      
    BTreeNode* children[MAX + 1];   
    bool leaf;                      
    // consructor 
    BTreeNode(){
        count = 0;
        leaf = true;
        for (int i = 0; i <= MAX; i++) children[i] = NULL;
    }
};

BTreeNode* root = NULL;
// Search key
bool search(BTreeNode* node, int key){
    if (!node) return false;
    int i = 0;
    while (i < node->count && key > node->key[i]) i++;
    if (i < node->count && key == node->key[i]) return true;
    if (node->leaf) return false;
    return search(node->children[i], key);
}

// Split full childd
void splitChild(BTreeNode* parent, int idx){
    BTreeNode* full = parent->children[idx];
    BTreeNode* newNode = new BTreeNode();
    newNode->leaf = full->leaf;
    newNode->count = MIN;  // t-1 keys
    
    // move right half to new node
    for (int j = 0; j < MIN; j++)
        newNode->key[j] = full->key[j + t];
    if (!full->leaf){
        for (int j = 0; j < t; j++)
            newNode->children[j] = full->children[j + t];
    }
    full->count = MIN;  // left half

    // shifting parents childrens
    for (int j = parent->count; j >= idx + 1; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[idx + 1] = newNode;

    
      for (int j = parent->count - 1; j >= idx; j--)
        parent->key[j + 1] = parent->key[j];
    parent->key[idx] = full->key[MIN];  
    parent->count++;
}

// iserting into non-full node
void insertNonFull(BTreeNode* node, int key){
    int i = node->count - 1;
    if (node->leaf){
        while (i >= 0 && key < node->key[i]){
            node->key[i + 1] = node->key[i];
            i--;
        }
        node->key[i + 1] = key;
        node->count++;
    } else{
        while (i >= 0 && key < node->key[i]) i--;
        i++;
        if (node->children[i]->count == MAX){
            splitChild(node, i);
            if (key > node->key[i]) i++;
        }
        insertNonFull(node->children[i], key);
    }
}

BTreeNode* insert(BTreeNode* r, int key, bool& inserted, bool& duplicate){
    if (search(r, key)){
        duplicate = true;
        return r;
    }

    if (!r){
        r = new BTreeNode();
        r->leaf = true;
        r->key[0] = key;
        r->count = 1;
        inserted = true;
        return r;
    }

    if (r->count == MAX){
        BTreeNode* newRoot = new BTreeNode();
        newRoot->leaf = false;
        newRoot->children[0] = r;
        splitChild(newRoot, 0);
        int i = 0;
        if (key > newRoot->key[0]) i++;
        insertNonFull(newRoot->children[i], key);
        inserted = true;
        return newRoot;
    } else{
        insertNonFull(r, key);
        inserted = true;
        return r;
    }
}

// Merge children (for delete)
void merge(BTreeNode* parent, int idx){
    BTreeNode* left = parent->children[idx];
    BTreeNode* right = parent->children[idx + 1];
    left->key[left->count] = parent->key[idx];
    left->count++;
    for (int j = 0; j < right->count; j++)
        left->key[left->count + j] = right->key[j];
    left->count += right->count;
    if (!left->leaf){
        for (int j = 0; j <= right->count; j++)
            left->children[left->count + j - 1] = right->children[j];
    }
    for (int j = idx + 1; j < parent->count; j++)
        parent->key[j - 1] = parent->key[j];
    for (int j = idx + 2; j <= parent->count; j++)
        parent->children[j - 1] = parent->children[j];

    parent->count--;
    delete right;
}

// borrowing from sibling node
void borrowFromLeft(BTreeNode* parent, int idx){
    BTreeNode* child = parent->children[idx];
    BTreeNode* sibling = parent->children[idx - 1];
    for (int j = child->count; j > 0; j--)
        child->key[j] = child->key[j - 1];
    if (!child->leaf){
        for (int j = child->count + 1; j > 0; j--)
            child->children[j] = child->children[j - 1];
    }
    child->key[0] = parent->key[idx - 1];
    child->count++;
    parent->key[idx - 1] = sibling->key[sibling->count - 1];
    sibling->count--;
    if (!child->leaf)
        child->children[0] = sibling->children[sibling->count + 1];
}

// Borrow from right sibling
void borrowFromRight(BTreeNode* parent, int idx){
    BTreeNode* child = parent->children[idx];
    BTreeNode* sibling = parent->children[idx + 1];
    child->key[child->count] = parent->key[idx];
    child->count++;
    parent->key[idx] = sibling->key[0];
    sibling->count--;
    for (int j = 0; j < sibling->count; j++)
        sibling->key[j] = sibling->key[j + 1];
    if (!child->leaf){
        child->children[child->count] = sibling->children[0];
        for (int j = 0; j <= sibling->count; j++)
            sibling->children[j] = sibling->children[j + 1];
    }
}

// Delete from leaf
void deleteFromLeaf(BTreeNode* node, int key){
    int idx = 0;
    while (idx < node->count && node->key[idx] != key) idx++;
    for (int j = idx; j < node->count - 1; j++)
        node->key[j] = node->key[j + 1];
    node->count--;
}

// delete from non-leaf
    void deleteFromNonLeaf(BTreeNode* node, int idx);

// Main delete function
void deleteKey(BTreeNode* node, int key){
    int idx = 0;
    while (idx < node->count && key > node->key[idx]) idx++;
    if (idx < node->count && key == node->key[idx]){
        if (node->leaf)
            deleteFromLeaf(node, key);
        else
            deleteFromNonLeaf(node, idx);
    } else{
        if (node->leaf) return;

        BTreeNode* child = node->children[idx];
        bool last = (idx == node->count);
        if (child->count == MIN){
            if (idx > 0 && node->children[idx - 1]->count > MIN)
                borrowFromLeft(node, idx);
            else if (!last && node->children[idx + 1]->count > MIN)
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

void deleteFromNonLeaf(BTreeNode* node, int idx){
    int key = node->key[idx];
    if (node->children[idx]->count > MIN){
        BTreeNode* pred = node->children[idx];
        while (!pred->leaf) pred = pred->children[pred->count];
        int predKey = pred->key[pred->count - 1];
        node->key[idx] = predKey;
        deleteKey(node->children[idx], predKey);
    }
        else if (node->children[idx + 1]->count > MIN){
        BTreeNode* succ = node->children[idx + 1];
        while (!succ->leaf) succ = succ->children[0];
        int succKey = succ->key[0];
        node->key[idx] = succKey;
        deleteKey(node->children[idx + 1], succKey);
    }
    else{
        merge(node, idx);
        deleteKey(node->children[idx], key);
    }
}

// final delete wrapper fxn 
BTreeNode* deleteNode(BTreeNode* r, int key, bool& deleted, bool& notPresent){
    if (!r){
        notPresent = true;
        return r;
    }
    if (!search(r, key)){
        notPresent = true;
        return r;
    }
    if (r->count == 1 && !r->leaf){
        BTreeNode* left = r->children[0];
        BTreeNode* right = r->children[1];
        if (left->count == MIN && right->count == MIN){
            merge(r, 0);
            delete r;
            r = left;
        }
    }
    deleteKey(r, key);
    deleted = true;

    if (r->count == 0 && !r->leaf){
        BTreeNode* temp = r;
        r = r->children[0];
        delete temp;
    }

    return r;
}

// Level-order print
void printLevelOrder(BTreeNode* r){
    if (!r) return;
    queue<BTreeNode*> q;
    q.push(r);
    while (!q.empty()){
        int sz = q.size();
        bool first = true;
        for (int i = 0; i < sz; i++){
            BTreeNode* cur = q.front(); q.pop();
            for (int j = 0; j < cur->count; j++){
                if (!first) cout << " ";
                cout << cur->key[j];
                first = false;
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

root = NULL;
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
