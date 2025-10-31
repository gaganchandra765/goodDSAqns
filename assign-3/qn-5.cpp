#include <bits/stdc++.h>
using namespace std;

/*
Demo of RB-ENUMERATE using std::set (red-black tree).
Input format (simple demo):
n
k1 k2 ... kn
a b

Output: all keys in [a,b] in ascending order (space separated), followed by newline.

This demonstrates the exact algorithm: start = lower_bound(a) (O(log n)),
then iterate using ++it until value > b (total O(m + log n)).
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    set<int> T;
    for (int i = 0; i < n; ++i) {
        int v; cin >> v;
        T.insert(v); // std::set ignores duplicates automatically
    }

    long long a, b;
    if (!(cin >> a >> b)) return 0;

    auto it = T.lower_bound((int)a); // first element >= a; O(log n)
    bool first = true;
    while (it != T.end() && *it <= b) { // iterate at most m steps
        if (!first) cout << ' ';
        cout << *it;
        first = false;
        ++it;
    }
    cout << '\n';
    return 0;
}

