int normalize(int x, int m); // ensure x in [0..m)
int inverse(int a, int m) {
	a = normalize(a, m);
	int x, y;
	int g = ext_gcd(a, m, x, y);
	if (g != 1) return -1;
	return normalize(x, m);
}
void build_fact(int p); // build fact[0..N] % p
int C_prime(int n, int k, int p) {
	if (k < 0 || k > n) return 0;
	return fact[n] * inverse(fact[n - k] * fact[k] % p, p) % p;
}
int C_lucas(int n, int k, int p) {
	if (k < 0 || k > n) return 0;
	build_fact(p);
	int res = 1;
	while (n || k) {
		int ni = n % p, ki = k % p;
		if (ki > ni) return 0;
		res = res * C_prime(ni, ki, p) % p;
		n /= p; k /= p;
	}
	return res;
}
int factorial_p_exp(int n, int p); // find e of p^e in factorization of n!
int C_prime_pow(int n, int k, int p, int p_pow, int power_count) {
	if (k < 0 || k > n) return 0;
	int exp = factorial_p_exp(n, p) - factorial_p_exp(k, p) - factorial_p_exp(n - k, p);
	if (exp >= power_count) return 0;

	vector<int> f(p_pow + 1, 1);
	for (int i = 1; i <= p_pow; i++) {
		f[i] = f[i - 1];
		if (__gcd(i, p) == 1)
			f[i] = f[i] * i % p_pow;
	}

	auto calc_f = [&](int x) {
		int r = 1;
		if (x == 0) return 1LL;
		r = powmod(f[p_pow], x / p_pow, p_pow);
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
	int res = gn * inverse(gnk * gk % p_pow, p_pow) % p_pow;
	return res * powmod(p, exp, p_pow) % p_pow;
}
int C(int n, int k, int m)
{
	if (k < 0 || k > n) return 0;
	if (m == 1) return 0;

	vector<int> rs, ms;
	for (int p = 2; p * p <= m; p++) {
		if (m % p != 0) continue;
		int p_pow = 1, cnt = 0;
		while (m % p == 0) {
			p_pow *= p;
			m /= p;
			cnt++;
		}
		int r = (p_pow == p) ? C_lucas(n, k, p) : C_prime_pow(n, k, p, p_pow, cnt);
		rs.push_back(r); ms.push_back(p_pow);
	}
	if (m > 1) {
		int p = m;
		int r = C_lucas(n, k, p);
		rs.push_back(r); ms.push_back(p);
	}
	return crt(rs, ms).first;
}
