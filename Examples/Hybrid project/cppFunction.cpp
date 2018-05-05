#include <vector>
#include <cstdlib>
using namespace std;

vector<int> createVector(int maxValue, int elementAmount){
	vector<int> v;
	for(int i = 0; i < elementAmount; i++)
		v.push_back(rand()%maxValue);
	return v;
}