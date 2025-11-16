struct info {
    int x, y1, y2, op; 

    bool operator < (const info &other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return op > other.op;
    }
};

struct dat {
    int x1, y1, x2, y2;
}rec[maxn];

vector<info>event;

int pre[maxn];
int cnt[maxn * 4], len[maxn * 4];

void update(int id, int l, int r, int u, int v, int val) {
    if (l > v || r < u) return;
    if (l >= u && r <= v) {
        cnt[id] += val;
        if (cnt[id] != 0) {
            len[id] = pre[r] - pre[l - 1];
        }
        else if (cnt[id] == 0) {
            if (l != r) {
                len[id] = len[id * 2] + len[id * 2 + 1];
            }
            else len[id] = 0;
        }
        return;
    }
    int mid = (l + r) >> 1;
    update(id * 2, l, mid, u, v, val);
    update(id * 2 + 1, mid + 1, r, u, v, val);
    if (cnt[id] == 0) len[id] = len[id * 2] + len[id * 2 + 1];
}
set<int>myset;

void magicFunc() {
    cin >> n;
    FOR(i,1,n) {
        int x1, x2, y1, y2;
        cin >> x1 >> y1 >> x2 >> y2;
       myset.insert(y1); myset.insert(y2);
        rec[i] = {x1, y1, x2, y2};
    }
    vector<int>compress(myset.begin(), myset.end());
    sort(all(compress));
    for (int i = 1; i < (int)compress.size(); ++i) {
        pre[i] = pre[i - 1] + compress[i] - compress[i - 1];
    }
    FOR(i,(int)compress.size(),maxn - 5) {
        pre[i] = pre[i - 1];
    }
    FOR(i,1,n) {
        rec[i].y1 = lower_bound(all(compress), rec[i].y1) - compress.begin();
        rec[i].y2 = lower_bound(all(compress), rec[i].y2) - compress.begin();
    }
    for (int i = 1; i <= n; ++i) {
        auto [x1, y1, x2, y2] = rec[i];
        event.push_back({x1, y1 + 1, y2, 1});
        event.push_back({x2, y1 + 1, y2, -1});
    }
    sort(all(event));
    int ans = 0;
    update(1,1,N,event[0].y1,event[0].y2,event[0].op);

    for (int i = 1; i < (int)event.size(); ++i) {
        ans += len[1] * (event[i].x - event[i - 1].x);
        update(1,1,N,event[i].y1,event[i].y2,event[i].op);
    }
    cout << ans;
}