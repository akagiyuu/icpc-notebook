typedef complex<double> cd;
const double PI = acos(-1.0);
vector<cd> roots;
void init_fft(int n) {
	roots.resize(n >> 1);
	const double angle = 2.0 * PI / n;
	const cd step(cos(angle), sin(angle));
	roots[0] = cd(1.0, 0.0);
	for (int i = 1; i < n / 2; ++i)
		roots[i] = roots[i - 1] * step;
}
void fft(vector<cd> &a, bool invert) {
	int n = a.size();
	int lg = __builtin_ctz(n);
	vector<int> rev(n);
	for (int i = 1; i < n; ++i)
		rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
	for (int i = 0; i < n; ++i) {
		int j = rev[i];
		if (i < j) swap(a[i], a[j]);
	}
	for (int len = 2; len <= n; len <<= 1) {
		const int step = n / len;
		const int half = len >> 1;
		for (int i = 0; i < n; i += len) {
			for (int j = 0; j < half; ++j) {
				cd w = roots[j * step];
				if (invert) w = conj(w);
				cd u = a[i + j];
				cd v = a[i + j + half] * w;
				a[i + j] = u + v;
				a[i + j + half] = u - v;
			}
		}
	}
	if (invert)
		for (int i = 0; i < n; i++)
			a[i] /= n;
}
