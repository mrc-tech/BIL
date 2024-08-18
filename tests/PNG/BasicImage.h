#ifndef MRC_BASIC_IMAGE
#define MRC_BASIC_IMAGE

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class BasicImage
{
	protected:
		unsigned int _width;
		unsigned int _height;
		
		std::vector<unsigned char> data; // data in array format (array of bytes)
		
		unsigned int bytes_per_pixel;
		unsigned int bytes_per_row;
		
		
	public:
		BasicImage() : _width(0), _height(0), bytes_per_pixel(3) {}
		BasicImage(const std::string fileName) : _width(0), _height(0), bytes_per_pixel(0) { load_file(fileName); } // É NECESSARIO CHE SI SETTINO LE VARIABILI INTERNE??? no se lo fa load_file...
		BasicImage(const unsigned int width, const unsigned int height);
		
		//setters / getters:
		void set_pixel(const unsigned int x, const unsigned int y, const unsigned char red, const unsigned char green, const unsigned char blue);
		void get_pixel(const unsigned int x, const unsigned int y, unsigned char& red, unsigned char& green, unsigned char& blue) const;
		
		// save and load bitmap files
		void save_file(std::string fileName, unsigned char quality);
		void load_file(std::string fileName);
		
		
	protected:
		//auxiliary functions:
		template<typename T> inline void write_to_stream(std::ofstream& stream,const T& t) const { stream.write(reinterpret_cast<const char*>(&t),sizeof(T)); }
		template<typename T> inline void read_from_stream(std::ifstream& stream,T& t) { stream.read(reinterpret_cast<char*>(&t),sizeof(T)); }
		
//		inline bool big_endian() const { unsigned int v = 0x01; return (1 != reinterpret_cast<char*>(&v)[0]); }
//		inline unsigned short flip(const unsigned short& v) const { return ((v >> 8) | (v << 8)); }
//		inline unsigned int flip(const unsigned int& v) const { return ( ((v & 0xFF000000) >> 0x18) | ((v & 0x000000FF) << 0x18) | ((v & 0x00FF0000) >> 0x08) | ((v & 0x0000FF00) << 0x08) ); }
		
};


// CONSTANTS =================================================================================================================================



// DEFINITIONS ================================================================================================================================




BasicImage::BasicImage(const unsigned int width, const unsigned int height)
: _width(width), _height(height), bytes_per_pixel(3)
{
	bytes_per_row = _width * bytes_per_pixel;
	data.resize(_height * bytes_per_row);
	std::fill(data.begin(), data.end(), 0x00);
}






void BasicImage::set_pixel(const unsigned int x, const unsigned int y, const unsigned char red, const unsigned char green, const unsigned char blue)
{
	if( (x<0) || (y<0) || (x>=static_cast<int>(_width)) || (y>=static_cast<int>(_height)) ) return;
	
	// Set pixel color in RGB mode
	const unsigned int y_offset = y * bytes_per_row;
	const unsigned int x_offset = x * bytes_per_pixel;
	const unsigned int offset   = y_offset + x_offset;
	
	data[offset + 0] = red;
	data[offset + 1] = green;
	data[offset + 2] = blue;
}


void BasicImage::get_pixel(const unsigned int x, const unsigned int y, unsigned char& red, unsigned char& green, unsigned char& blue) const
{
	if( (x<0) || (y<0) || (x>=static_cast<int>(_width)) || (y>=static_cast<int>(_height)) ) return;
	
	// Get pixel color in RGB mode
	const unsigned int y_offset = y * bytes_per_row;
	const unsigned int x_offset = x * bytes_per_pixel;
	const unsigned int offset   = y_offset + x_offset;
	
	red   = data[offset + 0];
	green = data[offset + 1];
	blue  = data[offset + 2];
}






void BasicImage::save_file(std::string fileName, unsigned char quality)
{
	// To be overloaded...
}


void BasicImage::load_file(std::string fileName)
{
	// To be overloaded...
}












// AUXILIARY FUNCTIONS ================================================================================================================================


inline std::size_t file_size(const std::string& file_name)
{
	std::ifstream file(file_name.c_str(), std::ios::in | std::ios::binary);
	if (!file) return 0;
	file.seekg(0, std::ios::end);
	return static_cast<std::size_t> (file.tellg());
}




#endif
