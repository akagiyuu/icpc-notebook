int j = 0, k = 0;
auto a = ps[0], b = ps[1];
auto n1 = rotate(b - a, PI / 6.), n2 = rotate(b - a, 5. * PI / 6.);
for (int t = 0; t < n; t++) {
	if (dot(ps[t], n1) > dot(ps[j], n1)) j = t;
	if (dot(ps[t], n2) > dot(ps[k], n2)) k = t;
}
for (int i = 0; i < n; i++) {
	auto a = ps[i], b = ps[nxt(i)];
	auto n1 = rotate(b - a, PI / 6.), n2 = rotate(b - a, 5. * PI / 6.);
	while (dot(ps[nxt(j)], n1) > dot(ps[j], n1)) j = nxt(j);
	while (dot(ps[nxt(k)], n2) > dot(ps[k], n2)) k = nxt(k);
}
