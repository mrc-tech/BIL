/*
	creates a bigger version of a BMP file
*/
#include "../include/Image.hpp"
#include "../include/imageFunctions/upSample.h"



int main(int argc, char **argv)
{
	unsigned mul = 5; // multiply coefficient (default = 5)
	Image input;
	std::string fName;
	
	if(argc < 2){
		std::cout << "Bitmap file enlarger - (c) 2021 Marchi Technology" << std::endl;
		std::cout << "Usage: " << argv[0] << " <fileName> <scale>" << std::endl;
		return 0;
	}
	if(argc > 2) mul = atoi(argv[2]);
	fName = std::string(argv[1]);
	
	input.load_bmp(fName);
	
	Image img = upSample(input, mul); // creates an enlarged version of the input image
	
	// prints the grid lines between input pixels:
	for(int y=0; y<input.height(); y++){
		for(int x=0; x<input.width(); x++){
			color c = input.get_pixel(x, y);
			c.R -= 50; c.G -= 50; c.B -= 50; // a bit darker (NAIVE METHOD)
			img.penColor(c);
			img.rect(x*mul, y*mul, (x+1)*mul, (y+1)*mul); // only top and left lines per pixel
		}
	}
	
	fName.insert(fName.length()-4, "_enlarged"); // add the string before the file extension ".BMP"
	img.save_bmp(fName);
	
	return 0;
}
