const int N = 1e6 + 1;
int last_prime = -1;
int fact[N] = { 1 };
int fact_filled = 0;

void ensure_fact(int n, int p) {
	if (p != last_prime) {
		fill(fact, fact + N, 1);
		fact_filled = 0;
		last_prime = p;
	}
	n = min(n, N);
	for (int i = fact_filled + 1; i <= n; i++) fact[i] = fact[i - 1] * i % p;
	fact_filled = max(fact_filled, n);
}
int nCk_prime(int n, int k, int p) {
	if (k < 0 || k > n) return 0;
	ensure_fact(n, p);
	int num = fact[n];
	int den = fact[k] * fact[n - k] % p;
	int inv_den = mod_inv(den, p);
	if (inv_den == -1) return 0;
	return num * inv_den % p;
}
int nCk_lucas(int n, int k, int p) {
	if (k < 0 || k > n) return 0;
	int res = 1;
	while (n || k) {
		int ni = n % p, ki = k % p;
		if (ki > ni) return 0;
		res = res * nCk_prime(ni, ki, p) % p;
		n /= p;
		k /= p;
	}
	return res;
}
int factorial_p_exp(int n, int p) {
	int cnt = 0;
	for (int q = p; q <= n; q *= p) cnt += n / q;
	return cnt;
}
int nCk_prime_power(int n, int k, int p, int p_pow, int power_count) {
	if (k < 0 || k > n) return 0;
	int exp = factorial_p_exp(n, p) - factorial_p_exp(k, p) - factorial_p_exp(n - k, p);
	if (exp >= power_count) return 0;

	vector<int> f(p_pow + 1, 1);
	for (int i = 1; i <= p_pow; i++) {
		f[i] = f[i - 1];
		if (gcd(i, p) == 1)
			f[i] = f[i] * i % p_pow;
	}

	auto calc_f = [&](int x) {
		int r = 1;
		if (x == 0) return 1LL;
		r = mod_pow(f[p_pow], x / p_pow, p_pow);
		r = r * f[x % p_pow] % p_pow;
		return r;
	};

	auto calc_g = [&](int x) {
		int r = 1;
		for (int cur = 1; x / cur; cur *= p)
			r = r * calc_f(x / cur) % p_pow;
		return r;
	};

	int gn = calc_g(n), gk = calc_g(k), gnk = calc_g(n - k);
	int denom = gk * gnk % p_pow;
	int inv_denom = mod_inv(denom, p_pow);
	if (inv_denom == -1) return 0;

	int res = gn * inv_denom % p_pow;
	res = res * mod_pow(p, exp, p_pow) % p_pow;
	return res;
}
int nCk(int n, int k, int m) {
	if (k < 0 || k > n) return 0;
	if (m == 1) return 0;
	int M = m, res = 0;

	for (int p = 2; p * p <= m; p++) {
		if (m % p != 0) continue;
		int p_pow = 1, cnt = 0;
		while (m % p == 0) {
			p_pow *= p;
			m /= p;
			cnt++;
		}
		int r = (p_pow == p) ? nCk_lucas(n, k, p) : nCk_prime_power(n, k, p, p_pow, cnt);
		if (r == 0) continue;
		int Mi = M / p_pow;
		int inv = mod_inv(Mi % p_pow, p_pow);
		if (inv == -1) continue;
		int term = r * inv % p_pow;
		term = term * Mi % M;
		res = (res + term) % M;
	}

	if (m > 1) {
		int p = m;
		int r = nCk_lucas(n, k, p);
		if (r > 0) {
			int Mi = M / p;
			int inv = mod_inv(Mi % p, p);
			if (inv != -1) {
				int term = r * inv % p;
				term = term * Mi % M;
				res = (res + term) % M;
			}
		}
	}

	return (res % M + M) % M;
}
