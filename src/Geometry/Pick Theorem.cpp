long long on_border(const vector<P>& poly){
    long long res = 0;
    int i, j, n = poly.size();
    for (i = 0, j = n - 1; i < n; j = i++)
        res += gcd(abs(poly[i].x - poly[j].x), abs(poly[i].y - poly[j].y));
    return res;
}
long long on_interior(const vector<P>& poly){
    long long res = 2 + 2 * area(poly) - on_border(poly);
    return res >> 1;
}
