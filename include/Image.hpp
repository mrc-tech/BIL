/****************************************
Simple Image Library (SIL)

Author:  Andrea Marchi (diescc@gmail.com)
version: v0.3 (19/08/2021)
****************************************/
#ifndef MRC_IMAGE_HPP
#define MRC_IMAGE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "imageFormats/Bitmap.h"

typedef  unsigned int   uint;
typedef  unsigned char  byte;


class Image
{
	public:
		struct color {
			byte R; // Red channel
			byte G; // Green channel
			byte B; // Blue channel
		};
		
	private:
		uint W; // width
		uint H; // height
		std::vector<color> data;
		
		unsigned _penWidth;
		color _penColor;
		
	public:
		Image() : W(0), H(0) { _penWidth = 1; _penColor = {0,0,0}; }
		Image(const uint width, const uint height) : W(width), H(height) { data.resize(W*H); clear(); _penWidth = 1; _penColor = {0,0,0}; }
		Image(const Image& image) : W(image.W), H(image.H) { data.resize(W*H); data = image.data; _penWidth = image._penWidth; _penColor = image._penColor; }
		Image(std::string fileName) { load_bmp(fileName); } // IN REALTA` DIPENDE DAL FILE! DEVO FARE UNA ROUTINE CHE IN BASE ALL'ESTENSIONE CARICA UN FOTMATO PIUTTOSTO CHE UN ATRO
		
		//getters:
		inline uint width()  const { return W; }
		inline uint height() const { return H; }
		inline uint size() const { return W*H; }
		inline color get_pixel(const uint x, const uint y) const { if( (x<0) || (y<0) || (x>=static_cast<int>(W)) || (y>=static_cast<int>(H)) ) return {0,0,0}; return data[y*W + x]; }
		
		//setters:
		inline void set_pixel(const uint x, const uint y, const color c) { if( (x<0) || (y<0) || (x>=static_cast<int>(W)) || (y>=static_cast<int>(H)) ) return; data[y*W + x] = c; }
		
		//funcions:
		inline bool operator ! () { return (data.size() == 0) || (W == 0) || (H == 0); } //check if there is some data
		inline void clear(const color value = {0x00,0x00,0x00}) { std::fill(data.begin(), data.end(), value); } //clear the image with byte v
		void resize(int width,int height);
		
		//drawers:
		void drawPoint(int x,int y); //draw a point with internal penWidth and penColor
		void line(int x0,int y0,int x1,int y1); //draw a line
		void rect(int x0,int y0,int x1,int y1); //draw a rectangle
		void circ(int centerx,int centery,int radius); //draw a circle
		void poly(std::vector<int> coords, bool closed); //draw a polyline
		void bezier(int startPtX,int startPtY,int startControlX,int startControlY,int endPtX, int endPtY,int endControlX, int endControlY); //draw a Bezier curve
		void text(int x0,int y0, std::string text, std::vector<std::string> font);
		void ellipse(int centerx, int centery, int a, int b);
		void insertImage(int x,int y, Image img);
		
		inline void penColor(const byte& R,const byte& G,const byte& B) { _penColor = {R,G,B}; }
		inline void penWidth(const uint& width) { _penWidth = width; }
		
		//saveFile:
		void save_bmp(const std::string& fileName) const;
		
		//loadFile:
		void load_bmp(const std::string& fileName);
		
		
	private:
		//auxiliary funcions:
//		inline bool big_endian() const { uint v = 0x01; return (1 != reinterpret_cast<char*>(&v)[0]); }
//		template<typename T> inline void read_from_stream(std::ifstream& stream,T& t) { stream.read(reinterpret_cast<char*>(&t),sizeof(T)); }
//		template<typename T> inline void write_to_stream(std::ofstream& stream,const T& t) const { stream.write(reinterpret_cast<const char*>(&t),sizeof(T)); }
		
		
		
};


typedef Image::color color; // makes "color" a global type

// =================================================================================================================================





void Image::save_bmp(const std::string& fileName) const
{
	Bitmap image(W, H);
	
	for(uint y=0; y<H; y++)
		for(uint x=0; x<W; x++)
			image.set_pixel(x,y, data[y*W+x].R, data[y*W+x].G, data[y*W+x].B);
	
	image.save_file(fileName);
}




