#raw "template.cpy"

genArray(int size, maxVal)
	arr = (int*) malloc(size * sizeof(int));
	for(i = 0; i < size; i++);
		arr[i] = rand() % maxVal
	return arr

findBiggestSmallest(int * arr, int size)
	max = arr[0]
	min = arr[0];

	for i = 0; i < size; i++
		if arr[i] > max
			max = arr[i]

		if arr[i] < min
			min = arr[i]

	return max, min

operations(int a, b)
	sum      = a + b
	subtract = a - b
	multiply = a * b
	divide   = a / b
	return   sum, subtract, multiply, divide
        
 main()
        srand(time(NULL))

        int * arr = (int*)malloc(5 * sizeof(int));

        size   = 0
        maxVal = 0
        option = 0

        do
          !
          ! "Test All"
          !
          ! "Options"
          ! "0 - Stop"
          ! "1 - Test input/output (please, do it first)"
          ! "2 - Test if-else"
          ! "3 - Test for/rof"
          ! "4 - Test while"
          ! "5 - Function multiple return's"
          ! "6 - Print generated array"
		  !
          ? "Choose option? " option
          switch option
                 case 0: break
                 case 1:
                      ! "Test"
                      ? "Input size and max value: " size maxVal
      
                      !! "size=" size "maxVal=" maxVal
                      !
                      ! endl
                      break
                 case 2:
                      if size < 0
                         ! "ERROR: Negative value"
                      else if size == 0
                         ! "ERROR: Zero value"
                      else
                         ! "size=" size " passed test"
                      !
                      break
                 case 3:
                      !! "for up:"
                      for x size
                          !! x
                      !
                      !! "for down:"
                      rof x size
                         !! x
                      !
                      !! "for up:"
                      for x 2 size
                          !! x
                      !
                      !! "for down:"
                      rof x size 2
                         !! x
                      !
                      !
                      break
                 case 4:
                      ! "while up:"
                      x = 0
                      while x++ < size
                            !! x
                      !
                      ! "while down:"
                      x = size
                      while x-- > 0
                            !! x
                      !
                      !
                      break
                 case 5:
                      sum, _, mult = operations(20, 10)
                      ! "sum=" sum 
                      ! "mult=" mult
                      !
                      break
                 case 6:
                      arr = genArray(size, maxVal)
					  
                      max, min = findBiggestSmallest(arr, size)
                      
                      !! "Array:"
                      for i = 0; i < size; i++
                          !! arr[i]
                      !
                      ! "Biggest number:" max
                      ! "Smallest number:" min
                      !
                      break
                 default:
                      ! "\nERROR: Invalid option.\n"
        while ( option != 0 );
        !
