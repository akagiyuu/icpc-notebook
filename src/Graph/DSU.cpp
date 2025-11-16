#include <bits/stdc++.h>
using namespace std;

struct DSURollback {
    int n;
    vector<int> parent, sz;
    int comps;                     // number of connected components
    vector<pair<int,int>> ops;     // stack of operations: (a, prev_sz_b)

    void init(int _n) {
        n = _n;
        parent.resize(n+1);
        sz.assign(n+1, 1);
        for (int i = 1; i <= n; ++i) parent[i] = i;
        comps = n;
        ops.clear();
    }

    int find(int x) const {
        while (parent[x] != x) x = parent[x];
        return x;
    }

    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) {
            ops.emplace_back(-1, -1); // marker: nothing changed
            return false;
        }
        if (sz[a] > sz[b]) swap(a, b); // ensure a is smaller
        ops.emplace_back(a, sz[b]);
        parent[a] = b;
        sz[b] += sz[a];
        --comps;
        return true;
    }

    size_t snapshot() const { return ops.size(); }

    void rollback(size_t snap) {
        while (ops.size() > snap) {
            auto pr = ops.back(); ops.pop_back();
            int a = pr.first;
            if (a == -1) {
                continue;
            }
            int prev_sz_b = pr.second;
            int b = parent[a];        // currently a's parent is b (we're rolling back in reverse order)
            sz[b] = prev_sz_b;        // restore size of b
            parent[a] = a;            // restore parent of a
            ++comps;
        }
    }

    void rollback_one() {
        if (ops.empty()) return;
        rollback(ops.size()-1);
    }

    int components() const { return comps; }
};

