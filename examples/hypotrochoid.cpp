/**************************************************
https://en.wikipedia.org/wiki/Parametric_equation
**************************************************/
#include <cmath>

#include "../include/Image.hpp"


int main()
{
	Image img(800,800);
	
	double a,b,c,d; //function constants
	int j,k; //function exponents
	
	//parameters configuration:
	j=3; k=3;
//	a=80; b=1; c=80; d=1;
	a=80; b=1; c=1; d=80;
	
	double t=0; //variable
	int mul = 200, offset = 400;
	while(t<10){
		double x = cos(a*t)-pow(cos(b*t),j);
		double y = sin(c*t)-pow(sin(d*t),k);
		img.set_pixel(mul*x+offset, mul*y+offset, {255,255,255});
		t += 0.000001;
	}
	
	img.save_bmp("hypotrochoid.bmp");
	
	
	return 0;
}
