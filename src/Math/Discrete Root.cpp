int generator(int p) {
	vector<int> fact;
	int phi = p - 1, n = phi;
	// TODO: implement factorize phi and save to fact
	for (int res = 2; res <= p; ++res) {
		bool ok = true;
		for (int factor : fact)
			if (powmod(res, phi / factor, p) == 1) {
				ok = false;
				break;
			}
		if (ok) return res;
	}
	return -1;
}
vector<int> discrete_root(int n, int k, int a) {
	if (a == 0) return 0;

	int g = generator(n);
	if (g == -1) return {};

	int base = powmod(g, k, n);
	int log = discrete_log(base, a, n);
	if (log == -1) return {};

	int phi = n - 1;
	int delta = phi / gcd(k, phi);

	vector<int> ans;
	for (long long cur_y = log % delta; cur_y < phi; cur_y += delta)
		ans.push_back(powmod(g, cur_y, n));

	return ans;
}
