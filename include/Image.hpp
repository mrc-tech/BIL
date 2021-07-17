/***************************************
Image library

Author: Andrea Marchi (diescc@gmail.com)
2021
***************************************/
#ifndef MRC_IMAGE_HPP
#define MRC_IMAGE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Bitmap.h"

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
		
	public:
		Image() : W(0), H(0) {}
		Image(const uint width, const uint height) : W(width), H(height) { data.resize(W*H); }
		Image(const Image& image) : W(image.W), H(image.H) { data.resize(W*H); }
		
		//getters:
		inline uint width()  const { return W; }
		inline uint height() const { return H; }
		inline uint size() const { return W*H; }
		inline color get_pixel(const uint x, const uint y) const { return data[y*W + x]; }
		
		//setters:
		inline void set_pixel(const uint x, const uint y, const color c) { data[y*W + x] = c; }
		
		//funcions:
		inline bool operator ! () { return (data.size() == 0) || (W == 0) || (H == 0); } //check if there is some data
		inline void clear(const color value = {0x00,0x00,0x00}) { std::fill(data.begin(), data.end(), value); } //clear the image with byte v
		
		
		//saveFile:
		void save_bmp(const std::string&) const;
		
		
	private:
		//auxiliary funcions:
		inline bool big_endian() const { uint v = 0x01; return (1 != reinterpret_cast<char*>(&v)[0]); }
		template<typename T> inline void read_from_stream(std::ifstream& stream,T& t) { stream.read(reinterpret_cast<char*>(&t),sizeof(T)); }
		template<typename T> inline void write_to_stream(std::ofstream& stream,const T& t) const { stream.write(reinterpret_cast<const char*>(&t),sizeof(T)); }
		
		
		
};


typedef Image::color color;

// =================================================================================================================================





void Image::save_bmp(const std::string& fileName) const
{
	Bitmap image(W, H);
	
	for(uint y=0; y<H; y++)
		for(uint x=0; x<W; x++)
			image.set_pixel(x,y, data[y*W+x].R, data[y*W+x].G, data[y*W+x].B);
	
	image.save_file(fileName);
}













// PALETTE =================================================================================================================================

enum palette_name{
	e_red,           e_scarlet,      e_vermilion,        e_tangelo,         e_orange,
	e_gamboge,       e_amber,        e_gold,             e_yellow,          e_apple_green,
	e_lime_green,    e_spring_bud,   e_chartreuse_green, e_pistachio,       e_harlequin,
	e_sap_green,     e_green,        e_emerald_green,    e_malachite_green, e_sea_green,
	e_spring_green,  e_aquamarine,   e_turquoise,        e_opal,            e_cyan,
	e_arctic_blue,   e_cerulean,     e_cornflower_blue,  e_azure,           e_cobalt_blue,
	e_sapphire_blue, e_phthalo_blue, e_blue,             e_persian_blue,    e_indigo,
	e_blue_violet,   e_violet,       e_purple,           e_mulberry,        e_heliotrope,
	e_magenta,       e_orchid,       e_fuchsia,          e_cerise,          e_rose,
	e_raspberry,     e_crimson,      e_amaranth,         e_white,           e_black
};

const color palette_colormap[] = {
	{255,   0,   0}, {255,  31,   0}, {255,  63,   0}, {255,  95,   0}, {255, 127,   0},
	{255, 159,   0}, {255, 191,   0}, {255, 223,   0}, {255, 255,   0}, {223, 255,   0},
	{191, 255,   0}, {159, 255,   0}, {127, 255,   0}, { 95, 255,   0}, { 63, 255,   0},
	{ 31, 255,   0}, {  0, 255,   0}, {  0, 255,  31}, {  0, 255,  63}, {  0, 255,  95},
	{  0, 255, 127}, {  0, 255, 159}, {  0, 255, 191}, {  0, 255, 223}, {  0, 255, 255},
	{  0, 223, 255}, {  0, 191, 255}, {  0, 159, 255}, {  0, 127, 255}, {  0,  95, 255},
	{  0,  63, 255}, {  0,  31, 255}, {  0,   0, 255}, { 31,   0, 255}, { 63,   0, 255},
	{ 95,   0, 255}, {127,   0, 255}, {159,   0, 255}, {191,   0, 255}, {223,   0, 255},
	{255,   0, 255}, {255,   0, 223}, {255,   0, 191}, {255,   0, 159}, {255,   0, 127},
	{255,   0,  95}, {255,   0,  63}, {255,   0,  31}, {255, 255, 255}, {  0,   0,   0}
};





#endif
