for (int l = 1; l <= n;)
    int q = n / l; l = n / q + 1; v.push_back(q);
