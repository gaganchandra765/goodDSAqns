#include <iostream>
#include <string>
using namespace std;

struct Tree{
    string nodes[1000];
    int size;
    Tree(){size = 0;}
    void printArray(){
    int maxIdx = 1;
    for(int i = 1; i < 1000; i++){
        if(nodes[i] != "" && nodes[i] != "NULL") maxIdx = i;
 }
    for(int i = 1; i <= maxIdx; i++){
        if(nodes[i] == "" || nodes[i] == "NULL") cout << "NULL ";
        else cout << nodes[i] << " ";
 }    cout << endl;}
};
bool buildTree(string inorder[], string preorder[], int inStart, int inEnd, int preStart, int preEnd,
               string nodes[], int pos){
    if(inStart > inEnd) return true; 
    if(preStart > preEnd) return false; 
    string root = preorder[preStart];
    int rootIndex = -1;
    for(int i=inStart;i<=inEnd;i++){
        if(inorder[i] == root){
            rootIndex = i;
            break;
     }
 }
    if(rootIndex == -1) return false; 
    nodes[pos] = root; 
    int leftSize = rootIndex - inStart;
    int rightSize = inEnd - rootIndex;
    if(leftSize > 0){
        if(!buildTree(inorder, preorder, inStart, rootIndex-1, preStart+1, preStart+leftSize, nodes, 2*pos))
            return false;
 }else nodes[2*pos] = "NULL";
    if(rightSize > 0){
        if(!buildTree(inorder, preorder, rootIndex+1, inEnd, preStart+leftSize+1, preEnd, nodes, 2*pos+1))
            return false;
 }else nodes[2*pos+1] = "NULL";
    return true;
}


int main(){
    int k;
    cin >> k;
    cin.ignore();
    for(int idx=0;idx<k;idx++){
        string inorder[1000], preorder[1000];
        string line;
        getline(cin, line);
        int i=0;
        string word="";
        for(char c: line){
            if(c==' '){
                if(word!=""){inorder[i++] = word; word="";}
         }else word.push_back(c);
     }
        if(word!="") inorder[i++] = word;
        int n = i;
        getline(cin, line);
        i=0; word="";
        for(char c: line){
            if(c==' '){
                if(word!=""){preorder[i++] = word; word="";}
         }else word.push_back(c);
     }
        if(word!="") preorder[i++] = word;
        Tree t;
        for(int j=0;j<1000;j++) t.nodes[j] = "";
        t.size = 0;
        bool ok = buildTree(inorder, preorder, 0, n-1, 0, n-1, t.nodes, 1);
        if(!ok) cout << "ERROR"<<endl;
        else{
            int maxIdx=1;
            for(int j=1;j<1000;j++){
                if(t.nodes[j] != "") maxIdx=j;
         }
            t.size = maxIdx;
            t.printArray();
     }}
    return 0;
}

