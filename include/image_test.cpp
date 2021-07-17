#include <iostream>

#include "Image.hpp"

using namespace std;



int main()
{
	Image img(800,600);
	img.clear({0xFF,0xA5,0x00}); //set all pixels orange
	img.save_bmp("prova.bmp");
	
	return 0;
}
