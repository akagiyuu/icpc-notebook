// FULLY PERSISTENT DSU - Time: O(log^2 N) | Space: 2N + 2Q*log2(N) nodes
// p[u] < 0: u is root, size = -p[u] | p[u] > 0: parent of u
// Usage: ver[0] = build(1, n);
// - Revert to k : ver[i] = ver[k];
// - Merge at k  : ver[i] = unite(ver[k], u, v);
// - Same comp?  : find(ver[k], u).first == find(ver[k], v).first
// - Comp size   : -find(ver[k], u).second
const int MAX_NODES = 1e7 + 5; // N, Q <= 2e5 -> ~9e6 nodes (~110MB)
struct Node { int l, r, val; } tr[MAX_NODES];
int n_nodes = 0, n, ver[200005];

int build(int l, int r) {
    int id = ++n_nodes;
    if (l == r) return tr[id].val = -1, id;
    int m = (l + r) >> 1;
    return tr[id] = {build(l, m), build(m + 1, r), 0}, id;
}
int upd(int old, int l, int r, int p, int v) {
    int id = ++n_nodes; tr[id] = tr[old];
    if (l == r) return tr[id].val = v, id;
    int m = (l + r) >> 1;
    if (p <= m) tr[id].l = upd(tr[old].l, l, m, p, v);
    else tr[id].r = upd(tr[old].r, m + 1, r, p, v);
    return id;
}
int get(int id, int l, int r, int p) {
    while (l < r) {
        int m = (l + r) >> 1;
        if (p <= m) id = tr[id].l, r = m;
        else id = tr[id].r, l = m + 1;
    }
    return tr[id].val;
}
pair<int, int> find(int rt, int x) { // returns {root, -size}
    int px = get(rt, 1, n, x);
    return px < 0 ? make_pair(x, px) : find(rt, px);
}
int unite(int rt, int a, int b) {
    auto [ra, sa] = find(rt, a); auto [rb, sb] = find(rt, b);
    if (ra == rb) return rt;
    if (sa > sb) swap(ra, rb), swap(sa, sb); // note: sa, sb < 0
    rt = upd(rt, 1, n, ra, sa + sb);
    return upd(rt, 1, n, rb, ra);
}