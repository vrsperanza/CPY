#include <iostream>
#include <time.h>
using namespace std

genArray(int size, maxVal)
	arr = (int*)malloc(size * sizeof(int));
	for(i = 0; i < size; i++);
		arr[i] = rand()%maxVal
	return arr

findBiggest(int * arr, int size)
	biggest = arr[0]
	for i = 0; i < size; i++
		if arr[i] > biggest
			biggest = arr[i]
	return biggest

findSmallest(int * arr, int size)
	smallest = arr[0]
	for i = 0; i < size; i++
		if(arr[i] < smallest)
			smallest = arr[i]
	return smallest

main()
	srand(time(NULL))
	! "Input array size and max value"
	int size, maxVal
	cin size maxVal
	
	arr = genArray(size, maxVal)
	
	biggest = findBiggest(arr, size)
	smallest = findSmallest(arr, size)
	
	cout "Array: "
	for i = 0; i < size; i++
		cout arr[i] " "
	cout endl
	
	! "Biggest number:" biggest
	! "Smallest number:" smallest