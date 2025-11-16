for (int k = 0; k < n; k++)
    for (int mask = 0; mask < (1 << n); mask++)
        if (mask & (1 << k)) dp[mask] += dp[mask ^ (1 << k)];
