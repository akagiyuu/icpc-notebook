#include<bits/stdc++.h>

using namespace std;

const double PI = acos(-1);
const double EPS = 1e-8;
const double INF = 1e18;

typedef complex<double> P;
typedef vector<P> G;
struct L : public vector<P> {
	L(const P &a, const P &b) { push_back(a); push_back(b); }
	L() {}
};
struct C {
	P p;
	double r;
	C(const P &p, double r) : p(p) , r(r) {}
};

#define curr(P, i) P[i]
#define next(P, i) P[(i + 1) % P.size()]

bool operator<(const P &a, const P &b) {
	// if (abs(a-b)<EPS) return 0;
	return real(a) != real(b) ? real(a) < real(b) : imag(a) < imag(b);
}
double cross(const P &a, const P &b) { return imag(conj(a) * b); }
double dot(const P &a, const P &b) { return real(conj(a) * b); }
int ccw(P a, P b, P c) {
	b -= a;
	c -= a;
	if (cross(b, c) > 0) return +1; // counter clockwise
	if (cross(b, c) < 0) return -1; // clockwise
	if (dot(b, c) < 0) return +2; // c--a--b on line
	if (norm(b) < norm(c)) return -2; // a--b--c on line
	return 0;
}

bool intersectLL(const L &l, const L &m) {
	return abs(cross(l[1] - l[0], m[1] - m[0])) > EPS || // non-parallel
	       abs(cross(l[1] - l[0], m[0] - l[0])) < EPS; // same line
}
bool intersectLS(const L &l, const L &s) {
	return cross(l[1] - l[0], s[0] - l[0]) * cross(l[1] - l[0], s[1] - l[0]) < EPS;
}
bool intersectLP(const L &l, const P &p) {
	return abs(cross(l[1] - p, l[0] - p)) < EPS;
}
bool intersectSS(const L &s, const L &t) {
	return ccw(s[0], s[1], t[0]) * ccw(s[0], s[1], t[1]) <= 0 && ccw(t[0], t[1], s[0]) * ccw(t[0], t[1], s[1]) <= 0;
}
bool intersectSS2(const L &s, const L &t) {
	for (int i = 0; i < 2; i++) {
		if (ccw(s[0], s[1], t[i]) == 0) {
			int c = ccw(s[0], s[1], t[!i]);
			if (s[0] == t[i]) {
				if (c != -2 && c)
					return 0;
			} else if (s[1] == t[i]) {
				if (c != 2 && c)
					return 0;
			} else if (abs(c) == 1)
				return 0;
		}
	}
	return ccw(s[0], s[1], t[0]) * ccw(s[0], s[1], t[1]) <= 0 && ccw(t[0], t[1], s[0]) * ccw(t[0], t[1], s[1]) <= 0;
}
bool intersectSP(const L &s, const P &p) {
	return abs(s[0] - p) + abs(s[1] - p) - abs(s[1] - s[0]) < EPS; // triangle inequality
}
P projection(const L &l, const P &p) {
	double t = dot(p - l[0], l[0] - l[1]) / norm(l[0] - l[1]);
	return l[0] + t * (l[0] - l[1]);
}
P reflection(const L &l, const P &p) {
    return p + P(2, 0) * (projection(l, p) - p);
}
double distanceLP(const L &l, const P &p) { return abs(p - projection(l, p)); }
double distanceLL(const L &l, const L &m) { return intersectLL(l, m) ? 0 : distanceLP(l, m[0]); }
double distanceLS(const L &l, const L &s) {
	if (intersectLS(l, s)) return 0;
	return min(distanceLP(l, s[0]), distanceLP(l, s[1]));
}
double distanceSP(const L &s, const P &p) {
	const P r = projection(s, p);
	if (intersectSP(s, r)) return abs(r - p);
	return min(abs(s[0] - p), abs(s[1] - p));
}
double distanceSS(const L &s, const L &t) {
	if (intersectSS(s, t)) return 0;
	return min(min(distanceSP(s, t[0]), distanceSP(s, t[1])), min(distanceSP(t, s[0]), distanceSP(t, s[1])));
}
P crosspoint(const L &l, const L &m) {
	double A = cross(l[1] - l[0], m[1] - m[0]);
	double B = cross(l[1] - l[0], l[1] - m[0]);
	if (abs(A) < EPS && abs(B) < EPS) return m[0]; // same line
	assert(abs(A) >= EPS);
	return m[0] + B / A * (m[1] - m[0]);
}
double area(const G &g) {
	double A = 0;
	for (int i = 0; i < (int)g.size(); ++i)
		A += cross(g[i], next(g, i));
	return abs(A / 2);
 }
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
			if (cr <= EPS)
				lower.pop_back();
			else
				break;
		}
		lower.push_back(p);
	}
	for (int i = (int)a.size() - 1; i >= 0; --i) {
		P p = a[i];
		while (upper.size() >= 2) {
			P A = upper[upper.size() - 2];
			P B = upper[upper.size() - 1];
			double cr = cross(B - A, p - A);
			// use cr < -EPS to keep collinear
			if (cr <= EPS)
				upper.pop_back();
			else
				break;
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
G convex_cut(const G &g, const L &l) {
	G Q;
	for (int i = 0; i < (int)g.size(); i++) {
		P A = curr(g, i), B = next(g, i);
		if (ccw(l[0], l[1], A) != -1)
			Q.push_back(A);
		if (ccw(l[0], l[1], A) * ccw(l[0], l[1], B) < 0)
			Q.push_back(crosspoint(L(A, B), l));
	}
	return Q;
}
P centroid(const vector<P> &v) {
	double S = 0;
	P res = P(0, 0);
	for (int i = 0; i < (int)v.size(); i++) {
		int j = i + 1;
		if (j == (int)v.size()) j = 0;
		double tmp = cross(v[i], v[j]);
		S += tmp;
		res += (v[i] + v[j]) * tmp;
	}
	S /= 2;
	res /= 6 * S;
	return res;
}
double min_manhattanSP(const L &l, const P &p) {
	double res = INF;
	L xl = L(p, p + P(1, 0));
	if (intersectLS(xl, l)) {
		P cp = crosspoint(xl, l);
		double d = abs(p - cp);
		res = min(res, d);
	}
	L yl = L(p, p + P(0, 1));
	if (intersectLS(yl, l)) {
		P cp = crosspoint(yl, l);
		double d = abs(p - cp);
		res = min(res, d);
	}
	res = min(res, abs(l[0].real() - p.real()) + abs(l[0].imag() - p.imag()));
	res = min(res, abs(l[1].real() - p.real()) + abs(l[1].imag() - p.imag()));
	return res;
}
bool convex_contain(const G &g, const P &p) {
	for (int i = 0; i < (int)g.size(); i++)
		if (ccw(g[i], next(g, i), p) == -1)
			return 0;
	return 1;
}
bool intersectGG(const G &g1, const G &g2) {
	if (convex_contain(g1, g2[0])) return 1;
	if (convex_contain(g2, g1[0])) return 1;
	for (int i = 0; i < (int)g1.size(); i++) {
		for (int j = 0; j < (int)g2.size(); j++) {
			if (intersectSS(L(g1[i], next(g1, i)), L(g2[j], next(g2, j))))
				return 1;
		}
	}
	return 0;
}
double distanceGP(const G &g, const P &p) {
	if (convex_contain(g, p)) return 0;
	double res = INF;
	for (int i = 0; i < (int)g.size(); i++) {
		res = min(res, distanceSP(L(g[i], next(g, i)), p));
	}
	return res;
}
L bisector(const P &a, const P &b) {
	P A = (a + b) * P(0.5, 0);
	return L(A, A + (b - a) * P(0, PI / 2));
}
G voronoi_cell(G g, const vector<P> &v, int s) {
	for (int i = 0; i < (int)v.size(); i++)
		if (i != s)
			g = convex_cut(g, bisector(v[s], v[i]));
	return g;
}
double angle(const P &a, const P &b) { // Goc dinh huong a -> b [0,2pi)
	double ret = arg(b) - arg(a);
	return (ret >= 0) ? ret : ret + 2 * PI;
}
double angle2(const P &a, const P &b) { return min(angle(a, b), angle(b, a)); }

P rotate(P p, double ang) {
	return p * P(cos(ang), sin(ang));
}
L rotate(L l, double ang) {
	return L(rotate(l[0], ang), rotate(l[1], ang));
}
int intersectCL(const C &c, const L &l, vector<P> &res) {
    P pr = projection(l, c.p); // projection of center onto line
    double d = abs(pr - c.p);
    if (d > c.r + EPS) return 0;
    if (abs(d - c.r) < EPS) {
        res.push_back(pr);
        return 1;
    }
    double len = abs(l[1] - l[0]);
    P dir = (l[1] - l[0]) / len; // unit direction along line
    double h = sqrt(max(0.0, c.r * c.r - d * d));
    res.push_back(pr + dir * h);
    res.push_back(pr - dir * h);
    return 2;
}
int intersectCC(const C &c1, const C &c2, vector<P> &res) {
    double d = abs(c2.p - c1.p);
    if (d < EPS) { // concentric
        if (abs(c1.r - c2.r) < EPS) return -1; // coincident
        return 0;
    }
    if (d > c1.r + c2.r + EPS) return 0; // too far
    if (d < fabs(c1.r - c2.r) - EPS) return 0; // one inside another
    double a = (c1.r*c1.r - c2.r*c2.r + d*d) / (2.0 * d);
    P mid = c1.p + (c2.p - c1.p) * (a / d); // point on radical line
    P perp = (c2.p - c1.p) * P(0, 1); // rotated by +90 deg
    L line(mid - perp, mid + perp);   // any two distinct points on the radical line
    return intersectCL(c1, line, res);
}
vector<L> tangentCC(const C &c1, const C &c2) {
    vector<L> res;
    P p1 = c1.p, p2 = c2.p;
    double r1 = c1.r, r2 = c2.r;
    double d = abs(p2 - p1);
    if (d < EPS) return res; // concentric: no (distinct) tangents
    P u = (p2 - p1) / d;
    for (int s = -1; s <= 1; s += 2) { // s = -1 (internal tangents), s = +1 (external tangents)
        double r = (r1 - s * r2) / d;
        if (r * r > 1 + EPS) continue; // no solution
        double h = sqrt(max(0.0, 1 - r * r));
        P v = u * P(0, 1); // perpendicular to u
        for (int sign = -1; sign <= 1; sign += 2) {
            P dir = u * r + v * (h * sign); // unit vector
            P t1 = p1 + dir * r1;
            P t2 = p2 + dir * (r2 * s);
            res.push_back(L(t1, t2));
        }
    }
    return res;
}
