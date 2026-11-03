// f[i] = c[i] + f[i - 1] * r[1] + ... + f[0] * r[i];
void cdq(int l, int r) {
	if (l >= r) return;
	if (l == r - 1) {
		f[i] += c[i];
		return;
	}
	int mid = (l + r) / 2;
	cdq_g(l, mid);
	// f[l..mid] = [f[l], ..., f[mid - 1]]
	c = multiply(f[l..mid], r[0..(r - l)]);
	for (int i = mid; i < r; i++) f[i] += c[i - l];
	cdq_g(mid, r);
}
