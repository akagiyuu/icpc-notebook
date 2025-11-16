// calculate log p(x) mod x^n
poly_t &log_inplace(size_t n)
{
    assert(a[0] == T(1));
    mod_xk_inplace(n);
    return (inv_inplace(n) *= mod_xk_inplace(n).deriv()).mod_xk_inplace(n - 1).integr_inplace();
}
poly_t log(size_t n) const
{
    return poly_t(*this).log_inplace(n);
}

poly_t &mul_truncate(poly_t const &t, size_t k)
{
    fft::mul_truncate(a, t.a, k);
    return normalize();
}

poly_t &exp_inplace(size_t n)
{
    if (is_zero())
    {
        return *this = T(1);
    }
    assert(a[0] == T(0));
    a[0] = 1;
    size_t a = 1;
    while (a < n)
    {
        poly_t C = log(2 * a).div_xk_inplace(a) - substr(a, 2 * a);
        *this -= C.mul_truncate(*this, a).mul_xk_inplace(a);
        a *= 2;
    }
    return mod_xk_inplace(n);
}

poly_t exp(size_t n) const
{ // calculate exp p(x) mod x^n
    return poly_t(*this).exp_inplace(n);
}

poly_t pow_bin(int64_t k, size_t n) const
{ // O(n log n log k)
    if (k == 0)
    {
        return poly_t(1).mod_xk(n);
    }
    else
    {
        auto t = pow(k / 2, n);
        t = (t * t).mod_xk(n);
        return (k % 2 ? *this * t : t).mod_xk(n);
    }
}
poly_t pow_bin(int64_t k, size_t n) const
{ // O(n log n log k)
    if (k == 0)
    {
        return poly_t(1).mod_xk(n);
    }
    else
    {
        auto t = pow(k / 2, n);
        t = (t * t).mod_xk(n);
        return (k % 2 ? *this * t : t).mod_xk(n);
    }
}

poly_t circular_closure(size_t m) const
{
    if (deg() == -1)
    {
        return *this;
    }
    auto t = *this;
    for (size_t i = t.deg(); i >= m; i--)
    {
        t.a[i - m] += t.a[i];
    }
    t.a.resize(std::min(t.a.size(), m));
    return t;
}

static poly_t mul_circular(poly_t const &a, poly_t const &b, size_t m)
{
    return (a.circular_closure(m) * b.circular_closure(m)).circular_closure(m);
}

poly_t powmod_circular(int64_t k, size_t m) const
{
    if (k == 0)
    {
        return poly_t(1);
    }
    else
    {
        auto t = powmod_circular(k / 2, m);
        t = mul_circular(t, t, m);
        if (k % 2)
        {
            t = mul_circular(t, *this, m);
        }
        return t;
    }
}

poly_t powmod(int64_t k, poly_t const &md) const
{
    return poly::impl::powmod(*this, k, md);
}

// O(d * n) with the derivative trick from
// https://codeforces.com/blog/entry/73947?#comment-581173
poly_t pow_dn(int64_t k, size_t n) const
{
    if (n == 0)
    {
        return poly_t(T(0));
    }
    assert((*this)[0] != T(0));
    Vector Q(n);
    Q[0] = bpow(a[0], k);
    auto a0inv = a[0].inv();
    for (int i = 1; i < (int)n; i++)
    {
        for (int j = 1; j <= std::min(deg(), i); j++)
        {
            Q[i] += a[j] * Q[i - j] * (T(k) * T(j) - T(i - j));
        }
        Q[i] *= small_inv<T>(i) * a0inv;
    }
    return Q;
}

// calculate p^k(n) mod x^n in O(n log n)
// might be quite slow due to high constant
poly_t pow(int64_t k, size_t n) const
{
    if (is_zero())
    {
        return k ? *this : poly_t(1);
    }
    size_t i = trailing_xk();
    if (i > 0)
    {
        return k >= int64_t(n + i - 1) / (int64_t)i ? poly_t(T(0)) : div_xk(i).pow(k, n - i * k).mul_xk(i * k);
    }
    if (std::min(deg(), (int)n) <= magic)
    {
        return pow_dn(k, n);
    }
    if (k <= magic)
    {
        return pow_bin(k, n);
    }
    T j = a[i];
    poly_t t = *this / j;
    return bpow(j, k) * (t.log(n) * T(k)).exp(n).mod_xk(n);
}

Poly inv(int n) const
{
    assert(n > 0);
    assert(!a.empty() && a[0].v != 0);
    Poly r(vector<Mint>{a[0].inv()});
    int cur = 1;
    while (cur < n)
    {
        int need = min(2 * cur, n);
        Poly pr = mul_trim(*this, r, need);
        pr.a.resize(need, Mint(0));
        for (int i = 0; i < need; ++i)
            pr.a[i] = Mint(0) - pr.a[i];
        pr.a[0] += Mint(2);
        r = mul_trim(r, pr, need);
        r.a.resize(need);
        r.norm();
        cur = need;
    }
    if ((int)r.a.size() > n)
        r.a.resize(n);
    r.norm();
    return r;
}

pair<Poly, Poly> div_mod(const Poly &b) const
{
    Poly A = *this;
    A.norm();
    Poly B = b;
    B.norm();
    assert(!B.empty());
    if (A.deg() < B.deg())
        return {Poly{}, A};
    int n = A.a.size(), m = B.a.size(), k = n - m;
    vector<Mint> q(k + 1, Mint(0));
    Mint ib = B.a.back().inv();
    for (int i = k; i >= 0; --i)
    {
        Mint coef = A.a[m - 1 + i] * ib;
        q[i] = coef;
        for (int j = 0; j < m; ++j)
            A.a[j + i] -= coef * B.a[j];
    }
    A.norm();
    return {Poly(move(q)), A};
}