void Image::load_bmp(const std::string& fileName)
{
	Bitmap image;
	
	image.load_file(fileName); // load Bitmap file
	
	//save Bitmap data to Image:
	W = image.width();
	H = image.height();
	data.resize(W*H); // allocate memory
	for(uint y=0; y<H; y++)
		for(uint x=0; x<W; x++){
			unsigned char R,G,B;
			image.get_pixel(x,y,  R,G,B);
			this->set_pixel(x,y, {R,G,B});
		}
}




void Image::drawPoint(int x, int y)
{
	switch(_penWidth)
	{
		case 1: set_pixel(x,y, _penColor);
				break;
		case 2: {
				set_pixel(x  ,y  , _penColor);
				set_pixel(x+1,y  , _penColor);
				set_pixel(x+1,y+1, _penColor);
				set_pixel(x  ,y+1, _penColor);
				}
                break;
		case 3: {
				set_pixel(x  ,y-1, _penColor);
				set_pixel(x-1,y-1, _penColor);
				set_pixel(x+1,y-1, _penColor);
				
				set_pixel(x  ,y  , _penColor);
				set_pixel(x-1,y  , _penColor);
				set_pixel(x+1,y  , _penColor);
				
				set_pixel(x  ,y+1, _penColor);
				set_pixel(x-1,y+1, _penColor);
				set_pixel(x+1,y+1, _penColor);
				}
				break;
		default: set_pixel(x,y, _penColor); break;
	}
}



void Image::line(int x0, int y0, int x1, int y1)
{
	bool steep = false;
	int sx = ((x1 - x0) > 0) ? 1 : -1;
	int sy = ((y1 - y0) > 0) ? 1 : -1;
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	
	if (dy > dx){
		std::swap(x0,y0);
		std::swap(dx,dy);
		std::swap(sx,sy);
		steep = true;
	}
	int e = 2 * dy - dx;
	
	for(int i=0; i<dx; ++i){
		if(steep) drawPoint(y0,x0);
		else      drawPoint(x0,y0);
		
		while(e >= 0){
			y0 += sy;
			e -= (dx << 1);
		}
		
		x0 += sx;
		e  += (dy << 1);
	}
	
	drawPoint(x1,y1);
}

void Image::rect(int x0,int y0,int x1,int y1)
{
	line(x0,y0,x1,y0);
	line(x0,y0,x0,y1);
	line(x1,y1,x1,y0);
	line(x1,y1,x0,y1);
}

void Image::circ(int centerx, int centery, int radius)
{
	int x = 0;
	int d = (1 - radius) << 1;
	
	while(radius >= 0){
		drawPoint(centerx + x, centery + radius);
		drawPoint(centerx + x, centery - radius);
		drawPoint(centerx - x, centery + radius);
		drawPoint(centerx - x, centery - radius);
		
		if((d + radius) > 0) d -= ((--radius) << 1) - 1;
		if(x > d) d += ((++x) << 1) + 1;
	}
}


void Image::poly(std::vector<int> coords, bool closed = false)
{
	//draws a polyline stored in the coords vectror in the sequence (x0, y0, x1, y1, x2, y2, ... , xn, yn)
	for(int i=3; i<coords.size(); i+=2) line(coords[i-3], coords[i-2], coords[i-1], coords[i]); //draw the polyline 
	if(closed && (coords.size() > 3)) line(coords[0], coords[1], coords[coords.size()-2], coords[coords.size()-1]); //close the polyline
}


void Image::bezier(int startPtX,int startPtY,int startControlX,int startControlY,int endPtX,int endPtY,int endControlX,int endControlY)
{
	// An implementation of a Bezier curve.
	double cx = 3.0*(startControlX - startPtX);
	double bx = 3.0*(endControlX - startControlX) - cx;
	double ax = double(endPtX - startPtX - cx - bx);
	
	double cy = 3.0*(startControlY - startPtY);
	double by = 3.0*(endControlY - startControlY) - cy;
	double ay = double(endPtY - startPtY - cy - by);
	
	double x = startPtX;
	double y = startPtY;
	
	for(double t=0.0; t<=1.005; t+=0.005){
		double const newx = startPtX + t*(double(cx) + t*(double(bx) + t*(double(ax))));
		double const newy = startPtY + t*(double(cy) + t*(double(by) + t*(double(ay))));
		this->line(int(x),int(y),int(newx),int(newy));
		x = newx;
		y = newy;
	}
}



