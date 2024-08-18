#include <iostream>

#include "JPEGimage.h"

using namespace std;




int main()
{
	JPEGimage img(10,10);
	
	for(auto i=0; i<10; i++)
		for(auto j=0; j<10; j++)
			img.set_pixel(i,j, 0xFF,0xAD,0x00);
	
	img.save_file("prova.jpg", 100);
	
	return 0;
}
