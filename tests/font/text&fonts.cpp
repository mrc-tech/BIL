#include <iostream>
#include <bitset> // NON FUNZIONANO I BITSET PER DESCRIVERE I FONT. DICE CHE SONO TROPPI BIT PER ENTRARE IN UN INT...
#include <vector>
#include <string>

#include "../../include/Image.hpp"

using namespace std;


struct font {
	int height = 16; //height of the current font in pixels
	int width  = 16; //length of current pixels (PUO` CAMBIARE?, non credo...)
	int N = 96; //number of characters (16*6)
	vector<string> data = {
		"0000000000000000000000000000000000001111111100000000111111110000001100000000110000110000000011000011000000001100001100000000110000111111111111000011111111111100001100000000110000110000000011000011000000001100001100000000110000000000000000000000000000000000", // A
		"0000000000000000000000000000000000111111111100000011111111110000001100000000110000110000000011000011111111110000001111111111000000110000000011000011000000001100001100000000110000110000000011000011111111110000001111111111000000000000000000000000000000000000", // B
		"0000000000000000000000000000000000001111111100000000111111110000001100000000110000110000000011000011000000000000001100000000000000110000000000000011000000000000001100000000110000110000000011000000111111110000000011111111000000000000000000000000000000000000", // C
		"0000000000000000000000000000000000111111110000000011111111000000001100000011000000110000001100000011000000001100001100000000110000110000000011000011000000001100001100000011000000110000001100000011111111000000001111111100000000000000000000000000000000000000", // D
		"0000000000000000000000000000000000111111111111000011111111111100001100000000000000110000000000000011111111110000001111111111000000110000000000000011000000000000001100000000000000110000000000000011111111111100001111111111110000000000000000000000000000000000", // E
		"0000000000000000000000000000000000111111111111000011111111111100001100000000000000110000000000000011111111110000001111111111000000110000000000000011000000000000001100000000000000110000000000000011000000000000001100000000000000000000000000000000000000000000", // F
		"0000000000000000000000000000000000001111111100000000111111110000001100000000110000110000000011000011000000000000001100000000000000110000111111000011000011111100001100000000110000110000000011000000111111110000000011111111000000000000000000000000000000000000", // G
		"0000000000000000000000000000000000110000000011000011000000001100001100000000110000110000000011000011111111111100001111111111110000110000000011000011000000001100001100000000110000110000000011000011000000001100001100000000110000000000000000000000000000000000", // H
		"0000000000000000000000000000000000001111111111000000111111111100000000001100000000000000110000000000000011000000000000001100000000000000110000000000000011000000000000001100000000000000110000000000111111111100000011111111110000000000000000000000000000000000", // I
		"0000000000000000000000000000000000000000000011000000000000001100000000000000110000000000000011000000000000001100000000000000110000110000000011000011000000001100001100000000110000110000000011000000111111110000000011111111000000000000000000000000000000000000", // J
		"0000000000000000000000000000000000110000001100000011000000110000001100001100000000110000110000000011111100000000001111110000000000110000110000000011000011000000001100000011000000110000001100000011000000001100001100000000110000000000000000000000000000000000", // K
		"0000000000000000000000000000000000110000000000000011000000000000001100000000000000110000000000000011000000000000001100000000000000110000000000000011000000000000001100000000000000110000000000000011111111111100001111111111110000000000000000000000000000000000", // L
		"0000000000000000000000000000000000110000000011000011000000001100001111000011110000111100001111000011001111001100001100111100110000110000000011000011000000001100001100000000110000110000000011000011000000001100001100000000110000000000000000000000000000000000", // M
		"0000000000000000000000000000000000110000000011000011000000001100001111000000110000111100000011000011001100001100001100110000110000110000110011000011000011001100001100000011110000110000001111000011000000001100001100000000110000000000000000000000000000000000", // N
		"0000000000000000000000000000000000001111111100000000111111110000001100000000110000110000000011000011000000001100001100000000110000110000000011000011000000001100001100000000110000110000000011000000111111110000000011111111000000000000000000000000000000000000", // O
		"0000000000000000000000000000000000111111111100000011111111110000001100000000110000110000000011000011000000001100001100000000110000111111111100000011111111110000001100000000000000110000000000000011000000000000001100000000000000000000000000000000000000000000", // P
		"0000000000000000000000000000000000001111111100000000111111110000001100000000110000110000000011000011000000001100001100000000110000110011000011000011001100001100001100001100110000110000110011000000111111110000000011111111000000000000000000000000000000000000", // Q
		"0000000000000000000000000000000000111111111100000011111111110000001100000000110000110000000011000011000000001100001100000000110000111111111100000011111111110000001100000011000000110000001100000011000000001100001100000000110000000000000000000000000000000000", // R
		"0000000000000000000000000000000000001111111100000000111111110000001100000000000000110000000000000000111111110000000011111111000000000000000011000000000000001100001100000000110000110000000011000000111111110000000011111111000000000000000000000000000000000000", // S
		"0000000000000000000000000000000011111111111111001111111111111100000000110000000000000011000000000000001100000000000000110000000000000011000000000000001100000000000000110000000000000011000000000000001100000000000000110000000000000000000000000000000000000000", // T
		"0000000000000000000000000000000000110000000011000011000000001100001100000000110000110000000011000011000000001100001100000000110000110000000011000011000000001100001100000000110000110000000011000000111111110000000011111111000000000000000000000000000000000000", // U
		"0000000000000000000000000000000000110000000011000011000000001100001100000000110000110000000011000011000000001100001100000000110000110000000011000011000000001100000011000011000000001100001100000000001111000000000000111100000000000000000000000000000000000000", // V
		"0000000000000000000000000000000000110000000011000011000000001100001100000000110000110000000011000011000000001100001100000000110000110000000011000011000000001100001100111100110000110011110011000000110000110000000011000011000000000000000000000000000000000000", // W
		"0000000000000000000000000000000000110000000011000011000000001100000011000011000000001100001100000000001111000000000000111100000000000011110000000000001111000000000011000011000000001100001100000011000000001100001100000000110000000000000000000000000000000000", // X
		"0000000000000000000000000000000011000000000011001100000000001100001100000011000000110000001100000000110011000000000011001100000000000011000000000000001100000000000000110000000000000011000000000000001100000000000000110000000000000000000000000000000000000000", // Y
		"0000000000000000000000000000000000111111111111000011111111111100000000000011000000000000001100000000000011000000000000001100000000000011000000000000001100000000000011000000000000001100000000000011111111111100001111111111110000000000000000000000000000000000", // Z
		
		
		
	};
} BASIC; //16x16 pixels (ZX Spectrum font)





