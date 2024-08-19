#include <iostream>

using namespace std;

#include "Perlin_noise.h"
#include "../include/Image.hpp"

int main()
{
	Image img(500,500);
	double m = 50.;
	
	for(int x=0; x<img.width(); x++){
		for(int y=0; y<img.height(); y++){
			double noise = Noise2D(x/m, y/m);
			byte c = (byte)(noise*128) + 128;
//			c = (c*5) % 256; // crea delle ondine...
			img.set_pixel(x,y, {c,c,c});
//			img.set_pixel(x,y, copper_colormap[c]); // copper_ hot_ hsv_ jet_ prism_ vga_ yarg_ 
		}
	}
	
	img.save_png("Perlin_noise.png");
		
	return 0;
}
