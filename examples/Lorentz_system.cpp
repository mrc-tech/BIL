#include <iostream>
#include <vector>

#include "../include/Image.hpp"

using namespace std;


int main()
{
	Image img(800,800);
	
	//system parameters:
	double x,y,z; //point coordinates
	double a,b,c; //system parameters (often called sigma, rho, beta)
	
	//initialize system:
	x = 1;  y = 0;  z = 0;
	a = 10; b = 28; c = 8/3.;
	
	//calculate trajectory:
	vector<int> coords;
	double t = 0; //indipendent variable (time)
	double Dt = 0.01; //timestep (Euler explicit method stability is sensible to Dt)
	while(t < 50){
		//solve the non-linear differential equations (Euler explicit method):
		x += (a * (y-x)) * Dt;
		y += (x * (b-z) - y) * Dt;
		z += (x*y - c*z) * Dt;
		
//		coords.push_back(12*y+400); coords.push_back(400-12*x); //save x-y palne projection
		coords.push_back(15*x+400); coords.push_back(800-15*z); //save x-z palne projection
		
		cout << "\rt = " << t << " s  : (" << x << " " << y << " " << z << ")            "; //slows down the simulation!
		
		t += Dt; //increment time
	}
	
	//draw trajectory to image:
	img.penColor(255,255,255);
	img.poly(coords);
	
	//writes system info:
	img.penColor(150,150,150);
	img.text(0,0, "dx/dt = a*(y-x) \ndy/dt = x*(b-z)-y \ndz/dt = x*y-c*z \n\n", IBM_EGA_8x14);
	char buf[256];
	sprintf(buf,"a=%.1f, b=%.1f, c=%.1f",a,b,c);
	img.text(0,14*4, buf, IBM_EGA_8x14);
	sprintf(buf,"initial state: (1, 0, 0)");
	img.text(0,14*5, buf, IBM_EGA_8x14);
	
	img.penColor(80,80,80);
	img.text(1,img.height()-8-1, "Copyright (c) 2021 Marchi Technology", Portfolio_6x8);
	
	img.save_png("Lorentz_system.png");
	
	
	return 0;
}
