pair<int, int> f(int n) {
    if(n == 0) return { 0, 1 };
    auto [a, b] = fib(n >> 1);
    int c = a * (2 * b - a);
    int d = a * a + b * b;
    if(n & 1) return { d, c + d };
    else return {c, d};
}
