#include <iostream>
#include <string>
int main()
	int a, b;
	std::string str;
	? "Type two integers and a string: " a b "uuuuhhhh, two integers done!\nOnly one string to go: " str "Well done!\n"
	
	!
	! "Following line is produced by command: ! a b str \"Second string\""
	! a b str "Second string"
	!
	
	!! "!! is a quick print withought ending the line..."
	! " Ending the line in the following command exemplified!"
	
	! "Following line is produced by command: ?? a b str"
	?? a b str
	
	//Cout still works, and can optionally imply "<<"
	std::cout "A is valued " a std::endl
	std::cout << "B is valued " << b << std::endl