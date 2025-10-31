#include <iostream>
#include <string>
using namespace std;

struct Stack{
    int arr[1000];  
    int top;
    Stack(){top = -1;}
    void push(int val){arr[++top] = val;}
    int pop(){return arr[top--];}
    int peek(){return arr[top];}
    bool empty(){return top == -1;}
};

struct Tree{
    string nodes[1000]; 
    int size;
    Tree(){size = 0;}
    void readInput(string arr[], int n){
        size = n;
        for(int i=1;i<=n;i++) nodes[i] = arr[i];
   }
    bool valid(int idx){
        return idx <= size && nodes[idx] != "NULL";
   }
};

void inorder(Tree &t){
    Stack st;
    int curr = 1;
    while(curr <= t.size || !st.empty()){
        while(t.valid(curr)){
            st.push(curr);
            curr = 2*curr; 
       }
        if(st.empty()) break;
        curr = st.pop();
        cout << t.nodes[curr] << " ";
        curr = 2*curr + 1; // upd
   }
    cout << endl;
}

void preorder(Tree &t){
    if(!t.valid(1)){cout <<endl; return;}
    Stack st;
    st.push(1);
    while(!st.empty()){
        int idx = st.pop();
        cout << t.nodes[idx] << " ";
        int right = 2*idx+1, left = 2*idx;
        if(t.valid(right)) st.push(right);
        if(t.valid(left)) st.push(left);
   }
    cout << endl;
}

void postorder(Tree &t){
    if(!t.valid(1)){cout << endl; return;}
    Stack st1, st2;
    st1.push(1);
    while(!st1.empty()){
        int idx = st1.pop();
        st2.push(idx);
        int left = 2*idx, right = 2*idx+1;
        if(t.valid(left)) st1.push(left);
        if(t.valid(right)) st1.push(right);
   }
    while(!st2.empty()){
        cout << t.nodes[st2.pop()] << " ";
   }
    cout << endl;
}

int main(){
    int k;
    cin >> k;
    cin.ignore();
    for(int tc=0; tc<k; tc++){
        string arr[1000];
        int i=1;
        string temp;
        
        string line;
        getline(cin, line);
        int pos = 0;
        string word = "";
        for(char c: line){
            if(c==' '){
                if(word!=""){
                    arr[i++] = word;
                    word="";
               }
           } else word.push_back(c);
       }
        if(word!="") arr[i++] = word;
        Tree t;
        t.readInput(arr, i-1);

        inorder(t);
        preorder(t);
        postorder(t);
   }
    return 0;
}

