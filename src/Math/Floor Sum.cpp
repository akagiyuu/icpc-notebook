// sum_{i = 0}^{n} floor(ax + b / c)
int floor_sum(int a, int b, int c, int n) {
	if (a == 0) return (b / c) * (n + 1);
	if (a >= c or b >= c)
		return ((n * (n + 1)) / 2) * (a / c) + (n + 1) * (b / c) + floor_sum(a % c, b % c, c, n);
	int m = (a * n + b) / c;
	return m * n - floor_sum(c, c - b - 1, a, m - 1);
}
