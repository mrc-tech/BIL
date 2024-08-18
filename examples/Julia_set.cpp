#include <complex>
#include <cmath>

#include "../include/Image.hpp"
#include "../include/utils/UTILS.h"



int main()
{
	Image fractal(600,600); //1000x600 is 5/3 ~= 1.667 (circa golden raio 1.618...)
	fractal.clear();
	
	std::complex<double> c; // constant
	std::complex<double> z; // function variable
	
//	c = {-0.8, 0.156}; //classic
//	c = {-0.74543, 0.11301};
//	c = {-0.4, 0.6};
//	c = {-0.835, -0.2321};
	c = {0, -0.8}; // beautiful image
	
	
	const unsigned int max_iterations = 1024;
	const double eps = 5; //maximum allowable magnitude for complex number z
	
	for(int y=0; y<fractal.height(); y++){
		for(int x=0; x<fractal.width(); x++){
			double xmin = -1.5, xmax = 1.5, ymin = -1.5, ymax = 1.5;
			z.real(interp1({0,(double)fractal.width()},  {xmin,xmax}, (double)x)); //maps from {0, width} to {xmin, xmax}
			z.imag(interp1({0,(double)fractal.height()}, {ymax,ymin}, (double)y)); //maps from {0, height} to {ymax, ymin} (reversed axis)
			
			for(int i=0; i<max_iterations; i++){
				z = pow(z,2) + c; // z(n+1) = z(n)^2 + c
				
				if(abs(z) > eps){
//					int index = (int)interp1({0,max_iterations},{0,1000}, (double)i); //linear scale
					int index = (int)interp1({log(1),log(max_iterations+1)},{0,1000}, (double)log(i+1)); //logarithmic scale
					
					fractal.set_pixel(x,y, copper_colormap[index]);
//					fractal.set_pixel(x,y, yarg_colormap[index]);
					break;
				}
			}
		}
	}
	
	fractal.save_bmp("Julia_set.bmp");
	
	return 0;
}
