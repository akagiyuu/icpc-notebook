ll dp[20][200][2][2];
int digits[20];
ll dfs(int pos, int sum, bool tight, bool leading) {
    if (pos == -1) return (sum % 3 == 0 && !leading);
    ll &res = dp[pos][sum][tight][leading];
    if (res != -1) return res;
    res = 0;
    int limit = tight ? digits[pos] : 9;
    for (int d = 0; d <= limit; d++) {
        bool ntight = tight && (d == limit);
        bool nlead = leading && (d == 0);
        res += dfs(pos - 1, (sum + d) % 3, ntight, nlead);
    }
    return res;
 }
ll solve(ll x) {
    if (x < 0) return 0;
    int len = 0;
    while (x > 0) {
        digits[len++] = x % 10;
        x /= 10;
    }
    if (len == 0) digits[len++] = 0;
    memset(dp, -1, sizeof(dp));
    return dfs(len - 1, 0, 1, 1);
}
