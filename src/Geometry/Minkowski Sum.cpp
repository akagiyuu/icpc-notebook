G minkowski_sum(const G &A0, const G &B0) {
	G A = convex_hull(A0), B = convex_hull(B0);
	if (A.empty() || B.empty()) return {};
	int ia = int(min_element(A.begin(), A.end()) - A.begin());
	int ib = int(min_element(B.begin(), B.end()) - B.begin());
	int n = (int)A.size(), m = (int)B.size();
	G a(n), b(m);
	for (int i = 0; i < n; ++i) a[i] = A[(ia + i) % n];
	for (int j = 0; j < m; ++j) b[j] = B[(ib + j) % m];
	vector<P> r;
	r.reserve(n + m);
	r.push_back(a[0] + b[0]);
	int i = 0, j = 0;
	while (i < n && j < m) {
		P va = a[(i + 1) % n] - a[i], vb = b[(j + 1) % m] - b[j];
		if (cross(va, vb) >= -EPS) {
			r.push_back(r.back() + va);
			++i;
		} else {
			r.push_back(r.back() + vb);
			++j;
		}
	}
	while (i < n) {
		r.push_back(r.back() + (a[(i + 1) % n] - a[i]));
		++i;
	}
	while (j < m) {
		r.push_back(r.back() + (b[(j + 1) % m] - b[j]));
		++j;
	}
	if (!r.empty() && abs(r.front() - r.back()) < EPS) r.pop_back();
	return convex_hull(r);
}
