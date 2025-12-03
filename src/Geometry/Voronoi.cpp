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
G voronoi_cell(G g, const vector<P> &v, int s) {
	for (int i = 0; i < v.size(); i++)
		if (i != s) g = convex_cut(g, bisector(v[s], v[i]));
	return g;
}
