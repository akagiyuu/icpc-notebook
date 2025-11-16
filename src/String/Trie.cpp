struct trie
{
    int cnt;
    int id;
    int next[26];
    void init(int _cnt, int _id)
    {
        fu(j, 0, 25) next[j] = 0;
        cnt = _cnt;
        id = _id;
    }
};
trie node[N * 5];
int cnt_node;
void add(string x){
    int u = 0;
    fu(j, 0, (int)x.size() - 1)
    {
        int kt = x[j] - 'a';
        if(node[u].next[kt]) {
            u = node[u].next[kt];
            node[u].cnt++;
        }
        else {
            cnt_node++;
            node[u].next[kt] = cnt_node;
            u = cnt_node;
            node[u].cnt++;
        }
    }
}
int find_pref(string x)
{
    int u = 0;
    fu(j, 0, (int)x.size() - 1)
    {
        int kt = x[j] - 'a';
        if(node[u].next[kt]) {
            u = node[u].next[kt];
        }
        else {
            return 0;
        }
    }
    return node[u].cnt;
}
