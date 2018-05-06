#include <iostream>
#include <time.h>
#include <tuple>
using namespace std

genArray(int size, maxVal)
	arr = (int*)malloc(size * sizeof(int));
	for i size
		arr[i] = rand()%maxVal
	return arr

findBiggestSmallest(int * arr, int size)
	max = arr[0]
	min = arr[0];
	
	for i size
		if arr[i] > max
			max = arr[i]
		
		if arr[i] < min
			min = arr[i]
		
	return max, min
	
main()
	srand(time(NULL))
	int size, maxVal
	? "Input array size and max value: " size maxVal
	
	arr = genArray(size, maxVal)
	
	max, min = findBiggestSmallest(arr, size)
	
	!! "Array:"
	for i = 0; i < size; i++
		!! arr[i]
	!
	
	! "Biggest number:" max
	! "Smallest number:" min