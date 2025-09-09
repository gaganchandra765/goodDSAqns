#include <iostream>
#include <sstream>
#include <string>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) {
        data = val;
        next = nullptr;
    }
};

struct linkedList {
    Node* head;

    linkedList() {
        head = nullptr;
    }

    void insertRight(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            return;
        }
        Node* tempNode = head;
        while (tempNode->next != nullptr) {
            tempNode = tempNode->next;
        }
        tempNode->next = newNode;
    }

    void displayLL() {
        Node* tempNode = head;
        bool first = true;
        while (tempNode != nullptr) {
            if (!first) cout << ",";
            cout << tempNode->data;
            first = false;
            tempNode = tempNode->next;
        }
        cout << endl;
    }

    void oddFirstOrdering() {
        if (head == nullptr || head->next == nullptr) {
            return;
        }

        Node* odd = head;
        Node* even = head->next;
        Node* headEven = even;

        while (even != nullptr && even->next != nullptr) {
            odd->next = even->next;
            odd = odd->next;
            even->next = odd->next;
            even = even->next;
        }
        odd->next = headEven;
    }
};

int main() {
    linkedList ll;
    string line;
    getline(cin, line); // read full line of input

    stringstream ss(line);
    string token;

    while (getline(ss, token, ',')) {
        int num = stoi(token);
        if (num == -1) break; // stop at -1
        ll.insertRight(num);
    }

    ll.oddFirstOrdering();

    ll.displayLL();

    return 0;
}

