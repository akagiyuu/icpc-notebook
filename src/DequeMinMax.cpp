void sliding_window_minmax(const vector<int>& a, int k) {
    int n = a.size();
    deque<int> dq_min, dq_max;

    vector<int> minv, maxv;
    minv.reserve(n); maxv.reserve(n);

    for (int i = 0; i < n; i++) {
        while (!dq_min.empty() && a[dq_min.back()] >= a[i])
            dq_min.pop_back();
        dq_min.push_back(i);
        if (dq_min.front() <= i - k) dq_min.pop_front();

        while (!dq_max.empty() && a[dq_max.back()] <= a[i])
            dq_max.pop_back();
        dq_max.push_back(i);
        if (dq_max.front() <= i - k) dq_max.pop_front();

        if (i >= k - 1) {
            minv.push_back(a[dq_min.front()]);
            maxv.push_back(a[dq_max.front()]);
        }
    }
}
