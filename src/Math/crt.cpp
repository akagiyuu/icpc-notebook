pair<int, int> crt(const vector<int> &r, const vector<int> &m) {
	int r0 = 0, m0 = 1;
	for (int i = 0; i < (int)r.size(); i++) {
		int r1 = r[i] % m[i], m1 = m[i];
		if (r1 < 0) r1 += m1;
		int x, y;
		int g = gcd(m0, m1, x, y);
		if ((r1 - r0) % g != 0) return { -1, 0 };
		int lcm = m0 / g * m1;
		int tmp = (r1 - r0) / g % (m1 / g) * x % (m1 / g);
		r0 = (r0 + m0 * tmp) % lcm;
		if (r0 < 0) r0 += lcm;
		m0 = lcm;
	}
	return { r0, m0 };
}
