void preprocess() {
    for (int k = 0; k <= LGM; ++k) {
        for (int i = 1; i + (1 << k) - 1 <= m; ++i) {
            for (int l = 0; l <= LGN; ++l) {
                for (int j = 1; j + (1 << l) - 1 <= n; ++j) {
                    if (k == 0) {
                        if (l == 0) {
                            st[0][i][0][j] = a[i][j];
                        }
                        else {
                            st[0][i][l][j] = min(st[0][i][l - 1][j], st[0][i][l - 1][j + (1 << (l - 1))]);
                        }
                    }
                    else {
                        st[k][i][l][j] = min(st[k - 1][i][l][j], st[k - 1][i + (1 << (k - 1))][l][j]);
                    }
                }
            }
        }
    }
}


int get_rmq_2d(int x, int y, int a, int b) {
    int k = __lg(a - x + 1);
    int l = __lg(b - y + 1);
    return min({ st[k][x][l][y],
                 st[k][x][l][b - (1 << l) + 1],
                 st[k][a - (1 << k) + 1][l][y],
                 st[k][a - (1 << k) + 1][l][b - (1 << l) + 1] });
}
