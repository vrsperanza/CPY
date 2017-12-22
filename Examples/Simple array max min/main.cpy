#include <iostream>
#include <time.h>
#include <tuple>
using namespace std

genArray(int size, maxVal)
	arr = (int*)malloc(size * sizeof(int));
	for(i = 0; i < size; i++);
		arr[i] = rand()%maxVal
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
	
main()
	srand(time(NULL))
	! "Input array size and max value"
	int size, maxVal
	cin size maxVal
	
	arr = genArray(size, maxVal)
	
	max, min = findBiggestSmallest(arr, size)
	
	cout "Array: "
	for i = 0; i < size; i++
		cout arr[i] " "
	cout endl
	
	! "Biggest number:" max
	! "Smallest number:" min