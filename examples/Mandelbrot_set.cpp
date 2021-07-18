#include <cmath>

#include "../include/Image.hpp"
#include "../include/utils/UTILS.h"



int main()
{
	Image fractal(1000,600); //1000x600 is 5/3 ~= 1.667 (circa golden raio 1.618...)
	fractal.clear();
	
	double cr,    ci;
	double nextr, nexti;
	double prevr, previ;
	
	const unsigned int max_iterations = 1024;
	
	for(int y=0; y<fractal.height(); y++){
		for(int x=0; x<fractal.width(); x++){
			double ratio = (double) fractal.width() / fractal.height();
			double xmin, xmax = 1, ymin = -1, ymax = 1;
			xmin = xmax - ratio * (ymax-ymin); //comes from keep-aspect-ratio equation: width/height = (xmax-xmin)/(ymax-ymin)
			cr = interp1({0,(double)fractal.width()},  {xmin,xmax}, (double)x); //maps from {0, width} to {xmin, xmax}
			ci = interp1({0,(double)fractal.height()}, {ymax,ymin}, (double)y); //maps from {0, height} to {ymax, ymin} (reversed axis)
			
			// z(0) = 0
			nextr = nexti = 0;
			prevr = previ = 0;
			
			for(int i=0; i<max_iterations; i++){
				prevr = nextr;
				previ = nexti;
				
				// z(n+1) = z(n)^2 + c
				nextr =     prevr*prevr - previ*previ + cr;
				nexti = 2 * prevr*previ + ci;
				
				if((nextr*nextr + nexti*nexti) > 4){
					using namespace std;
					
					const double z = sqrt(nextr*nextr + nexti*nexti);
					
					const unsigned int index = static_cast<unsigned int> (1000.0 * log2(1.75 + i - log2(log2(z))) / log2(max_iterations)); //https://en.wikipedia.org/wiki/Mandelbrot_set#Continuous_.28smooth.29_coloring
					
//					fractal.set_pixel(x,y, yarg_colormap[index]);
					fractal.set_pixel(x,y, copper_colormap[index]);
					
					break;
				}
				
			}
		}
	}
	
	fractal.save_bmp("Mandelbrot_set.bmp");
	
	return 0;
}
