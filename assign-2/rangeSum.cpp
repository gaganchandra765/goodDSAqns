#include <iostream>
#include <string>
using namespace std;

struct Tree{
    string nodes[1000]; 
    int size;
    Tree(){size = 0;}
    bool valid(int idx){return idx <= size && nodes[idx] != "" && nodes[idx] != "null" && nodes[idx] != "NULL";}
};

int rangeSum(Tree &t, int idx, int low, int high){
    if(!t.valid(idx)) return 0;
    int val = stoi(t.nodes[idx]);
    int sum = 0;
    if(val >= low) sum += rangeSum(t, 2*idx, low, high); 
    if(val <= high) sum += rangeSum(t, 2*idx+1, low, high);
    if(val >= low && val <= high) sum += val; 
    return sum;
}
int main(){
    Tree t;
    for(int i=0;i<1000;i++) t.nodes[i] = "";
    string s;
    int idx = 1;
    while(cin >> s){
        if(s == "\n") break;
        t.nodes[idx++] = s;
        if(cin.peek() == '\n') break;
   }
    t.size = idx - 1;
    int low, high;
    cin >> low >> high;
    int ans = rangeSum(t, 1, low, high);
    cout << ans << endl;
    return 0;
}

