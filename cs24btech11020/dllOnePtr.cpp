#include <iostream>
# include <cstdint>
using namespace std;

struct Node {
    int data;
    Node* np;   // XOR of prev and next
    Node(int val) {
        data = val;
        np = nullptr;
    }
};

struct XORLinkedList {
    Node* head;
    Node* tail;

    XORLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    // XOR function for 2 nodes as input 
    Node* XOR(Node* a, Node* b) {
        return (Node*)((uintptr_t)(a) ^ (uintptr_t)(b));
    }

    // Insert at head (so newly inserted element appears first)
    void insertHead(int value) {
        Node* newNode = new Node(value);
        newNode->np = XOR(nullptr, head);
        if (head != nullptr) {
            Node* next = XOR(nullptr, head->np);
            head->np = XOR(newNode, next);
        } else {
            tail = newNode; // first node also becomes tail
        }
        head = newNode;
    }

    // Delete node with value
    bool deleteValue(int value) {
        Node* curr = head;
        Node* prev = nullptr;
        Node* next;

        while (curr != nullptr) {
            if (curr->data == value) {
                // Found node to delete
                next = XOR(prev, curr->np);

                if (prev != nullptr) {
                    Node* prevPrev = XOR(curr, prev->np);
                    prev->np = XOR(prevPrev, next);
                } else {
                    head = next; // deleting head
                }

                if (next != nullptr) {
                    Node* nextNext = XOR(curr, next->np);
                    next->np = XOR(prev, nextNext);
                } else {
                    tail = prev; // deleting tail
                }

                delete curr;
                return true;
            }
            next = XOR(prev, curr->np);
            prev = curr;
            curr = next;
        }
        return false; // value not found
    }

    // Display list from head
    void display() {
        Node* curr = head;
        Node* prev = nullptr;
        Node* next;
        cout << "Our dll till now : "<<endl;
        if (curr == nullptr) {
            cout << "NULL" << endl;
            return;
        }
        while (curr != nullptr) {
            cout << curr->data << " ";
            next = XOR(prev, curr->np);
            prev = curr;
            curr = next;
        }
        cout << endl;
    }

    // Search for value
    bool isPresent(int value) {
        Node* curr = head;
        Node* prev = nullptr;
        while (curr != nullptr) {
            if (curr->data == value) return true;
            Node* next = XOR(prev, curr->np);
            prev = curr;
            curr = next;
        }
        return false;
    }

    // Reversing - just interhchange the pointers 
    void reverseList() {
        Node* temp = head;
        head = tail;
        tail = temp;
    }
};

int main() {
    int k;
    cin >> k;
    XORLinkedList dll;

    while (k--) {
        int op, x;
        cin >> op;
        switch (op) {
            case 1: // Search
                cin >> x;
                if (dll.isPresent(x))
                    cout << "Element is present" << endl;
                else
                    cout << "Element is not present" << endl;
                break;
            case 2: // Insert
                cin >> x;
                dll.insertHead(x);
                dll.display();
                break;
            case 3: // Delete
                cin >> x;
                if (dll.deleteValue(x))
                    dll.display();
                else
                    cout << "Element is not present" << endl;
                break;
            case 4: // Reverse
                dll.reverseList();
                dll.display();
                break;
            default:
                cout << "Invalid operation" << endl;
        }
    }
    return 0;
}

