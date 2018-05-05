#include <iostream>
#include <string>
int main()
	int a, b;
	std::string str;
	? "Type two integers: " a b "Well done! Now type a string: " str "Congratulations!\n"
	
	!
	! "Following line is produced by command: ! a b str \"Second string\""
	! a b str "Second string"
	!
	
	!! "!! is a quick print without ending the line..."
	! " Ending the line in the following command exemplified!"
	
	! "Following line is produced by command: ?? a b str"
	?? a b str
	
	//Cout still works, and can optionally imply "<<"
	std::cout "A is valued " a std::endl
	std::cout << "B is valued " << b << std::endl