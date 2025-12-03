G convex_hull(G ps) {
	if (ps.size() <= 1) return ps;
	sort(ps.begin(), ps.end());

	G a;
	for (auto &p : ps) {
		if (a.empty() || abs(a.back() - p) > EPS)
			a.push_back(p);
	}
	if (a.size() <= 1) return a;

	G lower, upper;
	for (auto &p : a) {
		while (lower.size() >= 2) {
			P A = lower[lower.size() - 2];
			P B = lower[lower.size() - 1];
			double cr = cross(B - A, p - A);
			// use cr < -EPS to keep collinear
			if (cr <= EPS) lower.pop_back();
			else break;
		}
		lower.push_back(p);
	}
	for (int i = a.size() - 1; i >= 0; --i) {
		P p = a[i];
		while (upper.size() >= 2) {
			P A = upper[upper.size() - 2];
			P B = upper[upper.size() - 1];
			double cr = cross(B - A, p - A);
			// use cr < -EPS to keep collinear
			if (cr <= EPS) upper.pop_back();
			else break;
		}
		upper.push_back(p);
	}

	lower.pop_back();
	upper.pop_back();
	G res = lower;
	res.insert(res.end(), upper.begin(), upper.end());

	if (res.empty()) return a;
	return res;
}
