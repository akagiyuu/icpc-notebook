struct PersistentSeg {
    struct Node {
        ll sum;
        int l, r;
        Node(): sum(0), l(0), r(0) {}
    };
    vector<Node> seg;
    int n; // array size (1..n)

    PersistentSeg(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        seg.clear();
        seg.reserve(1 << 20); // recommend to reserve approximate size: (n + updates*logn)
        seg.emplace_back();   // index 0 is null node with sum=0
    }
    int newNode() {
        seg.emplace_back();
        return (int)seg.size() - 1;
    }
    // build from 1..n (arr 1-indexed). returns root index.
    int build(const vector<ll> &arr) {
        function<int(int,int)> build_rec = [&](int l, int r)->int {
            int id = newNode();
            if (l == r) {
                seg[id].sum = arr[l];
                return id;
            }
            int m = (l + r) >> 1;
            seg[id].l = build_rec(l, m);
            seg[id].r = build_rec(m+1, r);
            seg[id].sum = seg[seg[id].l].sum + seg[seg[id].r].sum;
            return id;
        };
        if (n <= 0) return 0;
        return build_rec(1, n);
    }
    // point add: create a new version from prev_root, adding 'delta' at position pos
    int update_add(int prev_root, int pos, ll delta) {
        function<int(int,int,int)> upd = [&](int prev, int l, int r)->int {
            int id = newNode();
            seg[id] = seg[prev]; // copy
            if (l == r) {
                seg[id].sum += delta;
                return id;
            }
            int m = (l + r) >> 1;
            if (pos <= m) seg[id].l = upd(seg[prev].l, l, m);
            else seg[id].r = upd(seg[prev].r, m+1, r);
            seg[id].sum = seg[seg[id].l].sum + seg[seg[id].r].sum;
            return id;
        };
        return upd(prev_root, 1, n);
    }
    // point assign: set position pos to value 'val' (we implement by reading old val via query if needed)
    int update_set(int prev_root, int pos, ll val) {
        ll cur = query_point(prev_root, pos);
        return update_add(prev_root, pos, val - cur);
    }
    // query sum on version 'root' in [ql, qr]
    ll query(int root, int ql, int qr) const {
        function<ll(int,int,int)> qry = [&](int id, int l, int r)->ll {
            if (id == 0 || ql > r || qr < l) return 0LL;
            if (ql <= l && r <= qr) return seg[id].sum;
            int m = (l + r) >> 1;
            return qry(seg[id].l, l, m) + qry(seg[id].r, m+1, r);
        };
        if (root == 0) return 0;
        return qry(root, 1, n);
    }
    // query single point value
    ll query_point(int root, int pos) const {
        function<ll(int,int,int)> qp = [&](int id, int l, int r)->ll {
            if (id == 0) return 0LL;
            if (l == r) return seg[id].sum;
            int m = (l + r) >> 1;
            if (pos <= m) return qp(seg[id].l, l, m);
            else return qp(seg[id].r, m+1, r);
        };
        return qp(root, 1, n);
    }
    // kth: find smallest index pos such that prefix_sum(pos) >= k (1-indexed)
    // requires that tree stores **non-negative counts** (e.g., frequency tree)
    int kth(int root, ll k) const {
        if (root == 0 || seg[root].sum < k) return -1;
        int id = root;
        int l = 1, r = n;
        while (l < r) {
            int m = (l + r) >> 1;
            ll leftSum = seg[seg[id].l].sum;
            if (k <= leftSum) {
                id = seg[id].l;
                r = m;
            } else {
                k -= leftSum;
                id = seg[id].r;
                l = m + 1;
            }
        }
        return l;
    }
};
