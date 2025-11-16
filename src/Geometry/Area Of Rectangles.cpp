struct Event {
	int x, y1, y2, op;
	bool operator<(const Event &other) const { return x != other.x ? x < other.x : op > other.op; }
};
struct Rectangle { int x1, y1, x2, y2; };

int pre[N];
int cnt[N * 4], len[N * 4];
void update(int id, int l, int r, int u, int v, int val)
{
	if (l > v || r < u)
		return;
	if (l >= u && r <= v) {
		cnt[id] += val;
		if (cnt[id] != 0) {
			len[id] = pre[r] - pre[l - 1];
		} else if (cnt[id] == 0) {
			if (l != r) len[id] = len[id * 2] + len[id * 2 + 1];
			else len[id] = 0;
		}
		return;
	}
	int mid = (l + r) >> 1;
	update(id * 2, l, mid, u, v, val);
	update(id * 2 + 1, mid + 1, r, u, v, val);
	if (cnt[id] == 0) len[id] = len[id * 2] + len[id * 2 + 1];
}
int area(vector<Rectangle> &recs)
{
	int n = recs.size();
	set<int> s;
	for (auto rec : recs) {
		s.insert(rec.y1);
		s.insert(rec.y2);
	}
	vector<int> compress(s.begin(), s.end());
	sort(compress.begin(), compress.end());
	for (int i = 1; i < (int)compress.size(); ++i)
		pre[i] = pre[i - 1] + compress[i] - compress[i - 1];
	for (int i = compress.size(); i < N; i++)
		pre[i] = pre[i - 1];
	for (int i = 0; i < n; i++) {
		recs[i].y1 = lower_bound(compress.begin(), compress.end(), recs[i].y1) - compress.begin();
		recs[i].y2 = lower_bound(compress.begin(), compress.end(), recs[i].y2) - compress.begin();
	}
	vector<Event> event;
	for (auto [x1, y1, x2, y2] : recs) {
		event.push_back({ x1, y1 + 1, y2, 1 });
		event.push_back({ x2, y1 + 1, y2, -1 });
	}
	sort(event.begin(), event.end());
	int res = 0;
	update(1, 1, N, event[0].y1, event[0].y2, event[0].op);
	for (int i = 1; i < (int)event.size(); ++i) {
		res += len[1] * (event[i].x - event[i - 1].x);
		update(1, 1, N, event[i].y1, event[i].y2, event[i].op);
	}
	return res;
}
