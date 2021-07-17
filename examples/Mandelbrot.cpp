#include <cmath>

#include "../include/Image.hpp"




int main()
{
	Image fractal(800,600);
	fractal.clear();
	
	double cr,    ci;
	double nextr, nexti;
	double prevr, previ;
	
	const unsigned int max_iterations = 1024;
	
	for(int y=0; y<fractal.height(); y++){
		for(int x=0; x<fractal.width(); x++){
			cr = 1.5 *(2.0 * x / fractal.width()  - 1.0) - 0.5;
			ci =      (2.0 * y / fractal.height() - 1.0);
			
			nextr = nexti = 0;
			prevr = previ = 0;
			
			for(int i=0; i<max_iterations; i++){
				prevr = nextr;
				previ = nexti;
				
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
