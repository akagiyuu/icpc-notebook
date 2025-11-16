// pollard_rho.cpp
// Factorizes n up to ~1e18 using deterministic Miller-Rabin + Pollard's Rho
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;
using u128 = __uint128_t;

static std::mt19937_64 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());

ull mulmod(ull a, ull b, ull mod) {
    return (ull)((u128)a * b % mod);
}

ull modpow(ull a, ull d, ull mod) {
    ull res = 1;
    while (d) {
        if (d & 1) res = mulmod(res, a, mod);
        a = mulmod(a, a, mod);
        d >>= 1;
    }
    return res;
}

bool isPrimeDet(ull n) {
    if (n < 2) return false;
    for (ull p : {2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull, 31ull, 37ull}) {
        if (n % p == 0) return n == p;
    }
    // Deterministic Miller-Rabin bases for 64-bit integers
    // Commonly used set: 2,325,9375,28178,450775,9780504,1795265022
    ull d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    auto check = [&](ull a)->bool {
        if (a % n == 0) return true;
        ull x = modpow(a, d, n);
        if (x == 1 || x == n - 1) return true;
        for (int r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) return true;
        }
        return false;
    };
    const ull bases[] = {2ull, 325ull, 9375ull, 28178ull, 450775ull, 9780504ull, 1795265022ull};
    for (ull a : bases) {
        if (a >= n) continue;
        if (!check(a)) return false;
    }
    return true;
}

ull gcd_ull(ull a, ull b) {
    while (b) {
        ull t = a % b;
        a = b;
        b = t;
    }
    return a;
}

ull pollards_rho(ull n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    ull c = std::uniform_int_distribution<ull>(1, n-1)(rng);
    ull x = std::uniform_int_distribution<ull>(2, n-1)(rng);
    ull y = x;
    ull d = 1;
    auto f = [&](ull x)->ull { return (mulmod(x, x, n) + c) % n; };
    int iter = 0;
    while (d == 1) {
        x = f(x);
        y = f(f(y));
        ull diff = x > y ? x - y : y - x;
        d = gcd_ull(diff, n);
        if (++iter > 100000) { // retry with new parameters if stuck
            return pollards_rho(n);
        }
    }
    if (d == n) return pollards_rho(n);
    return d;
}

void factorRec(ull n, vector<ull> &out) {
    if (n == 1) return;
    if (isPrimeDet(n)) {
        out.push_back(n);
        return;
    }
    ull d = pollards_rho(n);
    factorRec(d, out);
    factorRec(n / d, out);
}

vector<ull> factor(ull n) {
    vector<ull> res;
    factorRec(n, res);
    sort(res.begin(), res.end());
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ull n;
    if (!(cin >> n)) return 0;
    if (n == 0) {
        cout << "0\n";
        return 0;
    }
    if (n == 1) {
        cout << "1\n";
        return 0;
    }
    vector<ull> facs = factor(n);
    // print as prime factors (sorted)
    for (size_t i = 0; i < facs.size(); ++i) {
        if (i) cout << ' ';
        cout << facs[i];
    }
    cout << '\n';
    return 0;
}