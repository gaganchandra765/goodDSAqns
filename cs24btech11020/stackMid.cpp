#include <iostream>
using namespace std;

struct Node{
    int data;
    Node* prev;
    Node* next;
    Node(int val){
        data = val;
        prev = nullptr;
        next = nullptr;
    }
};

struct doublyLinkedList{
    Node* head;
    doublyLinkedList(){ head = nullptr; }

    void insertHead(int value){
        Node* newNode = new Node(value);
        if(head != nullptr){
            newNode->next = head;
            head->prev = newNode;
        }
        head = newNode;
    }

    void deleteByPos(int pos){
        if(pos<=0 || head==nullptr) return;
        Node* temp = head;
        if(pos==1){
            head = head->next;
            if(head) head->prev = nullptr;
            delete temp;
            return;
        }
        for(int i=1;i<pos-1;i++) temp = temp->next;
        Node* toDelete = temp->next;
        if(!toDelete) return;
        temp->next = toDelete->next;
        if(toDelete->next) toDelete->next->prev = temp;
        delete toDelete;
    }
};

struct stackDll{
    doublyLinkedList dll;
    Node* midptr;
    int count;

    stackDll(){ midptr = nullptr; count = 0; }

    void push(int value){
        dll.insertHead(value);
        count++;
        if(count==1) midptr = dll.head;
        else if(count % 2 == 0) midptr = midptr->prev; // move mid back for lower middle
    }

    int pop(){
        if(count==0) return -1;
        int val = dll.head->data;
        dll.deleteByPos(1);
        count--;
        if(count % 2 == 1 && midptr!=nullptr) midptr = midptr->next;
        if(count==0) midptr = nullptr;
        return val;
    }

    int getMiddle(){
        if(count==0 || midptr==nullptr) return -1;
        return midptr->data;
    }

    int deleteMiddle(){
        if(count==0 || midptr==nullptr) return -1;
        int val = midptr->data;
        Node* temp = dll.head;
        int pos = 1;
        while(temp != midptr){ temp = temp->next; pos++; }
        dll.deleteByPos(pos);
        count--;
        if(count==0) midptr = nullptr;
        else if(count % 2 == 0) midptr = midptr->prev ? midptr->prev : dll.head;
        else midptr = midptr->next ? midptr->next : dll.head;
        return val;
    }
};

int main(){
    stackDll st;
    int Q;
    cin >> Q;
    while(Q--){
        int op;
        cin >> op;
        if(op==1){
            int x; cin >> x;
            st.push(x);
        } else if(op==2){
            int val = st.pop();
            if(val==-1) cout << "Stack Underflow\n";
            else cout << val << "\n";
        } else if(op==3){
            int val = st.getMiddle();
            if(val==-1) cout << "Stack Empty\n";
            else cout << val << "\n";
        } else if(op==4){
            int val = st.deleteMiddle();
            if(val==-1) cout << "Stack Empty\n";
            else cout << val << "\n";
        }
    }
    return 0;
}

