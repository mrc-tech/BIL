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
	img.text(0,0, "dx/dt = a*(y-x) \ndy/dt = x*(b-x)-y \ndz/dt = x*y-c*z \n\n", ZX_Spectrum_16x16);
	char buf[256];
	sprintf(buf,"a=%.1f, b=%.1f, c=%.1f",a,b,c);
	img.text(0,16*4, buf, ZX_Spectrum_16x16);
	sprintf(buf,"initial state: (1, 0, 0)");
	img.text(0,16*5, buf, ZX_Spectrum_16x16);
	
	img.save_bmp("Lorentz_system.bmp");
	
	
	return 0;
}
