#include <iostream>
#include <vector>
using namespace std;

// 🌟 Our AVL Tree Node Structure 🌟
struct node {
    int data;
    int height;
    node* left;
    node* right;
    
    // Constructor to make life easier 😊
    node(int val) {
        data = val;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

// Global counters for rotations 🎉
int single_rotations = 0;
int double_rotations = 0;

// Helper functions we'll need 🚀
int getHeight(node* root) {
    if (root == nullptr) return 0;
    return root->height;
}

int getBalance(node* root) {
    if (root == nullptr) return 0;
    return getHeight(root->left) - getHeight(root->right);
}

void updateHeight(node* root) {
    if (root == nullptr) return;
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
}

// 🌟 RIGHT ROTATION (for LL case) 🌟
node* rightRotate(node* y) {
    node* x = y->left;
    node* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    updateHeight(y);
    updateHeight(x);
    
    single_rotations++; // 🎉 Yay! Single rotation!
    return x;
}

// 🌟 LEFT ROTATION (for RR case) 🌟
node* leftRotate(node* x) {
    node* y = x->right;
    node* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    updateHeight(x);
    updateHeight(y);
    
    single_rotations++; // 🎉 Another single rotation!
    return y;
}

// 🎭 DOUBLE ROTATION: Left then Right (for LR case)
node* leftRightRotate(node* root) {
    root->left = leftRotate(root->left);
    double_rotations++; // 🎉 Double rotation starts!
    return rightRotate(root);
}

// 🎭 DOUBLE ROTATION: Right then Left (for RL case)
node* rightLeftRotate(node* root) {
    root->right = rightRotate(root->right);
    double_rotations++; // 🎉 Double rotation!
    return leftRotate(root);
}

// 🌟 SEARCH FUNCTION 🌟
node* search(node* root, int key) {
    if (root == nullptr) {
        cout << "Error: Key " << key << " not found!" << endl;
        return nullptr;
    }
    if (root->data == key) {
        return root;
    }
    if (key < root->data) {
        return search(root->left, key);
    } else {
        return search(root->right, key);
    }
}

// 🌟 INSERT FUNCTION 🌟
node* insert(node* root, int key) {
    // Step 1: Normal BST insert
    if (root == nullptr) {
        return new node(key);
    }
    
    if (key < root->data) {
        root->left = insert(root->left, key);
    } else if (key > root->data) {
        root->right = insert(root->right, key);
    } else {
        // Duplicate key? Let's ignore it (as per standard AVL)
        return root;
    }
    
    // Step 2: Update height
    updateHeight(root);
    
    // Step 3: Get balance factor
    int balance = getBalance(root);
    
    // Step 4: Check for imbalance and rotate!
    
    // Left Left Case (LL) - Single Right Rotation
    if (balance > 1 && key < root->left->data) {
        return rightRotate(root);
    }
    
    // Right Right Case (RR) - Single Left Rotation
    if (balance < -1 && key > root->right->data) {
        return leftRotate(root);
    }
    
    // Left Right Case (LR) - Double Rotation
    if (balance > 1 && key > root->left->data) {
        return leftRightRotate(root);
    }
    
    // Right Left Case (RL) - Double Rotation
    if (balance < -1 && key < root->right->data) {
        return rightLeftRotate(root);
    }
    
    return root; // No rotation needed
}

// 🌟 FIND MINIMUM NODE (for deletion) 🌟
node* minValueNode(node* root) {
    node* current = root;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// 🌟 DELETE FUNCTION 🌟
node* deleteNode(node* root, int key) {
    // Step 1: Normal BST delete
    if (root == nullptr) {
        return root;
    }
    
    if (key < root->data) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    } else {
        // Node to be deleted found!
        
        // Case 1: Leaf node
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return nullptr;
        }
        // Case 2: One child
        else if (root->left == nullptr) {
            node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            node* temp = root->left;
            delete root;
            return temp;
        }
        // Case 3: Two children
        else {
            node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data);
        }
    }
    
    // Step 2: Update height
    updateHeight(root);
    
    // Step 3: Get balance factor
    int balance = getBalance(root);
    
    // Step 4: Rebalance if needed
    
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0) {
        return rightRotate(root);
    }
    
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        return leftRightRotate(root);
    }
    
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0) {
        return leftRotate(root);
    }
    
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        return rightLeftRotate(root);
    }
    
    return root;
}

// 🌟 IN-ORDER TRAVERSAL (to collect keys) 🌟
void inorder(vector<int>& result, node* root) {
    if (root == nullptr) return;
    inorder(result, root->left);
    result.push_back(root->data);
    inorder(result, root->right);
}

// 🎉 MAIN FUNCTION - LET'S DO THIS! 🎉
int main() {
    int N;
    cin >> N;
    
    node* root = nullptr;
    
    // Process N operations
    for (int i = 0; i < N; i++) {
        char op;
        int x;
        cin >> op >> x;
        
        if (op == 'I') {
            root = insert(root, x);
        } else if (op == 'D') {
            // Check if key exists before deleting
            if (search(root, x) != nullptr) {
                root = deleteNode(root, x);
            }
            // If not found, search already printed error
        }
    }
    
    // 🌟 Collect inorder traversal
    vector<int> inorder_list;
    inorder(inorder_list, root);
    
    // 🌟 OUTPUT AS REQUIRED! 🎯
    cout << inorder_list.size() << endl;
    cout << "Inorder:";
    for (int i = 0; i < inorder_list.size(); i++) {
        cout << " " << inorder_list[i];
    }
    cout << endl;
    cout << "Single rotations: " << single_rotations << endl;
    cout << "Double rotations: " << double_rotations << endl;
    
    return 0;
}
