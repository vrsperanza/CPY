#include "cppFunction.h"
#include "cpyFunction.h"
#include <vector>
#include <iostream>
using namespace std;

void printVector(vector<int> v){
	for(int i : v)
		cout << i << " ";
	cout << endl;
}

int main(){
	cout << "Input vector size and max value: ";
	tuple<int, int> input = readTwoIntegers();
	
	int size = get<0>(input);
	int maxVal = get<1>(input);
	
	vector<int> v = createVector(maxVal, size);
	cout << "Vector: ";
	printVector(v);
	cout << "Sum: "<< vectorSum(v) << endl;
}