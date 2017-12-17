//This is the solution to http://codeforces.com/contest/897/problem/A
//A. Scarborough Fair
#raw "template.cpy"

int main()
	int n, m
	string s
	cin >> n >> m >> s
	while m--
		int l, r
		char c, cn
		cin >> l >> r >> c >> cn
		l--; r--;
		for i = l; i <= r; i++
			if s[i] == c
				s[i] = cn
	cout << s