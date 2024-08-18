#ifndef MRC_BITMAP
#define MRC_BITMAP

#include "BasicImage.h"

#include <iostream>
#include <fstream>
#include <cstring> // for "memset"
#include <string>
#include <vector>


class Bitmap : public BasicImage
{
	public:
		
		enum channel_mode { rgb_mode = 0, bgr_mode = 1 };
		
		struct bitmap_file_header {
			unsigned short type;
			unsigned int   size;
			unsigned short reserved1;
			unsigned short reserved2;
			unsigned int   off_bits;
			
			unsigned int struct_size() const { return sizeof(type) + sizeof(size) + sizeof(reserved1) + sizeof(reserved2) + sizeof(off_bits); }
			
			void clear() { std::memset(this, 0x00, sizeof(bitmap_file_header)); }
		};
		
		struct bitmap_information_header {
			unsigned int   size;
			unsigned int   width;
			unsigned int   height;
			unsigned short planes;
			unsigned short bit_count;
			unsigned int   compression;
			unsigned int   size_image;
			unsigned int   x_pels_per_meter;
			unsigned int   y_pels_per_meter;
			unsigned int   clr_used;
			unsigned int   clr_important;
			
			unsigned int struct_size() const { return sizeof(size) + sizeof(width) + sizeof(height) + sizeof(planes) + sizeof(bit_count) + sizeof(compression) + sizeof(size_image) + sizeof(x_pels_per_meter) + sizeof(y_pels_per_meter) + sizeof(clr_used) + sizeof(clr_important); }
			
			void clear(){ std::memset(this, 0x00, sizeof(bitmap_information_header)); }
		};
		
	public:
//		using BasicImage::BasicImage; // inherit constructors (C++11)
		Bitmap() { _width=0; _height=0; bytes_per_pixel=0; }
		Bitmap(const std::string fileName) { _width=0; _height=0; bytes_per_pixel=0; load_file(fileName); } // É NECESSARIO CHE SI SETTINO LE VARIABILI INTERNE??? no se lo fa load_file...
		Bitmap(const unsigned int width, const unsigned int height);
		
		void save_file(std::string fileName); // overload
		void load_file(std::string fileName); // overload
		
	private:
		bitmap_file_header 			bfh;
		bitmap_information_header	bih;
		
		// auxilioary functions: QUESTE FUNZIONI FORSE SONO SIMILI A QUELLE IN png_utils.h (CONTROLLARE E SISTEMARE!!!!!!!!!!!!!!!)
		inline bool big_endian() const { unsigned int v = 0x01; return (1 != reinterpret_cast<char*>(&v)[0]); }
		inline unsigned short flip(const unsigned short& v) const { return ((v >> 8) | (v << 8)); }
		inline unsigned int flip(const unsigned int& v) const { return ( ((v & 0xFF000000) >> 0x18) | ((v & 0x000000FF) << 0x18) | ((v & 0x00FF0000) >> 0x08) | ((v & 0x0000FF00) << 0x08) ); }
		void read_bfh(std::ifstream& stream, bitmap_file_header& bfh);
		void write_bfh(std::ofstream& stream, const bitmap_file_header& bfh) const;
		void read_bih(std::ifstream& stream,bitmap_information_header& bih);
		void write_bih(std::ofstream& stream, const bitmap_information_header& bih) const;
		std::size_t file_size(const std::string& file_name) const;
		
};


// =================================================================================================================================


Bitmap::Bitmap(const unsigned int width, const unsigned int height)
{
	_width = width;
	_height = height;
	bytes_per_pixel = 3; // un byte per ogni componente RGB
	bytes_per_row = _width * bytes_per_pixel;
	data.resize(_height * bytes_per_row);
}