void Image::text(int x0,int y0, std::string text, std::vector<std::string> font)
{
	//prints ASCII chars from 32 (space) to 126 (tilde)
	int charWidth  = font[0][0];
	int charHeight = font[0][1];
	int x=x0, y=y0; // top-right corner of first character
	for(int i=0; i<text.length(); i++){
		if(text[i] == '\n') {
			//new line and carriage return
			x = x0;
			y0 += charHeight;
			y = y0;
			continue; //SOLUZIONE LEZZA E TEMPORANEA!! (per non far continuare con i cicli for)
		}
		if(text[i]<32 || text[i]>126) continue; //NON MI PIACE MOLTO USARE CONTINUE, MA FORSE E` L'UNICA SOLUZIONE
		for(int r=0; r<charHeight; r++) for(int c=0; c<charWidth; c++) if(font[text[i]-' '+1][r*charWidth+c] == '0') set_pixel(x+c, y+r, _penColor);
		x += charWidth;
		y = y0;
	}
}


void Image::ellipse(int centerx, int centery, int a, int b)
{
	int t1 = a * a;
	int t2 = t1 << 1;
	int t3 = t2 << 1;
	int t4 = b * b;
	int t5 = t4 << 1;
	int t6 = t5 << 1;
	int t7 = a * t5;
	int t8 = t7 << 1;
	int t9 = 0;
	
	int d1 = t2 - t7 + (t4 >> 1);
	int d2 = (t1 >> 1) - t8 + t5;
	int x  = a;
	int y  = 0;
	
	int negative_tx = centerx - x;
	int positive_tx = centerx + x;
	int negative_ty = centery - y;
	int positive_ty = centery + y;
	
	while(d2 < 0){
		drawPoint(positive_tx, positive_ty);
		drawPoint(positive_tx, negative_ty);
		drawPoint(negative_tx, positive_ty);
		drawPoint(negative_tx, negative_ty);
		
		y++;
		t9 = t9 + t3;
		
		if(d1 < 0){
			d1 = d1 + t9 + t2;
			d2 = d2 + t9;
		}else{
			x--;
			t8 = t8 - t6;
			d1 = d1 + (t9 + t2 - t8);
			d2 = d2 + (t9 + t5 - t8);
			negative_tx = centerx - x;
			positive_tx = centerx + x;
		}
		
		negative_ty = centery - y;
		positive_ty = centery + y;
	}
	
	do{
		drawPoint(positive_tx, positive_ty);
		drawPoint(positive_tx, negative_ty);
		drawPoint(negative_tx, positive_ty);
		drawPoint(negative_tx, negative_ty);
		
		x--;
		t8 = t8 - t6;
		
		if(d2 < 0){
			y++;
			t9 = t9 + t3;
			d2 = d2 + (t9 + t5 - t8);
			negative_ty = centery - y;
			positive_ty = centery + y;
		}else d2 = d2 + (t5 - t8);
		
		negative_tx = centerx - x;
		positive_tx = centerx + x;
	}while(x >= 0);
}


void Image::insertImage(int x,int y, Image img)
{
	// insert the "img" Image into the current image at position (x,y)
	for(int r=0; r<img.height(); r++)
		for(int c=0; c<img.width(); c++)
			set_pixel(x+c,y+r, img.get_pixel(c,r));
}


void Image::resize(int width,int height)
{
	// resize the image preserving the data
	Image img = *this; // copy the image
	
	W = width; // save the new width
	H = height; // save the new height
	data.resize(W*H); // resize the data array
	clear(); // clear the image
	
	insertImage(0,0, img); // insert the old image at position (0,0)
}






// EXTERNAL FUNCTIONS =================================================================================================================================

#include "imageFunctions/upSample.h"
//#include "imageFunctions/downSample.h"
//#include "imageFunctions/AntiAliasing.h"
//#include "imageFunctions/blur.h"

// PALETTE =================================================================================================================================

#include "palettes/palette.hpp"

// FONTS =================================================================================================================================

#include "fonts/ZX_Spectrum_16x16.h"
#include "fonts/IBM_EGA_8x14.h"
#include "fonts/Portfolio_6x8.h"
#include "fonts/Morse_font.h"


#endif

/*****************************************************
funzioni che potrei aggiungere:
 - path : come i path degli SVG, stesso formato.
 - fill : flood_fill da un punto dato un colore, e veder da solo i margini
 - dash : disegna una linea tratteggiata con pattern definibili da una stringa
 
*****************************************************/
