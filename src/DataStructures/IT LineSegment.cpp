struct Line {
    int a, b; // y = ax + b;

    ll eval(ll x) {
        return a * x + b;
    }
};

Line st[4 * maxn];

int n;
int x[maxn], w[maxn], e[maxn];
int d[maxn];

void update(int id, int l, int r, int u, int v, Line seg) {
    if (l > v || r < u) return;
    int mid = (l + r) >> 1;
    int lf = (id * 2), rt = (id * 2 + 1);
    if (l >= u && r <= v) {
        int lpos = d[l], rpos = d[r], midpos = d[mid];
        if (st[id].eval(lpos) > seg.eval(lpos) && st[id].eval(rpos) > seg.eval(rpos)) {
            return;
        }
        if (st[id].eval(lpos) <= seg.eval(lpos) && st[id].eval(rpos) <= seg.eval(rpos)) {
            st[id] = seg;
            return;
        }
        if (st[id].eval(lpos) > seg.eval(lpos) && st[id].eval(midpos) > seg.eval(midpos)) {
            update(rt, mid + 1, r, u, v, seg);
            return;
        }
        if (st[id].eval(lpos) <= seg.eval(lpos) && st[id].eval(midpos) <= seg.eval(midpos)) {
            update(rt, mid + 1, r, u, v, st[id]);
            st[id] = seg;
            return;
        }
        if (st[id].eval(midpos) > seg.eval(midpos) && st[id].eval(rpos) > seg.eval(rpos)) {
            update(lf, l, mid, u, v, seg);
            return;
        }
        if (st[id].eval(midpos) <= seg.eval(midpos) && st[id].eval(rpos) <= seg.eval(rpos)) {
            update(lf, l, mid, u, v, st[id]);
            st[id] = seg;
            return;
        }
    }
    update(lf, l, mid, u, v, seg);
    update(rt, mid + 1, r, u, v, seg);
}

ll get(int id, int l, int r, int pos) {
    if (l > pos || r < pos) return -oo;
    if (l == r) {
        return st[id].eval(d[pos]);
    }
    int mid = (l + r) >> 1;
    ll res = st[id].eval(d[pos]);
    res = max(res, max(get(id * 2, l, mid, pos), get(id * 2 + 1, mid + 1, r, pos)));
    return res;
}
