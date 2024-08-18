/*
	test the writePNG function
	for creating uncompressed PNG images
*/
#include "writePNG.h"


void test_rgb(void)
{
	unsigned char rgb[256 * 256 * 3], *p = rgb;
    
	std::ofstream file("rgb.png", std::ios::binary);
    
	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 256; x++) {
			*p++ = (unsigned char)x;	/* R */
			*p++ = (unsigned char)y;	/* G */
			*p++ = 128;					/* B */
		}
	}
	
	writePNG(file, 256, 256, rgb);
	
	file.close();
}

// --------------------------------------------------------------

int main(void)
{
	test_rgb();
	
	return 0;
}
