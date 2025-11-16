static std::mt19937_64 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());

const ull prime[] = {2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull, 31ull, 37ull};
const ull bases[] = {2ull, 325ull, 9375ull, 28178ull, 450775ull, 9780504ull, 1795265022ull};

bool is_prime(ull n) {
    if (n < 2) return false;
    for (ull p : prime) {
        if (n % p == 0) return n == p;
    }
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
    for (ull a : bases) {
        if (a >= n) continue;
        if (!check(a)) return false;
    }
    return true;
}
ull pollards_rho(ull n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    std::uniform_int_distribution<ull> dist(2, n - 1);

    while (true) {
        ull y = dist(rng);
        ull c = dist(rng);
        ull m = 128; // tuning parameter
        ull g = 1, r = 1, q = 1;
        auto f = [&](ull x)->ull { return (mulmod(x, x, n) + c) % n; };

        ull x, ys;
        while (g == 1) {
            x = y;
            for (ull i = 0; i < r; ++i) y = f(y);
            ull k = 0;
            while (k < r && g == 1) {
                ys = y;
                ull limit = min(m, r - k);
                for (ull i = 0; i < limit; ++i) {
                    y = f(y);
                    ull diff = x > y ? x - y : y - x;
                    q = mulmod(q, diff, n);
                }
                g = gcd(q, n);
                k += limit;
            }
            r <<= 1;
        }
        if (g == n) {
            do {
                ys = f(ys);
                ull diff = x > ys ? x - ys : ys - x;
                g = gcd(diff, n);
            } while (g == 1);
        }
        if (g != n && g != 0) return g;
    }
}
void factorize(ull n, unordered_map<ull, int> &out) {
    if (n == 1) return;
    if (is_prime(n)) { out[n]++; return; }
    ull d = pollards_rho(n);
    factorize(d, out);
    factorize(n / d, out);
}
