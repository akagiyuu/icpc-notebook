#define MAX 1048576
const int OR = 0; const int AND = 1; const int XOR = 2;

long long P1[MAX], P2[MAX];
void walsh_transform(long long *ar, int n, int conv_type) {
	if (!n) return;
	int m = n >> 1;
	walsh_transform(ar, m, conv_type);
	walsh_transform(ar + m, m, conv_type);
	for (int i = 0; i < m; i++) {
		long long x = ar[i], y = ar[i + m];
		if (conv_type == OR) ar[i] = x, ar[i + m] = x + y;
		if (conv_type == AND) ar[i] = x + y, ar[i + m] = y;
		if (conv_type == XOR) ar[i] = x + y, ar[i + m] = x - y;
	}
}
void inverse_walsh_transform(long long *ar, int n, int conv_type) {
	if (!n) return;
	int m = n >> 1;
	inverse_walsh_transform(ar, m, conv_type);
	inverse_walsh_transform(ar + m, m, conv_type);
	for (int i = 0; i < m; i++) {
		long long x = ar[i], y = ar[i + m];
		if (conv_type == OR) ar[i] = x, ar[i + m] = y - x;
		if (conv_type == AND) ar[i] = x - y, ar[i + m] = y;
		if (conv_type == XOR) ar[i] = (x + y) >> 1, ar[i + m] = (x - y) >> 1;
	}
}
