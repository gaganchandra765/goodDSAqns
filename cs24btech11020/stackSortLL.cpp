# include <iostream>
using namespace std;

struct Node{
// stored variables
int data;
Node* next;
// constructor

Node(int val){
  data = val;
  next = nullptr;
}
};

struct linkedList{
// stored variables
Node* head;
// constrcutor
linkedList(){
  head = nullptr;
}

void insertatEnd(int val){
Node* newNode = new Node(val);
if(head==nullptr){
head = newNode;
return;
}
Node* temp = head;
while(temp->next!=nullptr){
temp = temp->next;
}
temp->next = newNode;
}

void display(){
Node* temp = head;
while(temp!=nullptr){
cout << temp->data << " -> ";
temp= temp->next;
}
cout <<"NULL"<<endl;
}

void insertHead(int value){
Node* newNode = new Node(value);
newNode->next = head;
head = newNode;
}

void insertAtPos(int value , int pos){
if (pos<=0){
cout << "Bro you dumb"<<endl;
return;
}
if (pos==1){
insertHead(value);
}
Node* newNode = new Node(value);
Node* temp = head;
for (int i=1; temp!= nullptr && i<pos-1;i++){
temp = temp->next;
}
if (temp==nullptr){
cout <<"position out of range! " <<endl;
delete newNode;
return;
}
newNode -> next = temp->next;
temp->next = newNode;
}

bool isPresent(int value){
Node* temp = head;
while (temp != nullptr){
if (temp->data == value) return true;
temp = temp->next;
}
return false;
}

int length(){
Node* temp = head;
int length=0;
while(temp!=nullptr){
length++;
temp = temp->next;
}
return length;
}

void deleteByValue(int value){
if (head==nullptr) return;
if (!isPresent(value)){
cout << "The value " << value<< " is not present in the linkedList" << endl;
return ;
}
Node* temp = head;
while((temp->next)->data!=value){
temp = temp->next;
}
temp->next = (temp->next)->next;
(temp->next)->next = nullptr;
}

void deleteByPos(int pos){
if (pos>length()|| pos<=0){
cout << "pos out of bounds , cannot delete" <<endl;
return;
}
Node* temp = head;
for (int i=1;i<pos-1;i++){
temp = temp->next;
}
temp->next = (temp->next)->next;
(temp->next)->next = nullptr;
return;
}

Node* getMid(){
if (!head){
return nullptr;
}
Node* slow = head;
Node* fast = head->next;
while(fast && fast->next){
slow = slow->next;
fast = (fast->next)->next;
}
return slow;
}

};


struct stackLL{
linkedList ll;
void push(int value) { ll.insertHead(value); }
void pop() { if (!ll.head) return; ll.head = ll.head->next; }
int peek() { return ll.head ? ll.head->data : -1; }
bool isEmpty() { return ll.head == nullptr; }
void display() { ll.display(); }
};

linkedList mergeLists(linkedList &listOne, linkedList &listTwo) {
    linkedList newList;
    Node* head1 = listOne.head;
    Node* head2 = listTwo.head;

    while (head1 && head2) {
        if (head1->data >= head2->data) { // descending
            newList.insertatEnd(head1->data);
            head1 = head1->next;
        } else {
            newList.insertatEnd(head2->data);
            head2 = head2->next;
        }
    }

    while (head1) { newList.insertatEnd(head1->data); head1 = head1->next; }
    while (head2) { newList.insertatEnd(head2->data); head2 = head2->next; }

    return newList;
}

// Recursive merge sort on linkedList
linkedList mergeSort(linkedList &list) {
    if (!list.head || !list.head->next) return list; // 0 or 1 element

    Node* mid = list.getMid();
    linkedList leftList;
    linkedList rightList;

    // Split into two lists
    leftList.head = list.head;
    rightList.head = mid->next;
    mid->next = nullptr;

    // Recursively sort both halves
    leftList = mergeSort(leftList);
    rightList = mergeSort(rightList);

    // Merge sorted halves
    return mergeLists(leftList, rightList);
}

int main(void){

int n;
    cin >> n;
    stackLL stack;

    // Input from bottom to top
    int arr[n];
    for (int i = 0; i < n; i++) cin >> arr[i];
    for (int i = 0; i < n; i++) stack.push(arr[i]);

    // Perform merge sort on stack's linked list
    stack.ll = mergeSort(stack.ll);

    // Display sorted stack (top to bottom)
    stack.display();

return 0; 
}
