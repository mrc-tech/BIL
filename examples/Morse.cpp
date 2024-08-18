#include <iostream>
#include <string>

#include "../include/Image.hpp"

using namespace std;


int main()
{
//	vector<string> font = IBM_EGA_8x14;
	vector<string> font = Morse_font;
	int charWidth  = font[0][0];
	int charHeight = font[0][1];
	
	int nCols; // number of columns
	string text; // text 
	
	cout << "Text: ";
	getline(cin, text);
	cout << "Number of columns: ";
	cin >> nCols;
	
	int nRows = text.size()/nCols+1;
	
	Image img(nCols*charWidth, nRows*charHeight);
	
	img.penColor(255,255,255);
	for(int r=0; r<nRows; r++){
		for(int c=0; c<nCols; c++){
			string temp;
			if((r*nCols+c) >= text.size()) temp = " "; else temp = text[r*nCols+c];
			img.text(c*charWidth, r*charHeight, temp, font);
		}
	}
	
	unsigned scale = 4;
	img = upSample(img, scale);
	img.penColor(50,50,50);
	for(int r=0; r<nRows; r++) img.line(0,r*scale*charHeight, img.width(),r*scale*charHeight);
	for(int c=0; c<nCols; c++) img.line(c*scale*charWidth,0, c*scale*charWidth,img.height());
	
	img.save_png("Morse.png");
	
	return 0;
}
