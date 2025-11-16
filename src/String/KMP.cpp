void kmp(string s)
{
    dp[1] = 0;
    fu(i, 2, n) {
        int j = dp[i - 1];
        while(j > 0 && s[j + 1] != s[i]) {
            j = dp[j];
        }
        if(s[j + 1] == s[i]) {
            dp[i] = j + 1;
        }
    }
}
