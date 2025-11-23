C circle2(const P &a, const P &b) {
	P c = (a + b) * P(0.5, 0);
	return C(c, abs(a - c));
}
C circle3(const P &a, const P &b, const P &c) {
	double ax = real(a), ay = imag(a), bx = real(b), by = imag(b), cx = real(c), cy = imag(c);
	double d = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
	if (fabs(d) < EPS) return C(P(0, 0), -1);
	double A = ax * ax + ay * ay, B = bx * bx + by * by, Cc = cx * cx + cy * cy;
	P o = P((A * (by - cy) + B * (cy - ay) + Cc * (ay - by)) / d,
		(A * (cx - bx) + B * (ax - cx) + Cc * (bx - ax)) / d);
	return C(o, abs(o - a));
}
bool in_circle(const C &c, const P &p) { return c.r >= 0 && abs(p - c.p) <= c.r + EPS; }

C welzl(vector<P> &pts, vector<P> &R, int n) {
	if (n == 0 || R.size() == 3) {
		if (R.empty()) return C(P(0, 0), -1);
		if (R.size() == 1) return C(R[0], 0);
		if (R.size() == 2) return circle2(R[0], R[1]);
		return circle3(R[0], R[1], R[2]);
	}
	P p = pts[n - 1];
	C D = welzl(pts, R, n - 1);
	if (D.r >= 0 && in_circle(D, p)) return D;
	R.push_back(p);
	C D2 = welzl(pts, R, n - 1);
	R.pop_back();
	return D2;
}
C minimum_enclosing_circle(G pts) {
	random_device rd;
	mt19937 g(rd());
	shuffle(pts.begin(), pts.end(), g);
	vector<P> R;
	C ans = welzl(pts, R, (int)pts.size());
	if (ans.r < 0) return C(P(0, 0), 0);
	return ans;
}
