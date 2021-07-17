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

#include "palette.hpp"





#endif
