typedef complex<double> Complex;
const int LOG_FFT = 17;
const double PI = acos(-1.0);
static vector<Complex> fft_root(1 << LOG_FFT), inv_root(1 << LOG_FFT);

void init_fft() {
	int N = 1 << LOG_FFT;
	for (int i = 0; i < N; ++i) {
		double alpha = 2 * PI / N * i;
		fft_root[i] = Complex(cos(alpha), sin(alpha));
		inv_root[i] = Complex(cos(-alpha), sin(-alpha));
	}
}
unsigned round_up(unsigned v) {
	--v;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return v + 1;
}

int reverse_bits(int num, int lg) {
	int res = 0;
	for (int i = 0; i < lg; ++i)
		if ((num >> i) & 1)
			res |= 1 << (lg - i - 1);
	return res;
}

vector<Complex> fft(vector<Complex> a, bool invert) {
	int n = (int)a.size();
	int lg = 0;
	while ((1 << lg) < n) ++lg;

	int stride = (1 << LOG_FFT) / n;
	vector<Complex> roots(n);
	for (int i = 0; i < n; ++i)
		roots[i] = invert ? inv_root[stride * i] : fft_root[stride * i];

	for (int i = 0; i < n; ++i) {
		int rev = reverse_bits(i, lg);
		if (i < rev) swap(a[i], a[rev]);
	}

	for (int len = 2; len <= n; len <<= 1)
		for (int i = 0; i < n; i += len)
			for (int j = 0; j < (len >> 1); ++j) {
				Complex u = a[i + j];
				Complex v = a[i + j + (len >> 1)] * roots[n / len * j];
				a[i + j] = u + v;
				a[i + j + (len >> 1)] = u - v;
			}

	if (invert)
		for (int i = 0; i < n; ++i) a[i] /= n;
	return a;
}
