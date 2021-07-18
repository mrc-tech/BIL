#include "../include/Image.hpp"

using namespace std;



int main()
{
	Image img(400,300);
	img.clear({0xFF,0xA5,0x00}); //set all pixels orange (0xFFA500)
	img.save_bmp("image_test1.bmp");
	
	return 0;
}
