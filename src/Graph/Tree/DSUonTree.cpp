int n;
vector<vector<int>> g;
vector<int> val;    
vector<int> sz, heavy, st;
vector<int> in, out, ver;
int timer = 0;

void dfs_sz(int u, int p){
    sz[u] = 1; heavy[u] = -1;
    in[u] = timer; ver[timer++] = u;
    for(int v: g[u]) if(v!=p){
        dfs_sz(v,u);
        sz[u] += sz[v];
        if(heavy[u]==-1 || sz[v] > sz[heavy[u]]) heavy[u]=v;
    }
    out[u] = timer - 1;
}
unordered_map<int,int> cnt;
vector<ll> freqCnt;
int maxFreq = 0;

void add_color(int color){
    int old = cnt[color];
    int now = ++cnt[color];
    if((int)freqCnt.size() <= now) freqCnt.resize(now+1);
    if((int)freqCnt.size() <= old) freqCnt.resize(old+1);
    if(old>0) freqCnt[old]--;
    freqCnt[now]++;
    if(now > maxFreq) maxFreq = now;
}

void remove_color(int color){
    int now = --cnt[color];
    int old = now + 1;
    // update freqCnt
    freqCnt[old]--; freqCnt[now]++;
    if(freqCnt[maxFreq] == 0) maxFreq--;
}
vector<ll> ans;
void add_subtree(int u, int p){
    for(int i = in[u]; i <= out[u]; ++i){
        int node = ver[i];
        add_color(val[node]);
    }
}
void remove_subtree(int u, int p){
    for(int i = in[u]; i <= out[u]; ++i){
        int node = ver[i];
        remove_color(val[node]);
    }
}
void dfs(int u, int p, bool keep){
    for(int v: g[u]) if(v!=p && v!=heavy[u]){
        dfs(v, u, false);
    }
    if(heavy[u] != -1){
        dfs(heavy[u], u, true);
    }
    for(int v: g[u]) if(v!=p && v!=heavy[u]){
        for(int i = in[v]; i <= out[v]; ++i){
            int node = ver[i];
            add_color(val[node]);
        }
    }
    add_color(val[u]);
    ans[u] = freqCnt[maxFreq];

    if(!keep){
        for(int i = in[u]; i <= out[u]; ++i){
            int node = ver[i];
            int c = val[node];
            cnt[c] = 0;
        }
        freqCnt.assign(1,0);
        maxFreq = 0;
        cnt.clear();
    }
}
int main(){
    cin >> n;
    g.assign(n+1, {});
    val.assign(n+1, 0);
    for(int i=1;i<=n;i++) cin >> val[i];
    for(int i=0;i<n-1;i++){
        int u,v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    sz.assign(n+1,0); heavy.assign(n+1,-1);
    in.assign(n+1,0); out.assign(n+1,0);
    ver.assign(n+1,0); timer = 1;
    dfs_sz(1,0);
    ans.assign(n+1,0); freqCnt.assign(2,0);
    cnt.clear(); maxFreq = 0;
    dfs(1,0,true);
    for(int i=1;i<=n;i++)
        cout << ans[i] << (i==n?'\n':' ');
    return 0;
}
