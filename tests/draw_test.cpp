#include "../include/Image.hpp"

using namespace std;



int main()
{
	Image img(100,100); //create image 100x100 pixels
	img.clear({0,0,0}); //set all pixels black
	img.penColor(255,255,255); //set pen color to WHITE
	img.line(10,10,90,90);
	img.rect(10,10,90,90);
	img.circ(50,50,45);
	img.penColor(255,0,0); //set pen color to RED
	img.penWidth(3); //set pen width to 3 pixels
	img.line(40,50,60,50);
	img.line(50,40,50,60);
	
	img.save_bmp("draw_test1.bmp");
	
	img.clear();
	img.bezier(0,0,0,100, 100,100,100,0);
	img.penColor(255,255,255); img.penWidth(1);
	img.bezier(0,0,0,50, 100,100,100,50);
	
	img.save_bmp("draw_test2.bmp");
	
	return 0;
}
