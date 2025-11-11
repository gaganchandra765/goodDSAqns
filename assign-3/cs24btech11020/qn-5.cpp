#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n; 
    set<int> tree;
    for (int i = 0; i < n; i++){
        int num;
        cin >> num;
        tree.insert(num); 
    }
    int a, b;
    cin >> a >> b;
    auto it = tree.lower_bound(a);
    // printing all numbers from 'it' until we pass 'b'
    bool first = true; 
    while (it != tree.end() && *it <= b){
        if (!first){
            cout << " ";
        }
        cout << *it;   
        first = false;
        ++it;        
    }

    cout << endl; // new line at the end
    return 0;
}
