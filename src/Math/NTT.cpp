const int MOD = 998244353;
const int ROOT = 3; // Primitive root

void fft(vector<int> &a, bool invert) {
    int n = a.size();
    assert((n & (n - 1)) == 0);
    int lg = __builtin_ctz(n);
    for (int i = 0; i < n; ++i) {
        int j = 0;
        for (int k = 0; k < lg; ++k) if ((i&1<<k)!=0) j |= 1ll << (lg-k-1);
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len *= 2) {
        int wlen = powmod(ROOT, (MOD - 1) / len);
        if (invert) wlen = inverse(wlen);
        for (int i = 0; i < n; i += len) {
            int w = 1;
            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j];
                int v = a[i + j + len / 2] * w % MOD;
                a[i + j] = add(u, v);
                a[i + j + len / 2] = sub(u, v);
                w = w * wlen % MOD;
            }
        }
    }
    if (invert) {
        int mul = inverse(n);
        for (auto &x : a) x = x * mul % MOD;
    }
}
