#include <fstream>

#include "../include/Image.hpp"

using namespace std;


int main()
{
	Image img(1280,640);
	
	img.penColor(25,25,25);
	img.text(385,100, "single header", IBM_EGA_8x14, 5);
	
	
	img.penColor(255,255,255);
	img.text(540,250, "BIL", Portfolio_6x8, 12);
	img.text(410,400, "Basic Image Library", ISO_font, 3);
	
	img.penColor(100,100,100);
	int N = 20;
	int Delta_x = (img.width()-40)/N;
	int Delta_y = (img.height()-40)/N;
	for(int i=0; i<=N; i++){
		int x = i * Delta_x +20;
		int y = i * Delta_y +20;
//		cout << x << " " << y << endl;
		img.line(20, y-1, x, img.height()-21);
	}
	img.penColor(255,255,255);
	img.penWidth(2);
	img.rect(450,200, 830,380);
	img.penWidth(3);
	img.rect(20,20,img.width()-23,img.height()-22);
	
	img.save_png("cover.png");
}
