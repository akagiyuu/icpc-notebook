mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());
ll Rand(ll l, ll h)
{
    return l + rd() % (h - l + 1);
}
bool not_prime[N + 5];
void make_test()
{
    ofstream inp((NAME + ".inp").c_str());
    inp.close();
}

int main()
{
    srand(time(NULL));
    fu(itest, 1, NTEST)
    {
        make_test();
        system((NAME + ".exe").c_str());
        system((NAME + "_trau.exe").c_str());
        if(system(("fc " + NAME + ".out " + NAME + ".ans").c_str()) != 0)
        {
            cout << itest << ' ' << "WA";
            return 0;
        }
        else cout << itest << ' ' << "CR" << '\n';
    }
}
