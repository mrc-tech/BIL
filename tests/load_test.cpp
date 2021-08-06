#include <iostream>

#include "../include/Image.hpp"

using namespace std;


int main()
{
	Image img;
	
	img.load_bmp("draw_test2.bmp");
	
	cout << img.width() << "x" << img.height() << endl;
	
	img.penColor(255,255,255);
	img.line(img.width(),0, 0,img.height());
	img.penWidth(2);
	img.ellipse(img.width()/2, img.height()/2, img.width()/3, img.height()/2);
	
	img.save_bmp("draw_test2_modified.bmp");
	
	
	return 0;
}
