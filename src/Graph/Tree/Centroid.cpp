int dfs_size(int u, int p) {
    sz[u] = 1;
    for (int v : adj[u]) {
        if (v != p && !vis[v])
            sz[u] += dfs_size(v, u);
    }
    return sz[u];
}

int find_centroid(int u, int p, int tot) {
    for (int v : adj[u]) {
        if (v != p && !vis[v] && sz[v] > tot / 2)
            return find_centroid(v, u, tot);
    }
    return u;
}
void dfs_distance(int u, int p, int d, vector<int> &dist) {
    dist.push_back(d);
    for (int v : adj[u]) {
        if (v != p && !vis[v])
            dfs_distance(v, u, d + 1, dist);
    }
}

void decompose(int u, int p) {
    int tot = dfs_size(u, -1);
    int c = find_centroid(u, -1, tot);
    vis[c] = 1;
    par_centroid[c] = (p == -1 ? c : p);

    // xử lý tại centroid hiện tại
    process_centroid(c);

    // phân rã các nhánh con
    for (int v : adj[c]) {
        if (!vis[v])
            decompose(v, c);
    }
}