#include <iostream>
using namespace std;

class Rectangle
	public:
	int width, height;
    void set_values (int, int)
    int area()
		return width * height

void Rectangle::set_values (int x, y)
	//In this case, width and height are declared inside the Rectangle scope, thus not directly visible
	//known tag is to be used when you don't want an assignment to declare a diferent variable in it's first appearence
	known width, height
	width = x
	height = y

int main ()
	Rectangle rect
	
	int w, h
	
	? "Input rectangle width: " w
	? "Input rectangle height: " h
	rect.set_values(w, h)
	! "Rectangle area =" rect.area()
	return 0