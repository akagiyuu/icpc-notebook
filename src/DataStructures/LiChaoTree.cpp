struct Line {
    long long a, b;
    Line(long long _a = 0, long long _b = LLONG_MAX) : a(_a), b(_b) {}
    long long get(long long x) const { return a * x + b; }
};

struct LiChaoTree {
    struct Node {
        Line line;
        Node *l = nullptr, *r = nullptr;
    };
    Node *root = nullptr;
    long long minX, maxX;
    bool isMax;  // false = query min, true = query max

    LiChaoTree(long long _minX, long long _maxX, bool _isMax = false)
        : minX(_minX), maxX(_maxX), isMax(_isMax) {}

    inline long long better(long long y1, long long y2) const {
        return isMax ? max(y1, y2) : min(y1, y2);
    }

    void add_line(Line nw) { add_line(root, minX, maxX, nw); }

    void add_line(Node *&node, long long l, long long r, Line nw) {
        if (!node) {
            node = new Node();
            node->line = nw;
            return;
        }
        long long mid = (l + r) >> 1;

        bool left_better = (isMax ? nw.get(l) > node->line.get(l)
                                  : nw.get(l) < node->line.get(l));
        bool mid_better = (isMax ? nw.get(mid) > node->line.get(mid)
                                 : nw.get(mid) < node->line.get(mid));

        if (mid_better)
            swap(nw, node->line);

        if (r - l == 0) return;

        if (left_better != mid_better)
            add_line(node->l, l, mid, nw);
        else
            add_line(node->r, mid + 1, r, nw);
    }

    void add_segment(Line nw, long long L, long long R) {
        add_segment(root, minX, maxX, nw, L, R);
    }

    void add_segment(Node *&node, long long l, long long r, Line nw, long long L, long long R) {
        if (r < L || R < l) return;
        if (L <= l && r <= R) {
            add_line(node, l, r, nw);
            return;
        }
        long long mid = (l + r) >> 1;
        add_segment(node->l, l, mid, nw, L, R);
        add_segment(node->r, mid + 1, r, nw, L, R);
    }

    long long query(long long x) const { return query(root, minX, maxX, x); }

    long long query(Node *node, long long l, long long r, long long x) const {
        if (!node) return isMax ? LLONG_MIN : LLONG_MAX;
        long long res = node->line.get(x);
        if (l == r) return res;
        long long mid = (l + r) >> 1;
        if (x <= mid)
            res = better(res, query(node->l, l, mid, x));
        else
            res = better(res, query(node->r, mid + 1, r, x));
        return res;
    }
};