void Bitmap::save_file(std::string fileName)
{
	std::ofstream file(fileName.c_str(),std::ios::binary);
	
	if (!file){
		std::cerr << "bitmap_image::save_image(): Error - Could not open file " << fileName << " for writing!" << std::endl;
		return;
	}
	
	bitmap_information_header bih;
	bih.width            = _width;
	bih.height           = _height;
	bih.bit_count        = static_cast<unsigned short>(bytes_per_pixel << 3);
	bih.clr_important    = 0;
	bih.clr_used         = 0;
	bih.compression      = 0;
	bih.planes           = 1;
	bih.size             = bih.struct_size();
	bih.x_pels_per_meter = 0;
	bih.y_pels_per_meter = 0;
	bih.size_image       = (((bih.width * bytes_per_pixel) + 3) & 0x0000FFFC) * bih.height;
	
	bitmap_file_header bfh;
	bfh.type             = 19778;
	bfh.size             = bfh.struct_size() + bih.struct_size() + bih.size_image;
	bfh.reserved1        = 0;
	bfh.reserved2        = 0;
	bfh.off_bits         = bih.struct_size() + bfh.struct_size();
	
	write_bfh(file, bfh);
	write_bih(file, bih);
	
	unsigned int padding = (4 - ((3 * _width) % 4)) % 4;
	char padding_data[4] = { 0x00, 0x00, 0x00, 0x00 };
	
	for(unsigned int i=0; i<_height; i++){
		const unsigned char* data_ptr = &data[(bytes_per_row * (_height - i - 1))];
		file.write(reinterpret_cast<const char*>(data_ptr), sizeof(unsigned char) * bytes_per_pixel * _width);
		file.write(padding_data, padding);
	}
	
	file.close();
}

void Bitmap::load_file(std::string fileName)
{
	std::ifstream stream(fileName.c_str(), std::ios::binary);
	
	if(!stream){
		std::cerr << "Bitmap::load_file() ERROR: file " << fileName << " not found!" << std::endl;
		return;
	}
	
	_width  = 0;
	_height = 0;
	
	bitmap_file_header bfh;
	bitmap_information_header bih;
	
	bfh.clear();
	bih.clear();
	read_bfh(stream,bfh);
	read_bih(stream,bih);
	
    if(bfh.type != 19778){
		std::cerr << "Bitmap::load_file() ERROR: Invalid type value " << bfh.type << " expected 19778." << std::endl;
		bfh.clear(); bih.clear();
		stream.close();
		return;
	}
	
	if(bih.bit_count != 24){
		std::cerr << "Bitmap::load_file() ERROR: Invalid bit depth " << bih.bit_count << " expected 24." << std::endl;
		bfh.clear(); bih.clear();
		stream.close();
		return;
	}
	
	if(bih.size != bih.struct_size()){
		std::cerr << "Bitmap::load_file() ERROR: Invalid BIH size " << bih.size << " expected " << bih.struct_size() << std::endl;
		bfh.clear(); bih.clear();
		stream.close();
		return;
	}
	
	_width  = bih.width;
    _height = bih.height;
    
	bytes_per_pixel = bih.bit_count >> 3;
	bytes_per_row = _width * bytes_per_pixel;
	
	unsigned int padding = (4 - ((3 * _width) % 4)) % 4;
	char padding_data[4] = { 0x00, 0x00, 0x00, 0x00 };
	
	std::size_t bitmap_file_size = file_size(fileName);
	std::size_t bitmap_logical_size = (_height * bytes_per_row) + (_height * padding) + bih.struct_size() + bfh.struct_size();
	if(bitmap_file_size != bitmap_logical_size){
		std::cerr << "Bitmap::load_file() ERROR: Mismatch between logical and physical sizes of bitmap. Logical: " << bitmap_logical_size << " Physical: " << bitmap_file_size << std::endl;
		bfh.clear(); bih.clear();
		stream.close();
		return;
	}
    
	data.resize(_height * bytes_per_row); // allocate memory
	
	for(unsigned i=0; i<_height; i++){
		unsigned char* data_ptr = &data[((_height-i-1) * bytes_per_row)]; // read in inverted row order
		
		stream.read(reinterpret_cast<char*>(data_ptr), sizeof(char) * bytes_per_row);
		stream.read(padding_data, padding);
	}
}







