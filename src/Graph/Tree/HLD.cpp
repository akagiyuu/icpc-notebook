#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SegTree {
    int n;
    vector<ll> st, lz;
    SegTree() { n = 0; }
    SegTree(int _n) { init(_n); }
    void init(int _n) {
        n = max(1, _n);
        st.assign(4*n + 5, 0);
        lz.assign(4*n + 5, 0);
    }
    void build(int p, int l, int r, const vector<ll> &base) {
        if (l == r) {
            st[p] = base[l];
            return;
        }
        int m = (l + r) >> 1;
        build(p<<1, l, m, base);
        build(p<<1|1, m+1, r, base);
        st[p] = st[p<<1] + st[p<<1|1];
    }
    void build(const vector<ll> &base) {
        init((int)base.size()-1);
        build(1, 1, n, base);
    }
    inline void apply(int p, int l, int r, ll val) {
        st[p] += val * (r - l + 1);
        lz[p] += val;
    }
    inline void push(int p, int l, int r) {
        if (lz[p] != 0 && l != r) {
            int m = (l + r) >> 1;
            apply(p<<1, l, m, lz[p]);
            apply(p<<1|1, m+1, r, lz[p]);
            lz[p] = 0;
        }
    }
    void update(int p, int l, int r, int ql, int qr, ll val) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            apply(p, l, r, val);
            return;
        }
        push(p, l, r);
        int m = (l + r) >> 1;
        update(p<<1, l, m, ql, qr, val);
        update(p<<1|1, m+1, r, ql, qr, val);
        st[p] = st[p<<1] + st[p<<1|1];
    }
    ll query(int p, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return st[p];
        push(p, l, r);
        int m = (l + r) >> 1;
        return query(p<<1, l, m, ql, qr) + query(p<<1|1, m+1, r, ql, qr);
    }

    // wrappers (1-indexed positions)
    void range_add(int l, int r, ll val) {
        if (l > r) return;
        update(1, 1, n, l, r, val);
    }
    ll range_sum(int l, int r) {
        if (l > r) return 0;
        return query(1, 1, n, l, r);
    }
};

struct HLD {
    int n;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, sz;
    int cur_pos;
    SegTree seg;

    HLD() : n(0), cur_pos(1) {}
    HLD(int _n) { init(_n); }

    void init(int _n) {
        n = _n;
        adj.assign(n+1, {});
        parent.assign(n+1, 0);
        depth.assign(n+1, 0);
        heavy.assign(n+1, -1);
        head.assign(n+1, 0);
        pos.assign(n+1, 0);
        sz.assign(n+1, 0);
        cur_pos = 1;
    }
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 1) DFS to compute sizes and heavy child
    int dfs(int u, int p) {
        parent[u] = p;
        sz[u] = 1;
        int maxsz = 0;
        for (int v : adj[u]) if (v != p) {
            depth[v] = depth[u] + 1;
            int subsz = dfs(v, u);
            if (subsz > maxsz) {
                maxsz = subsz;
                heavy[u] = v;
            }
            sz[u] += subsz;
        }
        return sz[u];
    }

    // 2) Decompose: assign head and pos
    void decompose(int u, int h) {
        head[u] = h;
        pos[u] = cur_pos++;
        if (heavy[u] != -1) decompose(heavy[u], h);
        for (int v : adj[u]) if (v != parent[u] && v != heavy[u]) {
            decompose(v, v);
        }
    }

    // Public build call
    void build(int root = 1) {
        depth[root] = 0;
        dfs(root, 0);
        cur_pos = 1;
        decompose(root, root);
    }

    // LCA via HLD
    int lca(int a, int b) const {
        while (head[a] != head[b]) {
            if (depth[head[a]] > depth[head[b]]) a = parent[head[a]];
            else b = parent[head[b]];
        }
        return (depth[a] < depth[b]) ? a : b;
    }

    // Build segment tree from node values (1-indexed vector of size n+1)
    void buildSeg(const vector<ll> &nodeValues) {
        vector<ll> base(n+1, 0);
        for (int u = 1; u <= n; ++u) base[pos[u]] = nodeValues[u];
        seg.build(base);
    }
    
    void buildSegFromEdges(const vector<tuple<int,int,ll>> &edges) {
        vector<ll> base(n+1, 0);
        for (auto &e : edges) {
            int u,v; ll w; tie(u,v,w) = e;
            // deeper node stores the edge value
            if (parent[u] == v) base[pos[u]] = w;
            else if (parent[v] == u) base[pos[v]] = w;
            else {
                // if tree rooting not done yet, throw or ignore. It's expected that build(root) was called.
            }
        }
        seg.build(base);
    }

    void path_update(int u, int v, ll val, bool isEdge = false) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            seg.range_add(pos[head[u]], pos[u], val);
            u = parent[head[u]];
        }
        // now same head
        if (depth[u] < depth[v]) swap(u, v);
        // now u is deeper
        int L = pos[v];
        int R = pos[u];
        if (isEdge) L = pos[v] + 1; // exclude LCA node
        if (L <= R) seg.range_add(L, R, val);
    }

    ll path_query(int u, int v, bool isEdge = false) {
        ll res = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            res += seg.range_sum(pos[head[u]], pos[u]);
            u = parent[head[u]];
        }
        if (depth[u] < depth[v]) swap(u, v);
        int L = pos[v];
        int R = pos[u];
        if (isEdge) L = pos[v] + 1;
        if (L <= R) res += seg.range_sum(L, R);
        return res;
    }

    void subtree_update(int u, ll val) {
        seg.range_add(pos[u], pos[u] + sz[u] - 1, val);
    }
    ll subtree_query(int u) {
        return seg.range_sum(pos[u], pos[u] + sz[u] - 1);
    }

    void point_add(int u, ll val) { seg.range_add(pos[u], pos[u], val); }
    ll point_query(int u) { return seg.range_sum(pos[u], pos[u]); }

    void edge_add(int u, int v, ll val) {
        if (parent[u] == v) seg.range_add(pos[u], pos[u], val);
        else if (parent[v] == u) seg.range_add(pos[v], pos[v], val);
        else {
            path_update(u, v, val, true);
        }
    }

    ll edge_query(int u, int v) { return path_query(u, v, true); }
};

