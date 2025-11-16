int discrete_log(int a, int b, int m) {
	a %= m, b %= m;
	int n = sqrt(m) + 1;
	int an = 1;
	for (int i = 0; i < n; ++i) an = (an * a) % m;
	unordered_map<int, int> vals;
	for (int q = 0, cur = b; q <= n; ++q) {
		vals[cur] = q;
		cur = (cur * a) % m;
	}
	for (int p = 1, cur = 1; p <= n; ++p) {
		cur = (cur * an) % m;
		if (vals.count(cur)) {
			int ans = n * p - vals[cur];
			return ans;
		}
	}
	return -1;
}
