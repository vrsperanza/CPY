//This is the solution to http://codeforces.com/contest/897/problem/B
//B. Chtholly's request
#raw "template.cpy"

ll compose(ll half)
	sz = 0;
	aux = half
	res = half
	while aux
		sz++;
		res *= 10
		res += aux%10
		aux /= 10
	return res

int main()
	int k, p
	cin >> k >> p
	ll sum = 0
	ll half = 1
	while k--
		sum += compose(half)
		sum %= p
		half++
	cout << sum