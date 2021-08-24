#include <cmath> // for the "exp()" function

#include "../include/Image.hpp"

using namespace std;

inline color operator * (double w, color c) { if(w>1) return c; else return {(byte)(w*c.R), (byte)(w*c.G), (byte)(w*c.B)}; }
inline color operator + (color c1, color c2) { return {(byte)(c1.R+c2.R), (byte)(c1.G+c2.G), (byte)(c1.B+c2.B)}; }

color weighted_color(Image img, int x,int y);
Image blur(Image img);


int main()
{
	Image img(300,100); // merged figure
	Image initial(100,100); // initial figure
	Image blurred; // blurred version of the initial image
	
	initial.penColor(255,255,255);
	for(auto i=0; i<10; i++){
		double r = 30*exp(-0.3*i);
		initial.circ(50,50,r);
	}
	
	blurred = blur(initial);
	
	initial.penColor(255,55,55);
	initial.text(0,0, "Initial", IBM_EGA_8x14);
	blurred.penColor(55,55,255);
	blurred.text(0,0, "Blurred", IBM_EGA_8x14);
	
	
	img.insertImage(0,  0, initial);
	img.insertImage(100,0, blurred);
	
	for(auto i=0; i<3; i++) blurred = blur(blurred);
	img.insertImage(200,0, blurred);
	
	img = upSample(img, 4); // makes the image 4 times bigger
	
	img.save_bmp("blurring.bmp");
	
	return 0;
}

//====================================================================================

Image blur(Image img)
{
	Image res = img;
	
	for(auto y=0; y<img.height(); y++){
		for(auto x=0; x<img.width(); x++){
			res.set_pixel(x,y, weighted_color(img, x,y));
		}
	}
	
	return res;
}


color weighted_color(Image img, int x,int y)
{
	/*
		return the weighted color of the pixel at (x,y) based on neighbourgs pixels
		
		+---+---+---+
		| a | b | c |
		+---+---+---+
		| d | m | e | -> res = wm*m  + wa*a + wb*b + ... + wh*h
		+---+---+---+
		| f | g | h |
		+---+---+---+
	*/
	color res;
	
	bool isTop = 1; //if is present the top row (ie a,b,c)
	bool isBot = 1; //if is present the bottom row (ie f,g,h)
	bool isLeft  = 1; //if is present the left column (ie a,d,f)
	bool isRight = 1; //if is present the right column (ie c,e,h)
	if(x <= 0) isLeft = 0;
	if(x >= img.width()-1) isRight = 0;
	if(y <= 0) isTop = 0;
	if(y >= img.height()-1) isBot = 0;
	
	
	double wm=0.4, wa=0.05, wb=0.1, wc=0.05, wd=0.1, we=0.1, wf=0.05, wg=0.1, wh=0.05;
	
	color m = img.get_pixel(x  ,y  ); // center
	color a = img.get_pixel(x-1,y-1); // top left
	color b = img.get_pixel(x  ,y-1); // top
	color c = img.get_pixel(x+1,y-1); // top right
	color d = img.get_pixel(x-1,y  ); // left
	color e = img.get_pixel(x+1,y  ); // right
	color f = img.get_pixel(x-1,y+1); // bottom left
	color g = img.get_pixel(x  ,y+1); // bottom
	color h = img.get_pixel(x+1,y+1); // bottom right
	
	res = (wa*isTop*isLeft)*a + (wb*isTop)*b + (wc*isTop*isLeft)*c + 
		  (wd*isLeft)*d       + wm*m         + (we*isRight)*e      + 
		  (wf*isBot*isLeft)*f + (wg*isBot)*g + (wh*isBot*isRight)*h;
	
	return res;
}
