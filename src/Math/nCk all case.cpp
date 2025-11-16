#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const ll N = 1e6;
ll last_prime = 0;
ll cur = 0;
ll fact[N + 1] = { 1 };

ll powmod(ll a, ll b, ll m)
{
    a %= m;
    ll res = 1;
    while (b) {
        if (b & 1)
            res = res * a % m;

        a = a * a % m;
        b >>= 1;
    }
    return res;
}

ll pow(ll a, ll b)
{
    ll res = 1;
    for (ll i = 0; i < b; i++) {
        res *= a;
    }
    return res;
}

tuple<ll, ll, ll> gcd(ll a, ll b)
{
    ll x = 1, y = 0, x1 = 0, y1 = 1;
    while (b > 0) {
        ll p = a / b;
        tie(a, b) = make_pair(b, a % b);
        tie(x, x1) = make_pair(x1, x - x1 * p);
        tie(y, y1) = make_pair(y1, y - y1 * p);
    }
    return make_tuple(a, x, y);
}

ll inv(ll a, ll m)
{
    a %= m;
    if (a == 1)
        return 1;
    ll x, y, g;
    tie(g, x, y) = gcd(a, m);
    if (g != 1) {
        return -1;
    }
    return (x % m + m) % m;
}

ll nCk_prime(ll n, ll k, ll p)
{
    if (n < k)
        return 0;
    if (p != last_prime) {
        cur = 0;
        last_prime = p;
    }

    for (ll i = cur + 1; i <= n; i++) {
        fact[i] = fact[i - 1] * i % p;
    }

    ll res = fact[n] * inv(fact[k] * fact[n - k], p) % p;
    return res;
}

ll nCk_lucas(ll n, ll k, ll p)
{
    if (n < k)
        return 0;

    ll res = 1;
    while (n > 0) {
        res = res * nCk_prime(n % p, k % p, p) % p;
        n /= p;
        k /= p;
    }
    return res;
}

ll factorial_natural_log(ll n, ll p)
{
    ll res = 0;
    for (ll i = p; i <= n; i *= p) {
        res += n / i;
    }
    return res;
}

ll nCk_prime_power(ll n, ll k, ll p, ll pk, ll count)
{
    if (n < k)
        return 0;
    if (max(k, n - k) < p)
        return nCk_lucas(n, k, pk);

    ll exp = factorial_natural_log(n, p) - factorial_natural_log(k, p) - factorial_natural_log(n - k, p);
    if (exp >= count)
        return 0;

    vector<ll> f(pk + 1, 1);
    f[1] = 1;
    for (ll i = 2; i <= pk; i++) {
        f[i] = f[i - 1];
        if (std::gcd(i, p) == 1)
            f[i] = f[i] * i % pk;
    }

    auto cal_f = [f, pk](ll n) { return powmod(f[pk], n / pk, pk) * f[n % pk] % pk; };

    auto cal_g = [cal_f, p, pk](ll n) {
        ll cur = 1;
        ll res = 1;
        while (n / cur != 0) {
            res = res * cal_f(n / cur) % pk;
            cur *= p;
        }
        return res;
    };

    ll gn = cal_g(n);
    ll gk = cal_g(k);
    ll gnk = cal_g(n - k);

    ll res = gn * inv(gk * gnk, pk) % pk * pow(p, exp) % pk;

    return res;
}

ll nCk(ll n, ll k, ll m)
{
    ll M = m;
    ll res = 0;
    for (ll p = 2; p * p <= m; p++) {
        if (m % p != 0)
            continue;

        ll pk = 1;
        ll count = 1;
        while (m % p == 0) {
            pk *= p;
            m /= p;
            count++;
        }
        ll r;
        if (pk == p)
            r = nCk_lucas(n, k, p);
        else
            r = nCk_prime_power(n, k, p, pk, count);
        if (r > 0) {
            ll Mi = M / pk;
            ll cur = r * inv(Mi, pk) % M * Mi % M;
            res = (res + cur) % M;
        }
    }

    if (m > 1) {
        ll r = nCk_lucas(n, k, m);
        if (r > 0) {
            ll Mi = M / m;
            ll cur = r * inv(Mi, m) % M * Mi % M;
            res = (res + cur) % M;
        }
    }

    return res;
}