// SOS DP - Time: O(N * 2^N) | Space: O(2^N)
// Init: f[mask] = a[mask] | inv = 1: Mobius Inversion (recover a[] from f[])
// Tricks:
// - Count (x & y) == 0 : y is subset of (~x) -> query f[((1 << N) - 1) ^ x]
// - OR Convolution c[k] = sum_{i|j=k} a[i]*b[j]:
//     sos_sub(a, 0); sos_sub(b, 0); f[i] = a[i]*b[i]; sos_sub(f, 1);
// - AND Convolution c[k] = sum_{i&j=k} a[i]*b[j]:
//     sos_sup(a, 0); sos_sup(b, 0); f[i] = a[i]*b[i]; sos_sup(f, 1);
const int N = 20;
long long f[1 << N];

// Subset sum: f[mask] = sum(a[sub]) where (sub & mask) == sub
void sos_sub(long long f[], bool inv = 0) {
    for (int i = 0; i < N; ++i)
        for (int m = 0; m < (1 << N); ++m)
            if (m & (1 << i)) f[m] += (inv ? -1 : 1) * f[m ^ (1 << i)];
}
// Superset sum: f[mask] = sum(a[sup]) where (sup & mask) == mask
void sos_sup(long long f[], bool inv = 0) {
    for (int i = 0; i < N; ++i)
        for (int m = (1 << N) - 1; m >= 0; --m)
            if (!(m & (1 << i))) f[m] += (inv ? -1 : 1) * f[m ^ (1 << i)];
}