using Poly = vector<int>;

static inline void norm(Poly &a) {
    while (!a.empty() && a.back() == 0) a.pop_back();
}
static inline size_t trailing_xk(const Poly &a) {
    for (size_t i = 0; i < a.size(); ++i) if (a[i] != 0) return i;
    return a.size();
}
static Poly mul_truncate(const Poly &a, const Poly &b, size_t n);
static inline Poly mul_xk(const Poly &a, size_t k) {
    if (a.empty()) return {};
    Poly r;
    r.reserve(k + a.size());
    r.assign(k, 0);
    r.insert(r.end(), a.begin(), a.end());
    return r;
}
static inline Poly div_xk(const Poly &a, size_t k) {
    if (a.size() <= k) return {};
    return Poly(a.begin() + k, a.end());
}
static inline Poly mod_xk(const Poly &a, size_t k) {
    if (k == 0) return {};
    Poly r(min(a.size(), k));
    for (size_t i = 0; i < r.size(); ++i) r[i] = a[i];
    norm(r);
    return r;
}
static inline Poly derivative(const Poly &a) {
    if (a.size() <= 1) return {};
    Poly r(a.size() - 1);
    for (size_t i = 1; i < a.size(); ++i) r[i - 1] = mulmod(a[i], (int)i);
    norm(r);
    return r;
}
static Poly integral(const Poly &a) {
    Poly r(a.size() + 1, 0);
    for (size_t i = 0; i < a.size(); ++i) r[i + 1] = mulmod(a[i], inv_mod(i + 1));
    norm(r);
    return r;
}
static Poly invert_poly(const Poly &a_in, size_t n) {
    assert(n > 0);
    Poly a = a_in; norm(a);
    assert(!a.empty() && a[0] != 0);
    Poly r(1); r[0] = inv_mod(a[0]);
    size_t cur = 1;
    while (cur < n) {
        size_t need = min(cur * 2, n);
        Poly pr = mul_truncate(a, r, need);
        pr.resize(need, 0);
        for (size_t i = 0; i < need; ++i) pr[i] = (MOD - pr[i]) % MOD;
        pr[0] = addmod(pr[0], 2);
        r = mul_truncate(r, pr, need);
        r.resize(need);
        norm(r);
        cur = need;
    }
    r.resize(n);
    norm(r);
    return r;
}
static Poly log_poly(const Poly &p_in, size_t n) {
    assert(n > 0);
    Poly p = p_in; norm(p);
    assert(!p.empty() && p[0] == 1);
    Poly p_inv = invert_poly(p, n);
    Poly p_der = derivative(mod_xk(p, n));
    Poly prod = mul_truncate(p_der, p_inv, (n > 0) ? n - 1 : 0);
    Poly res = integral(prod);
    return mod_xk(res, n);
}
static Poly exp_poly(const Poly &p_in, size_t n) {
    if (n == 0) return {};
    Poly p = p_in; norm(p);
    if (p.empty()) { Poly one(1, 1); return mod_xk(one, n); }
    assert(!p.empty() && p[0] == 0);
    Poly f(1, 1);
    size_t cur = 1;
    while (cur < n) {
        size_t need = min(cur * 2, n);
        Poly lf = log_poly(f, need);
        Poly g = mod_xk(p, need);
        g.resize(need, 0);
        lf.resize(need, 0);
        for (size_t i = 0; i < need; ++i) g[i] = submod(g[i], lf[i]);
        g[0] = addmod(g[0], 1); // multiply f by (1 + p - log f)
        f = mul_truncate(f, g, need);
        f.resize(need);
        norm(f);
        cur = need;
    }
    return mod_xk(f, n);
}
static Poly pow_dn(const Poly &a_in, int k, size_t n) {
    if (n == 0) return {};
    Poly a = a_in; norm(a);
    assert(!a.empty() && a[0] != 0);
    Poly Q(n, 0);
    Q[0] = powmod_int(a[0], k);
    int a0inv = inv_mod(a[0]);
    int deg = (int)a.size() - 1;
    for (int i = 1; i < (int)n; ++i) {
        int sum = 0;
        int upper = min(deg, i);
        for (int j = 1; j <= upper; ++j) {
            if (a[j] == 0 || Q[i - j] == 0) continue;
            int term = (int)a[j] * Q[i - j] % MOD;
            int coef = ((k % MOD) * j % MOD - (i - j)) % MOD;
            if (coef < 0) coef += MOD;
            term = term * coef % MOD;
            sum += term;
            if (llabs(sum) > (1LL << 60)) sum %= MOD;
        }
        sum %= MOD;
        sum = sum * inv_mod(i) % MOD * a0inv % MOD;
        if (sum < 0) sum += MOD;
        Q[i] = int(sum);
    }
    norm(Q);
    return Q;
}
static Poly pow_poly(const Poly &p_in, int k, size_t n) {
    if (n == 0) return {};
    Poly p = p_in; norm(p);
    if (p.empty()) {
        if (k == 0) { Poly one(1, 1); return mod_xk(one, n); }
        return {};
    }
    size_t r = trailing_xk(p);
    if (r > 0) {
        if ((size_t)k * r >= n) return {};
        size_t new_n = n - r * (size_t)k;
        Poly q = div_xk(p, r);
        Poly res = pow_poly(q, k, new_n);
        return mul_xk(res, r * (size_t)k);
    }

    const int THRESHOLD = 32;
    int deg = (int)p.size() - 1;
    if (deg <= THRESHOLD) return pow_dn(p, k, n);

    if (k <= THRESHOLD) { // smaint k -> exponentiation by squaring using mul_truncate
        if (k == 0) { Poly one(1,1); return mod_xk(one,n); }
        Poly base = mod_xk(p, n);
        Poly res(1,1);
        int e = k;
        while (e) {
            if (e & 1) res = mul_truncate(res, base, n);
            base = mul_truncate(base, base, n);
            e >>= 1;
        }
        return mod_xk(res, n);
    }

    int j = p[0];
    int jinv = inv_mod(j);
    Poly t = p;
    for (size_t i = 0; i < t.size(); ++i) t[i] = mulmod(t[i], jinv);
    Poly logt = log_poly(t, n);
    int km = int((k % MOD + MOD) % MOD);
    for (size_t i = 0; i < logt.size(); ++i) logt[i] = mulmod(logt[i], km);
    Poly ex = exp_poly(logt, n);
    int jk = powmod_int(j, k);
    for (size_t i = 0; i < ex.size(); ++i) ex[i] = mulmod(ex[i], jk);
    return mod_xk(ex, n);
 }
static pair<Poly, Poly> div_mod(const Poly &A_in, const Poly &B_in) {
    Poly A = A_in, B = B_in;
    norm(A); norm(B);
    assert(!B.empty());
    if (A.size() < B.size()) return {Poly(), A};
    int n = (int)A.size(), m = (int)B.size(), k = n - m;
    Poly q(k + 1, 0);
    int ib = inv_mod(B[m - 1]);
    for (int i = k; i >= 0; --i) {
        int coef = mulmod(A[m - 1 + i], ib);
        q[i] = coef;
        if (coef == 0) continue;
        for (int j = 0; j < m; ++j) A[j + i] = submod(A[j + i], mulmod(coef, B[j]));
    }
    A.resize(m - 1);
    norm(q); norm(A);
    return {q, A};
}
