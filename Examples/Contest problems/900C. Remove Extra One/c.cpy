//This is the solution to http://codeforces.com/contest/900/problem/C
//C. Remove Extra One
#raw "template.cpy"

#define N 100007

int main()
	int n
	cin n
	a = 0
	b = 0
	int x
	
	int blk[N]
	memset(blk, 0, sizeof(blk))
	
	t = n
	while t--
		cin x
		if x > a
			b = a
			a = x
			blk[a]--
		else if x > b
			b = x
			blk[a]++
	
	int mb = -1000
	int mval = -1000
	//? n
	for int i = 1; i <= n; i++
		//? i blk[i]
		if blk[i] > mb
			mb = blk[i]
			mval = i
			
	! mval