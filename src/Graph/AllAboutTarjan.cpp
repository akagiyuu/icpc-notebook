/*
 * Notes:
 *  - 0-based node indexing throughout (nodes: 0..n-1)
 */

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ---------------- Tarjan SCC (recursive) ----------------
struct TarjanSCC {
    int n;
    vector<vector<int>> g;
    vector<int> disc, low, comp, st;
    vector<char> inSt;
    int timer, sccCnt;

    TarjanSCC(int n = 0) { init(n); }
    void init(int _n) {
        n = _n;
        g.assign(n, {});
        disc.assign(n, 0);
        low.assign(n, 0);
        comp.assign(n, -1);
        inSt.assign(n, 0);
        st.clear();
        timer = 0;
        sccCnt = 0;
    }
    void addEdge(int u, int v) {
        g[u].push_back(v);
    }
    void dfs(int u) {
        disc[u] = low[u] = ++timer;
        st.push_back(u);
        inSt[u] = 1;
        for (int v : g[u]) {
            if (!disc[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (inSt[v]) {
                low[u] = min(low[u], disc[v]);
            }
        }
        if (low[u] == disc[u]) {
            while (true) {
                int v = st.back(); st.pop_back(); inSt[v] = 0;
                comp[v] = sccCnt;
                if (v == u) break;
            }
            ++sccCnt;
        }
    }
    // Run Tarjan and fill comp[u] in [0..sccCnt-1]
    void run() {
        for (int i = 0; i < n; ++i) if (!disc[i]) dfs(i);
    }
    // Build condensation DAG (SCC graph), returns adjacency list with duplicate edges removed
    vector<vector<int>> buildCondensation() const {
        vector<vector<int>> cg(sccCnt);
        for (int u = 0; u < n; ++u) {
            for (int v : g[u]) {
                if (comp[u] != comp[v]) cg[comp[u]].push_back(comp[v]);
            }
        }
        for (auto &adj : cg) {
            sort(adj.begin(), adj.end());
            adj.erase(unique(adj.begin(), adj.end()), adj.end());
        }
        return cg;
    }
};

// ---------------- Bridges & Articulation Points (low-link) ----------------
struct BridgesAP {
    int n;
    // store (to, edge_id)
    vector<vector<pair<int,int>>> g;
    vector<int> tin, low;
    vector<char> visited, isArticulation;
    vector<pair<int,int>> bridges; // list of bridges (u,v) where u is parent of v in DFS tree
    int timer;

    BridgesAP(int n = 0) { init(n); }
    void init(int _n) {
        n = _n;
        g.assign(n, {});
        tin.assign(n, -1);
        low.assign(n, -1);
        visited.assign(n, 0);
        isArticulation.assign(n, 0);
        bridges.clear();
        timer = 0;
    }
    // add undirected edge with unique id
    void addEdge(int u, int v, int id) {
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }
    void dfs(int u, int peid) {
        visited[u] = 1;
        tin[u] = low[u] = ++timer;
        int children = 0;
        for (auto [v, id] : g[u]) {
            if (id == peid) continue;
            if (visited[v]) {
                // back edge
                low[u] = min(low[u], tin[v]);
            } else {
                ++children;
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) {
                    // (u,v) is a bridge
                    bridges.emplace_back(u, v);
                }
                if (peid != -1 && low[v] >= tin[u]) {
                    isArticulation[u] = 1;
                }
            }
        }
        if (peid == -1 && children > 1) isArticulation[u] = 1;
    }
    void run() {
        for (int i = 0; i < n; ++i) if (!visited[i]) dfs(i, -1);
    }
};
