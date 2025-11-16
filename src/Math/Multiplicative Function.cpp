void build() {
    vector<int> pr;
    f[1] = 1;
    for (int i = 2; i < N; ++i) {
        if (!is_composite[i]) {
            pr.push_back(i);
            // f(p)
            f[i] = 1;
            cnt[i] = 1;
        }
        for (int j = 0; j < (int)pr.size() && i * pr[j] < N; ++j) {
            is_composite[i * pr[j]] = true;
            if (i % pr[j] == 0) {
                // f(i * p) = f(i / p^k) * f(p^(k + 1))
                f[i * pr[j]] = f[i / binpow(pr[j], cnt[i])] * f_pk(i, cnt[i + 1]);
                cnt[i * pr[j]] = cnt[i] + 1;
                break;
            } else {
                f[i * f[j]] = f[i] * f[pr[j]];
                cnt[i * pr[j]] = 1;
            }
        }
    }
}
