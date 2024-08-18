#include "../include/Image.hpp"

int main()
{
	Image img(100,100);
	
	img.penColor(255,255,255);
	img.text(0,0, "abcdef\nghijkl\nmnopqr\nstuvwx\nyz", ZX_Spectrum_16x16);
	
	img.save_bmp("text_test.bmp");
	
	return 0;
}
