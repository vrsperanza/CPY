//This is the solution to http://codeforces.com/contest/900/problem/A
//A. Find Extra One
#raw "template.cpy"

int main()
	int n, x, y
	cin n
	s1 = 0
	s2 = 0
	while n--
		cin x y
		if x > 0
			s1++
		else
			s2++
	
	if(s1 > 1 && s2 > 1)
		! "No"
	else
		! "Yes"