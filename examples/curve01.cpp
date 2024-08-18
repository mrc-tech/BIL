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
	
	double t=-2.5; //variable
	int mul = 60, offset = 400;
	while(t<=2.5){
		double x = 3*cos(cos(7.32*round(t))) * 1.2*(1+cos(16.6*t));
		double y = 3*(pow(sin(16.6*t),2)*sin(7.32*t));
		img.set_pixel( mul*x+offset,  mul*y+offset, {255,255,255});
		img.set_pixel(-mul*x+offset,  mul*y+offset, {255,255,255});
		img.set_pixel( mul*y+offset,  mul*x+offset, {255,255,255});
		img.set_pixel( mul*y+offset, -mul*x+offset, {255,255,255});
		t += 0.000001;
	}
	
	img.save_png("curve01.png");
	
	
	return 0;
}
