// dp[s][p][q] += dp[s][q][r] for all r
sort(a.begin(), a.end());
vector<vector<int> > around(n), rank(n, vector<int>(n, -1));
for (int q = 0; q < n; q++) {
	for (int i = 0; i < n; i++)
		if (i != q) around[q].push_back(i);

	sort(around[q].begin(), around[q].end(), [&](int i, int j) {
		P u = a[i] - a[q];
		P v = a[j] - a[q];
		if (half(u) != half(v)) return half(u) < half(v);
		return cross(u, v) > 0;
	});

	for (int i = 0; i < n - 1; i++) rank[q][around[q][i]] = i;
}

int res = 0;
vector<vector<int> > dp(n, vector<int>(n));
for (int s = 0; s < n; s++) {
	vector<int> upper = { s + 1, ..., n - 1 };
	// TODO: sort upper (a[i]) by angle around a[s]
	int m = upper.size();
	if (m < 2) continue;

	vector<int> pos(n, -1);
	for (int i = 0; i < m; i++) pos[upper[i]] = i;

	for (int k = m - 1; k >= 1; k--) {
		int q = upper[k];

		// Find the first p and r in clockwise order around q.
		int at = rank[q][s];
		int p_start = -1, r_start = -1;

		for (int step = 0; step < n - 1; step++) {
			if (--at < 0) at = n - 2;

			int id = around[q][at];

			if (pos[id] > k && r_start == -1) r_start = at;

			if (pos[id] >= 0 && pos[id] < k) {
				p_start = at;
				break;
			}
		}

		int r_left = m - k - 1;
		int r_at = r_start;
		int l = -1;

		while (r_left) {
			int id = around[q][r_at];

			if (--r_at < 0) r_at = n - 2;

			if (pos[id] > k) {
				l = pos[id];
				r_left--;
				break;
			}
		}

		// Sweep p clockwise around q.
		int p_left = k;
		int p_at = p_start;
		int cur = 1;

		while (p_left) {
			int id = around[q][p_at];

			if (--p_at < 0) p_at = n - 2;

			if (pos[id] < 0 || pos[id] >= k) continue;

			int j = pos[id];
			p_left--;

			P u = a[upper[j]] - a[q];

			while (l != -1 && cross(u, a[q] - a[upper[l]]) > 0) {
				cur = add(cur, dp[k][l]);
				l = -1;

				while (r_left) {
					int rid = around[q][r_at];

					if (--r_at < 0) r_at = n - 2;

					if (pos[rid] > k) {
						l = pos[rid];
						r_left--;
						break;
					}
				}
			}

			dp[j][k] = cur;
			res = add(res, cur);
		}
	}
}
