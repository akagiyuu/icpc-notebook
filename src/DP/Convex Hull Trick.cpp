void addline(line a, int type)
{
    while(st.size() >= 2)
    {
        if((type == 1 && so_sanh(st[(int)st.size() - 2], st.back(), a)) || (type == 2 && so_sanh2(st[(int)st.size() - 2], st.back(), a))) st.pop_back();
        else break;
    }
    st.push_back(a);
}
ll querry(ll a)
{
    int l = 1, r = (int)st.size() - 1;
    int res = 0;
    while(l <= r)
    {
        int mid = (l + r) / 2;
        if(st[mid - 1].get_val(a) <= st[mid].get_val(a)) res = mid, l = mid + 1;
        else r = mid - 1;
    }
    ll val = 0;
    if((int)st.size() >= 1) val = st[0].get_val(a);
    return max(val, st[res].get_val(a));
}
