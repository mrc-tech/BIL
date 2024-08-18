#include <vector>
#include <cmath>

#include "../include/Image.hpp"
#include "../include/utils/interp1.h"
//#include "../include/utils/linspace.h"

using namespace std;

void plot(vector<double> x, vector<double> y, Image &img);


int main()
{
	Image img(500,300);
	
	vector<double> x = {1,2,3,4,5,6,7,8,9};
	vector<double> y = {0,1,1,2,1,5,4,3,2};
	
	plot(x,y, img);
	
	img.penColor(0,0,255); img.penWidth(1);
	img.rect(0,0, img.width()-1,img.height()-1); // draw image edges
	
	img.save_png("plot1.png");
	
	
	return 0;
}

//____________________________________________________________________________________

void plot(vector<double> x, vector<double> y, Image &img)
{
	int offset = 20; // pixel offset between image edge and axis
	double xmin = 0;  // x at the beginning of axis
	double xmax = 10; // x at the end of axis
	double ymin = 0;  // y at the beginning of axis
	double ymax = 5;  // y at the end of axis
	
	img.clear(palette_colormap[e_white]); // clear the image white
	
	// plot axis:
	img.penColor(0,0,0); // set the pen to black
	img.penWidth(1); // set the pen width to 2 pixels
	img.line(offset, img.height()-offset, img.width()-offset, img.height()-offset); // X axis
	img.line(offset, img.height()-offset, offset, offset); // Y axis
	
	// plot ticks:
	img.penWidth(1);
	int dim = 5; // tick dimension in pixels
	int Dx = (int)(img.width()-2*offset)/(xmax-xmin);
	for(int ix=offset; ix<=img.width()-offset; ix+=Dx){
		img.line(ix, img.height()-offset, ix, img.height()-offset+dim);
		img.text(ix-3, img.height()-offset+dim, "X", IBM_EGA_8x14);
	}
	int Dy = (int)(img.height()-2*offset)/(ymax-ymin);
	for(int iy=offset; iy<=img.height()-offset; iy+=Dy){
		img.line(offset, iy, offset-dim, iy);
		img.text(offset-dim-8, iy-7, "Y", IBM_EGA_8x14);
	}
	
	// plot grid:
	img.penColor(100,100,100);
	// TO BE COMPLETED...
	
	// plot data:
	img.penColor(255,0,0); // red pen
	img.penWidth(2); // pen width
	vector<int> coords;
	for(int i=0; i<x.size(); i++){
		coords.push_back(interp1({xmin,xmax}, {(double)offset,(double)(img.width()-offset)}  ,x[i]));
		coords.push_back(interp1({ymin,ymax}, {(double)(img.height()-offset),(double)offset} ,y[i]));
	}
	img.poly(coords);
	
//	img.penColor(100,100,100);
//	img.text(0,img.height()-8, "plot1: Copyright (c) 2021 Andrea Marchi", Portfolio_6x8);
}
