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
	
	!
	! "rof is a shorthand for reversed direction for loops, when step is not specified, it will assume step is -1"
	! "otherwise it will use the given step with given direction, becoming equivalent a for loop"
	!
	! "rof i 10"
	rof i 10
		!! i
	!
	!

	! "rof i 10 5"
	rof i 10 5
		!! i
	!
	!
	
	! "rof i 4 10 2"
	rof i 4 10 2
		!! i
	!
	!
	
	! "rof i 10 4 -2"
	rof i 10 4 -2
		!! i
	!
	!
	
	! "rof i = (float)0; i < 10; i += 0.5"
	rof i = (float)0; i < 10; i += 0.5
		!! i
	!
	!