void print_font(Image &img, font f, string text);


int main()
{
//	for(int i=0;i<6;i++) cout << BASIC.data[i]<< endl;
	
	font Consolas;
	Consolas.height = 4; //DEVO CAPIRE ANCORA COME FARE PER DEFINIRE ALTRI FONT OLTRE QUELLO BASIC...
	
	
	Image img(100,100);
	print_font(img, BASIC, "abcdef\nghijkl\nmnopqr\nstuvwx\nyz");
	img.save_bmp("text.bmp");
	
	return 0;
}


void print_font(Image &img, font f, string text)
{
	//prints ASCII chars from 32 (space) to 126 (tilde)
	int x0 = 0;
	int y0 = 0;
	int x=x0, y=y0;
	for(int i=0; i<text.length(); i++){
		if(text[i] == '\n') {
			//new line and carriage return
			x = x0;
			y0 += f.height;
			y = y0;
			continue; //SOLUZIONE LEZZA E TEMPORANEA!! (per non far continuare con i cicli for)
		}
		for(int r=0; r<f.height; r++) for(int c=0; c<f.width; c++) if(f.data[text[i]-'a'][r*f.width+c] == '1') img.set_pixel(x+c, y+r, {255,255,255});
		x += f.width;
		y = y0;
	}
}

