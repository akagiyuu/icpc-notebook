vector<int> ys, dp;
void build_lagrange() {
	int n = ys.size(), i, v, f;
	for (f = 1, i = 1; i < n; i++) f = mul(f, i);
	v = powmod(f, MOD - 2);

	vector<int> inv(n, v);
	for (i = n - 1; i > 0; i--) inv[i - 1] = mul(inv[i], i);

	dp.resize(n, 1);
	for (i = 0; i < n; i++) {
		dp[i] = mul(inv[i], inv[n - i - 1]);
		dp[i] = mul(dp[i], ys[i]);
	}
}
int interpolate(long long x) {
	int n = ys.size();
	if (x < n) return ys[x];
	int i, w;
	vector<int> X(n, 1), Y(n, 1);
	for (i = 1; i < n; i++) X[i] = mul(X[i - 1], (x - i + 1));

	for (i = n - 2; i >= 0; i--) Y[i] = mul(Y[i + 1], (x - i - 1));

	int res = 0;
	for (i = 0; i < n; i++) {
		w = mul(mul(X[i], Y[i]), dp[i]);
		if ((n - i + 1) & 1) w = sub(0, w);
        res = add(res, w);
	}
    return res;
}
