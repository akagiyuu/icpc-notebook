struct fenwick_2d
{
    vector<int> BIT[N + 5];
    vector<int> f[N + 5];


    void fakeget(int x, int y)
    {
        for(int i = x; i >= 1; i -=(i&(-i)))
        {
            BIT[i].push_back(y);
        }
    }


    void fakeupdate(int x, int y)
    {
        for(int i = x; i <= N; i +=(i&(-i)))
        {
            BIT[i].push_back(y);
        }
    }


    void update(int x, int y, int val)
    {


        for(int i = x; i <= N; i +=(i&(-i)))
        {
            int l = lower_bound(BIT[i].begin(), BIT[i].end(), y) - BIT[i].begin() + 1;
            for(int j = l; j <= (int)BIT[i].size(); j+=(j&(-j)))
            {
                f[i][j - 1] = max(f[i][j - 1], val);
            }
        }
    }


    int get(int x, int y)
    {
        int res = 0;
        for(int i = x; i >= 1; i -=(i&(-i)))
        {
            int l = lower_bound(BIT[i].begin(), BIT[i].end(), y) - BIT[i].begin() + 1;
            for(int j = l; j >= 1; j-=(j&(-j)))
            {
                res = max(res, f[i][j - 1]);
            }
        }
        return res;
    }
};
