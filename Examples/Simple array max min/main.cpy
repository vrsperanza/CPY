#include <iostream>
#include <time.h>
using namespace std

int * genArray(int size, maxVal)
	int * arr = (int*)malloc(size * sizeof(int));
	for(int i = 0; i < size; i++);
		arr[i] = rand()%maxVal
	return arr

int findBiggest(int * arr, int size)
	int biggest = arr[0]
	for int i = 0; i < size; i++
		if arr[i] > biggest
			biggest = arr[i]
	return biggest

int findSmallest(int * arr, int size)
	int smallest = arr[0]
	for int i = 0; i < size; i++
		if(arr[i] < smallest)
			smallest = arr[i]
	return smallest

int main()
	srand(time(NULL))
	! "Input array size and max value"
	int size, maxVal
	cin size maxVal
	
	int * arr = genArray(size, maxVal)
	
	int biggest = findBiggest(arr, size)
	int smallest = findSmallest(arr, size)
	
	cout "Array: "
	for i = 0; i < size; i++
		cout arr[i] " "
	cout endl
	
	! "Biggest number:" biggest
	! "Smallest number:" smallest