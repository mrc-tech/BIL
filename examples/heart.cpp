/**************************************************
https://en.wikipedia.org/wiki/Parametric_equation
**************************************************/
#include <cmath>

#include "../include/Image.hpp"


int main()
{
	Image img(800,800);
	
	
	img.clear({50,0,0});
	
	img.penColor(200,150,150);
//	img.penColor(0,0,0);
	img.penWidth(5);
	
	double t=0; //variable
	int mul = 1200, offset = 400;
	while(t<=3.14159265){
		double x = 1/6.*sin(2*t)*(1+cos(80*t))*(1-1/12.*pow(sin(2*t),8));
		double y = -1/2.*pow(2*t/3.141592-1,2)+1/7.*sin(2*t)*pow(sin( 80*t),3);
		img.drawPoint( mul*x+offset,  -mul*y+offset-250);
		t += 0.000001;
	}
	
	img.penColor(117,21,30); // rosso Sapienza
	img.penWidth(3);
	
	t=0; //variable
	mul = 1200, offset = 400;
	while(t<=3.14159265){
		double x = 1/6.*sin(2*t)*(1+cos(80*t))*(1-1/12.*pow(sin(2*t),8));
		double y = -1/2.*pow(2*t/3.141592-1,2)+1/7.*sin(2*t)*pow(sin( 80*t),3);
		img.drawPoint( mul*x+offset,  -mul*y+offset-250);
		t += 0.000001;
	}
	
	img.save_png("heart.png");
	
	
	return 0;
}