void Bitmap::read_bfh(std::ifstream& stream, bitmap_file_header& bfh)
{
	read_from_stream(stream, bfh.type     );
	read_from_stream(stream, bfh.size     );
	read_from_stream(stream, bfh.reserved1);
	read_from_stream(stream, bfh.reserved2);
	read_from_stream(stream, bfh.off_bits );
	
	if (big_endian()){
		bfh.type      = flip(bfh.type     );
		bfh.size      = flip(bfh.size     );
		bfh.reserved1 = flip(bfh.reserved1);
		bfh.reserved2 = flip(bfh.reserved2);
		bfh.off_bits  = flip(bfh.off_bits );
	}
}

void Bitmap::write_bfh(std::ofstream& stream, const bitmap_file_header& bfh) const
{
	if(big_endian()){
		write_to_stream(stream,flip(bfh.type     ));
		write_to_stream(stream,flip(bfh.size     ));
		write_to_stream(stream,flip(bfh.reserved1));
		write_to_stream(stream,flip(bfh.reserved2));
		write_to_stream(stream,flip(bfh.off_bits ));
	}else{
		write_to_stream(stream,bfh.type     );
		write_to_stream(stream,bfh.size     );
		write_to_stream(stream,bfh.reserved1);
		write_to_stream(stream,bfh.reserved2);
		write_to_stream(stream,bfh.off_bits );
	}
}

void Bitmap::read_bih(std::ifstream& stream,bitmap_information_header& bih)
{
	read_from_stream(stream,bih.size            );
	read_from_stream(stream,bih.width           );
	read_from_stream(stream,bih.height          );
	read_from_stream(stream,bih.planes          );
	read_from_stream(stream,bih.bit_count       );
	read_from_stream(stream,bih.compression     );
	read_from_stream(stream,bih.size_image      );
	read_from_stream(stream,bih.x_pels_per_meter);
	read_from_stream(stream,bih.y_pels_per_meter);
	read_from_stream(stream,bih.clr_used        );
	read_from_stream(stream,bih.clr_important   );
	
	if(big_endian()){
		bih.size          = flip(bih.size               );
		bih.width         = flip(bih.width              );
		bih.height        = flip(bih.height             );
		bih.planes        = flip(bih.planes             );
		bih.bit_count     = flip(bih.bit_count          );
		bih.compression   = flip(bih.compression        );
		bih.size_image    = flip(bih.size_image         );
		bih.x_pels_per_meter = flip(bih.x_pels_per_meter);
		bih.y_pels_per_meter = flip(bih.y_pels_per_meter);
		bih.clr_used      = flip(bih.clr_used           );
		bih.clr_important = flip(bih.clr_important      );
	}
}

void Bitmap::write_bih(std::ofstream& stream, const bitmap_information_header& bih) const
{
	if(big_endian()){
		write_to_stream(stream,flip(bih.size            ));
		write_to_stream(stream,flip(bih.width           ));
		write_to_stream(stream,flip(bih.height          ));
		write_to_stream(stream,flip(bih.planes          ));
		write_to_stream(stream,flip(bih.bit_count       ));
		write_to_stream(stream,flip(bih.compression     ));
		write_to_stream(stream,flip(bih.size_image      ));
		write_to_stream(stream,flip(bih.x_pels_per_meter));
		write_to_stream(stream,flip(bih.y_pels_per_meter));
		write_to_stream(stream,flip(bih.clr_used        ));
		write_to_stream(stream,flip(bih.clr_important   ));
	}else{
		write_to_stream(stream,bih.size            );
		write_to_stream(stream,bih.width           );
		write_to_stream(stream,bih.height          );
		write_to_stream(stream,bih.planes          );
		write_to_stream(stream,bih.bit_count       );
		write_to_stream(stream,bih.compression     );
		write_to_stream(stream,bih.size_image      );
		write_to_stream(stream,bih.x_pels_per_meter);
		write_to_stream(stream,bih.y_pels_per_meter);
		write_to_stream(stream,bih.clr_used        );
		write_to_stream(stream,bih.clr_important   );
	}
}


std::size_t Bitmap::file_size(const std::string& file_name) const
{
	std::ifstream file(file_name.c_str(),std::ios::in | std::ios::binary);
	if (!file) return 0;
	file.seekg (0, std::ios::end);
	return static_cast<std::size_t>(file.tellg());
}




#endif
