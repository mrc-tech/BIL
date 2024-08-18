/************************************
Prints all 7-bits US-ASCII characters
there are 96 printable ASCII chars
************************************/
#include <string>
#include <vector>

#include "../include/Image.hpp"

int main()
{
//	std::vector<std::string> font = ZX_Spectrum_16x16;
	std::vector<std::string> font = IBM_EGA_8x14;
	
	int cW = font[0][0]; // font width
	int cH = font[0][1]; // font height

	Image img(16*cW,(8-2)*cH);
	
	img.penColor(255,255,255);
	for(int y=0; y<(8-2); y++){
		for(int x=0; x<16; x++){
			std::string str = " "; str[0] = (char)((y+2)*16+x);
			img.text(x*cW, y*cH, str, font);
		}
	}
	img.save_bmp("text.bmp");
	
	return 0;
}
