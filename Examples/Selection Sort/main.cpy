/*
	This is a simple, working, selection sort example
	Here you can see how to work with multiple files and
	imply variables's types, curly brackets, parenthesis and semicolons
	
	To compile this code simply run cpy example.cpy
	
	OBS:
		The -k flag keeps the generated source files after compilation,
			this way you can see how your code looks in c++ (and share your code in c++)!
		The -b flag makes your c++ source code more readable by adding some linebreaks
*/
#include "sort.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std
int * genArray(int size)
	//arr implies it's tipe from atribution, thus becoming int *
	arr = (int*)malloc(sizeof(int) * size)
	
	//i does the same, becoming an integer
	for i size
		arr[i] = rand()%size
	return arr

void printArray(int * arr, int size)
	for i size
		cout << arr[i] << " "
	cout << endl
	
int main(int argc, char ** argv)
	n = 10
	if argc == 2
		n = atoi(argv[1])

	srand(time(NULL))
	arr = genArray(n)
	printArray(arr, n)
	selectionSort(arr, n)
	printArray(arr, n)
	
