//This is the solution to http://codeforces.com/contest/897/problem/C
//C. Nephren gives a riddle
#raw "template.cpy"

#define N 100007

ll size[N];

string f0 = "What are you doing at the end of the world? Are you busy? Will you save us?";
string f1 = "What are you doing while sending \""
string f2 = "\"? Are you busy? Will you send \""
string f3 = "\"?"

void setSize()
	size[0] = f0.size();
	for i = 1; i < 56; i++
		size[i] = f1.size() + f2.size() + f3.size() + 2*size[i-1];
	for i = 56; i < N; i++
		size[i] = size[i-1]

void query(ll n, k)
	if n == 0
		if k < f0.size()
			cout << f0[k]
		else
			cout << '.'
		return

	if k < f1.size()
		cout << f1[k]
		return
		
	k -= f1.size()
	
	if k < size[n-1]
		query(n-1, k)
		return
		
	k -= size[n-1]
	if k < f2.size()
		cout << f2[k]
		return
		
	k -= f2.size()
	
	if k < size[n-1]
		query(n-1, k)
		return
		
	k -= size[n-1]
	
	if k < f3.size()
		cout << f3[k]
		return
		
	cout << '.'
	return
	
	
int main()
	setSize()
	int q
	cin >> q
	while q--
		ll n, k
		cin >> n >> k
		k--
		query(n, k)