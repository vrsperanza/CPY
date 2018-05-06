#include <iostream>

main()
	! "for i 10"
	for i 10
		!! i
	!
	!

	! "for i 5 10"
	for i 5 10
		!! i
	!
	!
	
	! "for i 4 10 2"
	for i 4 10 2
		!! i
	!
	!
	
	! "for i 10 4 -2"
	for i 10 4 -2
		!! i
	!
	!
	
	! "for i = (float)0; i < 10; i += 0.5"
	for i = (float)0; i < 10; i += 0.5
		!! i
	!
	!
	
	! "Note that:"
	! "for i = 0; i < 10; i += 0.5"
	! "will not work in this case because i is initialized as a integer"