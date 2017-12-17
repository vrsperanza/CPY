#include <iostream>
#include <string>

int main()
	int a, b;
	std::string str;
	! "Type two integers and a string"
	std::cin a b str
	
	!
	! "Following line is produced by command: ! a b str \"Second string\""
	! a b str "Second string"
	!
	
	! "Following line is produced by command: ? a b str"
	? a b str
	
	//Cout still works, and can optionally imply "<<"
	std::cout "A is valued " a std::endl
	std::cout << "B is valued " << b << std::endl