/****************************************
Basic Image Library (BIL)

2021-2024
Author:  Andrea Marchi (diescc@gmail.com)
version: v1.0 (19/08/2024)
****************************************/
#ifndef MRC_IMAGE_HPP
#define MRC_IMAGE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


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
		inline unsigned int width()  const { return _width;  }
		inline unsigned int height() const { return _height; }
		
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




#endif // MRC_BASIC_IMAGE


#ifndef MRC_BITMAP
#define MRC_BITMAP


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




#endif // MRC_BITMAP








/************************************************************
ZLIB

ToDo:
 - Mettere tutto dentro una struct? con funzioni private e 
   pubbliche, in modo tale che è semplice capire l'utilizzo
   della libreria (ad esempio zlib.deflate() / zlib.inflate())
***********************************************************/
#ifndef MRC_ZLIB
#define MRC_ZLIB

//#include <vector> ?????
#include <assert.h>
#include <string.h>

#define HASH_SIZE 16384

//#define STBIW_UCHAR(x) (unsigned char) ((x) & 0xff)

#define stbiw__sbraw(a) ((int *) (void *) (a) - 2)
#define stbiw__sbm(a)   stbiw__sbraw(a)[0]
#define stbiw__sbn(a)   stbiw__sbraw(a)[1]

#define stbiw__sbneedgrow(a,n)  ((a)==0 || stbiw__sbn(a)+n >= stbiw__sbm(a))
#define stbiw__sbmaybegrow(a,n) (stbiw__sbneedgrow(a,(n)) ? stbiw__sbgrow(a,n) : 0)
#define stbiw__sbgrow(a,n)  stbiw__sbgrowf((void **) &(a), (n), sizeof(*(a)))

#define stbiw__sbpush(a, v)      (stbiw__sbmaybegrow(a,1), (a)[stbiw__sbn(a)++] = (v))
#define stbiw__sbcount(a)        ((a) ? stbiw__sbn(a) : 0)
#define stbiw__sbfree(a)         ((a) ? STBIW_FREE(stbiw__sbraw(a)),0 : 0)

#define stbiw__zlib_flush() (out = stbiw__zlib_flushf(out, &bitbuf, &bitcount))
#define stbiw__zlib_add(code,codebits)  (bitbuf |= (code) << bitcount, bitcount += (codebits), stbiw__zlib_flush())
// default huffman tables
#define stbiw__zlib_huffa(b,c)  stbiw__zlib_add(stbiw__zlib_bitrev(b,c),c)
#define stbiw__zlib_huff1(n)  stbiw__zlib_huffa(0x30 + (n), 8)
#define stbiw__zlib_huff2(n)  stbiw__zlib_huffa(0x190 + (n)-144, 9)
#define stbiw__zlib_huff3(n)  stbiw__zlib_huffa(0 + (n)-256,7)
#define stbiw__zlib_huff4(n)  stbiw__zlib_huffa(0xc0 + (n)-280,8)
#define stbiw__zlib_huff(n)  ((n) <= 143 ? stbiw__zlib_huff1(n) : (n) <= 255 ? stbiw__zlib_huff2(n) : (n) <= 279 ? stbiw__zlib_huff3(n) : stbiw__zlib_huff4(n))
#define stbiw__zlib_huffb(n) ((n) <= 143 ? stbiw__zlib_huff1(n) : stbiw__zlib_huff2(n))

#define stbiw__wpng4(o,a,b,c,d) ((o)[0]=(unsigned char)(a & 0xFF),(o)[1]=(unsigned char)(b & 0xFF),(o)[2]=(unsigned char)(c & 0xFF),(o)[3]=(unsigned char)(d & 0xFF),(o)+=4)
#define stbiw__wp32(data,v) stbiw__wpng4(data, (v)>>24,(v)>>16,(v)>>8,(v));
#define stbiw__wptag(data,s) stbiw__wpng4(data, s[0],s[1],s[2],s[3])

#define STBIW_REALLOC(p,newsz)  realloc(p,newsz)
#define STBIW_REALLOC_SIZED(p,oldsz,newsz) STBIW_REALLOC(p,newsz)

static void *stbiw__sbgrowf(void **arr, int increment, int itemsize)
{
   int m = *arr ? 2*stbiw__sbm(*arr)+increment : increment+1;
   void *p = STBIW_REALLOC_SIZED(*arr ? stbiw__sbraw(*arr) : 0, *arr ? (stbiw__sbm(*arr)*itemsize + sizeof(int)*2) : 0, itemsize * m + sizeof(int)*2);
   assert(p);
   if (p) {
      if (!*arr) ((int *) p)[1] = 0;
      *arr = (void *) ((int *) p + 2);
      stbiw__sbm(*arr) = m;
   }
   return *arr;
}


static unsigned char *stbiw__zlib_flushf(unsigned char *data, unsigned int *bitbuffer, int *bitcount)
{
   while (*bitcount >= 8) {
      stbiw__sbpush(data, (unsigned char)(*bitbuffer & 0xff));
      *bitbuffer >>= 8;
      *bitcount -= 8;
   }
   return data;
}

static unsigned int stbiw__zhash(unsigned char *data)
{
   unsigned int hash = data[0] + (data[1] << 8) + (data[2] << 16);
   hash ^= hash << 3;
   hash += hash >> 5;
   hash ^= hash << 4;
   hash += hash >> 17;
   hash ^= hash << 25;
   hash += hash >> 6;
   return hash;
}

static unsigned int stbiw__zlib_countm(unsigned char *a, unsigned char *b, int limit)
{
   int i;
   for (i=0; i < limit && i < 258; ++i) if (a[i] != b[i]) break;
   return i;
}

static int stbiw__zlib_bitrev(int code, int codebits)
{
   int res=0;
   while (codebits--) {
      res = (res << 1) | (code & 1);
      code >>= 1;
   }
   return res;
}


unsigned int zlib_crc32(unsigned char *buffer, int len)
{
	// CRC32B:
	// Reflected: 0xEDB88320: &1 >> [128] = poly , [ 8] = poly >> 8 VALID
	// Poly: x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1 [0xEDB88320]
	static unsigned int crc_table[256] = {
		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
		0x0eDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
		0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
		0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
		0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
		0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
		0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
		0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
		0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
		0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
		0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
		0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
		0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
		0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
		0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
		0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
		0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
		0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
		0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
		0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
		0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
		0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
		0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
		0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
		0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
		0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
		0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
		0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
		0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
		0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
		0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
		0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
	};
	
	unsigned int crc = ~0u;
	for(int i=0; i<len; i++) crc = (crc >> 8) ^ crc_table[buffer[i] ^ (crc & 0xFF)];
	return ~crc;
}

unsigned zlib_crc32_slow(unsigned char* data, int length)
{
	// lento perché ricalcola ogni volta la tabella dal polinomio
	static unsigned* crcTable;
	unsigned crc; //32 bit unsigned data
	unsigned poly = 0xEDB88320; //polynomial
	if(crcTable == nullptr){
		crcTable=new unsigned[256];
		for(unsigned n=0;n<256;n++){
			crc = n;
			for(auto k=0;k<8;k++){
				if((crc & 1) == 1) crc = poly ^ (crc >> 1);
				else               crc >>= 1;
			}
			crcTable[n] = crc;
		}
	}
	crc = ~0u;
	for(auto i=0; i<length; i++) crc = (crc >> 8) ^ crcTable[data[i] ^ (crc & 0xFF)]; // Reversed
	return ~crc; //same as c ^ 0xFFFFFFFF
}

static void stbiw__wpcrc(unsigned char **data, int len)
{
   unsigned int crc = zlib_crc32(*data - len - 4, len+4);
   stbiw__wp32(*data, crc);
}

static unsigned char stbiw__paeth(int a, int b, int c)
{
   int p = a + b - c, pa = abs(p-a), pb = abs(p-b), pc = abs(p-c);
   if (pa <= pb && pa <= pc) return (unsigned char)(a & 0xFF);
   if (pb <= pc) return (unsigned char)(b & 0xFF);
   return (unsigned char)(c & 0xFF);
}


// =================================================================================



unsigned char * zlib_compress(unsigned char *data, int data_len, int *out_len, int quality)
{
	static unsigned short lengthc[]  = { 3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258, 259 };
	static unsigned char  lengtheb[] = { 0,0,0,0,0,0,0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4,  4,  5,  5,  5,  5,  0 };
	static unsigned short distc[]    = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577, 32768 };
	static unsigned char  disteb[]   = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13 };
	unsigned int bitbuf = 0;
	int bitcount = 0;
	unsigned char *out = nullptr;
//	unsigned char ***hash_table = (unsigned char***) malloc(zlib_ZHASH * sizeof(unsigned char**)); // OLD
	unsigned char ***hash_table = new unsigned char ** [HASH_SIZE * sizeof(unsigned char**)]; // COME MAI TUTTI QUESTI * ??
	if (hash_table == nullptr) return nullptr; // return error if it cannot allocate memory
	if (quality < 5) quality = 5;
	
	stbiw__sbpush(out, 0x78);   // DEFLATE 32K window
	stbiw__sbpush(out, 0x5e);   // FLEVEL = 1
	stbiw__zlib_add(1,1);  // BFINAL = 1
	stbiw__zlib_add(1,2);  // BTYPE = 1 -- fixed huffman
	
	for(int i=0; i < HASH_SIZE; i++) hash_table[i] = nullptr; // reset the hash_table
	
	int count = 0;
	while (count < data_len-3) {
		// hash next 3 bytes of data to be compressed
		int h = stbiw__zhash(data+count)&(HASH_SIZE-1), best=3;
		unsigned char *bestloc = 0;
		unsigned char **hlist = hash_table[h];
		int n = stbiw__sbcount(hlist);
		for(int j=0; j<n; ++j){
			if(hlist[j]-data > count-32768) { // if entry lies within window
				int d = stbiw__zlib_countm(hlist[j], data+count, data_len-count);
				if(d >= best) { best=d; bestloc=hlist[j]; }
			}
		}
		// when hash table entry is too long, delete half the entries
		if(hash_table[h] && stbiw__sbn(hash_table[h]) == 2*quality) {
			memmove(hash_table[h], hash_table[h]+quality, sizeof(hash_table[h][0])*quality);
			stbiw__sbn(hash_table[h]) = quality;
		}
		stbiw__sbpush(hash_table[h],data+count);
		
		if(bestloc){
			// "lazy matching" - check match at *next* byte, and if it's better, do cur byte as literal
			h = stbiw__zhash(data+count+1)&(HASH_SIZE-1);
			hlist = hash_table[h];
			n = stbiw__sbcount(hlist);
			for(int j=0; j<n; ++j){
				if(hlist[j]-data > count-32767) {
					int e = stbiw__zlib_countm(hlist[j], data+count+1, data_len-count-1);
					if(e > best) { // if next match is better, bail on current match
						bestloc = nullptr;
						break;
					}
				}
			}
		}
		
		if(bestloc){
			int temp;
			int d = (int) (data+count - bestloc); // distance back
			assert(d <= 32767 && best <= 258);
			for(temp=0; best > lengthc[temp+1]-1; ++temp);
			stbiw__zlib_huff(temp+257); // SCRIVE DOPO IL FOR???
			if(lengtheb[temp]) stbiw__zlib_add(best - lengthc[temp], lengtheb[temp]);
			for(temp=0; d > distc[temp+1]-1; ++temp);
			stbiw__zlib_add(stbiw__zlib_bitrev(temp,5),5);
			if(disteb[temp]) stbiw__zlib_add(d - distc[temp], disteb[temp]);
			count += best;
		}else{
			stbiw__zlib_huffb(data[count]);
			count++;
		}
	}
	// write out final bytes
	for(;count < data_len; ++count) stbiw__zlib_huffb(data[count]);
	stbiw__zlib_huff(256); // end of block
	// pad with 0 bits to byte boundary
	while(bitcount) stbiw__zlib_add(0,1);
	
	//clear hash_table from memory
//	for (int i=0; i<HASH_SIZE; ++i) delete [] hash_table[i];
	delete [] hash_table;
	
	// store uncompressed instead if compression was worse
	if(stbiw__sbn(out) > data_len + 2 + ((data_len+32766)/32767)*5){
		stbiw__sbn(out) = 2;  // truncate to DEFLATE 32K window and FLEVEL = 1
		for(int j = 0; j < data_len;){
			int blocklen = data_len - j;
			if(blocklen > 32767) blocklen = 32767;
			stbiw__sbpush(out, data_len - j == blocklen); // BFINAL = ?, BTYPE = 0 -- no compression
			stbiw__sbpush(out, (unsigned char)(blocklen & 0xFF)); // LEN
			stbiw__sbpush(out, (unsigned char)((blocklen >> 8) & 0xFF));
			stbiw__sbpush(out, (unsigned char)(~blocklen & 0xFF)); // NLEN
			stbiw__sbpush(out, (unsigned char)((~blocklen >> 8) & 0xFF));
			memcpy(out+stbiw__sbn(out), data+j, blocklen);
			stbiw__sbn(out) += blocklen;
			j += blocklen;
		}
	}
	
	{
		// compute adler32 on input
		unsigned int s1=1, s2=0;
		int blocklen = (int) (data_len % 5552);
		count = 0;
		while (count < data_len) {
			for (int i=0; i < blocklen; i++) { s1 += data[count+i]; s2 += s1; }
			s1 %= 65521; s2 %= 65521; // 65521 : largest prime smaller than 65536
			count += blocklen;
			blocklen = 5552;
		}
		stbiw__sbpush(out, (unsigned char)((s2 >> 8) & 0xFF));
		stbiw__sbpush(out, (unsigned char)(s2 & 0xFF));
		stbiw__sbpush(out, (unsigned char)((s1 >> 8) & 0xFF));
		stbiw__sbpush(out, (unsigned char)(s1 & 0xFF));
	}
	
	*out_len = stbiw__sbn(out);
	
	// make returned pointer freeable
	memmove(stbiw__sbraw(out), out, *out_len);
	return (unsigned char *) stbiw__sbraw(out);
}


#endif // MRC_ZLIB





#ifndef MRC_PNG_UTILS
#define MRC_PNG_UTILS


#include <vector>
#include <fstream>




// Reverse bits to change endianness
template <typename T>
T reverse(T n, unsigned bits_num)
{
    T rv = 0;
REVERSE_BITS:
    for (int i = 0; i < bits_num; i++)
    {
#pragma HLS PIPELINE
        rv <<= 1;
        rv |= (n & 0x01);
        n >>= 1;
    }
    return rv;
}
// usare sizeof(T) invece di bits_num????







inline std::vector<unsigned char> str2bvec(std::string str)
{
	// converte una stringa in un vettore di bytes
	std::vector<unsigned char> res;
	for(int i=0; i<str.length(); i++) res.push_back(str[i]);
	return res;
}





template<typename T>
inline void write_to_stream(std::ofstream& stream, const std::vector<T>& t)
{
	for(auto i=0; i<t.size(); i++)
		stream.write(reinterpret_cast<const char*>(&t[i]), sizeof(T));
}



template<typename T>
inline void append_to_vector(std::vector<T> &vec, std::vector<T> v)
{
	// append the vector v to the vector vec
	for(auto i=0; i<v.size(); i++)
		vec.push_back(v[i]);
}



inline std::vector<unsigned char> u32bigEndian2vec(int32_t x)
{
	// convert a 32 bit integer into a byte vector in Big-Endian notation
	std::vector<unsigned char> res;
	
	res.push_back((x & 0xFF000000) >> 24);
	res.push_back((x & 0x00FF0000) >> 16);
	res.push_back((x & 0x0000FF00) >>  8);
	res.push_back((x & 0x000000FF)      );
	
	return res;
}


template<typename T>
inline std::vector<unsigned char> bigEndian2vec(T x)
{
	// convert an integer into a byte vector in Big-Endian notation
	std::vector<unsigned char> res;
	
	if(sizeof(T) == 1){
		// 8 bit input
		res.push_back(x);
	}else
	if(sizeof(T) == 2){
		// 16 bit input
		res.push_back((x & 0xFF00) >> 8);
		res.push_back((x & 0x00FF)     );
	}else
	if(sizeof(T) == 4){
		// 32 bit input
		res.push_back((x & 0xFF000000) >> 24);
		res.push_back((x & 0x00FF0000) >> 16);
		res.push_back((x & 0x0000FF00) >>  8);
		res.push_back((x & 0x000000FF)      );
	}else
	if(sizeof(T) == 8){
		// 64 bit input
		res.push_back((x & 0xFF00000000000000) >> 56);
		res.push_back((x & 0x00FF000000000000) >> 48);
		res.push_back((x & 0x0000FF0000000000) >> 40);
		res.push_back((x & 0x000000FF00000000) >> 32);
		res.push_back((x & 0x00000000FF000000) >> 24);
		res.push_back((x & 0x0000000000FF0000) >> 16);
		res.push_back((x & 0x000000000000FF00) >>  8);
		res.push_back((x & 0x00000000000000FF)      );
	}
	
	
	return res;
}


#endif // MRC_PNG_UTILS





/****************************************************
                     PNG image

ToDo:
 - forse dovrei mettere le funzioni di calcolo del
   CRC32B nella libreria "zlib"
 - 

****************************************************/
#ifndef MRC_PNG_IMAGE
#define MRC_PNG_IMAGE


#include <iostream> // TEMPORANEO, PER IL DEBUG!!!!!!!!!!!!!!

#define CHUNK 32768 // 32 Kb

class PNGimage : public BasicImage
{
	public:
		using BasicImage::BasicImage; // inherit constructors (C++11)
		
		// save and load bitmap files
		void save_file(std::string fileName, int stride_bytes);
		void load_file(std::string fileName);
		
		
	private:
		//auxiliary functions:
		void write_chunk(std::ofstream& stream, std::vector<unsigned char> data, std::string type);
		unsigned int crc32(unsigned char *buffer, int len);
		
		void encode_png_line(int y, int stride_bytes, int filter_type, signed char *line_buffer);
};


// CONSTANTS =================================================================================================================================



// DEFINITIONS ================================================================================================================================





void PNGimage::save_file(std::string fileName, int stride_bytes=0) // NON SO CHE COSA SERVE stride_bytes
{
	std::ofstream file(fileName, std::ios::binary);
	
	// 1) writes file header:
	unsigned char sig[8] = { 0x89,'P','N','G',0x0D,0x0A,0x1A,0x0A }; // header starting combination (8-byte signature)
	write_to_stream(file, sig);
	
	// 2) "CHUNKS":
	
	// 2.1) Image Header "IHDR" chunk (13 data bytes total):
	std::vector<unsigned char> temp;
//	temp.push_back((_width &0xFF000000)>>24); temp.push_back((_width &0x00FF0000)>>16); temp.push_back((_width &0x0000FF00)>>8); temp.push_back((_width &0x000000FF)); //image Width  (Big-Endian)
//	temp.push_back((_height&0xFF000000)>>24); temp.push_back((_height&0x00FF0000)>>16); temp.push_back((_height&0x0000FF00)>>8); temp.push_back((_height&0x000000FF)); //image Height (Big-Endian)
	append_to_vector(temp, bigEndian2vec(_width)); // image Width (Big-Endian)
	append_to_vector(temp, bigEndian2vec(_height)); // image Height (Big-Endian)
	temp.push_back(8); // bit depth (1 byte, values 1, 2, 4, 8, or 16)
	temp.push_back(2); // color type (1 byte, values 0:grayscale, 2:RGB, 3:indexed(palette), 4:grayscale&alpha, or 6:RGBA)
	temp.push_back(0); // compression method (1 byte, value 0)
	temp.push_back(0); // filter method (1 byte, value 0)
	temp.push_back(0); // interlace method (1 byte, values 0 "no interlace" or 1 "Adam7 interlace")
	
	write_chunk(file, temp, "IHDR");
	
	// 2.2) Data "IDAT" chunk:
	temp.clear();
//	write_chunk(file, {0x08, 0xD7, 0x63, 0xF8, 0xCF, 0xC0, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00}, "IDAT"); // hardcoded red single pixel (ci sta il canale alpha all'inizio??)
	/*
	{0x08,     0xD7,     0x63,     0xF8,     0xCF,     0xC0,     0x00,     0x00,     0x03,     0x01,     0x01,     0x00    }
	{00001000, 11010111, 01100011, 11111000, 11001111, 11000000, 00000000, 00000000, 00000011, 00000001, 00000001, 00000000}
	*/
	
	//#################################################################################################################################################
	
	for(int i=0; i<data.size(); i+=_width*3+1) data.insert(data.begin() + i, 0); // aggiunge all'inizio di ogni scanline il filtro usato (00)
	
	unsigned error = 0;
	int zlibsize = 0;
	
	unsigned char *pollo, *temp2;
	pollo = (unsigned char*)malloc(data.size());
	for(size_t i=0; i<data.size(); i++) pollo[i] = data[i];

	temp2 = zlib_compress(pollo, data.size(), &zlibsize, 1);
	
	for(size_t i=0; i<zlibsize; i++) temp.push_back(temp2[i]); // copia i dati compressi da temp2 a temp (da puntatore a vector)
//	for(size_t i=0; i<zlibsize; i++) std::cout << std::hex << (int)temp2[i] << " "; std::cout << std::endl;
//	for(auto x : temp) std::cout << std::hex << (int)x << " "; std::cout << std::endl;
	
	free(pollo);
	free(temp2);
	
	//#################################################################################################################################################
	
//	char b[10000];
//	// zlib struct
//    z_stream defstream;
//    defstream.zalloc = Z_NULL;
//    defstream.zfree = Z_NULL;
//    defstream.opaque = Z_NULL;
//    // setup "a" as the input and "b" as the compressed output
//    defstream.avail_in = (uInt)_width; // size of input
//    defstream.avail_out = (uInt)sizeof(b); // size of output
//    
//    for(auto scanline=0; scanline<_height; scanline++){
//		defstream.next_in = (Bytef *)&data[_width*scanline]; // input char array
////		defstream.next_in = (Bytef *)data.data(); // input char array
//		defstream.next_out = (Bytef *)b; // output char array
//		// the actual compression work.
//		deflateInit(&defstream, Z_BEST_COMPRESSION);
//		deflate(&defstream, Z_FINISH);
//		deflateEnd(&defstream);
//		// update the vector
//		for(auto i=0; i<strlen(b); i++) temp.push_back(b[i]);
//	}
//	write_chunk(file, temp, "IDAT");

	//#################################################################################################################################################


	write_chunk(file, temp, "IDAT");












	
	// 2.3) Image End chunk (0 length):
	temp.clear();
	write_chunk(file, temp, "IEND");
	
	
	
	
	
	




	
	



	
//STBIWDEF unsigned char *stbi_write_png_to_mem(
//const unsigned char *pixels, // DATA
//int stride_bytes, // CHE COS'E`???
//int x, 			// width
//int y,			// height
//int n, 			// bytes_per_pixel
//int *out_len)
	
//	int stbi_write_png_compression_level = 8; // NON CAMBIA????
//	int stbi_write_force_png_filter = -1; // A COSA SERVE?????
//	
//	int force_filter = stbi_write_force_png_filter;
//	int ctype[5] = { -1, 0, 4, 2, 6 };
//	unsigned char *out,*o, *filt, *zlib;
//	signed char *line_buffer;
//	int zlen;
//	
//	if(stride_bytes == 0) stride_bytes = bytes_per_row;
//	if(force_filter >= 5) force_filter = -1;
//	
//	filt = new unsigned char[(bytes_per_row+1) * _height]; if (!filt) { file.close(); return;}
//	line_buffer = new signed char[bytes_per_row]; if (!line_buffer) { delete [] filt; file.close(); return; }
//	for(int j=0; j<_height; j++){
//		int filter_type;
//		if(force_filter > -1){
//			filter_type = force_filter;
//			encode_png_line(j, stride_bytes, force_filter, line_buffer);
//		}else{ // Estimate the best filter by running through all of them:
//			int best_filter = 0, best_filter_val = 0x7FFFFFFF, est;
//			for(filter_type = 0; filter_type < 5; filter_type++){
//				encode_png_line(j, stride_bytes, force_filter, line_buffer);
//				// Estimate the entropy of the line using this filter; the less, the better.
//				est = 0;
//				for(int i=0; i<bytes_per_row; i++) est += abs((signed char) line_buffer[i]);
//				if(est < best_filter_val){
//					best_filter_val = est;
//					best_filter = filter_type;
//				}
//			}
//			if(filter_type != best_filter) {  // If the last iteration already got us the best filter, don't redo it
//				encode_png_line(j, stride_bytes, force_filter, line_buffer);
//				filter_type = best_filter;
//			}
//		}
//		// when we get here, filter_type contains the filter type, and line_buffer contains the data
//		filt[j*(bytes_per_row+1)] = (unsigned char) filter_type;
//		memmove(filt+j*(bytes_per_row+1)+1, line_buffer, bytes_per_row);
//	}
//	delete [] line_buffer;
//	zlib = zlib_compress(filt, _height*(bytes_per_row+1), &zlen, stbi_write_png_compression_level);
//	delete [] filt;
//	if(!zlib) { file.close(); return; }
//	
//	// each tag requires 12 bytes of overhead
//	out = new unsigned char[8 + 12+13 + 12+zlen + 12];
//	if (!out) { file.close(); return; }
////	*out_len = 8 + 12+13 + 12+zlen + 12;
//	
//	o=out;
////	memmove(o,sig,8); o+= 8;
//	
//	stbiw__wp32(o, 13); // header length
//		write_to_stream(file, *o); o=out;
////	stbiw__wptag(o, "IHDR");
//	write_to_stream(file, "IHDR");
//	stbiw__wp32(o, _width);
//		write_to_stream(file, *o); o=out;
//	stbiw__wp32(o, _height);
//		write_to_stream(file, *o); o=out;
////	*o++ = 8;
//	write_to_stream(file, 8);
////	*o++ = (unsigned char)(ctype[bytes_per_pixel] & 0xFF);
//	write_to_stream(file, (unsigned char)(ctype[bytes_per_pixel] & 0xFF));
////	*o++ = 0;
////	*o++ = 0;
////	*o++ = 0;
//	write_to_stream(file, (unsigned char)0);
//	write_to_stream(file, (unsigned char)0);
//	write_to_stream(file, (unsigned char)0);
//	stbiw__wpcrc(&o,13);
//		write_to_stream(file, *o); o=out;
//	
//	stbiw__wp32(o, zlen);
//		write_to_stream(file, *o); o=out;
////	stbiw__wptag(o, "IDAT");
//	write_to_stream(file, "IDAT");
//	memmove(o, zlib, zlen); o += zlen;
//		write_to_stream(file, *o); o=out;
//	delete [] zlib;
//	stbiw__wpcrc(&o, zlen);
//		write_to_stream(file, *o); o=out;
//	
//	stbiw__wp32(o,0);
//		write_to_stream(file, *o); o=out;
////	stbiw__wptag(o, "IEND");
//	write_to_stream(file, "IEND");
//	stbiw__wpcrc(&o,0);
//		write_to_stream(file, *o); o=out;
//	
////	STBIW_ASSERT(o == out + *out_len);
	
	file.close();
}


void PNGimage::load_file(std::string fileName)
{
	
}











// AUXILIARY FUNCTIONS ================================================================================================================================


// @OPTIMIZE: provide an option that always forces left-predict or paeth predict
//void PNGimage::encode_png_line(int y, int stride_bytes, int filter_type, signed char *line_buffer)
//{
//	unsigned char * pixels = new unsigned char[data.size()];
//	for(int i=0; i<data.size(); i++) pixels[i] = data[i];
//	static int stbi__flip_vertically_on_write = 0;
//   static int mapping[]  = { 0,1,2,3,4 };
//   static int firstmap[] = { 0,1,0,5,6 };
//   int *mymap = (y != 0) ? mapping : firstmap;
//   int i;
//   int type = mymap[filter_type];
//   unsigned char *z = pixels + stride_bytes * (stbi__flip_vertically_on_write ? _height-1-y : y);
////   unsigned char *z = (unsigned char*)data[stride_bytes * (stbi__flip_vertically_on_write ? _height-1-y : y)]; // NON FUNZIONA
//   int signed_stride = stbi__flip_vertically_on_write ? -stride_bytes : stride_bytes;
//
//   if (type==0) {
//      memcpy(line_buffer, z, bytes_per_row);
//      return;
//   }
//
//   // first loop isn't optimized since it's just one pixel
//   for (i = 0; i < bytes_per_pixel; ++i) {
//      switch (type) {
//         case 1: line_buffer[i] = z[i]; break;
//         case 2: line_buffer[i] = z[i] - z[i-signed_stride]; break;
//         case 3: line_buffer[i] = z[i] - (z[i-signed_stride]>>1); break;
//         case 4: line_buffer[i] = (signed char) (z[i] - stbiw__paeth(0,z[i-signed_stride],0)); break;
//         case 5: line_buffer[i] = z[i]; break;
//         case 6: line_buffer[i] = z[i]; break;
//      }
//   }
//   switch (type) {
//      case 1: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - z[i-bytes_per_pixel]; break;
//      case 2: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - z[i-signed_stride]; break;
//      case 3: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - ((z[i-bytes_per_pixel] + z[i-signed_stride])>>1); break;
//      case 4: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - stbiw__paeth(z[i-bytes_per_pixel], z[i-signed_stride], z[i-signed_stride-bytes_per_pixel]); break;
//      case 5: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - (z[i-bytes_per_pixel]>>1); break;
//      case 6: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - stbiw__paeth(z[i-bytes_per_pixel], 0,0); break;
//   }
//}





void PNGimage::write_chunk(std::ofstream& stream, std::vector<unsigned char> data, std::string type)
{
	unsigned length = data.size(); //length of chunk (4 bytes [32 bits])
	char chunk_type[4] = {type[0],type[1],type[2],type[3]};
	
	write_to_stream(stream, _byteswap_ulong(length)); //writes length of chunk (4 bytes Big-Endian)
	write_to_stream(stream, chunk_type); //writes the chunk type (4 bytes)
	
	for(auto i=0; i<length; i++){
		write_to_stream(stream, data[i]); //write data to stream (byte per byte)
	}
	
	unsigned char* buf = new unsigned char[4+length]; //CRC32B of chunk_type (4 bytes) and data (length bytes)
	for(int i=0; i<4; i++) buf[i] = chunk_type[i];
	for(int i=0; i<length; i++) buf[i+4] = data[i];
	
	write_to_stream(stream, _byteswap_ulong(crc32(buf,length+4))); //write CRC32 in Big-Endian
}


unsigned int PNGimage::crc32(unsigned char *buffer, int len)
{
	// CRC32B:
	// Reflected: 0xEDB88320: &1 >> [128] = poly , [ 8] = poly >> 8 VALID
	// Poly: x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1
	static unsigned int crc_table[256] = {
		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
		0x0eDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
		0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
		0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
		0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
		0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
		0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
		0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
		0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
		0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
		0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
		0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
		0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
		0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
		0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
		0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
		0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
		0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
		0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
		0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
		0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
		0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
		0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
		0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
		0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
		0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
		0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
		0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
		0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
		0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
		0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
		0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
	};
	
	unsigned int crc = ~0u;
	for(int i=0; i<len; i++) crc = (crc >> 8) ^ crc_table[buffer[i] ^ (crc & 0xff)];
	return ~crc;
}


//unsigned crc32(unsigned char* data, int length)
//{
//	static unsigned* crcTable;
//	unsigned crc; //32 bit unsigned data
//	unsigned poly = 0xEDB88320; //polynomial
//	if(crcTable == nullptr){
//		crcTable=new unsigned[256];
//		for(unsigned n=0;n<256;n++){
//			crc = n;
//			for(auto k=0;k<8;k++){
//				if((crc & 1) == 1) crc = poly ^ (crc >> 1);
//				else               crc >>= 1;
//			}
//			crcTable[n] = crc;
//		}
//	}
//	crc = ~0u;
//	for(auto i=0; i<length; i++) crc = (crc >> 8) ^ crcTable[data[i] ^ (crc & 0xFF)];
//	return ~crc; //same as c ^ 0xFFFFFFFF
//}










// PNG SPECIFICATION ================================================================================================================================


/*****************************************************************************************
Color Type and Allowed Bit Depth:
   Color    Allowed    Interpretation
   Type    Bit Depths
   
   0       1,2,4,8,16  Each pixel is a grayscale sample.
   2       8,16        Each pixel is an R,G,B triple.
   3       1,2,4,8     Each pixel is a palette index; a PLTE chunk must appear.
   4       8,16        Each pixel is a grayscale sample, followed by an alpha sample.
   6       8,16        Each pixel is an R,G,B triple, followed by an alpha sample.
******************************************************************************************/

/******************************************************************************************
Critical chunks:
    IHDR	must be the first chunk; it contains (in this order) the image's width (4 bytes);
			height (4 bytes); bit depth (1 byte, values 1, 2, 4, 8, or 16); 
			color type (1 byte, values 0, 2, 3, 4, or 6); compression method (1 byte, value 0); 
			filter method (1 byte, value 0); 
			and interlace method (1 byte, values 0 "no interlace" or 1 "Adam7 interlace") 
			(13 data bytes total).
    PLTE	contains the palette: a list of colors. The PLTE chunk is essential for color 
			type 3 (indexed color). It is optional for color types 2 and 6 (truecolor and 
			truecolor with alpha) and it must not appear for color types 0 and 4 (grayscale 
			and grayscale with alpha).
    IDAT	contains the image, which may be split among multiple IDAT chunks. Such splitting 
			increases filesize slightly, but makes it possible to generate a PNG in a streaming 
			manner. The IDAT chunk contains the actual image data, which is the output stream 
			of the compression algorithm.
    IEND	marks the image end; the data field of the IEND chunk has 0 bytes/is empty.

Ancillary chunks:
    bKGD	gives the default background color. It is intended for use when there is no better 
			choice available, such as in standalone image viewers (but not web browsers; see 
			below for more details).
    cHRM 	gives the chromaticity coordinates of the display primaries and white point.
    dSIG 	is for storing digital signatures.
    eXIf 	stores Exif metadata.
    gAMA 	specifies gamma. The gAMA chunk contains only 4 bytes, and its value represents 
			the gamma value multiplied by 100,000; for example, 
			the gamma value 1/3.4 calculates to 29411.7647059 ((1/3.4)*(100,000)) and is 
			converted to an integer (29412) for storage.
    hIST 	can store the histogram, or total amount of each color in the image.
    iCCP 	is an ICC color profile.
    iTXt 	contains a keyword and UTF-8 text, with encodings for possible compression and 
			translations marked with language tag. The Extensible Metadata Platform (XMP) uses 
			this chunk with a keyword 'XML:com.adobe.xmp'
    pHYs 	holds the intended pixel size (or pixel aspect ratio); the pHYs contains "Pixels 
			per unit, X axis" (4 bytes), "Pixels per unit, Y axis" (4 bytes), and "Unit specifier" 
			(1 byte) for a total of 9 bytes.[22]
    sBIT 	(significant bits) indicates the color-accuracy of the source data; this chunk 
			contains a total of between 1 and 13 bytes.
    sPLT 	suggests a palette to use if the full range of colors is unavailable.
    sRGB 	indicates that the standard sRGB color space is used; the sRGB chunk contains 
			only 1 byte, which is used for "rendering intent" (4 values—0, 1, 2, and 3—are 
			defined for rendering intent).
    sTER 	stereo-image indicator chunk for stereoscopic images.
    tEXt 	can store text that can be represented in ISO/IEC 8859-1, with one key-value pair 
			for each chunk. The "key" must be between 1 and 79 characters long. Separator 
			is a null character. The "value" can be any length, including zero up to the maximum 
			permissible chunk size minus the length of the keyword and separator. Neither "key" nor 
			"value" can contain null character. Leading or trailing spaces are also disallowed.
    tIME 	stores the time that the image was last changed.
    tRNS 	contains transparency information. For indexed images, it stores alpha channel values 
			for one or more palette entries. For truecolor and grayscale images, it stores a single 
			pixel value that is to be regarded as fully transparent.
    zTXt 	contains compressed text (and a compression method marker) with the same limits as tEXt.

The lowercase first letter in these chunks indicates that they are not needed for the PNG specification. 
The lowercase last letter in some chunks indicates that they are safe to copy, even if the application 
concerned does not understand them. 

****************************************************************************************************/



#endif // MRC_PNG_IMAGE

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
		void text(int x,int y, std::string text, std::vector<std::string> font, unsigned scale);
		void ellipse(int centerx, int centery, int a, int b);
		void insertImage(int x,int y, Image img);
		void fillrect(int x0,int y0, int x1,int y1);
		
		inline void penColor(const byte& R,const byte& G,const byte& B) { _penColor = {R,G,B}; }
		inline void penColor(const color& c) { _penColor = c; }
		inline void penWidth(const uint& width) { _penWidth = width; }
		
		//saveFile:
		void save_bmp(const std::string& fileName) const;
		void save_png(const std::string& fileName) const; // DA METTERE ENTRAMBI IN UN UNICO METODO CHE VEDE L'ESTENSIONE...!!!!!!!!!!!!
		
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


void Image::save_png(const std::string& fileName) const
{
	PNGimage image(W, H);
	
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
		case 5: {
				set_pixel(x-1,y-2, _penColor);
				set_pixel(x  ,y-2, _penColor);
				set_pixel(x+1,y-2, _penColor);
				
				set_pixel(x-2,y-1, _penColor);
				set_pixel(x-1,y-1, _penColor);
				set_pixel(x  ,y-1, _penColor);
				set_pixel(x+1,y-1, _penColor);
				set_pixel(x+2,y-1, _penColor);
				
				set_pixel(x-2,y  , _penColor);
				set_pixel(x-1,y  , _penColor);
				set_pixel(x  ,y  , _penColor);
				set_pixel(x+1,y  , _penColor);
				set_pixel(x+2,y  , _penColor);
				
				set_pixel(x-2,y+1, _penColor);
				set_pixel(x-1,y+1, _penColor);
				set_pixel(x  ,y+1, _penColor);
				set_pixel(x+1,y+1, _penColor);
				set_pixel(x+2,y+1, _penColor);
				
				set_pixel(x-1,y+2, _penColor);
				set_pixel(x  ,y+2, _penColor);
				set_pixel(x+1,y+2, _penColor);
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

void Image::fillrect(int x0,int y0,int x1,int y1)
{
	line(x0,y0,x1,y0);
	line(x0,y0,x0,y1);
	line(x1,y1,x1,y0);
	line(x1,y1,x0,y1);
	for(int i=(y0<y1?y0:y1); i<=(y0>y1?y0:y1); i++) line(x0,i,x1,i);
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



void Image::text(int x,int y, std::string text, std::vector<std::string> font, unsigned scale=1)
{
	//prints ASCII chars from 32 (space) to 126 (tilde)
	int charWidth  = font[0][0];
	int charHeight = font[0][1];
	int _x=x, _y=y; // top-right corner of first character
	for(int i=0; i<text.length(); i++){
		if(text[i] == '\n') {
			//new line and carriage return
			_x = x;
			y += charHeight*scale;
			_y = y;
			continue; //SOLUZIONE LEZZA E TEMPORANEA!! (per non far continuare con i cicli for)
		}
		if(text[i]<32 || text[i]>126) continue; //NON MI PIACE MOLTO USARE CONTINUE, MA FORSE E` L'UNICA SOLUZIONE
		for(int r=0; r<charHeight; r++)
			for(int c=0; c<charWidth; c++)
				if(font[text[i]-' '+1][r*charWidth+c] == '0'){
					for(int ix=0; ix<scale; ix++)
					for(int iy=0; iy<scale; iy++)
					set_pixel(_x+c*scale+ix, _y+r*scale+iy, _penColor);
				}
		_x += charWidth*scale;
		_y = y;
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

#ifndef MRC_UPSAMPLE
#define MRC_UPSAMPLE


Image upSample(Image img, unsigned scale=2)
{
	Image res(img.width()*scale, img.height()*scale);
	
	for(int y=0; y<img.height(); y++){
		for(int x=0; x<img.width(); x++){
			color c = img.get_pixel(x,y);
			for(int ix=0; ix<scale; ix++) for(int iy=0; iy<scale; iy++) res.set_pixel((x*scale)+ix, (y*scale)+iy, c);
		}
	}
	
	return res;
}


#endif
//#include "imageFunctions/downSample.h"
//#include "imageFunctions/AntiAliasing.h"
//#include "imageFunctions/blur.h"

// PALETTE =================================================================================================================================

enum palette_name
{
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

const color autumn_colormap[1000] = {
   {255,   0,   0}, {255,   0,   0}, {255,   1,   0}, {255,   1,   0}, {255,   1,   0},
   {255,   1,   0}, {255,   2,   0}, {255,   2,   0}, {255,   2,   0}, {255,   2,   0},
   {255,   3,   0}, {255,   3,   0}, {255,   3,   0}, {255,   3,   0}, {255,   4,   0},
   {255,   4,   0}, {255,   4,   0}, {255,   4,   0}, {255,   5,   0}, {255,   5,   0},
   {255,   5,   0}, {255,   5,   0}, {255,   6,   0}, {255,   6,   0}, {255,   6,   0},
   {255,   6,   0}, {255,   7,   0}, {255,   7,   0}, {255,   7,   0}, {255,   7,   0},
   {255,   8,   0}, {255,   8,   0}, {255,   8,   0}, {255,   8,   0}, {255,   9,   0},
   {255,   9,   0}, {255,   9,   0}, {255,   9,   0}, {255,  10,   0}, {255,  10,   0},
   {255,  10,   0}, {255,  10,   0}, {255,  11,   0}, {255,  11,   0}, {255,  11,   0},
   {255,  11,   0}, {255,  12,   0}, {255,  12,   0}, {255,  12,   0}, {255,  13,   0},
   {255,  13,   0}, {255,  13,   0}, {255,  13,   0}, {255,  14,   0}, {255,  14,   0},
   {255,  14,   0}, {255,  14,   0}, {255,  15,   0}, {255,  15,   0}, {255,  15,   0},
   {255,  15,   0}, {255,  16,   0}, {255,  16,   0}, {255,  16,   0}, {255,  16,   0},
   {255,  17,   0}, {255,  17,   0}, {255,  17,   0}, {255,  17,   0}, {255,  18,   0},
   {255,  18,   0}, {255,  18,   0}, {255,  18,   0}, {255,  19,   0}, {255,  19,   0},
   {255,  19,   0}, {255,  19,   0}, {255,  20,   0}, {255,  20,   0}, {255,  20,   0},
   {255,  20,   0}, {255,  21,   0}, {255,  21,   0}, {255,  21,   0}, {255,  21,   0},
   {255,  22,   0}, {255,  22,   0}, {255,  22,   0}, {255,  22,   0}, {255,  23,   0},
   {255,  23,   0}, {255,  23,   0}, {255,  23,   0}, {255,  24,   0}, {255,  24,   0},
   {255,  24,   0}, {255,  25,   0}, {255,  25,   0}, {255,  25,   0}, {255,  25,   0},
   {255,  26,   0}, {255,  26,   0}, {255,  26,   0}, {255,  26,   0}, {255,  27,   0},
   {255,  27,   0}, {255,  27,   0}, {255,  27,   0}, {255,  28,   0}, {255,  28,   0},
   {255,  28,   0}, {255,  28,   0}, {255,  29,   0}, {255,  29,   0}, {255,  29,   0},
   {255,  29,   0}, {255,  30,   0}, {255,  30,   0}, {255,  30,   0}, {255,  30,   0},
   {255,  31,   0}, {255,  31,   0}, {255,  31,   0}, {255,  31,   0}, {255,  32,   0},
   {255,  32,   0}, {255,  32,   0}, {255,  32,   0}, {255,  33,   0}, {255,  33,   0},
   {255,  33,   0}, {255,  33,   0}, {255,  34,   0}, {255,  34,   0}, {255,  34,   0},
   {255,  34,   0}, {255,  35,   0}, {255,  35,   0}, {255,  35,   0}, {255,  35,   0},
   {255,  36,   0}, {255,  36,   0}, {255,  36,   0}, {255,  37,   0}, {255,  37,   0},
   {255,  37,   0}, {255,  37,   0}, {255,  38,   0}, {255,  38,   0}, {255,  38,   0},
   {255,  38,   0}, {255,  39,   0}, {255,  39,   0}, {255,  39,   0}, {255,  39,   0},
   {255,  40,   0}, {255,  40,   0}, {255,  40,   0}, {255,  40,   0}, {255,  41,   0},
   {255,  41,   0}, {255,  41,   0}, {255,  41,   0}, {255,  42,   0}, {255,  42,   0},
   {255,  42,   0}, {255,  42,   0}, {255,  43,   0}, {255,  43,   0}, {255,  43,   0},
   {255,  43,   0}, {255,  44,   0}, {255,  44,   0}, {255,  44,   0}, {255,  44,   0},
   {255,  45,   0}, {255,  45,   0}, {255,  45,   0}, {255,  45,   0}, {255,  46,   0},
   {255,  46,   0}, {255,  46,   0}, {255,  46,   0}, {255,  47,   0}, {255,  47,   0},
   {255,  47,   0}, {255,  47,   0}, {255,  48,   0}, {255,  48,   0}, {255,  48,   0},
   {255,  48,   0}, {255,  49,   0}, {255,  49,   0}, {255,  49,   0}, {255,  50,   0},
   {255,  50,   0}, {255,  50,   0}, {255,  50,   0}, {255,  51,   0}, {255,  51,   0},
   {255,  51,   0}, {255,  51,   0}, {255,  52,   0}, {255,  52,   0}, {255,  52,   0},
   {255,  52,   0}, {255,  53,   0}, {255,  53,   0}, {255,  53,   0}, {255,  53,   0},
   {255,  54,   0}, {255,  54,   0}, {255,  54,   0}, {255,  54,   0}, {255,  55,   0},
   {255,  55,   0}, {255,  55,   0}, {255,  55,   0}, {255,  56,   0}, {255,  56,   0},
   {255,  56,   0}, {255,  56,   0}, {255,  57,   0}, {255,  57,   0}, {255,  57,   0},
   {255,  57,   0}, {255,  58,   0}, {255,  58,   0}, {255,  58,   0}, {255,  58,   0},
   {255,  59,   0}, {255,  59,   0}, {255,  59,   0}, {255,  59,   0}, {255,  60,   0},
   {255,  60,   0}, {255,  60,   0}, {255,  60,   0}, {255,  61,   0}, {255,  61,   0},
   {255,  61,   0}, {255,  62,   0}, {255,  62,   0}, {255,  62,   0}, {255,  62,   0},
   {255,  63,   0}, {255,  63,   0}, {255,  63,   0}, {255,  63,   0}, {255,  64,   0},
   {255,  64,   0}, {255,  64,   0}, {255,  64,   0}, {255,  65,   0}, {255,  65,   0},
   {255,  65,   0}, {255,  65,   0}, {255,  66,   0}, {255,  66,   0}, {255,  66,   0},
   {255,  66,   0}, {255,  67,   0}, {255,  67,   0}, {255,  67,   0}, {255,  67,   0},
   {255,  68,   0}, {255,  68,   0}, {255,  68,   0}, {255,  68,   0}, {255,  69,   0},
   {255,  69,   0}, {255,  69,   0}, {255,  69,   0}, {255,  70,   0}, {255,  70,   0},
   {255,  70,   0}, {255,  70,   0}, {255,  71,   0}, {255,  71,   0}, {255,  71,   0},
   {255,  71,   0}, {255,  72,   0}, {255,  72,   0}, {255,  72,   0}, {255,  72,   0},
   {255,  73,   0}, {255,  73,   0}, {255,  73,   0}, {255,  74,   0}, {255,  74,   0},
   {255,  74,   0}, {255,  74,   0}, {255,  75,   0}, {255,  75,   0}, {255,  75,   0},
   {255,  75,   0}, {255,  76,   0}, {255,  76,   0}, {255,  76,   0}, {255,  76,   0},
   {255,  77,   0}, {255,  77,   0}, {255,  77,   0}, {255,  77,   0}, {255,  78,   0},
   {255,  78,   0}, {255,  78,   0}, {255,  78,   0}, {255,  79,   0}, {255,  79,   0},
   {255,  79,   0}, {255,  79,   0}, {255,  80,   0}, {255,  80,   0}, {255,  80,   0},
   {255,  80,   0}, {255,  81,   0}, {255,  81,   0}, {255,  81,   0}, {255,  81,   0},
   {255,  82,   0}, {255,  82,   0}, {255,  82,   0}, {255,  82,   0}, {255,  83,   0},
   {255,  83,   0}, {255,  83,   0}, {255,  83,   0}, {255,  84,   0}, {255,  84,   0},
   {255,  84,   0}, {255,  84,   0}, {255,  85,   0}, {255,  85,   0}, {255,  85,   0},
   {255,  86,   0}, {255,  86,   0}, {255,  86,   0}, {255,  86,   0}, {255,  87,   0},
   {255,  87,   0}, {255,  87,   0}, {255,  87,   0}, {255,  88,   0}, {255,  88,   0},
   {255,  88,   0}, {255,  88,   0}, {255,  89,   0}, {255,  89,   0}, {255,  89,   0},
   {255,  89,   0}, {255,  90,   0}, {255,  90,   0}, {255,  90,   0}, {255,  90,   0},
   {255,  91,   0}, {255,  91,   0}, {255,  91,   0}, {255,  91,   0}, {255,  92,   0},
   {255,  92,   0}, {255,  92,   0}, {255,  92,   0}, {255,  93,   0}, {255,  93,   0},
   {255,  93,   0}, {255,  93,   0}, {255,  94,   0}, {255,  94,   0}, {255,  94,   0},
   {255,  94,   0}, {255,  95,   0}, {255,  95,   0}, {255,  95,   0}, {255,  95,   0},
   {255,  96,   0}, {255,  96,   0}, {255,  96,   0}, {255,  96,   0}, {255,  97,   0},
   {255,  97,   0}, {255,  97,   0}, {255,  98,   0}, {255,  98,   0}, {255,  98,   0},
   {255,  98,   0}, {255,  99,   0}, {255,  99,   0}, {255,  99,   0}, {255,  99,   0},
   {255, 100,   0}, {255, 100,   0}, {255, 100,   0}, {255, 100,   0}, {255, 101,   0},
   {255, 101,   0}, {255, 101,   0}, {255, 101,   0}, {255, 102,   0}, {255, 102,   0},
   {255, 102,   0}, {255, 102,   0}, {255, 103,   0}, {255, 103,   0}, {255, 103,   0},
   {255, 103,   0}, {255, 104,   0}, {255, 104,   0}, {255, 104,   0}, {255, 104,   0},
   {255, 105,   0}, {255, 105,   0}, {255, 105,   0}, {255, 105,   0}, {255, 106,   0},
   {255, 106,   0}, {255, 106,   0}, {255, 106,   0}, {255, 107,   0}, {255, 107,   0},
   {255, 107,   0}, {255, 107,   0}, {255, 108,   0}, {255, 108,   0}, {255, 108,   0},
   {255, 108,   0}, {255, 109,   0}, {255, 109,   0}, {255, 109,   0}, {255, 110,   0},
   {255, 110,   0}, {255, 110,   0}, {255, 110,   0}, {255, 111,   0}, {255, 111,   0},
   {255, 111,   0}, {255, 111,   0}, {255, 112,   0}, {255, 112,   0}, {255, 112,   0},
   {255, 112,   0}, {255, 113,   0}, {255, 113,   0}, {255, 113,   0}, {255, 113,   0},
   {255, 114,   0}, {255, 114,   0}, {255, 114,   0}, {255, 114,   0}, {255, 115,   0},
   {255, 115,   0}, {255, 115,   0}, {255, 115,   0}, {255, 116,   0}, {255, 116,   0},
   {255, 116,   0}, {255, 116,   0}, {255, 117,   0}, {255, 117,   0}, {255, 117,   0},
   {255, 117,   0}, {255, 118,   0}, {255, 118,   0}, {255, 118,   0}, {255, 118,   0},
   {255, 119,   0}, {255, 119,   0}, {255, 119,   0}, {255, 119,   0}, {255, 120,   0},
   {255, 120,   0}, {255, 120,   0}, {255, 120,   0}, {255, 121,   0}, {255, 121,   0},
   {255, 121,   0}, {255, 122,   0}, {255, 122,   0}, {255, 122,   0}, {255, 122,   0},
   {255, 123,   0}, {255, 123,   0}, {255, 123,   0}, {255, 123,   0}, {255, 124,   0},
   {255, 124,   0}, {255, 124,   0}, {255, 124,   0}, {255, 125,   0}, {255, 125,   0},
   {255, 125,   0}, {255, 125,   0}, {255, 126,   0}, {255, 126,   0}, {255, 126,   0},
   {255, 126,   0}, {255, 127,   0}, {255, 127,   0}, {255, 127,   0}, {255, 127,   0},
   {255, 128,   0}, {255, 128,   0}, {255, 128,   0}, {255, 128,   0}, {255, 129,   0},
   {255, 129,   0}, {255, 129,   0}, {255, 129,   0}, {255, 130,   0}, {255, 130,   0},
   {255, 130,   0}, {255, 130,   0}, {255, 131,   0}, {255, 131,   0}, {255, 131,   0},
   {255, 131,   0}, {255, 132,   0}, {255, 132,   0}, {255, 132,   0}, {255, 132,   0},
   {255, 133,   0}, {255, 133,   0}, {255, 133,   0}, {255, 133,   0}, {255, 134,   0},
   {255, 134,   0}, {255, 134,   0}, {255, 135,   0}, {255, 135,   0}, {255, 135,   0},
   {255, 135,   0}, {255, 136,   0}, {255, 136,   0}, {255, 136,   0}, {255, 136,   0},
   {255, 137,   0}, {255, 137,   0}, {255, 137,   0}, {255, 137,   0}, {255, 138,   0},
   {255, 138,   0}, {255, 138,   0}, {255, 138,   0}, {255, 139,   0}, {255, 139,   0},
   {255, 139,   0}, {255, 139,   0}, {255, 140,   0}, {255, 140,   0}, {255, 140,   0},
   {255, 140,   0}, {255, 141,   0}, {255, 141,   0}, {255, 141,   0}, {255, 141,   0},
   {255, 142,   0}, {255, 142,   0}, {255, 142,   0}, {255, 142,   0}, {255, 143,   0},
   {255, 143,   0}, {255, 143,   0}, {255, 143,   0}, {255, 144,   0}, {255, 144,   0},
   {255, 144,   0}, {255, 144,   0}, {255, 145,   0}, {255, 145,   0}, {255, 145,   0},
   {255, 145,   0}, {255, 146,   0}, {255, 146,   0}, {255, 146,   0}, {255, 147,   0},
   {255, 147,   0}, {255, 147,   0}, {255, 147,   0}, {255, 148,   0}, {255, 148,   0},
   {255, 148,   0}, {255, 148,   0}, {255, 149,   0}, {255, 149,   0}, {255, 149,   0},
   {255, 149,   0}, {255, 150,   0}, {255, 150,   0}, {255, 150,   0}, {255, 150,   0},
   {255, 151,   0}, {255, 151,   0}, {255, 151,   0}, {255, 151,   0}, {255, 152,   0},
   {255, 152,   0}, {255, 152,   0}, {255, 152,   0}, {255, 153,   0}, {255, 153,   0},
   {255, 153,   0}, {255, 153,   0}, {255, 154,   0}, {255, 154,   0}, {255, 154,   0},
   {255, 154,   0}, {255, 155,   0}, {255, 155,   0}, {255, 155,   0}, {255, 155,   0},
   {255, 156,   0}, {255, 156,   0}, {255, 156,   0}, {255, 156,   0}, {255, 157,   0},
   {255, 157,   0}, {255, 157,   0}, {255, 157,   0}, {255, 158,   0}, {255, 158,   0},
   {255, 158,   0}, {255, 159,   0}, {255, 159,   0}, {255, 159,   0}, {255, 159,   0},
   {255, 160,   0}, {255, 160,   0}, {255, 160,   0}, {255, 160,   0}, {255, 161,   0},
   {255, 161,   0}, {255, 161,   0}, {255, 161,   0}, {255, 162,   0}, {255, 162,   0},
   {255, 162,   0}, {255, 162,   0}, {255, 163,   0}, {255, 163,   0}, {255, 163,   0},
   {255, 163,   0}, {255, 164,   0}, {255, 164,   0}, {255, 164,   0}, {255, 164,   0},
   {255, 165,   0}, {255, 165,   0}, {255, 165,   0}, {255, 165,   0}, {255, 166,   0},
   {255, 166,   0}, {255, 166,   0}, {255, 166,   0}, {255, 167,   0}, {255, 167,   0},
   {255, 167,   0}, {255, 167,   0}, {255, 168,   0}, {255, 168,   0}, {255, 168,   0},
   {255, 168,   0}, {255, 169,   0}, {255, 169,   0}, {255, 169,   0}, {255, 169,   0},
   {255, 170,   0}, {255, 170,   0}, {255, 170,   0}, {255, 171,   0}, {255, 171,   0},
   {255, 171,   0}, {255, 171,   0}, {255, 172,   0}, {255, 172,   0}, {255, 172,   0},
   {255, 172,   0}, {255, 173,   0}, {255, 173,   0}, {255, 173,   0}, {255, 173,   0},
   {255, 174,   0}, {255, 174,   0}, {255, 174,   0}, {255, 174,   0}, {255, 175,   0},
   {255, 175,   0}, {255, 175,   0}, {255, 175,   0}, {255, 176,   0}, {255, 176,   0},
   {255, 176,   0}, {255, 176,   0}, {255, 177,   0}, {255, 177,   0}, {255, 177,   0},
   {255, 177,   0}, {255, 178,   0}, {255, 178,   0}, {255, 178,   0}, {255, 178,   0},
   {255, 179,   0}, {255, 179,   0}, {255, 179,   0}, {255, 179,   0}, {255, 180,   0},
   {255, 180,   0}, {255, 180,   0}, {255, 180,   0}, {255, 181,   0}, {255, 181,   0},
   {255, 181,   0}, {255, 181,   0}, {255, 182,   0}, {255, 182,   0}, {255, 182,   0},
   {255, 183,   0}, {255, 183,   0}, {255, 183,   0}, {255, 183,   0}, {255, 184,   0},
   {255, 184,   0}, {255, 184,   0}, {255, 184,   0}, {255, 185,   0}, {255, 185,   0},
   {255, 185,   0}, {255, 185,   0}, {255, 186,   0}, {255, 186,   0}, {255, 186,   0},
   {255, 186,   0}, {255, 187,   0}, {255, 187,   0}, {255, 187,   0}, {255, 187,   0},
   {255, 188,   0}, {255, 188,   0}, {255, 188,   0}, {255, 188,   0}, {255, 189,   0},
   {255, 189,   0}, {255, 189,   0}, {255, 189,   0}, {255, 190,   0}, {255, 190,   0},
   {255, 190,   0}, {255, 190,   0}, {255, 191,   0}, {255, 191,   0}, {255, 191,   0},
   {255, 191,   0}, {255, 192,   0}, {255, 192,   0}, {255, 192,   0}, {255, 192,   0},
   {255, 193,   0}, {255, 193,   0}, {255, 193,   0}, {255, 193,   0}, {255, 194,   0},
   {255, 194,   0}, {255, 194,   0}, {255, 195,   0}, {255, 195,   0}, {255, 195,   0},
   {255, 195,   0}, {255, 196,   0}, {255, 196,   0}, {255, 196,   0}, {255, 196,   0},
   {255, 197,   0}, {255, 197,   0}, {255, 197,   0}, {255, 197,   0}, {255, 198,   0},
   {255, 198,   0}, {255, 198,   0}, {255, 198,   0}, {255, 199,   0}, {255, 199,   0},
   {255, 199,   0}, {255, 199,   0}, {255, 200,   0}, {255, 200,   0}, {255, 200,   0},
   {255, 200,   0}, {255, 201,   0}, {255, 201,   0}, {255, 201,   0}, {255, 201,   0},
   {255, 202,   0}, {255, 202,   0}, {255, 202,   0}, {255, 202,   0}, {255, 203,   0},
   {255, 203,   0}, {255, 203,   0}, {255, 203,   0}, {255, 204,   0}, {255, 204,   0},
   {255, 204,   0}, {255, 204,   0}, {255, 205,   0}, {255, 205,   0}, {255, 205,   0},
   {255, 205,   0}, {255, 206,   0}, {255, 206,   0}, {255, 206,   0}, {255, 207,   0},
   {255, 207,   0}, {255, 207,   0}, {255, 207,   0}, {255, 208,   0}, {255, 208,   0},
   {255, 208,   0}, {255, 208,   0}, {255, 209,   0}, {255, 209,   0}, {255, 209,   0},
   {255, 209,   0}, {255, 210,   0}, {255, 210,   0}, {255, 210,   0}, {255, 210,   0},
   {255, 211,   0}, {255, 211,   0}, {255, 211,   0}, {255, 211,   0}, {255, 212,   0},
   {255, 212,   0}, {255, 212,   0}, {255, 212,   0}, {255, 213,   0}, {255, 213,   0},
   {255, 213,   0}, {255, 213,   0}, {255, 214,   0}, {255, 214,   0}, {255, 214,   0},
   {255, 214,   0}, {255, 215,   0}, {255, 215,   0}, {255, 215,   0}, {255, 215,   0},
   {255, 216,   0}, {255, 216,   0}, {255, 216,   0}, {255, 216,   0}, {255, 217,   0},
   {255, 217,   0}, {255, 217,   0}, {255, 217,   0}, {255, 218,   0}, {255, 218,   0},
   {255, 218,   0}, {255, 218,   0}, {255, 219,   0}, {255, 219,   0}, {255, 219,   0},
   {255, 220,   0}, {255, 220,   0}, {255, 220,   0}, {255, 220,   0}, {255, 221,   0},
   {255, 221,   0}, {255, 221,   0}, {255, 221,   0}, {255, 222,   0}, {255, 222,   0},
   {255, 222,   0}, {255, 222,   0}, {255, 223,   0}, {255, 223,   0}, {255, 223,   0},
   {255, 223,   0}, {255, 224,   0}, {255, 224,   0}, {255, 224,   0}, {255, 224,   0},
   {255, 225,   0}, {255, 225,   0}, {255, 225,   0}, {255, 225,   0}, {255, 226,   0},
   {255, 226,   0}, {255, 226,   0}, {255, 226,   0}, {255, 227,   0}, {255, 227,   0},
   {255, 227,   0}, {255, 227,   0}, {255, 228,   0}, {255, 228,   0}, {255, 228,   0},
   {255, 228,   0}, {255, 229,   0}, {255, 229,   0}, {255, 229,   0}, {255, 229,   0},
   {255, 230,   0}, {255, 230,   0}, {255, 230,   0}, {255, 230,   0}, {255, 231,   0},
   {255, 231,   0}, {255, 231,   0}, {255, 232,   0}, {255, 232,   0}, {255, 232,   0},
   {255, 232,   0}, {255, 233,   0}, {255, 233,   0}, {255, 233,   0}, {255, 233,   0},
   {255, 234,   0}, {255, 234,   0}, {255, 234,   0}, {255, 234,   0}, {255, 235,   0},
   {255, 235,   0}, {255, 235,   0}, {255, 235,   0}, {255, 236,   0}, {255, 236,   0},
   {255, 236,   0}, {255, 236,   0}, {255, 237,   0}, {255, 237,   0}, {255, 237,   0},
   {255, 237,   0}, {255, 238,   0}, {255, 238,   0}, {255, 238,   0}, {255, 238,   0},
   {255, 239,   0}, {255, 239,   0}, {255, 239,   0}, {255, 239,   0}, {255, 240,   0},
   {255, 240,   0}, {255, 240,   0}, {255, 240,   0}, {255, 241,   0}, {255, 241,   0},
   {255, 241,   0}, {255, 241,   0}, {255, 242,   0}, {255, 242,   0}, {255, 242,   0},
   {255, 242,   0}, {255, 243,   0}, {255, 243,   0}, {255, 243,   0}, {255, 244,   0},
   {255, 244,   0}, {255, 244,   0}, {255, 244,   0}, {255, 245,   0}, {255, 245,   0},
   {255, 245,   0}, {255, 245,   0}, {255, 246,   0}, {255, 246,   0}, {255, 246,   0},
   {255, 246,   0}, {255, 247,   0}, {255, 247,   0}, {255, 247,   0}, {255, 247,   0},
   {255, 248,   0}, {255, 248,   0}, {255, 248,   0}, {255, 248,   0}, {255, 249,   0},
   {255, 249,   0}, {255, 249,   0}, {255, 249,   0}, {255, 250,   0}, {255, 250,   0},
   {255, 250,   0}, {255, 250,   0}, {255, 251,   0}, {255, 251,   0}, {255, 251,   0},
   {255, 251,   0}, {255, 252,   0}, {255, 252,   0}, {255, 252,   0}, {255, 252,   0},
   {255, 253,   0}, {255, 253,   0}, {255, 253,   0}, {255, 253,   0}, {255, 254,   0},
   {255, 254,   0}, {255, 254,   0}, {255, 254,   0}, {255, 255,   0}, {255, 255,   0}
};

const color copper_colormap[1000] = {
   {  0,   0,   0}, {  0,   0,   0}, {  1,   0,   0}, {  1,   1,   0}, {  1,   1,   1},
   {  2,   1,   1}, {  2,   1,   1}, {  2,   1,   1}, {  3,   2,   1}, {  3,   2,   1},
   {  3,   2,   1}, {  4,   2,   1}, {  4,   2,   2}, {  4,   3,   2}, {  4,   3,   2},
   {  5,   3,   2}, {  5,   3,   2}, {  5,   3,   2}, {  6,   4,   2}, {  6,   4,   2},
   {  6,   4,   3}, {  7,   4,   3}, {  7,   4,   3}, {  7,   5,   3}, {  8,   5,   3},
   {  8,   5,   3}, {  8,   5,   3}, {  9,   5,   3}, {  9,   6,   4}, {  9,   6,   4},
   { 10,   6,   4}, { 10,   6,   4}, { 10,   6,   4}, { 11,   7,   4}, { 11,   7,   4},
   { 11,   7,   4}, { 11,   7,   5}, { 12,   7,   5}, { 12,   8,   5}, { 12,   8,   5},
   { 13,   8,   5}, { 13,   8,   5}, { 13,   8,   5}, { 14,   9,   5}, { 14,   9,   6},
   { 14,   9,   6}, { 15,   9,   6}, { 15,   9,   6}, { 15,  10,   6}, { 16,  10,   6},
   { 16,  10,   6}, { 16,  10,   6}, { 17,  10,   7}, { 17,  11,   7}, { 17,  11,   7},
   { 18,  11,   7}, { 18,  11,   7}, { 18,  11,   7}, { 19,  12,   7}, { 19,  12,   7},
   { 19,  12,   8}, { 19,  12,   8}, { 20,  12,   8}, { 20,  13,   8}, { 20,  13,   8},
   { 21,  13,   8}, { 21,  13,   8}, { 21,  13,   9}, { 22,  14,   9}, { 22,  14,   9},
   { 22,  14,   9}, { 23,  14,   9}, { 23,  14,   9}, { 23,  15,   9}, { 24,  15,   9},
   { 24,  15,  10}, { 24,  15,  10}, { 25,  15,  10}, { 25,  16,  10}, { 25,  16,  10},
   { 26,  16,  10}, { 26,  16,  10}, { 26,  16,  10}, { 26,  17,  11}, { 27,  17,  11},
   { 27,  17,  11}, { 27,  17,  11}, { 28,  17,  11}, { 28,  18,  11}, { 28,  18,  11},
   { 29,  18,  11}, { 29,  18,  12}, { 29,  18,  12}, { 30,  19,  12}, { 30,  19,  12},
   { 30,  19,  12}, { 31,  19,  12}, { 31,  19,  12}, { 31,  20,  12}, { 32,  20,  13},
   { 32,  20,  13}, { 32,  20,  13}, { 33,  20,  13}, { 33,  21,  13}, { 33,  21,  13},
   { 34,  21,  13}, { 34,  21,  13}, { 34,  21,  14}, { 34,  22,  14}, { 35,  22,  14},
   { 35,  22,  14}, { 35,  22,  14}, { 36,  22,  14}, { 36,  23,  14}, { 36,  23,  14},
   { 37,  23,  15}, { 37,  23,  15}, { 37,  23,  15}, { 38,  24,  15}, { 38,  24,  15},
   { 38,  24,  15}, { 39,  24,  15}, { 39,  24,  15}, { 39,  25,  16}, { 40,  25,  16},
   { 40,  25,  16}, { 40,  25,  16}, { 41,  25,  16}, { 41,  26,  16}, { 41,  26,  16},
   { 41,  26,  17}, { 42,  26,  17}, { 42,  26,  17}, { 42,  27,  17}, { 43,  27,  17},
   { 43,  27,  17}, { 43,  27,  17}, { 44,  27,  17}, { 44,  28,  18}, { 44,  28,  18},
   { 45,  28,  18}, { 45,  28,  18}, { 45,  28,  18}, { 46,  29,  18}, { 46,  29,  18},
   { 46,  29,  18}, { 47,  29,  19}, { 47,  29,  19}, { 47,  30,  19}, { 48,  30,  19},
   { 48,  30,  19}, { 48,  30,  19}, { 48,  30,  19}, { 49,  31,  19}, { 49,  31,  20},
   { 49,  31,  20}, { 50,  31,  20}, { 50,  31,  20}, { 50,  32,  20}, { 51,  32,  20},
   { 51,  32,  20}, { 51,  32,  20}, { 52,  32,  21}, { 52,  33,  21}, { 52,  33,  21},
   { 53,  33,  21}, { 53,  33,  21}, { 53,  33,  21}, { 54,  34,  21}, { 54,  34,  21},
   { 54,  34,  22}, { 55,  34,  22}, { 55,  34,  22}, { 55,  34,  22}, { 56,  35,  22},
   { 56,  35,  22}, { 56,  35,  22}, { 56,  35,  22}, { 57,  35,  23}, { 57,  36,  23},
   { 57,  36,  23}, { 58,  36,  23}, { 58,  36,  23}, { 58,  36,  23}, { 59,  37,  23},
   { 59,  37,  23}, { 59,  37,  24}, { 60,  37,  24}, { 60,  37,  24}, { 60,  38,  24},
   { 61,  38,  24}, { 61,  38,  24}, { 61,  38,  24}, { 62,  38,  25}, { 62,  39,  25},
   { 62,  39,  25}, { 63,  39,  25}, { 63,  39,  25}, { 63,  39,  25}, { 63,  40,  25},
   { 64,  40,  25}, { 64,  40,  26}, { 64,  40,  26}, { 65,  40,  26}, { 65,  41,  26},
   { 65,  41,  26}, { 66,  41,  26}, { 66,  41,  26}, { 66,  41,  26}, { 67,  42,  27},
   { 67,  42,  27}, { 67,  42,  27}, { 68,  42,  27}, { 68,  42,  27}, { 68,  43,  27},
   { 69,  43,  27}, { 69,  43,  27}, { 69,  43,  28}, { 70,  43,  28}, { 70,  44,  28},
   { 70,  44,  28}, { 71,  44,  28}, { 71,  44,  28}, { 71,  44,  28}, { 71,  45,  28},
   { 72,  45,  29}, { 72,  45,  29}, { 72,  45,  29}, { 73,  45,  29}, { 73,  46,  29},
   { 73,  46,  29}, { 74,  46,  29}, { 74,  46,  29}, { 74,  46,  30}, { 75,  47,  30},
   { 75,  47,  30}, { 75,  47,  30}, { 76,  47,  30}, { 76,  47,  30}, { 76,  48,  30},
   { 77,  48,  30}, { 77,  48,  31}, { 77,  48,  31}, { 78,  48,  31}, { 78,  49,  31},
   { 78,  49,  31}, { 78,  49,  31}, { 79,  49,  31}, { 79,  49,  31}, { 79,  50,  32},
   { 80,  50,  32}, { 80,  50,  32}, { 80,  50,  32}, { 81,  50,  32}, { 81,  51,  32},
   { 81,  51,  32}, { 82,  51,  33}, { 82,  51,  33}, { 82,  51,  33}, { 83,  52,  33},
   { 83,  52,  33}, { 83,  52,  33}, { 84,  52,  33}, { 84,  52,  33}, { 84,  53,  34},
   { 85,  53,  34}, { 85,  53,  34}, { 85,  53,  34}, { 86,  53,  34}, { 86,  54,  34},
   { 86,  54,  34}, { 86,  54,  34}, { 87,  54,  35}, { 87,  54,  35}, { 87,  55,  35},
   { 88,  55,  35}, { 88,  55,  35}, { 88,  55,  35}, { 89,  55,  35}, { 89,  56,  35},
   { 89,  56,  36}, { 90,  56,  36}, { 90,  56,  36}, { 90,  56,  36}, { 91,  57,  36},
   { 91,  57,  36}, { 91,  57,  36}, { 92,  57,  36}, { 92,  57,  37}, { 92,  58,  37},
   { 93,  58,  37}, { 93,  58,  37}, { 93,  58,  37}, { 93,  58,  37}, { 94,  59,  37},
   { 94,  59,  37}, { 94,  59,  38}, { 95,  59,  38}, { 95,  59,  38}, { 95,  60,  38},
   { 96,  60,  38}, { 96,  60,  38}, { 96,  60,  38}, { 97,  60,  38}, { 97,  61,  39},
   { 97,  61,  39}, { 98,  61,  39}, { 98,  61,  39}, { 98,  61,  39}, { 99,  62,  39},
   { 99,  62,  39}, { 99,  62,  39}, {100,  62,  40}, {100,  62,  40}, {100,  63,  40},
   {101,  63,  40}, {101,  63,  40}, {101,  63,  40}, {101,  63,  40}, {102,  64,  41},
   {102,  64,  41}, {102,  64,  41}, {103,  64,  41}, {103,  64,  41}, {103,  65,  41},
   {104,  65,  41}, {104,  65,  41}, {104,  65,  42}, {105,  65,  42}, {105,  66,  42},
   {105,  66,  42}, {106,  66,  42}, {106,  66,  42}, {106,  66,  42}, {107,  67,  42},
   {107,  67,  43}, {107,  67,  43}, {108,  67,  43}, {108,  67,  43}, {108,  68,  43},
   {108,  68,  43}, {109,  68,  43}, {109,  68,  43}, {109,  68,  44}, {110,  69,  44},
   {110,  69,  44}, {110,  69,  44}, {111,  69,  44}, {111,  69,  44}, {111,  70,  44},
   {112,  70,  44}, {112,  70,  45}, {112,  70,  45}, {113,  70,  45}, {113,  71,  45},
   {113,  71,  45}, {114,  71,  45}, {114,  71,  45}, {114,  71,  45}, {115,  72,  46},
   {115,  72,  46}, {115,  72,  46}, {116,  72,  46}, {116,  72,  46}, {116,  73,  46},
   {116,  73,  46}, {117,  73,  46}, {117,  73,  47}, {117,  73,  47}, {118,  74,  47},
   {118,  74,  47}, {118,  74,  47}, {119,  74,  47}, {119,  74,  47}, {119,  75,  47},
   {120,  75,  48}, {120,  75,  48}, {120,  75,  48}, {121,  75,  48}, {121,  76,  48},
   {121,  76,  48}, {122,  76,  48}, {122,  76,  49}, {122,  76,  49}, {123,  77,  49},
   {123,  77,  49}, {123,  77,  49}, {123,  77,  49}, {124,  77,  49}, {124,  78,  49},
   {124,  78,  50}, {125,  78,  50}, {125,  78,  50}, {125,  78,  50}, {126,  79,  50},
   {126,  79,  50}, {126,  79,  50}, {127,  79,  50}, {127,  79,  51}, {127,  80,  51},
   {128,  80,  51}, {128,  80,  51}, {128,  80,  51}, {129,  80,  51}, {129,  81,  51},
   {129,  81,  51}, {130,  81,  52}, {130,  81,  52}, {130,  81,  52}, {130,  82,  52},
   {131,  82,  52}, {131,  82,  52}, {131,  82,  52}, {132,  82,  52}, {132,  83,  53},
   {132,  83,  53}, {133,  83,  53}, {133,  83,  53}, {133,  83,  53}, {134,  84,  53},
   {134,  84,  53}, {134,  84,  53}, {135,  84,  54}, {135,  84,  54}, {135,  85,  54},
   {136,  85,  54}, {136,  85,  54}, {136,  85,  54}, {137,  85,  54}, {137,  86,  54},
   {137,  86,  55}, {138,  86,  55}, {138,  86,  55}, {138,  86,  55}, {138,  87,  55},
   {139,  87,  55}, {139,  87,  55}, {139,  87,  55}, {140,  87,  56}, {140,  88,  56},
   {140,  88,  56}, {141,  88,  56}, {141,  88,  56}, {141,  88,  56}, {142,  89,  56},
   {142,  89,  57}, {142,  89,  57}, {143,  89,  57}, {143,  89,  57}, {143,  90,  57},
   {144,  90,  57}, {144,  90,  57}, {144,  90,  57}, {145,  90,  58}, {145,  91,  58},
   {145,  91,  58}, {145,  91,  58}, {146,  91,  58}, {146,  91,  58}, {146,  92,  58},
   {147,  92,  58}, {147,  92,  59}, {147,  92,  59}, {148,  92,  59}, {148,  93,  59},
   {148,  93,  59}, {149,  93,  59}, {149,  93,  59}, {149,  93,  59}, {150,  94,  60},
   {150,  94,  60}, {150,  94,  60}, {151,  94,  60}, {151,  94,  60}, {151,  95,  60},
   {152,  95,  60}, {152,  95,  60}, {152,  95,  61}, {153,  95,  61}, {153,  96,  61},
   {153,  96,  61}, {153,  96,  61}, {154,  96,  61}, {154,  96,  61}, {154,  97,  61},
   {155,  97,  62}, {155,  97,  62}, {155,  97,  62}, {156,  97,  62}, {156,  98,  62},
   {156,  98,  62}, {157,  98,  62}, {157,  98,  62}, {157,  98,  63}, {158,  99,  63},
   {158,  99,  63}, {158,  99,  63}, {159,  99,  63}, {159,  99,  63}, {159, 100,  63},
   {160, 100,  63}, {160, 100,  64}, {160, 100,  64}, {160, 100,  64}, {161, 101,  64},
   {161, 101,  64}, {161, 101,  64}, {162, 101,  64}, {162, 101,  65}, {162, 101,  65},
   {163, 102,  65}, {163, 102,  65}, {163, 102,  65}, {164, 102,  65}, {164, 102,  65},
   {164, 103,  65}, {165, 103,  66}, {165, 103,  66}, {165, 103,  66}, {166, 103,  66},
   {166, 104,  66}, {166, 104,  66}, {167, 104,  66}, {167, 104,  66}, {167, 104,  67},
   {168, 105,  67}, {168, 105,  67}, {168, 105,  67}, {168, 105,  67}, {169, 105,  67},
   {169, 106,  67}, {169, 106,  67}, {170, 106,  68}, {170, 106,  68}, {170, 106,  68},
   {171, 107,  68}, {171, 107,  68}, {171, 107,  68}, {172, 107,  68}, {172, 107,  68},
   {172, 108,  69}, {173, 108,  69}, {173, 108,  69}, {173, 108,  69}, {174, 108,  69},
   {174, 109,  69}, {174, 109,  69}, {175, 109,  69}, {175, 109,  70}, {175, 109,  70},
   {175, 110,  70}, {176, 110,  70}, {176, 110,  70}, {176, 110,  70}, {177, 110,  70},
   {177, 111,  70}, {177, 111,  71}, {178, 111,  71}, {178, 111,  71}, {178, 111,  71},
   {179, 112,  71}, {179, 112,  71}, {179, 112,  71}, {180, 112,  71}, {180, 112,  72},
   {180, 113,  72}, {181, 113,  72}, {181, 113,  72}, {181, 113,  72}, {182, 113,  72},
   {182, 114,  72}, {182, 114,  73}, {183, 114,  73}, {183, 114,  73}, {183, 114,  73},
   {183, 115,  73}, {184, 115,  73}, {184, 115,  73}, {184, 115,  73}, {185, 115,  74},
   {185, 116,  74}, {185, 116,  74}, {186, 116,  74}, {186, 116,  74}, {186, 116,  74},
   {187, 117,  74}, {187, 117,  74}, {187, 117,  75}, {188, 117,  75}, {188, 117,  75},
   {188, 118,  75}, {189, 118,  75}, {189, 118,  75}, {189, 118,  75}, {190, 118,  75},
   {190, 119,  76}, {190, 119,  76}, {190, 119,  76}, {191, 119,  76}, {191, 119,  76},
   {191, 120,  76}, {192, 120,  76}, {192, 120,  76}, {192, 120,  77}, {193, 120,  77},
   {193, 121,  77}, {193, 121,  77}, {194, 121,  77}, {194, 121,  77}, {194, 121,  77},
   {195, 122,  77}, {195, 122,  78}, {195, 122,  78}, {196, 122,  78}, {196, 122,  78},
   {196, 123,  78}, {197, 123,  78}, {197, 123,  78}, {197, 123,  78}, {198, 123,  79},
   {198, 124,  79}, {198, 124,  79}, {198, 124,  79}, {199, 124,  79}, {199, 124,  79},
   {199, 125,  79}, {200, 125,  79}, {200, 125,  80}, {200, 125,  80}, {201, 125,  80},
   {201, 126,  80}, {201, 126,  80}, {202, 126,  80}, {202, 126,  80}, {202, 126,  81},
   {203, 127,  81}, {203, 127,  81}, {203, 127,  81}, {204, 127,  81}, {204, 127,  81},
   {204, 128,  81}, {205, 128,  81}, {205, 128,  82}, {205, 128,  82}, {205, 128,  82},
   {206, 129,  82}, {206, 129,  82}, {206, 129,  82}, {207, 129,  82}, {207, 129,  82},
   {207, 130,  83}, {208, 130,  83}, {208, 130,  83}, {208, 130,  83}, {209, 130,  83},
   {209, 131,  83}, {209, 131,  83}, {210, 131,  83}, {210, 131,  84}, {210, 131,  84},
   {211, 132,  84}, {211, 132,  84}, {211, 132,  84}, {212, 132,  84}, {212, 132,  84},
   {212, 133,  84}, {212, 133,  85}, {213, 133,  85}, {213, 133,  85}, {213, 133,  85},
   {214, 134,  85}, {214, 134,  85}, {214, 134,  85}, {215, 134,  85}, {215, 134,  86},
   {215, 135,  86}, {216, 135,  86}, {216, 135,  86}, {216, 135,  86}, {217, 135,  86},
   {217, 136,  86}, {217, 136,  86}, {218, 136,  87}, {218, 136,  87}, {218, 136,  87},
   {219, 137,  87}, {219, 137,  87}, {219, 137,  87}, {220, 137,  87}, {220, 137,  87},
   {220, 138,  88}, {220, 138,  88}, {221, 138,  88}, {221, 138,  88}, {221, 138,  88},
   {222, 139,  88}, {222, 139,  88}, {222, 139,  89}, {223, 139,  89}, {223, 139,  89},
   {223, 140,  89}, {224, 140,  89}, {224, 140,  89}, {224, 140,  89}, {225, 140,  89},
   {225, 141,  90}, {225, 141,  90}, {226, 141,  90}, {226, 141,  90}, {226, 141,  90},
   {227, 142,  90}, {227, 142,  90}, {227, 142,  90}, {227, 142,  91}, {228, 142,  91},
   {228, 143,  91}, {228, 143,  91}, {229, 143,  91}, {229, 143,  91}, {229, 143,  91},
   {230, 144,  91}, {230, 144,  92}, {230, 144,  92}, {231, 144,  92}, {231, 144,  92},
   {231, 145,  92}, {232, 145,  92}, {232, 145,  92}, {232, 145,  92}, {233, 145,  93},
   {233, 146,  93}, {233, 146,  93}, {234, 146,  93}, {234, 146,  93}, {234, 146,  93},
   {235, 147,  93}, {235, 147,  93}, {235, 147,  94}, {235, 147,  94}, {236, 147,  94},
   {236, 148,  94}, {236, 148,  94}, {237, 148,  94}, {237, 148,  94}, {237, 148,  94},
   {238, 149,  95}, {238, 149,  95}, {238, 149,  95}, {239, 149,  95}, {239, 149,  95},
   {239, 150,  95}, {240, 150,  95}, {240, 150,  95}, {240, 150,  96}, {241, 150,  96},
   {241, 151,  96}, {241, 151,  96}, {242, 151,  96}, {242, 151,  96}, {242, 151,  96},
   {242, 152,  97}, {243, 152,  97}, {243, 152,  97}, {243, 152,  97}, {244, 152,  97},
   {244, 153,  97}, {244, 153,  97}, {245, 153,  97}, {245, 153,  98}, {245, 153,  98},
   {246, 154,  98}, {246, 154,  98}, {246, 154,  98}, {247, 154,  98}, {247, 154,  98},
   {247, 155,  98}, {248, 155,  99}, {248, 155,  99}, {248, 155,  99}, {249, 155,  99},
   {249, 156,  99}, {249, 156,  99}, {250, 156,  99}, {250, 156,  99}, {250, 156, 100},
   {250, 157, 100}, {251, 157, 100}, {251, 157, 100}, {251, 157, 100}, {252, 157, 100},
   {252, 158, 100}, {252, 158, 100}, {253, 158, 101}, {253, 158, 101}, {253, 158, 101},
   {253, 159, 101}, {253, 159, 101}, {254, 159, 101}, {254, 159, 101}, {254, 159, 101},
   {254, 160, 102}, {254, 160, 102}, {254, 160, 102}, {254, 160, 102}, {254, 160, 102},
   {255, 161, 102}, {255, 161, 102}, {255, 161, 102}, {255, 161, 103}, {255, 161, 103},
   {255, 162, 103}, {255, 162, 103}, {255, 162, 103}, {255, 162, 103}, {255, 162, 103},
   {255, 163, 103}, {255, 163, 104}, {255, 163, 104}, {255, 163, 104}, {255, 163, 104},
   {255, 164, 104}, {255, 164, 104}, {255, 164, 104}, {255, 164, 105}, {255, 164, 105},
   {255, 165, 105}, {255, 165, 105}, {255, 165, 105}, {255, 165, 105}, {255, 165, 105},
   {255, 166, 105}, {255, 166, 106}, {255, 166, 106}, {255, 166, 106}, {255, 166, 106},
   {255, 167, 106}, {255, 167, 106}, {255, 167, 106}, {255, 167, 106}, {255, 167, 107},
   {255, 168, 107}, {255, 168, 107}, {255, 168, 107}, {255, 168, 107}, {255, 168, 107},
   {255, 168, 107}, {255, 169, 107}, {255, 169, 108}, {255, 169, 108}, {255, 169, 108},
   {255, 169, 108}, {255, 170, 108}, {255, 170, 108}, {255, 170, 108}, {255, 170, 108},
   {255, 170, 109}, {255, 171, 109}, {255, 171, 109}, {255, 171, 109}, {255, 171, 109},
   {255, 171, 109}, {255, 172, 109}, {255, 172, 109}, {255, 172, 110}, {255, 172, 110},
   {255, 172, 110}, {255, 173, 110}, {255, 173, 110}, {255, 173, 110}, {255, 173, 110},
   {255, 173, 110}, {255, 174, 111}, {255, 174, 111}, {255, 174, 111}, {255, 174, 111},
   {255, 174, 111}, {255, 175, 111}, {255, 175, 111}, {255, 175, 111}, {255, 175, 112},
   {255, 175, 112}, {255, 176, 112}, {255, 176, 112}, {255, 176, 112}, {255, 176, 112},
   {255, 176, 112}, {255, 177, 113}, {255, 177, 113}, {255, 177, 113}, {255, 177, 113},
   {255, 177, 113}, {255, 178, 113}, {255, 178, 113}, {255, 178, 113}, {255, 178, 114},
   {255, 178, 114}, {255, 179, 114}, {255, 179, 114}, {255, 179, 114}, {255, 179, 114},
   {255, 179, 114}, {255, 180, 114}, {255, 180, 115}, {255, 180, 115}, {255, 180, 115},
   {255, 180, 115}, {255, 181, 115}, {255, 181, 115}, {255, 181, 115}, {255, 181, 115},
   {255, 181, 116}, {255, 182, 116}, {255, 182, 116}, {255, 182, 116}, {255, 182, 116},
   {255, 182, 116}, {255, 183, 116}, {255, 183, 116}, {255, 183, 117}, {255, 183, 117},
   {255, 183, 117}, {255, 184, 117}, {255, 184, 117}, {255, 184, 117}, {255, 184, 117},
   {255, 184, 117}, {255, 185, 118}, {255, 185, 118}, {255, 185, 118}, {255, 185, 118},
   {255, 185, 118}, {255, 186, 118}, {255, 186, 118}, {255, 186, 118}, {255, 186, 119},
   {255, 186, 119}, {255, 187, 119}, {255, 187, 119}, {255, 187, 119}, {255, 187, 119},
   {255, 187, 119}, {255, 188, 119}, {255, 188, 120}, {255, 188, 120}, {255, 188, 120},
   {255, 188, 120}, {255, 189, 120}, {255, 189, 120}, {255, 189, 120}, {255, 189, 121},
   {255, 189, 121}, {255, 190, 121}, {255, 190, 121}, {255, 190, 121}, {255, 190, 121},
   {255, 190, 121}, {255, 191, 121}, {255, 191, 122}, {255, 191, 122}, {255, 191, 122},
   {255, 191, 122}, {255, 192, 122}, {255, 192, 122}, {255, 192, 122}, {255, 192, 122},
   {255, 192, 123}, {255, 193, 123}, {255, 193, 123}, {255, 193, 123}, {255, 193, 123},
   {255, 193, 123}, {255, 194, 123}, {255, 194, 123}, {255, 194, 124}, {255, 194, 124},
   {255, 194, 124}, {255, 195, 124}, {255, 195, 124}, {255, 195, 124}, {255, 195, 124},
   {255, 195, 124}, {255, 196, 125}, {255, 196, 125}, {255, 196, 125}, {255, 196, 125},
   {255, 196, 125}, {255, 197, 125}, {255, 197, 125}, {255, 197, 125}, {255, 197, 126},
   {255, 197, 126}, {255, 198, 126}, {255, 198, 126}, {255, 198, 126}, {255, 198, 126},
   {255, 198, 126}, {255, 199, 126}, {255, 199, 127}, {255, 199, 127}, {255, 199, 127}
};

const color gray_colormap[1000] = {
   {255, 255, 255}, {255, 255, 255}, {254, 254, 254}, {254, 254, 254}, {254, 254, 254},
   {254, 254, 254}, {253, 253, 253}, {253, 253, 253}, {253, 253, 253}, {253, 253, 253},
   {252, 252, 252}, {252, 252, 252}, {252, 252, 252}, {252, 252, 252}, {251, 251, 251},
   {251, 251, 251}, {251, 251, 251}, {251, 251, 251}, {250, 250, 250}, {250, 250, 250},
   {250, 250, 250}, {250, 250, 250}, {249, 249, 249}, {249, 249, 249}, {249, 249, 249},
   {249, 249, 249}, {248, 248, 248}, {248, 248, 248}, {248, 248, 248}, {248, 248, 248},
   {247, 247, 247}, {247, 247, 247}, {247, 247, 247}, {247, 247, 247}, {246, 246, 246},
   {246, 246, 246}, {246, 246, 246}, {246, 246, 246}, {245, 245, 245}, {245, 245, 245},
   {245, 245, 245}, {245, 245, 245}, {244, 244, 244}, {244, 244, 244}, {244, 244, 244},
   {244, 244, 244}, {243, 243, 243}, {243, 243, 243}, {243, 243, 243}, {242, 242, 242},
   {242, 242, 242}, {242, 242, 242}, {242, 242, 242}, {241, 241, 241}, {241, 241, 241},
   {241, 241, 241}, {241, 241, 241}, {240, 240, 240}, {240, 240, 240}, {240, 240, 240},
   {240, 240, 240}, {239, 239, 239}, {239, 239, 239}, {239, 239, 239}, {239, 239, 239},
   {238, 238, 238}, {238, 238, 238}, {238, 238, 238}, {238, 238, 238}, {237, 237, 237},
   {237, 237, 237}, {237, 237, 237}, {237, 237, 237}, {236, 236, 236}, {236, 236, 236},
   {236, 236, 236}, {236, 236, 236}, {235, 235, 235}, {235, 235, 235}, {235, 235, 235},
   {235, 235, 235}, {234, 234, 234}, {234, 234, 234}, {234, 234, 234}, {234, 234, 234},
   {233, 233, 233}, {233, 233, 233}, {233, 233, 233}, {233, 233, 233}, {232, 232, 232},
   {232, 232, 232}, {232, 232, 232}, {232, 232, 232}, {231, 231, 231}, {231, 231, 231},
   {231, 231, 231}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230},
   {229, 229, 229}, {229, 229, 229}, {229, 229, 229}, {229, 229, 229}, {228, 228, 228},
   {228, 228, 228}, {228, 228, 228}, {228, 228, 228}, {227, 227, 227}, {227, 227, 227},
   {227, 227, 227}, {227, 227, 227}, {226, 226, 226}, {226, 226, 226}, {226, 226, 226},
   {226, 226, 226}, {225, 225, 225}, {225, 225, 225}, {225, 225, 225}, {225, 225, 225},
   {224, 224, 224}, {224, 224, 224}, {224, 224, 224}, {224, 224, 224}, {223, 223, 223},
   {223, 223, 223}, {223, 223, 223}, {223, 223, 223}, {222, 222, 222}, {222, 222, 222},
   {222, 222, 222}, {222, 222, 222}, {221, 221, 221}, {221, 221, 221}, {221, 221, 221},
   {221, 221, 221}, {220, 220, 220}, {220, 220, 220}, {220, 220, 220}, {220, 220, 220},
   {219, 219, 219}, {219, 219, 219}, {219, 219, 219}, {218, 218, 218}, {218, 218, 218},
   {218, 218, 218}, {218, 218, 218}, {217, 217, 217}, {217, 217, 217}, {217, 217, 217},
   {217, 217, 217}, {216, 216, 216}, {216, 216, 216}, {216, 216, 216}, {216, 216, 216},
   {215, 215, 215}, {215, 215, 215}, {215, 215, 215}, {215, 215, 215}, {214, 214, 214},
   {214, 214, 214}, {214, 214, 214}, {214, 214, 214}, {213, 213, 213}, {213, 213, 213},
   {213, 213, 213}, {213, 213, 213}, {212, 212, 212}, {212, 212, 212}, {212, 212, 212},
   {212, 212, 212}, {211, 211, 211}, {211, 211, 211}, {211, 211, 211}, {211, 211, 211},
   {210, 210, 210}, {210, 210, 210}, {210, 210, 210}, {210, 210, 210}, {209, 209, 209},
   {209, 209, 209}, {209, 209, 209}, {209, 209, 209}, {208, 208, 208}, {208, 208, 208},
   {208, 208, 208}, {208, 208, 208}, {207, 207, 207}, {207, 207, 207}, {207, 207, 207},
   {207, 207, 207}, {206, 206, 206}, {206, 206, 206}, {206, 206, 206}, {205, 205, 205},
   {205, 205, 205}, {205, 205, 205}, {205, 205, 205}, {204, 204, 204}, {204, 204, 204},
   {204, 204, 204}, {204, 204, 204}, {203, 203, 203}, {203, 203, 203}, {203, 203, 203},
   {203, 203, 203}, {202, 202, 202}, {202, 202, 202}, {202, 202, 202}, {202, 202, 202},
   {201, 201, 201}, {201, 201, 201}, {201, 201, 201}, {201, 201, 201}, {200, 200, 200},
   {200, 200, 200}, {200, 200, 200}, {200, 200, 200}, {199, 199, 199}, {199, 199, 199},
   {199, 199, 199}, {199, 199, 199}, {198, 198, 198}, {198, 198, 198}, {198, 198, 198},
   {198, 198, 198}, {197, 197, 197}, {197, 197, 197}, {197, 197, 197}, {197, 197, 197},
   {196, 196, 196}, {196, 196, 196}, {196, 196, 196}, {196, 196, 196}, {195, 195, 195},
   {195, 195, 195}, {195, 195, 195}, {195, 195, 195}, {194, 194, 194}, {194, 194, 194},
   {194, 194, 194}, {193, 193, 193}, {193, 193, 193}, {193, 193, 193}, {193, 193, 193},
   {192, 192, 192}, {192, 192, 192}, {192, 192, 192}, {192, 192, 192}, {191, 191, 191},
   {191, 191, 191}, {191, 191, 191}, {191, 191, 191}, {190, 190, 190}, {190, 190, 190},
   {190, 190, 190}, {190, 190, 190}, {189, 189, 189}, {189, 189, 189}, {189, 189, 189},
   {189, 189, 189}, {188, 188, 188}, {188, 188, 188}, {188, 188, 188}, {188, 188, 188},
   {187, 187, 187}, {187, 187, 187}, {187, 187, 187}, {187, 187, 187}, {186, 186, 186},
   {186, 186, 186}, {186, 186, 186}, {186, 186, 186}, {185, 185, 185}, {185, 185, 185},
   {185, 185, 185}, {185, 185, 185}, {184, 184, 184}, {184, 184, 184}, {184, 184, 184},
   {184, 184, 184}, {183, 183, 183}, {183, 183, 183}, {183, 183, 183}, {183, 183, 183},
   {182, 182, 182}, {182, 182, 182}, {182, 182, 182}, {181, 181, 181}, {181, 181, 181},
   {181, 181, 181}, {181, 181, 181}, {180, 180, 180}, {180, 180, 180}, {180, 180, 180},
   {180, 180, 180}, {179, 179, 179}, {179, 179, 179}, {179, 179, 179}, {179, 179, 179},
   {178, 178, 178}, {178, 178, 178}, {178, 178, 178}, {178, 178, 178}, {177, 177, 177},
   {177, 177, 177}, {177, 177, 177}, {177, 177, 177}, {176, 176, 176}, {176, 176, 176},
   {176, 176, 176}, {176, 176, 176}, {175, 175, 175}, {175, 175, 175}, {175, 175, 175},
   {175, 175, 175}, {174, 174, 174}, {174, 174, 174}, {174, 174, 174}, {174, 174, 174},
   {173, 173, 173}, {173, 173, 173}, {173, 173, 173}, {173, 173, 173}, {172, 172, 172},
   {172, 172, 172}, {172, 172, 172}, {172, 172, 172}, {171, 171, 171}, {171, 171, 171},
   {171, 171, 171}, {171, 171, 171}, {170, 170, 170}, {170, 170, 170}, {170, 170, 170},
   {169, 169, 169}, {169, 169, 169}, {169, 169, 169}, {169, 169, 169}, {168, 168, 168},
   {168, 168, 168}, {168, 168, 168}, {168, 168, 168}, {167, 167, 167}, {167, 167, 167},
   {167, 167, 167}, {167, 167, 167}, {166, 166, 166}, {166, 166, 166}, {166, 166, 166},
   {166, 166, 166}, {165, 165, 165}, {165, 165, 165}, {165, 165, 165}, {165, 165, 165},
   {164, 164, 164}, {164, 164, 164}, {164, 164, 164}, {164, 164, 164}, {163, 163, 163},
   {163, 163, 163}, {163, 163, 163}, {163, 163, 163}, {162, 162, 162}, {162, 162, 162},
   {162, 162, 162}, {162, 162, 162}, {161, 161, 161}, {161, 161, 161}, {161, 161, 161},
   {161, 161, 161}, {160, 160, 160}, {160, 160, 160}, {160, 160, 160}, {160, 160, 160},
   {159, 159, 159}, {159, 159, 159}, {159, 159, 159}, {159, 159, 159}, {158, 158, 158},
   {158, 158, 158}, {158, 158, 158}, {157, 157, 157}, {157, 157, 157}, {157, 157, 157},
   {157, 157, 157}, {156, 156, 156}, {156, 156, 156}, {156, 156, 156}, {156, 156, 156},
   {155, 155, 155}, {155, 155, 155}, {155, 155, 155}, {155, 155, 155}, {154, 154, 154},
   {154, 154, 154}, {154, 154, 154}, {154, 154, 154}, {153, 153, 153}, {153, 153, 153},
   {153, 153, 153}, {153, 153, 153}, {152, 152, 152}, {152, 152, 152}, {152, 152, 152},
   {152, 152, 152}, {151, 151, 151}, {151, 151, 151}, {151, 151, 151}, {151, 151, 151},
   {150, 150, 150}, {150, 150, 150}, {150, 150, 150}, {150, 150, 150}, {149, 149, 149},
   {149, 149, 149}, {149, 149, 149}, {149, 149, 149}, {148, 148, 148}, {148, 148, 148},
   {148, 148, 148}, {148, 148, 148}, {147, 147, 147}, {147, 147, 147}, {147, 147, 147},
   {147, 147, 147}, {146, 146, 146}, {146, 146, 146}, {146, 146, 146}, {145, 145, 145},
   {145, 145, 145}, {145, 145, 145}, {145, 145, 145}, {144, 144, 144}, {144, 144, 144},
   {144, 144, 144}, {144, 144, 144}, {143, 143, 143}, {143, 143, 143}, {143, 143, 143},
   {143, 143, 143}, {142, 142, 142}, {142, 142, 142}, {142, 142, 142}, {142, 142, 142},
   {141, 141, 141}, {141, 141, 141}, {141, 141, 141}, {141, 141, 141}, {140, 140, 140},
   {140, 140, 140}, {140, 140, 140}, {140, 140, 140}, {139, 139, 139}, {139, 139, 139},
   {139, 139, 139}, {139, 139, 139}, {138, 138, 138}, {138, 138, 138}, {138, 138, 138},
   {138, 138, 138}, {137, 137, 137}, {137, 137, 137}, {137, 137, 137}, {137, 137, 137},
   {136, 136, 136}, {136, 136, 136}, {136, 136, 136}, {136, 136, 136}, {135, 135, 135},
   {135, 135, 135}, {135, 135, 135}, {135, 135, 135}, {134, 134, 134}, {134, 134, 134},
   {134, 134, 134}, {133, 133, 133}, {133, 133, 133}, {133, 133, 133}, {133, 133, 133},
   {132, 132, 132}, {132, 132, 132}, {132, 132, 132}, {132, 132, 132}, {131, 131, 131},
   {131, 131, 131}, {131, 131, 131}, {131, 131, 131}, {130, 130, 130}, {130, 130, 130},
   {130, 130, 130}, {130, 130, 130}, {129, 129, 129}, {129, 129, 129}, {129, 129, 129},
   {129, 129, 129}, {128, 128, 128}, {128, 128, 128}, {128, 128, 128}, {128, 128, 128},
   {127, 127, 127}, {127, 127, 127}, {127, 127, 127}, {127, 127, 127}, {126, 126, 126},
   {126, 126, 126}, {126, 126, 126}, {126, 126, 126}, {125, 125, 125}, {125, 125, 125},
   {125, 125, 125}, {125, 125, 125}, {124, 124, 124}, {124, 124, 124}, {124, 124, 124},
   {124, 124, 124}, {123, 123, 123}, {123, 123, 123}, {123, 123, 123}, {123, 123, 123},
   {122, 122, 122}, {122, 122, 122}, {122, 122, 122}, {122, 122, 122}, {121, 121, 121},
   {121, 121, 121}, {121, 121, 121}, {120, 120, 120}, {120, 120, 120}, {120, 120, 120},
   {120, 120, 120}, {119, 119, 119}, {119, 119, 119}, {119, 119, 119}, {119, 119, 119},
   {118, 118, 118}, {118, 118, 118}, {118, 118, 118}, {118, 118, 118}, {117, 117, 117},
   {117, 117, 117}, {117, 117, 117}, {117, 117, 117}, {116, 116, 116}, {116, 116, 116},
   {116, 116, 116}, {116, 116, 116}, {115, 115, 115}, {115, 115, 115}, {115, 115, 115},
   {115, 115, 115}, {114, 114, 114}, {114, 114, 114}, {114, 114, 114}, {114, 114, 114},
   {113, 113, 113}, {113, 113, 113}, {113, 113, 113}, {113, 113, 113}, {112, 112, 112},
   {112, 112, 112}, {112, 112, 112}, {112, 112, 112}, {111, 111, 111}, {111, 111, 111},
   {111, 111, 111}, {111, 111, 111}, {110, 110, 110}, {110, 110, 110}, {110, 110, 110},
   {110, 110, 110}, {109, 109, 109}, {109, 109, 109}, {109, 109, 109}, {108, 108, 108},
   {108, 108, 108}, {108, 108, 108}, {108, 108, 108}, {107, 107, 107}, {107, 107, 107},
   {107, 107, 107}, {107, 107, 107}, {106, 106, 106}, {106, 106, 106}, {106, 106, 106},
   {106, 106, 106}, {105, 105, 105}, {105, 105, 105}, {105, 105, 105}, {105, 105, 105},
   {104, 104, 104}, {104, 104, 104}, {104, 104, 104}, {104, 104, 104}, {103, 103, 103},
   {103, 103, 103}, {103, 103, 103}, {103, 103, 103}, {102, 102, 102}, {102, 102, 102},
   {102, 102, 102}, {102, 102, 102}, {101, 101, 101}, {101, 101, 101}, {101, 101, 101},
   {101, 101, 101}, {100, 100, 100}, {100, 100, 100}, {100, 100, 100}, {100, 100, 100},
   { 99,  99,  99}, { 99,  99,  99}, { 99,  99,  99}, { 99,  99,  99}, { 98,  98,  98},
   { 98,  98,  98}, { 98,  98,  98}, { 98,  98,  98}, { 97,  97,  97}, { 97,  97,  97},
   { 97,  97,  97}, { 96,  96,  96}, { 96,  96,  96}, { 96,  96,  96}, { 96,  96,  96},
   { 95,  95,  95}, { 95,  95,  95}, { 95,  95,  95}, { 95,  95,  95}, { 94,  94,  94},
   { 94,  94,  94}, { 94,  94,  94}, { 94,  94,  94}, { 93,  93,  93}, { 93,  93,  93},
   { 93,  93,  93}, { 93,  93,  93}, { 92,  92,  92}, { 92,  92,  92}, { 92,  92,  92},
   { 92,  92,  92}, { 91,  91,  91}, { 91,  91,  91}, { 91,  91,  91}, { 91,  91,  91},
   { 90,  90,  90}, { 90,  90,  90}, { 90,  90,  90}, { 90,  90,  90}, { 89,  89,  89},
   { 89,  89,  89}, { 89,  89,  89}, { 89,  89,  89}, { 88,  88,  88}, { 88,  88,  88},
   { 88,  88,  88}, { 88,  88,  88}, { 87,  87,  87}, { 87,  87,  87}, { 87,  87,  87},
   { 87,  87,  87}, { 86,  86,  86}, { 86,  86,  86}, { 86,  86,  86}, { 86,  86,  86},
   { 85,  85,  85}, { 85,  85,  85}, { 85,  85,  85}, { 84,  84,  84}, { 84,  84,  84},
   { 84,  84,  84}, { 84,  84,  84}, { 83,  83,  83}, { 83,  83,  83}, { 83,  83,  83},
   { 83,  83,  83}, { 82,  82,  82}, { 82,  82,  82}, { 82,  82,  82}, { 82,  82,  82},
   { 81,  81,  81}, { 81,  81,  81}, { 81,  81,  81}, { 81,  81,  81}, { 80,  80,  80},
   { 80,  80,  80}, { 80,  80,  80}, { 80,  80,  80}, { 79,  79,  79}, { 79,  79,  79},
   { 79,  79,  79}, { 79,  79,  79}, { 78,  78,  78}, { 78,  78,  78}, { 78,  78,  78},
   { 78,  78,  78}, { 77,  77,  77}, { 77,  77,  77}, { 77,  77,  77}, { 77,  77,  77},
   { 76,  76,  76}, { 76,  76,  76}, { 76,  76,  76}, { 76,  76,  76}, { 75,  75,  75},
   { 75,  75,  75}, { 75,  75,  75}, { 75,  75,  75}, { 74,  74,  74}, { 74,  74,  74},
   { 74,  74,  74}, { 74,  74,  74}, { 73,  73,  73}, { 73,  73,  73}, { 73,  73,  73},
   { 72,  72,  72}, { 72,  72,  72}, { 72,  72,  72}, { 72,  72,  72}, { 71,  71,  71},
   { 71,  71,  71}, { 71,  71,  71}, { 71,  71,  71}, { 70,  70,  70}, { 70,  70,  70},
   { 70,  70,  70}, { 70,  70,  70}, { 69,  69,  69}, { 69,  69,  69}, { 69,  69,  69},
   { 69,  69,  69}, { 68,  68,  68}, { 68,  68,  68}, { 68,  68,  68}, { 68,  68,  68},
   { 67,  67,  67}, { 67,  67,  67}, { 67,  67,  67}, { 67,  67,  67}, { 66,  66,  66},
   { 66,  66,  66}, { 66,  66,  66}, { 66,  66,  66}, { 65,  65,  65}, { 65,  65,  65},
   { 65,  65,  65}, { 65,  65,  65}, { 64,  64,  64}, { 64,  64,  64}, { 64,  64,  64},
   { 64,  64,  64}, { 63,  63,  63}, { 63,  63,  63}, { 63,  63,  63}, { 63,  63,  63},
   { 62,  62,  62}, { 62,  62,  62}, { 62,  62,  62}, { 62,  62,  62}, { 61,  61,  61},
   { 61,  61,  61}, { 61,  61,  61}, { 60,  60,  60}, { 60,  60,  60}, { 60,  60,  60},
   { 60,  60,  60}, { 59,  59,  59}, { 59,  59,  59}, { 59,  59,  59}, { 59,  59,  59},
   { 58,  58,  58}, { 58,  58,  58}, { 58,  58,  58}, { 58,  58,  58}, { 57,  57,  57},
   { 57,  57,  57}, { 57,  57,  57}, { 57,  57,  57}, { 56,  56,  56}, { 56,  56,  56},
   { 56,  56,  56}, { 56,  56,  56}, { 55,  55,  55}, { 55,  55,  55}, { 55,  55,  55},
   { 55,  55,  55}, { 54,  54,  54}, { 54,  54,  54}, { 54,  54,  54}, { 54,  54,  54},
   { 53,  53,  53}, { 53,  53,  53}, { 53,  53,  53}, { 53,  53,  53}, { 52,  52,  52},
   { 52,  52,  52}, { 52,  52,  52}, { 52,  52,  52}, { 51,  51,  51}, { 51,  51,  51},
   { 51,  51,  51}, { 51,  51,  51}, { 50,  50,  50}, { 50,  50,  50}, { 50,  50,  50},
   { 50,  50,  50}, { 49,  49,  49}, { 49,  49,  49}, { 49,  49,  49}, { 48,  48,  48},
   { 48,  48,  48}, { 48,  48,  48}, { 48,  48,  48}, { 47,  47,  47}, { 47,  47,  47},
   { 47,  47,  47}, { 47,  47,  47}, { 46,  46,  46}, { 46,  46,  46}, { 46,  46,  46},
   { 46,  46,  46}, { 45,  45,  45}, { 45,  45,  45}, { 45,  45,  45}, { 45,  45,  45},
   { 44,  44,  44}, { 44,  44,  44}, { 44,  44,  44}, { 44,  44,  44}, { 43,  43,  43},
   { 43,  43,  43}, { 43,  43,  43}, { 43,  43,  43}, { 42,  42,  42}, { 42,  42,  42},
   { 42,  42,  42}, { 42,  42,  42}, { 41,  41,  41}, { 41,  41,  41}, { 41,  41,  41},
   { 41,  41,  41}, { 40,  40,  40}, { 40,  40,  40}, { 40,  40,  40}, { 40,  40,  40},
   { 39,  39,  39}, { 39,  39,  39}, { 39,  39,  39}, { 39,  39,  39}, { 38,  38,  38},
   { 38,  38,  38}, { 38,  38,  38}, { 38,  38,  38}, { 37,  37,  37}, { 37,  37,  37},
   { 37,  37,  37}, { 37,  37,  37}, { 36,  36,  36}, { 36,  36,  36}, { 36,  36,  36},
   { 35,  35,  35}, { 35,  35,  35}, { 35,  35,  35}, { 35,  35,  35}, { 34,  34,  34},
   { 34,  34,  34}, { 34,  34,  34}, { 34,  34,  34}, { 33,  33,  33}, { 33,  33,  33},
   { 33,  33,  33}, { 33,  33,  33}, { 32,  32,  32}, { 32,  32,  32}, { 32,  32,  32},
   { 32,  32,  32}, { 31,  31,  31}, { 31,  31,  31}, { 31,  31,  31}, { 31,  31,  31},
   { 30,  30,  30}, { 30,  30,  30}, { 30,  30,  30}, { 30,  30,  30}, { 29,  29,  29},
   { 29,  29,  29}, { 29,  29,  29}, { 29,  29,  29}, { 28,  28,  28}, { 28,  28,  28},
   { 28,  28,  28}, { 28,  28,  28}, { 27,  27,  27}, { 27,  27,  27}, { 27,  27,  27},
   { 27,  27,  27}, { 26,  26,  26}, { 26,  26,  26}, { 26,  26,  26}, { 26,  26,  26},
   { 25,  25,  25}, { 25,  25,  25}, { 25,  25,  25}, { 25,  25,  25}, { 24,  24,  24},
   { 24,  24,  24}, { 24,  24,  24}, { 23,  23,  23}, { 23,  23,  23}, { 23,  23,  23},
   { 23,  23,  23}, { 22,  22,  22}, { 22,  22,  22}, { 22,  22,  22}, { 22,  22,  22},
   { 21,  21,  21}, { 21,  21,  21}, { 21,  21,  21}, { 21,  21,  21}, { 20,  20,  20},
   { 20,  20,  20}, { 20,  20,  20}, { 20,  20,  20}, { 19,  19,  19}, { 19,  19,  19},
   { 19,  19,  19}, { 19,  19,  19}, { 18,  18,  18}, { 18,  18,  18}, { 18,  18,  18},
   { 18,  18,  18}, { 17,  17,  17}, { 17,  17,  17}, { 17,  17,  17}, { 17,  17,  17},
   { 16,  16,  16}, { 16,  16,  16}, { 16,  16,  16}, { 16,  16,  16}, { 15,  15,  15},
   { 15,  15,  15}, { 15,  15,  15}, { 15,  15,  15}, { 14,  14,  14}, { 14,  14,  14},
   { 14,  14,  14}, { 14,  14,  14}, { 13,  13,  13}, { 13,  13,  13}, { 13,  13,  13},
   { 13,  13,  13}, { 12,  12,  12}, { 12,  12,  12}, { 12,  12,  12}, { 11,  11,  11},
   { 11,  11,  11}, { 11,  11,  11}, { 11,  11,  11}, { 10,  10,  10}, { 10,  10,  10},
   { 10,  10,  10}, { 10,  10,  10}, {  9,   9,   9}, {  9,   9,   9}, {  9,   9,   9},
   {  9,   9,   9}, {  8,   8,   8}, {  8,   8,   8}, {  8,   8,   8}, {  8,   8,   8},
   {  7,   7,   7}, {  7,   7,   7}, {  7,   7,   7}, {  7,   7,   7}, {  6,   6,   6},
   {  6,   6,   6}, {  6,   6,   6}, {  6,   6,   6}, {  5,   5,   5}, {  5,   5,   5},
   {  5,   5,   5}, {  5,   5,   5}, {  4,   4,   4}, {  4,   4,   4}, {  4,   4,   4},
   {  4,   4,   4}, {  3,   3,   3}, {  3,   3,   3}, {  3,   3,   3}, {  3,   3,   3},
   {  2,   2,   2}, {  2,   2,   2}, {  2,   2,   2}, {  2,   2,   2}, {  1,   1,   1},
   {  1,   1,   1}, {  1,   1,   1}, {  1,   1,   1}, {  0,   0,   0}, {  0,   0,   0}
};

const color hot_colormap[1000] = {
   { 11,   0,   0}, { 11,   0,   0}, { 12,   0,   0}, { 13,   0,   0}, { 13,   0,   0},
   { 14,   0,   0}, { 15,   0,   0}, { 15,   0,   0}, { 16,   0,   0}, { 17,   0,   0},
   { 17,   0,   0}, { 18,   0,   0}, { 19,   0,   0}, { 19,   0,   0}, { 20,   0,   0},
   { 21,   0,   0}, { 21,   0,   0}, { 22,   0,   0}, { 23,   0,   0}, { 23,   0,   0},
   { 24,   0,   0}, { 25,   0,   0}, { 25,   0,   0}, { 26,   0,   0}, { 27,   0,   0},
   { 27,   0,   0}, { 28,   0,   0}, { 29,   0,   0}, { 29,   0,   0}, { 30,   0,   0},
   { 31,   0,   0}, { 31,   0,   0}, { 32,   0,   0}, { 33,   0,   0}, { 33,   0,   0},
   { 34,   0,   0}, { 35,   0,   0}, { 35,   0,   0}, { 36,   0,   0}, { 37,   0,   0},
   { 37,   0,   0}, { 38,   0,   0}, { 39,   0,   0}, { 39,   0,   0}, { 40,   0,   0},
   { 41,   0,   0}, { 41,   0,   0}, { 42,   0,   0}, { 43,   0,   0}, { 43,   0,   0},
   { 44,   0,   0}, { 45,   0,   0}, { 45,   0,   0}, { 46,   0,   0}, { 47,   0,   0},
   { 47,   0,   0}, { 48,   0,   0}, { 49,   0,   0}, { 49,   0,   0}, { 50,   0,   0},
   { 51,   0,   0}, { 51,   0,   0}, { 52,   0,   0}, { 53,   0,   0}, { 54,   0,   0},
   { 54,   0,   0}, { 55,   0,   0}, { 56,   0,   0}, { 56,   0,   0}, { 57,   0,   0},
   { 58,   0,   0}, { 58,   0,   0}, { 59,   0,   0}, { 60,   0,   0}, { 60,   0,   0},
   { 61,   0,   0}, { 62,   0,   0}, { 62,   0,   0}, { 63,   0,   0}, { 64,   0,   0},
   { 64,   0,   0}, { 65,   0,   0}, { 66,   0,   0}, { 66,   0,   0}, { 67,   0,   0},
   { 68,   0,   0}, { 68,   0,   0}, { 69,   0,   0}, { 70,   0,   0}, { 70,   0,   0},
   { 71,   0,   0}, { 72,   0,   0}, { 72,   0,   0}, { 73,   0,   0}, { 74,   0,   0},
   { 74,   0,   0}, { 75,   0,   0}, { 76,   0,   0}, { 76,   0,   0}, { 77,   0,   0},
   { 78,   0,   0}, { 78,   0,   0}, { 79,   0,   0}, { 80,   0,   0}, { 80,   0,   0},
   { 81,   0,   0}, { 82,   0,   0}, { 82,   0,   0}, { 83,   0,   0}, { 84,   0,   0},
   { 84,   0,   0}, { 85,   0,   0}, { 86,   0,   0}, { 86,   0,   0}, { 87,   0,   0},
   { 88,   0,   0}, { 88,   0,   0}, { 89,   0,   0}, { 90,   0,   0}, { 90,   0,   0},
   { 91,   0,   0}, { 92,   0,   0}, { 92,   0,   0}, { 93,   0,   0}, { 94,   0,   0},
   { 94,   0,   0}, { 95,   0,   0}, { 96,   0,   0}, { 96,   0,   0}, { 97,   0,   0},
   { 98,   0,   0}, { 98,   0,   0}, { 99,   0,   0}, {100,   0,   0}, {100,   0,   0},
   {101,   0,   0}, {102,   0,   0}, {102,   0,   0}, {103,   0,   0}, {104,   0,   0},
   {104,   0,   0}, {105,   0,   0}, {106,   0,   0}, {106,   0,   0}, {107,   0,   0},
   {108,   0,   0}, {108,   0,   0}, {109,   0,   0}, {110,   0,   0}, {110,   0,   0},
   {111,   0,   0}, {112,   0,   0}, {112,   0,   0}, {113,   0,   0}, {114,   0,   0},
   {114,   0,   0}, {115,   0,   0}, {116,   0,   0}, {116,   0,   0}, {117,   0,   0},
   {118,   0,   0}, {119,   0,   0}, {119,   0,   0}, {120,   0,   0}, {121,   0,   0},
   {121,   0,   0}, {122,   0,   0}, {123,   0,   0}, {123,   0,   0}, {124,   0,   0},
   {125,   0,   0}, {125,   0,   0}, {126,   0,   0}, {127,   0,   0}, {127,   0,   0},
   {128,   0,   0}, {129,   0,   0}, {129,   0,   0}, {130,   0,   0}, {131,   0,   0},
   {131,   0,   0}, {132,   0,   0}, {133,   0,   0}, {133,   0,   0}, {134,   0,   0},
   {135,   0,   0}, {135,   0,   0}, {136,   0,   0}, {137,   0,   0}, {137,   0,   0},
   {138,   0,   0}, {139,   0,   0}, {139,   0,   0}, {140,   0,   0}, {141,   0,   0},
   {141,   0,   0}, {142,   0,   0}, {143,   0,   0}, {143,   0,   0}, {144,   0,   0},
   {145,   0,   0}, {145,   0,   0}, {146,   0,   0}, {147,   0,   0}, {147,   0,   0},
   {148,   0,   0}, {149,   0,   0}, {149,   0,   0}, {150,   0,   0}, {151,   0,   0},
   {151,   0,   0}, {152,   0,   0}, {153,   0,   0}, {153,   0,   0}, {154,   0,   0},
   {155,   0,   0}, {155,   0,   0}, {156,   0,   0}, {157,   0,   0}, {157,   0,   0},
   {158,   0,   0}, {159,   0,   0}, {159,   0,   0}, {160,   0,   0}, {161,   0,   0},
   {161,   0,   0}, {162,   0,   0}, {163,   0,   0}, {163,   0,   0}, {164,   0,   0},
   {165,   0,   0}, {165,   0,   0}, {166,   0,   0}, {167,   0,   0}, {167,   0,   0},
   {168,   0,   0}, {169,   0,   0}, {169,   0,   0}, {170,   0,   0}, {171,   0,   0},
   {171,   0,   0}, {172,   0,   0}, {173,   0,   0}, {173,   0,   0}, {174,   0,   0},
   {175,   0,   0}, {175,   0,   0}, {176,   0,   0}, {177,   0,   0}, {177,   0,   0},
   {178,   0,   0}, {179,   0,   0}, {179,   0,   0}, {180,   0,   0}, {181,   0,   0},
   {181,   0,   0}, {182,   0,   0}, {183,   0,   0}, {183,   0,   0}, {184,   0,   0},
   {185,   0,   0}, {186,   0,   0}, {186,   0,   0}, {187,   0,   0}, {188,   0,   0},
   {188,   0,   0}, {189,   0,   0}, {190,   0,   0}, {190,   0,   0}, {191,   0,   0},
   {192,   0,   0}, {192,   0,   0}, {193,   0,   0}, {194,   0,   0}, {194,   0,   0},
   {195,   0,   0}, {196,   0,   0}, {196,   0,   0}, {197,   0,   0}, {198,   0,   0},
   {198,   0,   0}, {199,   0,   0}, {200,   0,   0}, {200,   0,   0}, {201,   0,   0},
   {202,   0,   0}, {202,   0,   0}, {203,   0,   0}, {204,   0,   0}, {204,   0,   0},
   {205,   0,   0}, {206,   0,   0}, {206,   0,   0}, {207,   0,   0}, {208,   0,   0},
   {208,   0,   0}, {209,   0,   0}, {210,   0,   0}, {210,   0,   0}, {211,   0,   0},
   {212,   0,   0}, {212,   0,   0}, {213,   0,   0}, {214,   0,   0}, {214,   0,   0},
   {215,   0,   0}, {216,   0,   0}, {216,   0,   0}, {217,   0,   0}, {218,   0,   0},
   {218,   0,   0}, {219,   0,   0}, {220,   0,   0}, {220,   0,   0}, {221,   0,   0},
   {222,   0,   0}, {222,   0,   0}, {223,   0,   0}, {224,   0,   0}, {224,   0,   0},
   {225,   0,   0}, {226,   0,   0}, {226,   0,   0}, {227,   0,   0}, {228,   0,   0},
   {228,   0,   0}, {229,   0,   0}, {230,   0,   0}, {230,   0,   0}, {231,   0,   0},
   {232,   0,   0}, {232,   0,   0}, {233,   0,   0}, {234,   0,   0}, {234,   0,   0},
   {235,   0,   0}, {236,   0,   0}, {236,   0,   0}, {237,   0,   0}, {238,   0,   0},
   {238,   0,   0}, {239,   0,   0}, {240,   0,   0}, {240,   0,   0}, {241,   0,   0},
   {242,   0,   0}, {242,   0,   0}, {243,   0,   0}, {244,   0,   0}, {244,   0,   0},
   {245,   0,   0}, {246,   0,   0}, {246,   0,   0}, {247,   0,   0}, {248,   0,   0},
   {248,   0,   0}, {249,   0,   0}, {250,   0,   0}, {251,   0,   0}, {251,   0,   0},
   {252,   0,   0}, {253,   0,   0}, {253,   0,   0}, {254,   0,   0}, {255,   0,   0},
   {255,   0,   0}, {255,   1,   0}, {255,   2,   0}, {255,   2,   0}, {255,   3,   0},
   {255,   4,   0}, {255,   4,   0}, {255,   5,   0}, {255,   6,   0}, {255,   6,   0},
   {255,   7,   0}, {255,   8,   0}, {255,   8,   0}, {255,   9,   0}, {255,  10,   0},
   {255,  10,   0}, {255,  11,   0}, {255,  12,   0}, {255,  12,   0}, {255,  13,   0},
   {255,  14,   0}, {255,  14,   0}, {255,  15,   0}, {255,  16,   0}, {255,  16,   0},
   {255,  17,   0}, {255,  18,   0}, {255,  18,   0}, {255,  19,   0}, {255,  20,   0},
   {255,  20,   0}, {255,  21,   0}, {255,  22,   0}, {255,  22,   0}, {255,  23,   0},
   {255,  24,   0}, {255,  24,   0}, {255,  25,   0}, {255,  26,   0}, {255,  26,   0},
   {255,  27,   0}, {255,  28,   0}, {255,  28,   0}, {255,  29,   0}, {255,  30,   0},
   {255,  30,   0}, {255,  31,   0}, {255,  32,   0}, {255,  32,   0}, {255,  33,   0},
   {255,  34,   0}, {255,  34,   0}, {255,  35,   0}, {255,  36,   0}, {255,  36,   0},
   {255,  37,   0}, {255,  38,   0}, {255,  38,   0}, {255,  39,   0}, {255,  40,   0},
   {255,  40,   0}, {255,  41,   0}, {255,  42,   0}, {255,  42,   0}, {255,  43,   0},
   {255,  44,   0}, {255,  44,   0}, {255,  45,   0}, {255,  46,   0}, {255,  46,   0},
   {255,  47,   0}, {255,  48,   0}, {255,  48,   0}, {255,  49,   0}, {255,  50,   0},
   {255,  50,   0}, {255,  51,   0}, {255,  52,   0}, {255,  52,   0}, {255,  53,   0},
   {255,  54,   0}, {255,  54,   0}, {255,  55,   0}, {255,  56,   0}, {255,  56,   0},
   {255,  57,   0}, {255,  58,   0}, {255,  58,   0}, {255,  59,   0}, {255,  60,   0},
   {255,  60,   0}, {255,  61,   0}, {255,  62,   0}, {255,  63,   0}, {255,  63,   0},
   {255,  64,   0}, {255,  65,   0}, {255,  65,   0}, {255,  66,   0}, {255,  67,   0},
   {255,  67,   0}, {255,  68,   0}, {255,  69,   0}, {255,  69,   0}, {255,  70,   0},
   {255,  71,   0}, {255,  71,   0}, {255,  72,   0}, {255,  73,   0}, {255,  73,   0},
   {255,  74,   0}, {255,  75,   0}, {255,  75,   0}, {255,  76,   0}, {255,  77,   0},
   {255,  77,   0}, {255,  78,   0}, {255,  79,   0}, {255,  79,   0}, {255,  80,   0},
   {255,  81,   0}, {255,  81,   0}, {255,  82,   0}, {255,  83,   0}, {255,  83,   0},
   {255,  84,   0}, {255,  85,   0}, {255,  85,   0}, {255,  86,   0}, {255,  87,   0},
   {255,  87,   0}, {255,  88,   0}, {255,  89,   0}, {255,  89,   0}, {255,  90,   0},
   {255,  91,   0}, {255,  91,   0}, {255,  92,   0}, {255,  93,   0}, {255,  93,   0},
   {255,  94,   0}, {255,  95,   0}, {255,  95,   0}, {255,  96,   0}, {255,  97,   0},
   {255,  97,   0}, {255,  98,   0}, {255,  99,   0}, {255,  99,   0}, {255, 100,   0},
   {255, 101,   0}, {255, 101,   0}, {255, 102,   0}, {255, 103,   0}, {255, 103,   0},
   {255, 104,   0}, {255, 105,   0}, {255, 105,   0}, {255, 106,   0}, {255, 107,   0},
   {255, 107,   0}, {255, 108,   0}, {255, 109,   0}, {255, 109,   0}, {255, 110,   0},
   {255, 111,   0}, {255, 111,   0}, {255, 112,   0}, {255, 113,   0}, {255, 113,   0},
   {255, 114,   0}, {255, 115,   0}, {255, 115,   0}, {255, 116,   0}, {255, 117,   0},
   {255, 117,   0}, {255, 118,   0}, {255, 119,   0}, {255, 119,   0}, {255, 120,   0},
   {255, 121,   0}, {255, 121,   0}, {255, 122,   0}, {255, 123,   0}, {255, 123,   0},
   {255, 124,   0}, {255, 125,   0}, {255, 125,   0}, {255, 126,   0}, {255, 127,   0},
   {255, 128,   0}, {255, 128,   0}, {255, 129,   0}, {255, 130,   0}, {255, 130,   0},
   {255, 131,   0}, {255, 132,   0}, {255, 132,   0}, {255, 133,   0}, {255, 134,   0},
   {255, 134,   0}, {255, 135,   0}, {255, 136,   0}, {255, 136,   0}, {255, 137,   0},
   {255, 138,   0}, {255, 138,   0}, {255, 139,   0}, {255, 140,   0}, {255, 140,   0},
   {255, 141,   0}, {255, 142,   0}, {255, 142,   0}, {255, 143,   0}, {255, 144,   0},
   {255, 144,   0}, {255, 145,   0}, {255, 146,   0}, {255, 146,   0}, {255, 147,   0},
   {255, 148,   0}, {255, 148,   0}, {255, 149,   0}, {255, 150,   0}, {255, 150,   0},
   {255, 151,   0}, {255, 152,   0}, {255, 152,   0}, {255, 153,   0}, {255, 154,   0},
   {255, 154,   0}, {255, 155,   0}, {255, 156,   0}, {255, 156,   0}, {255, 157,   0},
   {255, 158,   0}, {255, 158,   0}, {255, 159,   0}, {255, 160,   0}, {255, 160,   0},
   {255, 161,   0}, {255, 162,   0}, {255, 162,   0}, {255, 163,   0}, {255, 164,   0},
   {255, 164,   0}, {255, 165,   0}, {255, 166,   0}, {255, 166,   0}, {255, 167,   0},
   {255, 168,   0}, {255, 168,   0}, {255, 169,   0}, {255, 170,   0}, {255, 170,   0},
   {255, 171,   0}, {255, 172,   0}, {255, 172,   0}, {255, 173,   0}, {255, 174,   0},
   {255, 174,   0}, {255, 175,   0}, {255, 176,   0}, {255, 176,   0}, {255, 177,   0},
   {255, 178,   0}, {255, 178,   0}, {255, 179,   0}, {255, 180,   0}, {255, 180,   0},
   {255, 181,   0}, {255, 182,   0}, {255, 182,   0}, {255, 183,   0}, {255, 184,   0},
   {255, 184,   0}, {255, 185,   0}, {255, 186,   0}, {255, 186,   0}, {255, 187,   0},
   {255, 188,   0}, {255, 188,   0}, {255, 189,   0}, {255, 190,   0}, {255, 190,   0},
   {255, 191,   0}, {255, 192,   0}, {255, 192,   0}, {255, 193,   0}, {255, 194,   0},
   {255, 195,   0}, {255, 195,   0}, {255, 196,   0}, {255, 197,   0}, {255, 197,   0},
   {255, 198,   0}, {255, 199,   0}, {255, 199,   0}, {255, 200,   0}, {255, 201,   0},
   {255, 201,   0}, {255, 202,   0}, {255, 203,   0}, {255, 203,   0}, {255, 204,   0},
   {255, 205,   0}, {255, 205,   0}, {255, 206,   0}, {255, 207,   0}, {255, 207,   0},
   {255, 208,   0}, {255, 209,   0}, {255, 209,   0}, {255, 210,   0}, {255, 211,   0},
   {255, 211,   0}, {255, 212,   0}, {255, 213,   0}, {255, 213,   0}, {255, 214,   0},
   {255, 215,   0}, {255, 215,   0}, {255, 216,   0}, {255, 217,   0}, {255, 217,   0},
   {255, 218,   0}, {255, 219,   0}, {255, 219,   0}, {255, 220,   0}, {255, 221,   0},
   {255, 221,   0}, {255, 222,   0}, {255, 223,   0}, {255, 223,   0}, {255, 224,   0},
   {255, 225,   0}, {255, 225,   0}, {255, 226,   0}, {255, 227,   0}, {255, 227,   0},
   {255, 228,   0}, {255, 229,   0}, {255, 229,   0}, {255, 230,   0}, {255, 231,   0},
   {255, 231,   0}, {255, 232,   0}, {255, 233,   0}, {255, 233,   0}, {255, 234,   0},
   {255, 235,   0}, {255, 235,   0}, {255, 236,   0}, {255, 237,   0}, {255, 237,   0},
   {255, 238,   0}, {255, 239,   0}, {255, 239,   0}, {255, 240,   0}, {255, 241,   0},
   {255, 241,   0}, {255, 242,   0}, {255, 243,   0}, {255, 243,   0}, {255, 244,   0},
   {255, 245,   0}, {255, 245,   0}, {255, 246,   0}, {255, 247,   0}, {255, 247,   0},
   {255, 248,   0}, {255, 249,   0}, {255, 249,   0}, {255, 250,   0}, {255, 251,   0},
   {255, 251,   0}, {255, 252,   0}, {255, 253,   0}, {255, 253,   0}, {255, 254,   0},
   {255, 255,   0}, {255, 255,   1}, {255, 255,   2}, {255, 255,   3}, {255, 255,   4},
   {255, 255,   5}, {255, 255,   6}, {255, 255,   7}, {255, 255,   8}, {255, 255,   9},
   {255, 255,  10}, {255, 255,  11}, {255, 255,  12}, {255, 255,  13}, {255, 255,  14},
   {255, 255,  15}, {255, 255,  16}, {255, 255,  17}, {255, 255,  18}, {255, 255,  19},
   {255, 255,  20}, {255, 255,  21}, {255, 255,  22}, {255, 255,  23}, {255, 255,  24},
   {255, 255,  25}, {255, 255,  26}, {255, 255,  27}, {255, 255,  28}, {255, 255,  29},
   {255, 255,  30}, {255, 255,  31}, {255, 255,  32}, {255, 255,  33}, {255, 255,  34},
   {255, 255,  35}, {255, 255,  36}, {255, 255,  37}, {255, 255,  38}, {255, 255,  39},
   {255, 255,  40}, {255, 255,  41}, {255, 255,  42}, {255, 255,  43}, {255, 255,  44},
   {255, 255,  45}, {255, 255,  46}, {255, 255,  47}, {255, 255,  48}, {255, 255,  49},
   {255, 255,  50}, {255, 255,  51}, {255, 255,  52}, {255, 255,  53}, {255, 255,  54},
   {255, 255,  55}, {255, 255,  56}, {255, 255,  57}, {255, 255,  58}, {255, 255,  59},
   {255, 255,  60}, {255, 255,  61}, {255, 255,  62}, {255, 255,  63}, {255, 255,  64},
   {255, 255,  65}, {255, 255,  66}, {255, 255,  67}, {255, 255,  68}, {255, 255,  69},
   {255, 255,  70}, {255, 255,  71}, {255, 255,  72}, {255, 255,  73}, {255, 255,  74},
   {255, 255,  75}, {255, 255,  76}, {255, 255,  77}, {255, 255,  78}, {255, 255,  79},
   {255, 255,  80}, {255, 255,  81}, {255, 255,  82}, {255, 255,  83}, {255, 255,  84},
   {255, 255,  85}, {255, 255,  86}, {255, 255,  87}, {255, 255,  88}, {255, 255,  89},
   {255, 255,  90}, {255, 255,  91}, {255, 255,  92}, {255, 255,  93}, {255, 255,  94},
   {255, 255,  95}, {255, 255,  96}, {255, 255,  97}, {255, 255,  98}, {255, 255,  99},
   {255, 255, 100}, {255, 255, 101}, {255, 255, 102}, {255, 255, 103}, {255, 255, 104},
   {255, 255, 105}, {255, 255, 106}, {255, 255, 107}, {255, 255, 108}, {255, 255, 109},
   {255, 255, 110}, {255, 255, 111}, {255, 255, 112}, {255, 255, 113}, {255, 255, 114},
   {255, 255, 115}, {255, 255, 116}, {255, 255, 117}, {255, 255, 118}, {255, 255, 119},
   {255, 255, 120}, {255, 255, 121}, {255, 255, 122}, {255, 255, 123}, {255, 255, 124},
   {255, 255, 125}, {255, 255, 126}, {255, 255, 127}, {255, 255, 128}, {255, 255, 129},
   {255, 255, 130}, {255, 255, 131}, {255, 255, 132}, {255, 255, 133}, {255, 255, 134},
   {255, 255, 135}, {255, 255, 136}, {255, 255, 137}, {255, 255, 138}, {255, 255, 139},
   {255, 255, 140}, {255, 255, 141}, {255, 255, 142}, {255, 255, 143}, {255, 255, 144},
   {255, 255, 145}, {255, 255, 146}, {255, 255, 147}, {255, 255, 148}, {255, 255, 149},
   {255, 255, 150}, {255, 255, 151}, {255, 255, 152}, {255, 255, 153}, {255, 255, 154},
   {255, 255, 155}, {255, 255, 157}, {255, 255, 158}, {255, 255, 159}, {255, 255, 160},
   {255, 255, 161}, {255, 255, 162}, {255, 255, 163}, {255, 255, 164}, {255, 255, 165},
   {255, 255, 166}, {255, 255, 167}, {255, 255, 168}, {255, 255, 169}, {255, 255, 170},
   {255, 255, 171}, {255, 255, 172}, {255, 255, 173}, {255, 255, 174}, {255, 255, 175},
   {255, 255, 176}, {255, 255, 177}, {255, 255, 178}, {255, 255, 179}, {255, 255, 180},
   {255, 255, 181}, {255, 255, 182}, {255, 255, 183}, {255, 255, 184}, {255, 255, 185},
   {255, 255, 186}, {255, 255, 187}, {255, 255, 188}, {255, 255, 189}, {255, 255, 190},
   {255, 255, 191}, {255, 255, 192}, {255, 255, 193}, {255, 255, 194}, {255, 255, 195},
   {255, 255, 196}, {255, 255, 197}, {255, 255, 198}, {255, 255, 199}, {255, 255, 200},
   {255, 255, 201}, {255, 255, 202}, {255, 255, 203}, {255, 255, 204}, {255, 255, 205},
   {255, 255, 206}, {255, 255, 207}, {255, 255, 208}, {255, 255, 209}, {255, 255, 210},
   {255, 255, 211}, {255, 255, 212}, {255, 255, 213}, {255, 255, 214}, {255, 255, 215},
   {255, 255, 216}, {255, 255, 217}, {255, 255, 218}, {255, 255, 219}, {255, 255, 220},
   {255, 255, 221}, {255, 255, 222}, {255, 255, 223}, {255, 255, 224}, {255, 255, 225},
   {255, 255, 226}, {255, 255, 227}, {255, 255, 228}, {255, 255, 229}, {255, 255, 230},
   {255, 255, 231}, {255, 255, 232}, {255, 255, 233}, {255, 255, 234}, {255, 255, 235},
   {255, 255, 236}, {255, 255, 237}, {255, 255, 238}, {255, 255, 239}, {255, 255, 240},
   {255, 255, 241}, {255, 255, 242}, {255, 255, 243}, {255, 255, 244}, {255, 255, 245},
   {255, 255, 246}, {255, 255, 247}, {255, 255, 248}, {255, 255, 249}, {255, 255, 250},
   {255, 255, 251}, {255, 255, 252}, {255, 255, 253}, {255, 255, 254}, {255, 255, 255}
};

const color hsv_colormap[1000] = {
   {255,   0,   0}, {255,   2,   0}, {255,   3,   0}, {255,   5,   0}, {255,   6,   0},
   {255,   8,   0}, {255,   9,   0}, {255,  11,   0}, {255,  12,   0}, {255,  14,   0},
   {255,  15,   0}, {255,  17,   0}, {255,  18,   0}, {255,  20,   0}, {255,  21,   0},
   {255,  23,   0}, {255,  24,   0}, {255,  26,   0}, {255,  27,   0}, {255,  29,   0},
   {255,  30,   0}, {255,  32,   0}, {255,  33,   0}, {255,  35,   0}, {255,  36,   0},
   {255,  38,   0}, {255,  39,   0}, {255,  41,   0}, {255,  42,   0}, {255,  44,   0},
   {255,  45,   0}, {255,  47,   0}, {255,  48,   0}, {255,  50,   0}, {255,  51,   0},
   {255,  53,   0}, {255,  54,   0}, {255,  56,   0}, {255,  57,   0}, {255,  59,   0},
   {255,  60,   0}, {255,  62,   0}, {255,  63,   0}, {255,  65,   0}, {255,  66,   0},
   {255,  68,   0}, {255,  69,   0}, {255,  71,   0}, {255,  72,   0}, {255,  74,   0},
   {255,  75,   0}, {255,  77,   0}, {255,  78,   0}, {255,  80,   0}, {255,  81,   0},
   {255,  83,   0}, {255,  84,   0}, {255,  86,   0}, {255,  87,   0}, {255,  89,   0},
   {255,  90,   0}, {255,  92,   0}, {255,  93,   0}, {255,  95,   0}, {255,  96,   0},
   {255,  98,   0}, {255, 100,   0}, {255, 101,   0}, {255, 103,   0}, {255, 104,   0},
   {255, 106,   0}, {255, 107,   0}, {255, 109,   0}, {255, 110,   0}, {255, 112,   0},
   {255, 113,   0}, {255, 115,   0}, {255, 116,   0}, {255, 118,   0}, {255, 119,   0},
   {255, 121,   0}, {255, 122,   0}, {255, 124,   0}, {255, 125,   0}, {255, 127,   0},
   {255, 128,   0}, {255, 130,   0}, {255, 131,   0}, {255, 133,   0}, {255, 134,   0},
   {255, 136,   0}, {255, 137,   0}, {255, 139,   0}, {255, 140,   0}, {255, 142,   0},
   {255, 143,   0}, {255, 145,   0}, {255, 146,   0}, {255, 148,   0}, {255, 149,   0},
   {255, 151,   0}, {255, 152,   0}, {255, 154,   0}, {255, 155,   0}, {255, 157,   0},
   {255, 158,   0}, {255, 160,   0}, {255, 161,   0}, {255, 163,   0}, {255, 164,   0},
   {255, 166,   0}, {255, 167,   0}, {255, 169,   0}, {255, 170,   0}, {255, 172,   0},
   {255, 173,   0}, {255, 175,   0}, {255, 176,   0}, {255, 178,   0}, {255, 179,   0},
   {255, 181,   0}, {255, 182,   0}, {255, 184,   0}, {255, 185,   0}, {255, 187,   0},
   {255, 188,   0}, {255, 190,   0}, {255, 191,   0}, {255, 193,   0}, {255, 194,   0},
   {255, 196,   0}, {255, 197,   0}, {255, 199,   0}, {255, 201,   0}, {255, 202,   0},
   {255, 204,   0}, {255, 205,   0}, {255, 207,   0}, {255, 208,   0}, {255, 210,   0},
   {255, 211,   0}, {255, 213,   0}, {255, 214,   0}, {255, 216,   0}, {255, 217,   0},
   {255, 219,   0}, {255, 220,   0}, {255, 222,   0}, {255, 223,   0}, {255, 225,   0},
   {255, 226,   0}, {255, 228,   0}, {255, 229,   0}, {255, 231,   0}, {255, 232,   0},
   {255, 234,   0}, {255, 235,   0}, {255, 237,   0}, {255, 238,   0}, {255, 239,   0},
   {254, 240,   0}, {254, 242,   0}, {253, 243,   0}, {253, 244,   0}, {252, 245,   0},
   {252, 246,   0}, {251, 247,   0}, {251, 248,   0}, {250, 249,   0}, {250, 250,   0},
   {249, 251,   0}, {249, 252,   0}, {248, 253,   0}, {248, 254,   0}, {247, 255,   0},
   {246, 255,   0}, {245, 255,   0}, {243, 255,   0}, {242, 255,   0}, {240, 255,   0},
   {239, 255,   0}, {237, 255,   0}, {236, 255,   0}, {234, 255,   0}, {233, 255,   0},
   {231, 255,   0}, {230, 255,   0}, {228, 255,   0}, {227, 255,   0}, {225, 255,   0},
   {224, 255,   0}, {222, 255,   0}, {221, 255,   0}, {219, 255,   0}, {218, 255,   0},
   {216, 255,   0}, {215, 255,   0}, {213, 255,   0}, {211, 255,   0}, {210, 255,   0},
   {208, 255,   0}, {207, 255,   0}, {205, 255,   0}, {204, 255,   0}, {202, 255,   0},
   {201, 255,   0}, {199, 255,   0}, {198, 255,   0}, {196, 255,   0}, {195, 255,   0},
   {193, 255,   0}, {192, 255,   0}, {190, 255,   0}, {189, 255,   0}, {187, 255,   0},
   {186, 255,   0}, {184, 255,   0}, {183, 255,   0}, {181, 255,   0}, {180, 255,   0},
   {178, 255,   0}, {177, 255,   0}, {175, 255,   0}, {174, 255,   0}, {172, 255,   0},
   {171, 255,   0}, {169, 255,   0}, {168, 255,   0}, {166, 255,   0}, {165, 255,   0},
   {163, 255,   0}, {162, 255,   0}, {160, 255,   0}, {159, 255,   0}, {157, 255,   0},
   {156, 255,   0}, {154, 255,   0}, {153, 255,   0}, {151, 255,   0}, {150, 255,   0},
   {148, 255,   0}, {147, 255,   0}, {145, 255,   0}, {144, 255,   0}, {142, 255,   0},
   {141, 255,   0}, {139, 255,   0}, {138, 255,   0}, {136, 255,   0}, {135, 255,   0},
   {133, 255,   0}, {132, 255,   0}, {130, 255,   0}, {129, 255,   0}, {127, 255,   0},
   {126, 255,   0}, {124, 255,   0}, {123, 255,   0}, {121, 255,   0}, {120, 255,   0},
   {118, 255,   0}, {117, 255,   0}, {115, 255,   0}, {114, 255,   0}, {112, 255,   0},
   {110, 255,   0}, {109, 255,   0}, {107, 255,   0}, {106, 255,   0}, {104, 255,   0},
   {103, 255,   0}, {101, 255,   0}, {100, 255,   0}, { 98, 255,   0}, { 97, 255,   0},
   { 95, 255,   0}, { 94, 255,   0}, { 92, 255,   0}, { 91, 255,   0}, { 89, 255,   0},
   { 88, 255,   0}, { 86, 255,   0}, { 85, 255,   0}, { 83, 255,   0}, { 82, 255,   0},
   { 80, 255,   0}, { 79, 255,   0}, { 77, 255,   0}, { 76, 255,   0}, { 74, 255,   0},
   { 73, 255,   0}, { 71, 255,   0}, { 70, 255,   0}, { 68, 255,   0}, { 67, 255,   0},
   { 65, 255,   0}, { 64, 255,   0}, { 62, 255,   0}, { 61, 255,   0}, { 59, 255,   0},
   { 58, 255,   0}, { 56, 255,   0}, { 55, 255,   0}, { 53, 255,   0}, { 52, 255,   0},
   { 50, 255,   0}, { 49, 255,   0}, { 47, 255,   0}, { 46, 255,   0}, { 44, 255,   0},
   { 43, 255,   0}, { 41, 255,   0}, { 40, 255,   0}, { 38, 255,   0}, { 37, 255,   0},
   { 35, 255,   0}, { 34, 255,   0}, { 32, 255,   0}, { 31, 255,   0}, { 29, 255,   0},
   { 28, 255,   0}, { 26, 255,   0}, { 25, 255,   0}, { 23, 255,   0}, { 22, 255,   0},
   { 20, 255,   0}, { 19, 255,   0}, { 17, 255,   0}, { 16, 255,   0}, { 14, 255,   0},
   { 12, 255,   0}, { 11, 255,   0}, {  9, 255,   0}, {  8, 255,   0}, {  7, 255,   1},
   {  7, 255,   2}, {  6, 255,   3}, {  6, 255,   4}, {  5, 255,   5}, {  5, 255,   6},
   {  4, 255,   7}, {  4, 255,   8}, {  3, 255,   9}, {  3, 255,  10}, {  2, 255,  11},
   {  2, 255,  12}, {  1, 255,  13}, {  1, 255,  14}, {  0, 255,  15}, {  0, 255,  16},
   {  0, 255,  18}, {  0, 255,  19}, {  0, 255,  21}, {  0, 255,  22}, {  0, 255,  24},
   {  0, 255,  25}, {  0, 255,  27}, {  0, 255,  28}, {  0, 255,  30}, {  0, 255,  31},
   {  0, 255,  33}, {  0, 255,  34}, {  0, 255,  36}, {  0, 255,  37}, {  0, 255,  39},
   {  0, 255,  40}, {  0, 255,  42}, {  0, 255,  43}, {  0, 255,  45}, {  0, 255,  46},
   {  0, 255,  48}, {  0, 255,  49}, {  0, 255,  51}, {  0, 255,  52}, {  0, 255,  54},
   {  0, 255,  55}, {  0, 255,  57}, {  0, 255,  58}, {  0, 255,  60}, {  0, 255,  61},
   {  0, 255,  63}, {  0, 255,  64}, {  0, 255,  66}, {  0, 255,  67}, {  0, 255,  69},
   {  0, 255,  70}, {  0, 255,  72}, {  0, 255,  73}, {  0, 255,  75}, {  0, 255,  76},
   {  0, 255,  78}, {  0, 255,  79}, {  0, 255,  81}, {  0, 255,  82}, {  0, 255,  84},
   {  0, 255,  86}, {  0, 255,  87}, {  0, 255,  89}, {  0, 255,  90}, {  0, 255,  92},
   {  0, 255,  93}, {  0, 255,  95}, {  0, 255,  96}, {  0, 255,  98}, {  0, 255,  99},
   {  0, 255, 101}, {  0, 255, 102}, {  0, 255, 104}, {  0, 255, 105}, {  0, 255, 107},
   {  0, 255, 108}, {  0, 255, 110}, {  0, 255, 111}, {  0, 255, 113}, {  0, 255, 114},
   {  0, 255, 116}, {  0, 255, 117}, {  0, 255, 119}, {  0, 255, 120}, {  0, 255, 122},
   {  0, 255, 123}, {  0, 255, 125}, {  0, 255, 126}, {  0, 255, 128}, {  0, 255, 129},
   {  0, 255, 131}, {  0, 255, 132}, {  0, 255, 134}, {  0, 255, 135}, {  0, 255, 137},
   {  0, 255, 138}, {  0, 255, 140}, {  0, 255, 141}, {  0, 255, 143}, {  0, 255, 144},
   {  0, 255, 146}, {  0, 255, 147}, {  0, 255, 149}, {  0, 255, 150}, {  0, 255, 152},
   {  0, 255, 153}, {  0, 255, 155}, {  0, 255, 156}, {  0, 255, 158}, {  0, 255, 159},
   {  0, 255, 161}, {  0, 255, 162}, {  0, 255, 164}, {  0, 255, 165}, {  0, 255, 167},
   {  0, 255, 168}, {  0, 255, 170}, {  0, 255, 171}, {  0, 255, 173}, {  0, 255, 174},
   {  0, 255, 176}, {  0, 255, 177}, {  0, 255, 179}, {  0, 255, 180}, {  0, 255, 182},
   {  0, 255, 183}, {  0, 255, 185}, {  0, 255, 187}, {  0, 255, 188}, {  0, 255, 190},
   {  0, 255, 191}, {  0, 255, 193}, {  0, 255, 194}, {  0, 255, 196}, {  0, 255, 197},
   {  0, 255, 199}, {  0, 255, 200}, {  0, 255, 202}, {  0, 255, 203}, {  0, 255, 205},
   {  0, 255, 206}, {  0, 255, 208}, {  0, 255, 209}, {  0, 255, 211}, {  0, 255, 212},
   {  0, 255, 214}, {  0, 255, 215}, {  0, 255, 217}, {  0, 255, 218}, {  0, 255, 220},
   {  0, 255, 221}, {  0, 255, 223}, {  0, 255, 224}, {  0, 255, 226}, {  0, 255, 227},
   {  0, 255, 229}, {  0, 255, 230}, {  0, 255, 232}, {  0, 255, 233}, {  0, 255, 235},
   {  0, 255, 236}, {  0, 255, 238}, {  0, 255, 239}, {  0, 255, 241}, {  0, 255, 242},
   {  0, 255, 244}, {  0, 255, 245}, {  0, 255, 247}, {  0, 255, 248}, {  0, 255, 250},
   {  0, 255, 251}, {  0, 255, 253}, {  0, 255, 254}, {  0, 254, 255}, {  0, 253, 255},
   {  0, 251, 255}, {  0, 250, 255}, {  0, 248, 255}, {  0, 247, 255}, {  0, 245, 255},
   {  0, 244, 255}, {  0, 242, 255}, {  0, 241, 255}, {  0, 239, 255}, {  0, 238, 255},
   {  0, 236, 255}, {  0, 235, 255}, {  0, 233, 255}, {  0, 232, 255}, {  0, 230, 255},
   {  0, 229, 255}, {  0, 227, 255}, {  0, 225, 255}, {  0, 224, 255}, {  0, 222, 255},
   {  0, 221, 255}, {  0, 219, 255}, {  0, 218, 255}, {  0, 216, 255}, {  0, 215, 255},
   {  0, 213, 255}, {  0, 212, 255}, {  0, 210, 255}, {  0, 209, 255}, {  0, 207, 255},
   {  0, 206, 255}, {  0, 204, 255}, {  0, 203, 255}, {  0, 201, 255}, {  0, 200, 255},
   {  0, 198, 255}, {  0, 197, 255}, {  0, 195, 255}, {  0, 194, 255}, {  0, 192, 255},
   {  0, 191, 255}, {  0, 189, 255}, {  0, 188, 255}, {  0, 186, 255}, {  0, 185, 255},
   {  0, 183, 255}, {  0, 182, 255}, {  0, 180, 255}, {  0, 179, 255}, {  0, 177, 255},
   {  0, 176, 255}, {  0, 174, 255}, {  0, 173, 255}, {  0, 171, 255}, {  0, 170, 255},
   {  0, 168, 255}, {  0, 167, 255}, {  0, 165, 255}, {  0, 164, 255}, {  0, 162, 255},
   {  0, 161, 255}, {  0, 159, 255}, {  0, 158, 255}, {  0, 156, 255}, {  0, 155, 255},
   {  0, 153, 255}, {  0, 152, 255}, {  0, 150, 255}, {  0, 149, 255}, {  0, 147, 255},
   {  0, 146, 255}, {  0, 144, 255}, {  0, 143, 255}, {  0, 141, 255}, {  0, 140, 255},
   {  0, 138, 255}, {  0, 137, 255}, {  0, 135, 255}, {  0, 134, 255}, {  0, 132, 255},
   {  0, 131, 255}, {  0, 129, 255}, {  0, 128, 255}, {  0, 126, 255}, {  0, 124, 255},
   {  0, 123, 255}, {  0, 121, 255}, {  0, 120, 255}, {  0, 118, 255}, {  0, 117, 255},
   {  0, 115, 255}, {  0, 114, 255}, {  0, 112, 255}, {  0, 111, 255}, {  0, 109, 255},
   {  0, 108, 255}, {  0, 106, 255}, {  0, 105, 255}, {  0, 103, 255}, {  0, 102, 255},
   {  0, 100, 255}, {  0,  99, 255}, {  0,  97, 255}, {  0,  96, 255}, {  0,  94, 255},
   {  0,  93, 255}, {  0,  91, 255}, {  0,  90, 255}, {  0,  88, 255}, {  0,  87, 255},
   {  0,  85, 255}, {  0,  84, 255}, {  0,  82, 255}, {  0,  81, 255}, {  0,  79, 255},
   {  0,  78, 255}, {  0,  76, 255}, {  0,  75, 255}, {  0,  73, 255}, {  0,  72, 255},
   {  0,  70, 255}, {  0,  69, 255}, {  0,  67, 255}, {  0,  66, 255}, {  0,  64, 255},
   {  0,  63, 255}, {  0,  61, 255}, {  0,  60, 255}, {  0,  58, 255}, {  0,  57, 255},
   {  0,  55, 255}, {  0,  54, 255}, {  0,  52, 255}, {  0,  51, 255}, {  0,  49, 255},
   {  0,  48, 255}, {  0,  46, 255}, {  0,  45, 255}, {  0,  43, 255}, {  0,  42, 255},
   {  0,  40, 255}, {  0,  39, 255}, {  0,  37, 255}, {  0,  36, 255}, {  0,  34, 255},
   {  0,  33, 255}, {  0,  31, 255}, {  0,  30, 255}, {  0,  28, 255}, {  0,  26, 255},
   {  0,  25, 255}, {  0,  23, 255}, {  0,  22, 255}, {  0,  20, 255}, {  0,  19, 255},
   {  0,  17, 255}, {  0,  16, 255}, {  1,  15, 255}, {  1,  14, 255}, {  2,  13, 255},
   {  2,  12, 255}, {  3,  11, 255}, {  3,  10, 255}, {  4,   9, 255}, {  4,   8, 255},
   {  5,   7, 255}, {  5,   6, 255}, {  6,   5, 255}, {  6,   4, 255}, {  7,   3, 255},
   {  7,   2, 255}, {  8,   1, 255}, {  8,   0, 255}, { 10,   0, 255}, { 11,   0, 255},
   { 13,   0, 255}, { 14,   0, 255}, { 16,   0, 255}, { 17,   0, 255}, { 19,   0, 255},
   { 20,   0, 255}, { 22,   0, 255}, { 23,   0, 255}, { 25,   0, 255}, { 26,   0, 255},
   { 28,   0, 255}, { 29,   0, 255}, { 31,   0, 255}, { 32,   0, 255}, { 34,   0, 255},
   { 35,   0, 255}, { 37,   0, 255}, { 38,   0, 255}, { 40,   0, 255}, { 41,   0, 255},
   { 43,   0, 255}, { 44,   0, 255}, { 46,   0, 255}, { 47,   0, 255}, { 49,   0, 255},
   { 50,   0, 255}, { 52,   0, 255}, { 53,   0, 255}, { 55,   0, 255}, { 56,   0, 255},
   { 58,   0, 255}, { 59,   0, 255}, { 61,   0, 255}, { 62,   0, 255}, { 64,   0, 255},
   { 65,   0, 255}, { 67,   0, 255}, { 68,   0, 255}, { 70,   0, 255}, { 72,   0, 255},
   { 73,   0, 255}, { 75,   0, 255}, { 76,   0, 255}, { 78,   0, 255}, { 79,   0, 255},
   { 81,   0, 255}, { 82,   0, 255}, { 84,   0, 255}, { 85,   0, 255}, { 87,   0, 255},
   { 88,   0, 255}, { 90,   0, 255}, { 91,   0, 255}, { 93,   0, 255}, { 94,   0, 255},
   { 96,   0, 255}, { 97,   0, 255}, { 99,   0, 255}, {100,   0, 255}, {102,   0, 255},
   {103,   0, 255}, {105,   0, 255}, {106,   0, 255}, {108,   0, 255}, {109,   0, 255},
   {111,   0, 255}, {112,   0, 255}, {114,   0, 255}, {115,   0, 255}, {117,   0, 255},
   {118,   0, 255}, {120,   0, 255}, {121,   0, 255}, {123,   0, 255}, {124,   0, 255},
   {126,   0, 255}, {127,   0, 255}, {129,   0, 255}, {130,   0, 255}, {132,   0, 255},
   {133,   0, 255}, {135,   0, 255}, {136,   0, 255}, {138,   0, 255}, {139,   0, 255},
   {141,   0, 255}, {142,   0, 255}, {144,   0, 255}, {145,   0, 255}, {147,   0, 255},
   {148,   0, 255}, {150,   0, 255}, {151,   0, 255}, {153,   0, 255}, {154,   0, 255},
   {156,   0, 255}, {157,   0, 255}, {159,   0, 255}, {160,   0, 255}, {162,   0, 255},
   {163,   0, 255}, {165,   0, 255}, {166,   0, 255}, {168,   0, 255}, {169,   0, 255},
   {171,   0, 255}, {173,   0, 255}, {174,   0, 255}, {176,   0, 255}, {177,   0, 255},
   {179,   0, 255}, {180,   0, 255}, {182,   0, 255}, {183,   0, 255}, {185,   0, 255},
   {186,   0, 255}, {188,   0, 255}, {189,   0, 255}, {191,   0, 255}, {192,   0, 255},
   {194,   0, 255}, {195,   0, 255}, {197,   0, 255}, {198,   0, 255}, {200,   0, 255},
   {201,   0, 255}, {203,   0, 255}, {204,   0, 255}, {206,   0, 255}, {207,   0, 255},
   {209,   0, 255}, {210,   0, 255}, {212,   0, 255}, {213,   0, 255}, {215,   0, 255},
   {216,   0, 255}, {218,   0, 255}, {219,   0, 255}, {221,   0, 255}, {222,   0, 255},
   {224,   0, 255}, {225,   0, 255}, {227,   0, 255}, {228,   0, 255}, {230,   0, 255},
   {231,   0, 255}, {233,   0, 255}, {234,   0, 255}, {236,   0, 255}, {237,   0, 255},
   {239,   0, 255}, {240,   0, 255}, {242,   0, 255}, {243,   0, 255}, {245,   0, 255},
   {246,   0, 255}, {247,   0, 254}, {248,   0, 253}, {248,   0, 252}, {249,   0, 251},
   {249,   0, 250}, {250,   0, 249}, {250,   0, 248}, {251,   0, 247}, {251,   0, 246},
   {252,   0, 245}, {252,   0, 244}, {253,   0, 243}, {253,   0, 242}, {254,   0, 241},
   {254,   0, 240}, {255,   0, 239}, {255,   0, 238}, {255,   0, 236}, {255,   0, 235},
   {255,   0, 233}, {255,   0, 232}, {255,   0, 230}, {255,   0, 229}, {255,   0, 227},
   {255,   0, 226}, {255,   0, 224}, {255,   0, 223}, {255,   0, 221}, {255,   0, 220},
   {255,   0, 218}, {255,   0, 217}, {255,   0, 215}, {255,   0, 214}, {255,   0, 212},
   {255,   0, 211}, {255,   0, 209}, {255,   0, 208}, {255,   0, 206}, {255,   0, 205},
   {255,   0, 203}, {255,   0, 202}, {255,   0, 200}, {255,   0, 199}, {255,   0, 197},
   {255,   0, 196}, {255,   0, 194}, {255,   0, 193}, {255,   0, 191}, {255,   0, 190},
   {255,   0, 188}, {255,   0, 187}, {255,   0, 185}, {255,   0, 184}, {255,   0, 182},
   {255,   0, 181}, {255,   0, 179}, {255,   0, 178}, {255,   0, 176}, {255,   0, 175},
   {255,   0, 173}, {255,   0, 172}, {255,   0, 170}, {255,   0, 169}, {255,   0, 167},
   {255,   0, 166}, {255,   0, 164}, {255,   0, 163}, {255,   0, 161}, {255,   0, 160},
   {255,   0, 158}, {255,   0, 157}, {255,   0, 155}, {255,   0, 154}, {255,   0, 152},
   {255,   0, 151}, {255,   0, 149}, {255,   0, 148}, {255,   0, 146}, {255,   0, 145},
   {255,   0, 143}, {255,   0, 141}, {255,   0, 140}, {255,   0, 138}, {255,   0, 137},
   {255,   0, 135}, {255,   0, 134}, {255,   0, 132}, {255,   0, 131}, {255,   0, 129},
   {255,   0, 128}, {255,   0, 126}, {255,   0, 125}, {255,   0, 123}, {255,   0, 122},
   {255,   0, 120}, {255,   0, 119}, {255,   0, 117}, {255,   0, 116}, {255,   0, 114},
   {255,   0, 113}, {255,   0, 111}, {255,   0, 110}, {255,   0, 108}, {255,   0, 107},
   {255,   0, 105}, {255,   0, 104}, {255,   0, 102}, {255,   0, 101}, {255,   0,  99},
   {255,   0,  98}, {255,   0,  96}, {255,   0,  95}, {255,   0,  93}, {255,   0,  92},
   {255,   0,  90}, {255,   0,  89}, {255,   0,  87}, {255,   0,  86}, {255,   0,  84},
   {255,   0,  83}, {255,   0,  81}, {255,   0,  80}, {255,   0,  78}, {255,   0,  77},
   {255,   0,  75}, {255,   0,  74}, {255,   0,  72}, {255,   0,  71}, {255,   0,  69},
   {255,   0,  68}, {255,   0,  66}, {255,   0,  65}, {255,   0,  63}, {255,   0,  62},
   {255,   0,  60}, {255,   0,  59}, {255,   0,  57}, {255,   0,  56}, {255,   0,  54},
   {255,   0,  53}, {255,   0,  51}, {255,   0,  50}, {255,   0,  48}, {255,   0,  47},
   {255,   0,  45}, {255,   0,  44}, {255,   0,  42}, {255,   0,  40}, {255,   0,  39},
   {255,   0,  37}, {255,   0,  36}, {255,   0,  34}, {255,   0,  33}, {255,   0,  31},
   {255,   0,  30}, {255,   0,  28}, {255,   0,  27}, {255,   0,  25}, {255,   0,  24}
};

const color jet_colormap[1000] = {
   { 29,   0, 102}, { 23,   0, 107}, { 17,   0, 112}, { 12,   0, 117}, {  6,   0, 122},
   {  0,   0, 127}, {  0,   0, 128}, {  0,   0, 129}, {  0,   0, 129}, {  0,   0, 130},
   {  0,   0, 131}, {  0,   0, 132}, {  0,   0, 133}, {  0,   0, 133}, {  0,   0, 134},
   {  0,   0, 135}, {  0,   0, 136}, {  0,   0, 137}, {  0,   0, 138}, {  0,   0, 140},
   {  0,   0, 141}, {  0,   0, 142}, {  0,   0, 143}, {  0,   0, 145}, {  0,   0, 146},
   {  0,   0, 147}, {  0,   0, 148}, {  0,   0, 150}, {  0,   0, 151}, {  0,   0, 152},
   {  0,   0, 153}, {  0,   0, 154}, {  0,   0, 156}, {  0,   0, 157}, {  0,   0, 158},
   {  0,   0, 159}, {  0,   0, 160}, {  0,   0, 161}, {  0,   0, 163}, {  0,   0, 164},
   {  0,   0, 165}, {  0,   0, 166}, {  0,   0, 168}, {  0,   0, 169}, {  0,   0, 170},
   {  0,   0, 171}, {  0,   0, 173}, {  0,   0, 174}, {  0,   0, 175}, {  0,   0, 176},
   {  0,   0, 178}, {  0,   0, 179}, {  0,   0, 180}, {  0,   0, 181}, {  0,   0, 183},
   {  0,   0, 184}, {  0,   0, 185}, {  0,   0, 186}, {  0,   0, 188}, {  0,   0, 189},
   {  0,   0, 190}, {  0,   0, 191}, {  0,   0, 193}, {  0,   0, 194}, {  0,   0, 195},
   {  0,   0, 196}, {  0,   0, 197}, {  0,   0, 198}, {  0,   0, 200}, {  0,   0, 201},
   {  0,   0, 202}, {  0,   0, 203}, {  0,   0, 204}, {  0,   0, 206}, {  0,   0, 207},
   {  0,   0, 208}, {  0,   0, 209}, {  0,   0, 211}, {  0,   0, 212}, {  0,   0, 213},
   {  0,   0, 214}, {  0,   0, 216}, {  0,   0, 217}, {  0,   0, 218}, {  0,   0, 219},
   {  0,   0, 221}, {  0,   0, 222}, {  0,   0, 223}, {  0,   0, 225}, {  0,   0, 226},
   {  0,   0, 227}, {  0,   0, 228}, {  0,   0, 230}, {  0,   0, 231}, {  0,   0, 232},
   {  0,   0, 233}, {  0,   0, 234}, {  0,   0, 234}, {  0,   0, 235}, {  0,   0, 236},
   {  0,   0, 237}, {  0,   0, 238}, {  0,   0, 239}, {  0,   0, 239}, {  0,   0, 240},
   {  0,   0, 241}, {  0,   0, 242}, {  0,   0, 243}, {  0,   0, 244}, {  0,   0, 246},
   {  0,   0, 247}, {  0,   0, 248}, {  0,   0, 249}, {  0,   0, 250}, {  0,   0, 251},
   {  0,   0, 253}, {  0,   0, 254}, {  0,   0, 254}, {  0,   0, 254}, {  0,   0, 254},
   {  0,   0, 254}, {  0,   0, 254}, {  0,   0, 255}, {  0,   0, 255}, {  0,   0, 255},
   {  0,   0, 255}, {  0,   0, 255}, {  0,   0, 255}, {  0,   1, 255}, {  0,   1, 255},
   {  0,   2, 255}, {  0,   3, 255}, {  0,   3, 255}, {  0,   4, 255}, {  0,   5, 255},
   {  0,   6, 255}, {  0,   6, 255}, {  0,   7, 255}, {  0,   8, 255}, {  0,   9, 255},
   {  0,  10, 255}, {  0,  11, 255}, {  0,  12, 255}, {  0,  13, 255}, {  0,  14, 255},
   {  0,  15, 255}, {  0,  16, 255}, {  0,  17, 255}, {  0,  18, 255}, {  0,  19, 255},
   {  0,  21, 255}, {  0,  22, 255}, {  0,  23, 255}, {  0,  24, 255}, {  0,  25, 255},
   {  0,  26, 255}, {  0,  27, 255}, {  0,  28, 255}, {  0,  29, 255}, {  0,  30, 255},
   {  0,  31, 255}, {  0,  32, 255}, {  0,  34, 255}, {  0,  35, 255}, {  0,  36, 255},
   {  0,  37, 255}, {  0,  38, 255}, {  0,  39, 255}, {  0,  40, 255}, {  0,  41, 255},
   {  0,  42, 255}, {  0,  43, 255}, {  0,  44, 255}, {  0,  45, 255}, {  0,  46, 255},
   {  0,  48, 255}, {  0,  49, 255}, {  0,  50, 255}, {  0,  51, 255}, {  0,  52, 255},
   {  0,  53, 255}, {  0,  54, 255}, {  0,  55, 255}, {  0,  56, 255}, {  0,  57, 255},
   {  0,  58, 255}, {  0,  58, 255}, {  0,  59, 255}, {  0,  60, 255}, {  0,  60, 255},
   {  0,  61, 255}, {  0,  62, 255}, {  0,  63, 255}, {  0,  63, 255}, {  0,  64, 255},
   {  0,  65, 255}, {  0,  66, 255}, {  0,  67, 255}, {  0,  68, 255}, {  0,  69, 255},
   {  0,  71, 255}, {  0,  72, 255}, {  0,  73, 255}, {  0,  74, 255}, {  0,  75, 255},
   {  0,  76, 255}, {  0,  77, 255}, {  0,  78, 255}, {  0,  79, 255}, {  0,  80, 255},
   {  0,  81, 255}, {  0,  82, 255}, {  0,  84, 255}, {  0,  85, 255}, {  0,  86, 255},
   {  0,  87, 255}, {  0,  88, 255}, {  0,  89, 255}, {  0,  90, 255}, {  0,  91, 255},
   {  0,  92, 255}, {  0,  93, 255}, {  0,  94, 255}, {  0,  95, 255}, {  0,  96, 255},
   {  0,  98, 255}, {  0,  99, 255}, {  0, 100, 255}, {  0, 101, 255}, {  0, 102, 255},
   {  0, 103, 255}, {  0, 104, 255}, {  0, 105, 255}, {  0, 106, 255}, {  0, 107, 255},
   {  0, 108, 255}, {  0, 109, 255}, {  0, 111, 255}, {  0, 112, 255}, {  0, 113, 255},
   {  0, 114, 255}, {  0, 115, 255}, {  0, 116, 255}, {  0, 117, 255}, {  0, 118, 255},
   {  0, 119, 255}, {  0, 120, 255}, {  0, 121, 255}, {  0, 122, 255}, {  0, 123, 255},
   {  0, 125, 255}, {  0, 126, 255}, {  0, 127, 255}, {  0, 128, 255}, {  0, 129, 255},
   {  0, 130, 255}, {  0, 131, 255}, {  0, 132, 255}, {  0, 133, 255}, {  0, 134, 255},
   {  0, 135, 255}, {  0, 136, 255}, {  0, 138, 255}, {  0, 139, 255}, {  0, 140, 255},
   {  0, 141, 255}, {  0, 142, 255}, {  0, 143, 255}, {  0, 144, 255}, {  0, 145, 255},
   {  0, 146, 255}, {  0, 147, 255}, {  0, 148, 255}, {  0, 149, 255}, {  0, 150, 255},
   {  0, 150, 255}, {  0, 151, 255}, {  0, 152, 255}, {  0, 153, 255}, {  0, 153, 255},
   {  0, 154, 255}, {  0, 155, 255}, {  0, 155, 255}, {  0, 156, 255}, {  0, 157, 255},
   {  0, 158, 255}, {  0, 159, 255}, {  0, 161, 255}, {  0, 162, 255}, {  0, 163, 255},
   {  0, 164, 255}, {  0, 165, 255}, {  0, 166, 255}, {  0, 167, 255}, {  0, 168, 255},
   {  0, 169, 255}, {  0, 170, 255}, {  0, 171, 255}, {  0, 172, 255}, {  0, 173, 255},
   {  0, 175, 255}, {  0, 176, 255}, {  0, 177, 255}, {  0, 178, 255}, {  0, 179, 255},
   {  0, 180, 255}, {  0, 181, 255}, {  0, 182, 255}, {  0, 183, 255}, {  0, 184, 255},
   {  0, 185, 255}, {  0, 186, 255}, {  0, 188, 255}, {  0, 189, 255}, {  0, 190, 255},
   {  0, 191, 255}, {  0, 192, 255}, {  0, 193, 255}, {  0, 194, 255}, {  0, 195, 255},
   {  0, 196, 255}, {  0, 197, 255}, {  0, 198, 255}, {  0, 199, 255}, {  0, 200, 255},
   {  0, 202, 255}, {  0, 203, 255}, {  0, 204, 255}, {  0, 205, 255}, {  0, 206, 255},
   {  0, 207, 255}, {  0, 208, 255}, {  0, 209, 255}, {  0, 210, 255}, {  0, 211, 255},
   {  0, 212, 255}, {  0, 213, 255}, {  0, 215, 255}, {  0, 216, 255}, {  0, 217, 255},
   {  0, 218, 254}, {  0, 219, 253}, {  0, 220, 252}, {  0, 221, 252}, {  0, 222, 251},
   {  0, 223, 250}, {  0, 224, 250}, {  0, 225, 249}, {  0, 226, 248}, {  0, 227, 247},
   {  0, 229, 247}, {  1, 230, 246}, {  2, 231, 245}, {  3, 232, 244}, {  3, 233, 243},
   {  4, 234, 242}, {  5, 235, 241}, {  5, 236, 240}, {  6, 237, 239}, {  7, 238, 238},
   {  8, 239, 238}, {  8, 240, 237}, {  9, 241, 236}, { 10, 242, 236}, { 10, 242, 235},
   { 11, 243, 235}, { 11, 244, 234}, { 12, 245, 234}, { 13, 245, 233}, { 13, 246, 232},
   { 14, 247, 232}, { 15, 247, 231}, { 15, 248, 231}, { 16, 249, 230}, { 17, 249, 229},
   { 18, 250, 228}, { 18, 251, 227}, { 19, 251, 226}, { 20, 252, 225}, { 21, 253, 224},
   { 22, 253, 224}, { 23, 254, 223}, { 23, 254, 222}, { 24, 255, 221}, { 25, 255, 220},
   { 26, 255, 219}, { 27, 255, 218}, { 28, 255, 218}, { 29, 255, 217}, { 30, 255, 216},
   { 30, 255, 215}, { 31, 255, 214}, { 32, 255, 214}, { 33, 255, 213}, { 34, 255, 212},
   { 35, 255, 211}, { 36, 255, 210}, { 37, 255, 209}, { 38, 255, 208}, { 39, 255, 207},
   { 39, 255, 207}, { 40, 255, 206}, { 41, 255, 205}, { 42, 255, 204}, { 43, 255, 203},
   { 44, 255, 202}, { 45, 255, 201}, { 46, 255, 200}, { 47, 255, 199}, { 48, 255, 198},
   { 48, 255, 198}, { 49, 255, 197}, { 50, 255, 196}, { 51, 255, 195}, { 52, 255, 194},
   { 53, 255, 193}, { 54, 255, 192}, { 55, 255, 191}, { 55, 255, 191}, { 56, 255, 190},
   { 57, 255, 189}, { 58, 255, 188}, { 59, 255, 187}, { 60, 255, 186}, { 60, 255, 186},
   { 61, 255, 185}, { 62, 255, 184}, { 63, 255, 183}, { 64, 255, 182}, { 65, 255, 181},
   { 65, 255, 181}, { 66, 255, 180}, { 67, 255, 179}, { 68, 255, 178}, { 69, 255, 177},
   { 70, 255, 176}, { 71, 255, 175}, { 72, 255, 174}, { 73, 255, 173}, { 74, 255, 172},
   { 74, 255, 172}, { 75, 255, 171}, { 76, 255, 170}, { 77, 255, 169}, { 78, 255, 168},
   { 79, 255, 167}, { 80, 255, 166}, { 81, 255, 165}, { 82, 255, 164}, { 83, 255, 163},
   { 83, 255, 163}, { 84, 255, 162}, { 84, 255, 162}, { 85, 255, 161}, { 85, 255, 161},
   { 86, 255, 160}, { 87, 255, 159}, { 87, 255, 159}, { 88, 255, 158}, { 88, 255, 158},
   { 89, 255, 157}, { 89, 255, 157}, { 90, 255, 156}, { 91, 255, 155}, { 92, 255, 154},
   { 93, 255, 153}, { 94, 255, 152}, { 95, 255, 151}, { 96, 255, 150}, { 97, 255, 149},
   { 97, 255, 149}, { 98, 255, 148}, { 99, 255, 147}, {100, 255, 146}, {101, 255, 145},
   {102, 255, 144}, {102, 255, 143}, {103, 255, 142}, {104, 255, 141}, {105, 255, 140},
   {106, 255, 140}, {107, 255, 139}, {107, 255, 138}, {108, 255, 137}, {109, 255, 136},
   {110, 255, 135}, {111, 255, 134}, {112, 255, 134}, {113, 255, 133}, {114, 255, 132},
   {114, 255, 131}, {115, 255, 130}, {116, 255, 130}, {117, 255, 129}, {118, 255, 128},
   {119, 255, 127}, {120, 255, 126}, {121, 255, 125}, {122, 255, 124}, {123, 255, 123},
   {123, 255, 123}, {124, 255, 122}, {125, 255, 121}, {126, 255, 120}, {127, 255, 119},
   {128, 255, 118}, {129, 255, 117}, {130, 255, 116}, {130, 255, 115}, {131, 255, 114},
   {132, 255, 114}, {133, 255, 113}, {134, 255, 112}, {134, 255, 111}, {135, 255, 110},
   {136, 255, 109}, {137, 255, 108}, {138, 255, 107}, {139, 255, 107}, {140, 255, 106},
   {140, 255, 105}, {141, 255, 104}, {142, 255, 103}, {143, 255, 102}, {144, 255, 102},
   {145, 255, 101}, {146, 255, 100}, {147, 255,  99}, {148, 255,  98}, {149, 255,  97},
   {149, 255,  97}, {150, 255,  96}, {151, 255,  95}, {152, 255,  94}, {153, 255,  93},
   {154, 255,  92}, {155, 255,  91}, {156, 255,  90}, {157, 255,  89}, {157, 255,  89},
   {158, 255,  88}, {158, 255,  88}, {159, 255,  87}, {159, 255,  87}, {160, 255,  86},
   {161, 255,  85}, {161, 255,  85}, {162, 255,  84}, {162, 255,  84}, {163, 255,  83},
   {163, 255,  83}, {164, 255,  82}, {165, 255,  81}, {166, 255,  80}, {167, 255,  79},
   {168, 255,  78}, {169, 255,  77}, {170, 255,  76}, {171, 255,  75}, {172, 255,  74},
   {172, 255,  74}, {173, 255,  73}, {174, 255,  72}, {175, 255,  71}, {176, 255,  70},
   {177, 255,  69}, {178, 255,  68}, {179, 255,  67}, {180, 255,  66}, {181, 255,  65},
   {181, 255,  65}, {182, 255,  64}, {183, 255,  63}, {184, 255,  62}, {185, 255,  61},
   {186, 255,  60}, {186, 255,  60}, {187, 255,  59}, {188, 255,  58}, {189, 255,  57},
   {190, 255,  56}, {191, 255,  55}, {191, 255,  55}, {192, 255,  54}, {193, 255,  53},
   {194, 255,  52}, {195, 255,  51}, {196, 255,  50}, {197, 255,  49}, {198, 255,  48},
   {198, 255,  48}, {199, 255,  47}, {200, 255,  46}, {201, 255,  45}, {202, 255,  44},
   {203, 255,  43}, {204, 255,  42}, {205, 255,  41}, {206, 255,  40}, {207, 255,  39},
   {207, 255,  39}, {208, 255,  38}, {209, 255,  37}, {210, 255,  36}, {211, 255,  35},
   {212, 255,  34}, {213, 255,  33}, {214, 255,  32}, {214, 255,  31}, {215, 255,  30},
   {216, 255,  30}, {217, 255,  29}, {218, 255,  28}, {218, 255,  27}, {219, 255,  26},
   {220, 255,  25}, {221, 255,  24}, {222, 255,  23}, {223, 255,  23}, {224, 255,  22},
   {224, 255,  21}, {225, 255,  20}, {226, 255,  19}, {227, 255,  18}, {228, 255,  18},
   {229, 255,  17}, {230, 255,  16}, {231, 255,  15}, {231, 255,  15}, {232, 255,  14},
   {232, 255,  13}, {233, 255,  13}, {234, 255,  12}, {234, 255,  11}, {235, 255,  11},
   {235, 255,  10}, {236, 255,  10}, {236, 255,   9}, {237, 255,   8}, {238, 254,   8},
   {238, 253,   7}, {239, 252,   6}, {240, 251,   5}, {241, 250,   5}, {242, 249,   4},
   {243, 248,   3}, {244, 247,   3}, {245, 246,   2}, {246, 246,   1}, {247, 245,   0},
   {247, 243,   0}, {248, 242,   0}, {249, 242,   0}, {250, 241,   0}, {250, 240,   0},
   {251, 239,   0}, {252, 238,   0}, {252, 237,   0}, {253, 236,   0}, {254, 235,   0},
   {255, 234,   0}, {255, 233,   0}, {255, 232,   0}, {255, 231,   0}, {255, 230,   0},
   {255, 229,   0}, {255, 228,   0}, {255, 227,   0}, {255, 226,   0}, {255, 225,   0},
   {255, 224,   0}, {255, 223,   0}, {255, 222,   0}, {255, 221,   0}, {255, 220,   0},
   {255, 219,   0}, {255, 218,   0}, {255, 217,   0}, {255, 216,   0}, {255, 215,   0},
   {255, 214,   0}, {255, 213,   0}, {255, 212,   0}, {255, 211,   0}, {255, 210,   0},
   {255, 209,   0}, {255, 208,   0}, {255, 207,   0}, {255, 206,   0}, {255, 205,   0},
   {255, 204,   0}, {255, 203,   0}, {255, 202,   0}, {255, 201,   0}, {255, 200,   0},
   {255, 199,   0}, {255, 198,   0}, {255, 197,   0}, {255, 196,   0}, {255, 195,   0},
   {255, 194,   0}, {255, 193,   0}, {255, 192,   0}, {255, 191,   0}, {255, 190,   0},
   {255, 189,   0}, {255, 188,   0}, {255, 187,   0}, {255, 186,   0}, {255, 185,   0},
   {255, 184,   0}, {255, 183,   0}, {255, 182,   0}, {255, 180,   0}, {255, 179,   0},
   {255, 178,   0}, {255, 177,   0}, {255, 176,   0}, {255, 176,   0}, {255, 175,   0},
   {255, 175,   0}, {255, 174,   0}, {255, 173,   0}, {255, 173,   0}, {255, 172,   0},
   {255, 171,   0}, {255, 171,   0}, {255, 170,   0}, {255, 169,   0}, {255, 168,   0},
   {255, 167,   0}, {255, 166,   0}, {255, 165,   0}, {255, 164,   0}, {255, 163,   0},
   {255, 162,   0}, {255, 161,   0}, {255, 160,   0}, {255, 159,   0}, {255, 158,   0},
   {255, 157,   0}, {255, 156,   0}, {255, 155,   0}, {255, 154,   0}, {255, 153,   0},
   {255, 152,   0}, {255, 151,   0}, {255, 150,   0}, {255, 150,   0}, {255, 149,   0},
   {255, 147,   0}, {255, 146,   0}, {255, 146,   0}, {255, 145,   0}, {255, 144,   0},
   {255, 143,   0}, {255, 142,   0}, {255, 141,   0}, {255, 140,   0}, {255, 139,   0},
   {255, 138,   0}, {255, 137,   0}, {255, 136,   0}, {255, 135,   0}, {255, 134,   0},
   {255, 133,   0}, {255, 132,   0}, {255, 131,   0}, {255, 130,   0}, {255, 129,   0},
   {255, 128,   0}, {255, 127,   0}, {255, 126,   0}, {255, 125,   0}, {255, 124,   0},
   {255, 123,   0}, {255, 122,   0}, {255, 121,   0}, {255, 120,   0}, {255, 119,   0},
   {255, 118,   0}, {255, 117,   0}, {255, 116,   0}, {255, 115,   0}, {255, 114,   0},
   {255, 113,   0}, {255, 112,   0}, {255, 111,   0}, {255, 109,   0}, {255, 108,   0},
   {255, 107,   0}, {255, 106,   0}, {255, 105,   0}, {255, 104,   0}, {255, 103,   0},
   {255, 102,   0}, {255, 101,   0}, {255, 100,   0}, {255,  99,   0}, {255,  98,   0},
   {255,  97,   0}, {255,  96,   0}, {255,  95,   0}, {255,  94,   0}, {255,  93,   0},
   {255,  92,   0}, {255,  91,   0}, {255,  91,   0}, {255,  90,   0}, {255,  90,   0},
   {255,  89,   0}, {255,  88,   0}, {255,  88,   0}, {255,  87,   0}, {255,  86,   0},
   {255,  86,   0}, {255,  85,   0}, {255,  84,   0}, {255,  83,   0}, {255,  82,   0},
   {255,  81,   0}, {255,  80,   0}, {255,  79,   0}, {255,  78,   0}, {255,  77,   0},
   {255,  76,   0}, {255,  75,   0}, {255,  74,   0}, {255,  73,   0}, {255,  72,   0},
   {255,  71,   0}, {255,  70,   0}, {255,  69,   0}, {255,  68,   0}, {255,  67,   0},
   {255,  66,   0}, {255,  65,   0}, {255,  64,   0}, {255,  63,   0}, {255,  62,   0},
   {255,  61,   0}, {255,  60,   0}, {255,  59,   0}, {255,  58,   0}, {255,  57,   0},
   {255,  56,   0}, {255,  55,   0}, {255,  54,   0}, {255,  54,   0}, {255,  53,   0},
   {255,  51,   0}, {255,  50,   0}, {255,  49,   0}, {255,  48,   0}, {255,  47,   0},
   {255,  46,   0}, {255,  45,   0}, {255,  44,   0}, {255,  43,   0}, {255,  42,   0},
   {255,  41,   0}, {255,  40,   0}, {255,  39,   0}, {255,  38,   0}, {255,  37,   0},
   {255,  36,   0}, {255,  35,   0}, {255,  34,   0}, {255,  33,   0}, {255,  32,   0},
   {255,  31,   0}, {255,  30,   0}, {255,  29,   0}, {255,  28,   0}, {255,  27,   0},
   {255,  26,   0}, {255,  25,   0}, {255,  24,   0}, {254,  23,   0}, {254,  22,   0},
   {254,  21,   0}, {254,  20,   0}, {254,  19,   0}, {254,  18,   0}, {253,  17,   0},
   {251,  16,   0}, {250,  15,   0}, {249,  14,   0}, {248,  13,   0}, {247,  12,   0},
   {246,  11,   0}, {244,  10,   0}, {243,   9,   0}, {242,   8,   0}, {241,   7,   0},
   {240,   6,   0}, {239,   6,   0}, {239,   5,   0}, {238,   4,   0}, {237,   4,   0},
   {236,   3,   0}, {235,   3,   0}, {234,   2,   0}, {234,   1,   0}, {233,   1,   0},
   {232,   0,   0}, {231,   0,   0}, {230,   0,   0}, {228,   0,   0}, {227,   0,   0},
   {226,   0,   0}, {225,   0,   0}, {223,   0,   0}, {222,   0,   0}, {221,   0,   0},
   {219,   0,   0}, {218,   0,   0}, {217,   0,   0}, {216,   0,   0}, {214,   0,   0},
   {213,   0,   0}, {212,   0,   0}, {211,   0,   0}, {209,   0,   0}, {208,   0,   0},
   {207,   0,   0}, {206,   0,   0}, {204,   0,   0}, {203,   0,   0}, {202,   0,   0},
   {201,   0,   0}, {200,   0,   0}, {198,   0,   0}, {197,   0,   0}, {196,   0,   0},
   {195,   0,   0}, {194,   0,   0}, {193,   0,   0}, {191,   0,   0}, {190,   0,   0},
   {189,   0,   0}, {188,   0,   0}, {186,   0,   0}, {185,   0,   0}, {184,   0,   0},
   {183,   0,   0}, {181,   0,   0}, {180,   0,   0}, {179,   0,   0}, {178,   0,   0},
   {176,   0,   0}, {175,   0,   0}, {174,   0,   0}, {173,   0,   0}, {171,   0,   0},
   {170,   0,   0}, {169,   0,   0}, {168,   0,   0}, {166,   0,   0}, {165,   0,   0},
   {164,   0,   0}, {163,   0,   0}, {161,   0,   0}, {160,   0,   0}, {159,   0,   0},
   {158,   0,   0}, {157,   0,   0}, {156,   0,   0}, {154,   0,   0}, {153,   0,   0},
   {152,   0,   0}, {151,   0,   0}, {150,   0,   0}, {148,   0,   0}, {147,   0,   0},
   {146,   0,   0}, {145,   0,   0}, {143,   0,   0}, {142,   0,   0}, {141,   0,   0},
   {140,   0,   0}, {138,   0,   0}, {137,   0,   0}, {136,   0,   0}, {135,   0,   0},
   {134,   0,   0}, {133,   0,   0}, {133,   0,   0}, {132,   0,   0}, {131,   0,   0},
   {130,   0,   0}, {129,   0,   0}, {129,   0,   0}, {128,   0,   0}, {127,   0,   0},
   {122,   0,   9}, {117,   0,  18}, {112,   0,  27}, {107,   0,  36}, {102,   0,  45}
};

const color prism_colormap[1000] = {
   {255,   0,   0}, {255,   2,   0}, {255,   4,   0}, {255,   6,   0}, {255,   8,   0},
   {255,  10,   0}, {255,  11,   0}, {255,  13,   0}, {255,  15,   0}, {255,  17,   0},
   {255,  19,   0}, {255,  21,   0}, {255,  23,   0}, {255,  25,   0}, {255,  27,   0},
   {255,  29,   0}, {255,  31,   0}, {255,  33,   0}, {255,  34,   0}, {255,  36,   0},
   {255,  38,   0}, {255,  40,   0}, {255,  42,   0}, {255,  44,   0}, {255,  46,   0},
   {255,  48,   0}, {255,  50,   0}, {255,  52,   0}, {255,  54,   0}, {255,  56,   0},
   {255,  57,   0}, {255,  59,   0}, {255,  61,   0}, {255,  63,   0}, {255,  65,   0},
   {255,  67,   0}, {255,  69,   0}, {255,  71,   0}, {255,  73,   0}, {255,  75,   0},
   {255,  77,   0}, {255,  78,   0}, {255,  80,   0}, {255,  82,   0}, {255,  84,   0},
   {255,  86,   0}, {255,  88,   0}, {255,  90,   0}, {255,  92,   0}, {255,  94,   0},
   {255,  96,   0}, {255,  98,   0}, {255, 100,   0}, {255, 101,   0}, {255, 103,   0},
   {255, 105,   0}, {255, 107,   0}, {255, 109,   0}, {255, 111,   0}, {255, 113,   0},
   {255, 115,   0}, {255, 117,   0}, {255, 119,   0}, {255, 121,   0}, {255, 123,   0},
   {255, 124,   0}, {255, 126,   0}, {255, 128,   0}, {255, 130,   0}, {255, 132,   0},
   {255, 134,   0}, {255, 136,   0}, {255, 138,   0}, {255, 140,   0}, {255, 142,   0},
   {255, 144,   0}, {255, 145,   0}, {255, 147,   0}, {255, 149,   0}, {255, 151,   0},
   {255, 153,   0}, {255, 155,   0}, {255, 157,   0}, {255, 159,   0}, {255, 161,   0},
   {255, 163,   0}, {255, 165,   0}, {255, 167,   0}, {255, 168,   0}, {255, 170,   0},
   {255, 172,   0}, {255, 174,   0}, {255, 176,   0}, {255, 178,   0}, {255, 180,   0},
   {255, 182,   0}, {255, 184,   0}, {255, 186,   0}, {255, 188,   0}, {255, 190,   0},
   {255, 191,   0}, {255, 193,   0}, {255, 195,   0}, {255, 197,   0}, {255, 199,   0},
   {255, 201,   0}, {255, 203,   0}, {255, 205,   0}, {255, 207,   0}, {255, 209,   0},
   {255, 211,   0}, {255, 212,   0}, {255, 214,   0}, {255, 216,   0}, {255, 218,   0},
   {255, 220,   0}, {255, 222,   0}, {255, 224,   0}, {255, 226,   0}, {255, 228,   0},
   {255, 230,   0}, {255, 232,   0}, {255, 234,   0}, {255, 235,   0}, {255, 237,   0},
   {255, 239,   0}, {255, 241,   0}, {255, 243,   0}, {255, 245,   0}, {255, 247,   0},
   {255, 249,   0}, {255, 251,   0}, {255, 253,   0}, {255, 255,   0}, {252, 255,   0},
   {248, 255,   0}, {244, 255,   0}, {240, 255,   0}, {237, 255,   0}, {233, 255,   0},
   {229, 255,   0}, {225, 255,   0}, {221, 255,   0}, {217, 255,   0}, {214, 255,   0},
   {210, 255,   0}, {206, 255,   0}, {202, 255,   0}, {198, 255,   0}, {195, 255,   0},
   {191, 255,   0}, {187, 255,   0}, {183, 255,   0}, {179, 255,   0}, {175, 255,   0},
   {172, 255,   0}, {168, 255,   0}, {164, 255,   0}, {160, 255,   0}, {156, 255,   0},
   {152, 255,   0}, {149, 255,   0}, {145, 255,   0}, {141, 255,   0}, {137, 255,   0},
   {133, 255,   0}, {129, 255,   0}, {126, 255,   0}, {122, 255,   0}, {118, 255,   0},
   {114, 255,   0}, {110, 255,   0}, {106, 255,   0}, {103, 255,   0}, { 99, 255,   0},
   { 95, 255,   0}, { 91, 255,   0}, { 87, 255,   0}, { 83, 255,   0}, { 80, 255,   0},
   { 76, 255,   0}, { 72, 255,   0}, { 68, 255,   0}, { 64, 255,   0}, { 60, 255,   0},
   { 57, 255,   0}, { 53, 255,   0}, { 49, 255,   0}, { 45, 255,   0}, { 41, 255,   0},
   { 38, 255,   0}, { 34, 255,   0}, { 30, 255,   0}, { 26, 255,   0}, { 22, 255,   0},
   { 18, 255,   0}, { 15, 255,   0}, { 11, 255,   0}, {  7, 255,   0}, {  3, 255,   0},
   {  0, 254,   1}, {  0, 250,   5}, {  0, 247,   8}, {  0, 243,  12}, {  0, 239,  16},
   {  0, 235,  20}, {  0, 231,  24}, {  0, 227,  28}, {  0, 224,  31}, {  0, 220,  35},
   {  0, 216,  39}, {  0, 212,  43}, {  0, 208,  47}, {  0, 204,  51}, {  0, 201,  54},
   {  0, 197,  58}, {  0, 193,  62}, {  0, 189,  66}, {  0, 185,  70}, {  0, 181,  74},
   {  0, 178,  77}, {  0, 174,  81}, {  0, 170,  85}, {  0, 166,  89}, {  0, 162,  93},
   {  0, 159,  96}, {  0, 155, 100}, {  0, 151, 104}, {  0, 147, 108}, {  0, 143, 112},
   {  0, 139, 116}, {  0, 136, 119}, {  0, 132, 123}, {  0, 128, 127}, {  0, 124, 131},
   {  0, 120, 135}, {  0, 116, 139}, {  0, 113, 142}, {  0, 109, 146}, {  0, 105, 150},
   {  0, 101, 154}, {  0,  97, 158}, {  0,  93, 162}, {  0,  90, 165}, {  0,  86, 169},
   {  0,  82, 173}, {  0,  78, 177}, {  0,  74, 181}, {  0,  70, 185}, {  0,  67, 188},
   {  0,  63, 192}, {  0,  59, 196}, {  0,  55, 200}, {  0,  51, 204}, {  0,  47, 208},
   {  0,  44, 211}, {  0,  40, 215}, {  0,  36, 219}, {  0,  32, 223}, {  0,  28, 227},
   {  0,  25, 230}, {  0,  21, 234}, {  0,  17, 238}, {  0,  13, 242}, {  0,   9, 246},
   {  0,   5, 250}, {  0,   2, 253}, {  2,   0, 255}, {  4,   0, 255}, {  7,   0, 255},
   {  9,   0, 255}, { 12,   0, 255}, { 14,   0, 255}, { 17,   0, 255}, { 19,   0, 255},
   { 22,   0, 255}, { 25,   0, 255}, { 27,   0, 255}, { 30,   0, 255}, { 32,   0, 255},
   { 35,   0, 255}, { 37,   0, 255}, { 40,   0, 255}, { 42,   0, 255}, { 45,   0, 255},
   { 47,   0, 255}, { 50,   0, 255}, { 53,   0, 255}, { 55,   0, 255}, { 58,   0, 255},
   { 60,   0, 255}, { 63,   0, 255}, { 65,   0, 255}, { 68,   0, 255}, { 70,   0, 255},
   { 73,   0, 255}, { 76,   0, 255}, { 78,   0, 255}, { 81,   0, 255}, { 83,   0, 255},
   { 86,   0, 255}, { 88,   0, 255}, { 91,   0, 255}, { 93,   0, 255}, { 96,   0, 255},
   { 99,   0, 255}, {101,   0, 255}, {104,   0, 255}, {106,   0, 255}, {109,   0, 255},
   {111,   0, 255}, {114,   0, 255}, {116,   0, 255}, {119,   0, 255}, {122,   0, 255},
   {124,   0, 255}, {127,   0, 255}, {129,   0, 255}, {132,   0, 255}, {134,   0, 255},
   {137,   0, 255}, {139,   0, 255}, {142,   0, 255}, {144,   0, 255}, {147,   0, 255},
   {150,   0, 255}, {152,   0, 255}, {155,   0, 255}, {157,   0, 255}, {160,   0, 255},
   {162,   0, 255}, {165,   0, 255}, {167,   0, 255}, {170,   0, 255}, {171,   0, 251},
   {173,   0, 247}, {174,   0, 244}, {175,   0, 240}, {176,   0, 236}, {178,   0, 232},
   {179,   0, 228}, {180,   0, 224}, {181,   0, 221}, {183,   0, 217}, {184,   0, 213},
   {185,   0, 209}, {187,   0, 205}, {188,   0, 201}, {189,   0, 198}, {190,   0, 194},
   {192,   0, 190}, {193,   0, 186}, {194,   0, 182}, {196,   0, 178}, {197,   0, 175},
   {198,   0, 171}, {199,   0, 167}, {201,   0, 163}, {202,   0, 159}, {203,   0, 155},
   {204,   0, 152}, {206,   0, 148}, {207,   0, 144}, {208,   0, 140}, {210,   0, 136},
   {211,   0, 132}, {212,   0, 129}, {213,   0, 125}, {215,   0, 121}, {216,   0, 117},
   {217,   0, 113}, {218,   0, 110}, {220,   0, 106}, {221,   0, 102}, {222,   0,  98},
   {224,   0,  94}, {225,   0,  90}, {226,   0,  87}, {227,   0,  83}, {229,   0,  79},
   {230,   0,  75}, {231,   0,  71}, {233,   0,  67}, {234,   0,  64}, {235,   0,  60},
   {236,   0,  56}, {238,   0,  52}, {239,   0,  48}, {240,   0,  44}, {241,   0,  41},
   {243,   0,  37}, {244,   0,  33}, {245,   0,  29}, {247,   0,  25}, {248,   0,  21},
   {249,   0,  18}, {250,   0,  14}, {252,   0,  10}, {253,   0,   6}, {254,   0,   2},
   {255,   1,   0}, {255,   3,   0}, {255,   5,   0}, {255,   7,   0}, {255,   8,   0},
   {255,  10,   0}, {255,  12,   0}, {255,  14,   0}, {255,  16,   0}, {255,  18,   0},
   {255,  20,   0}, {255,  22,   0}, {255,  24,   0}, {255,  26,   0}, {255,  28,   0},
   {255,  29,   0}, {255,  31,   0}, {255,  33,   0}, {255,  35,   0}, {255,  37,   0},
   {255,  39,   0}, {255,  41,   0}, {255,  43,   0}, {255,  45,   0}, {255,  47,   0},
   {255,  49,   0}, {255,  51,   0}, {255,  52,   0}, {255,  54,   0}, {255,  56,   0},
   {255,  58,   0}, {255,  60,   0}, {255,  62,   0}, {255,  64,   0}, {255,  66,   0},
   {255,  68,   0}, {255,  70,   0}, {255,  72,   0}, {255,  74,   0}, {255,  75,   0},
   {255,  77,   0}, {255,  79,   0}, {255,  81,   0}, {255,  83,   0}, {255,  85,   0},
   {255,  87,   0}, {255,  89,   0}, {255,  91,   0}, {255,  93,   0}, {255,  95,   0},
   {255,  96,   0}, {255,  98,   0}, {255, 100,   0}, {255, 102,   0}, {255, 104,   0},
   {255, 106,   0}, {255, 108,   0}, {255, 110,   0}, {255, 112,   0}, {255, 114,   0},
   {255, 116,   0}, {255, 118,   0}, {255, 119,   0}, {255, 121,   0}, {255, 123,   0},
   {255, 125,   0}, {255, 127,   0}, {255, 129,   0}, {255, 131,   0}, {255, 133,   0},
   {255, 135,   0}, {255, 137,   0}, {255, 139,   0}, {255, 141,   0}, {255, 142,   0},
   {255, 144,   0}, {255, 146,   0}, {255, 148,   0}, {255, 150,   0}, {255, 152,   0},
   {255, 154,   0}, {255, 156,   0}, {255, 158,   0}, {255, 160,   0}, {255, 162,   0},
   {255, 163,   0}, {255, 165,   0}, {255, 167,   0}, {255, 169,   0}, {255, 171,   0},
   {255, 173,   0}, {255, 175,   0}, {255, 177,   0}, {255, 179,   0}, {255, 181,   0},
   {255, 183,   0}, {255, 185,   0}, {255, 186,   0}, {255, 188,   0}, {255, 190,   0},
   {255, 192,   0}, {255, 194,   0}, {255, 196,   0}, {255, 198,   0}, {255, 200,   0},
   {255, 202,   0}, {255, 204,   0}, {255, 206,   0}, {255, 208,   0}, {255, 209,   0},
   {255, 211,   0}, {255, 213,   0}, {255, 215,   0}, {255, 217,   0}, {255, 219,   0},
   {255, 221,   0}, {255, 223,   0}, {255, 225,   0}, {255, 227,   0}, {255, 229,   0},
   {255, 230,   0}, {255, 232,   0}, {255, 234,   0}, {255, 236,   0}, {255, 238,   0},
   {255, 240,   0}, {255, 242,   0}, {255, 244,   0}, {255, 246,   0}, {255, 248,   0},
   {255, 250,   0}, {255, 252,   0}, {255, 253,   0}, {254, 255,   0}, {250, 255,   0},
   {247, 255,   0}, {243, 255,   0}, {239, 255,   0}, {235, 255,   0}, {231, 255,   0},
   {227, 255,   0}, {224, 255,   0}, {220, 255,   0}, {216, 255,   0}, {212, 255,   0},
   {208, 255,   0}, {204, 255,   0}, {201, 255,   0}, {197, 255,   0}, {193, 255,   0},
   {189, 255,   0}, {185, 255,   0}, {181, 255,   0}, {178, 255,   0}, {174, 255,   0},
   {170, 255,   0}, {166, 255,   0}, {162, 255,   0}, {159, 255,   0}, {155, 255,   0},
   {151, 255,   0}, {147, 255,   0}, {143, 255,   0}, {139, 255,   0}, {136, 255,   0},
   {132, 255,   0}, {128, 255,   0}, {124, 255,   0}, {120, 255,   0}, {116, 255,   0},
   {113, 255,   0}, {109, 255,   0}, {105, 255,   0}, {101, 255,   0}, { 97, 255,   0},
   { 93, 255,   0}, { 90, 255,   0}, { 86, 255,   0}, { 82, 255,   0}, { 78, 255,   0},
   { 74, 255,   0}, { 70, 255,   0}, { 67, 255,   0}, { 63, 255,   0}, { 59, 255,   0},
   { 55, 255,   0}, { 51, 255,   0}, { 47, 255,   0}, { 44, 255,   0}, { 40, 255,   0},
   { 36, 255,   0}, { 32, 255,   0}, { 28, 255,   0}, { 25, 255,   0}, { 21, 255,   0},
   { 17, 255,   0}, { 13, 255,   0}, {  9, 255,   0}, {  5, 255,   0}, {  2, 255,   0},
   {  0, 253,   2}, {  0, 249,   6}, {  0, 245,  10}, {  0, 241,  14}, {  0, 237,  18},
   {  0, 234,  21}, {  0, 230,  25}, {  0, 226,  29}, {  0, 222,  33}, {  0, 218,  37},
   {  0, 214,  41}, {  0, 211,  44}, {  0, 207,  48}, {  0, 203,  52}, {  0, 199,  56},
   {  0, 195,  60}, {  0, 191,  64}, {  0, 188,  67}, {  0, 184,  71}, {  0, 180,  75},
   {  0, 176,  79}, {  0, 172,  83}, {  0, 168,  87}, {  0, 165,  90}, {  0, 161,  94},
   {  0, 157,  98}, {  0, 153, 102}, {  0, 149, 106}, {  0, 145, 110}, {  0, 142, 113},
   {  0, 138, 117}, {  0, 134, 121}, {  0, 130, 125}, {  0, 126, 129}, {  0, 123, 132},
   {  0, 119, 136}, {  0, 115, 140}, {  0, 111, 144}, {  0, 107, 148}, {  0, 103, 152},
   {  0, 100, 155}, {  0,  96, 159}, {  0,  92, 163}, {  0,  88, 167}, {  0,  84, 171},
   {  0,  80, 175}, {  0,  77, 178}, {  0,  73, 182}, {  0,  69, 186}, {  0,  65, 190},
   {  0,  61, 194}, {  0,  57, 198}, {  0,  54, 201}, {  0,  50, 205}, {  0,  46, 209},
   {  0,  42, 213}, {  0,  38, 217}, {  0,  34, 221}, {  0,  31, 224}, {  0,  27, 228},
   {  0,  23, 232}, {  0,  19, 236}, {  0,  15, 240}, {  0,  11, 244}, {  0,   8, 247},
   {  0,   4, 251}, {  0,   0, 255}, {  3,   0, 255}, {  5,   0, 255}, {  8,   0, 255},
   { 10,   0, 255}, { 13,   0, 255}, { 15,   0, 255}, { 18,   0, 255}, { 20,   0, 255},
   { 23,   0, 255}, { 26,   0, 255}, { 28,   0, 255}, { 31,   0, 255}, { 33,   0, 255},
   { 36,   0, 255}, { 38,   0, 255}, { 41,   0, 255}, { 43,   0, 255}, { 46,   0, 255},
   { 48,   0, 255}, { 51,   0, 255}, { 54,   0, 255}, { 56,   0, 255}, { 59,   0, 255},
   { 61,   0, 255}, { 64,   0, 255}, { 66,   0, 255}, { 69,   0, 255}, { 71,   0, 255},
   { 74,   0, 255}, { 77,   0, 255}, { 79,   0, 255}, { 82,   0, 255}, { 84,   0, 255},
   { 87,   0, 255}, { 89,   0, 255}, { 92,   0, 255}, { 94,   0, 255}, { 97,   0, 255},
   {100,   0, 255}, {102,   0, 255}, {105,   0, 255}, {107,   0, 255}, {110,   0, 255},
   {112,   0, 255}, {115,   0, 255}, {117,   0, 255}, {120,   0, 255}, {123,   0, 255},
   {125,   0, 255}, {128,   0, 255}, {130,   0, 255}, {133,   0, 255}, {135,   0, 255},
   {138,   0, 255}, {140,   0, 255}, {143,   0, 255}, {145,   0, 255}, {148,   0, 255},
   {151,   0, 255}, {153,   0, 255}, {156,   0, 255}, {158,   0, 255}, {161,   0, 255},
   {163,   0, 255}, {166,   0, 255}, {168,   0, 255}, {171,   0, 253}, {172,   0, 250},
   {173,   0, 246}, {174,   0, 242}, {176,   0, 238}, {177,   0, 234}, {178,   0, 230},
   {179,   0, 227}, {181,   0, 223}, {182,   0, 219}, {183,   0, 215}, {185,   0, 211},
   {186,   0, 208}, {187,   0, 204}, {188,   0, 200}, {190,   0, 196}, {191,   0, 192},
   {192,   0, 188}, {193,   0, 185}, {195,   0, 181}, {196,   0, 177}, {197,   0, 173},
   {199,   0, 169}, {200,   0, 165}, {201,   0, 162}, {202,   0, 158}, {204,   0, 154},
   {205,   0, 150}, {206,   0, 146}, {208,   0, 142}, {209,   0, 139}, {210,   0, 135},
   {211,   0, 131}, {213,   0, 127}, {214,   0, 123}, {215,   0, 119}, {216,   0, 116},
   {218,   0, 112}, {219,   0, 108}, {220,   0, 104}, {222,   0, 100}, {223,   0,  96},
   {224,   0,  93}, {225,   0,  89}, {227,   0,  85}, {228,   0,  81}, {229,   0,  77},
   {230,   0,  74}, {232,   0,  70}, {233,   0,  66}, {234,   0,  62}, {236,   0,  58},
   {237,   0,  54}, {238,   0,  51}, {239,   0,  47}, {241,   0,  43}, {242,   0,  39},
   {243,   0,  35}, {245,   0,  31}, {246,   0,  28}, {247,   0,  24}, {248,   0,  20},
   {250,   0,  16}, {251,   0,  12}, {252,   0,   8}, {253,   0,   5}, {255,   0,   1},
   {255,   2,   0}, {255,   3,   0}, {255,   5,   0}, {255,   7,   0}, {255,   9,   0},
   {255,  11,   0}, {255,  13,   0}, {255,  15,   0}, {255,  17,   0}, {255,  19,   0},
   {255,  21,   0}, {255,  23,   0}, {255,  25,   0}, {255,  26,   0}, {255,  28,   0},
   {255,  30,   0}, {255,  32,   0}, {255,  34,   0}, {255,  36,   0}, {255,  38,   0},
   {255,  40,   0}, {255,  42,   0}, {255,  44,   0}, {255,  46,   0}, {255,  47,   0},
   {255,  49,   0}, {255,  51,   0}, {255,  53,   0}, {255,  55,   0}, {255,  57,   0},
   {255,  59,   0}, {255,  61,   0}, {255,  63,   0}, {255,  65,   0}, {255,  67,   0},
   {255,  69,   0}, {255,  70,   0}, {255,  72,   0}, {255,  74,   0}, {255,  76,   0},
   {255,  78,   0}, {255,  80,   0}, {255,  82,   0}, {255,  84,   0}, {255,  86,   0},
   {255,  88,   0}, {255,  90,   0}, {255,  92,   0}, {255,  93,   0}, {255,  95,   0},
   {255,  97,   0}, {255,  99,   0}, {255, 101,   0}, {255, 103,   0}, {255, 105,   0},
   {255, 107,   0}, {255, 109,   0}, {255, 111,   0}, {255, 113,   0}, {255, 114,   0},
   {255, 116,   0}, {255, 118,   0}, {255, 120,   0}, {255, 122,   0}, {255, 124,   0},
   {255, 126,   0}, {255, 128,   0}, {255, 130,   0}, {255, 132,   0}, {255, 134,   0},
   {255, 136,   0}, {255, 137,   0}, {255, 139,   0}, {255, 141,   0}, {255, 143,   0},
   {255, 145,   0}, {255, 147,   0}, {255, 149,   0}, {255, 151,   0}, {255, 153,   0},
   {255, 155,   0}, {255, 157,   0}, {255, 159,   0}, {255, 160,   0}, {255, 162,   0},
   {255, 164,   0}, {255, 166,   0}, {255, 168,   0}, {255, 170,   0}, {255, 172,   0},
   {255, 174,   0}, {255, 176,   0}, {255, 178,   0}, {255, 180,   0}, {255, 181,   0},
   {255, 183,   0}, {255, 185,   0}, {255, 187,   0}, {255, 189,   0}, {255, 191,   0},
   {255, 193,   0}, {255, 195,   0}, {255, 197,   0}, {255, 199,   0}, {255, 201,   0},
   {255, 203,   0}, {255, 204,   0}, {255, 206,   0}, {255, 208,   0}, {255, 210,   0},
   {255, 212,   0}, {255, 214,   0}, {255, 216,   0}, {255, 218,   0}, {255, 220,   0},
   {255, 222,   0}, {255, 224,   0}, {255, 226,   0}, {255, 227,   0}, {255, 229,   0},
   {255, 231,   0}, {255, 233,   0}, {255, 235,   0}, {255, 237,   0}, {255, 239,   0},
   {255, 241,   0}, {255, 243,   0}, {255, 245,   0}, {255, 247,   0}, {255, 248,   0},
   {255, 250,   0}, {255, 252,   0}, {255, 254,   0}, {253, 255,   0}, {249, 255,   0},
   {245, 255,   0}, {241, 255,   0}, {237, 255,   0}, {234, 255,   0}, {230, 255,   0},
   {226, 255,   0}, {222, 255,   0}, {218, 255,   0}, {214, 255,   0}, {211, 255,   0},
   {207, 255,   0}, {203, 255,   0}, {199, 255,   0}, {195, 255,   0}, {191, 255,   0},
   {188, 255,   0}, {184, 255,   0}, {180, 255,   0}, {176, 255,   0}, {172, 255,   0},
   {168, 255,   0}, {165, 255,   0}, {161, 255,   0}, {157, 255,   0}, {153, 255,   0},
   {149, 255,   0}, {145, 255,   0}, {142, 255,   0}, {138, 255,   0}, {134, 255,   0},
   {130, 255,   0}, {126, 255,   0}, {123, 255,   0}, {119, 255,   0}, {115, 255,   0},
   {111, 255,   0}, {107, 255,   0}, {103, 255,   0}, {100, 255,   0}, { 96, 255,   0},
   { 92, 255,   0}, { 88, 255,   0}, { 84, 255,   0}, { 80, 255,   0}, { 77, 255,   0},
   { 73, 255,   0}, { 69, 255,   0}, { 65, 255,   0}, { 61, 255,   0}, { 57, 255,   0},
   { 54, 255,   0}, { 50, 255,   0}, { 46, 255,   0}, { 42, 255,   0}, { 38, 255,   0},
   { 34, 255,   0}, { 31, 255,   0}, { 27, 255,   0}, { 23, 255,   0}, { 19, 255,   0},
   { 15, 255,   0}, { 11, 255,   0}, {  8, 255,   0}, {  4, 255,   0}, {  0, 255,   0}
};

const color vga_colormap[1000] = {
   {255, 255, 255}, {254, 254, 254}, {253, 253, 253}, {252, 252, 252}, {251, 251, 251},
   {250, 250, 250}, {249, 249, 249}, {248, 248, 248}, {247, 247, 247}, {246, 246, 246},
   {245, 245, 245}, {244, 244, 244}, {244, 244, 244}, {243, 243, 243}, {242, 242, 242},
   {241, 241, 241}, {240, 240, 240}, {239, 239, 239}, {238, 238, 238}, {237, 237, 237},
   {236, 236, 236}, {235, 235, 235}, {234, 234, 234}, {233, 233, 233}, {232, 232, 232},
   {231, 231, 231}, {230, 230, 230}, {229, 229, 229}, {228, 228, 228}, {227, 227, 227},
   {226, 226, 226}, {225, 225, 225}, {224, 224, 224}, {223, 223, 223}, {222, 222, 222},
   {221, 221, 221}, {221, 221, 221}, {220, 220, 220}, {219, 219, 219}, {218, 218, 218},
   {217, 217, 217}, {216, 216, 216}, {215, 215, 215}, {214, 214, 214}, {213, 213, 213},
   {212, 212, 212}, {211, 211, 211}, {210, 210, 210}, {209, 209, 209}, {208, 208, 208},
   {207, 207, 207}, {206, 206, 206}, {205, 205, 205}, {204, 204, 204}, {203, 203, 203},
   {202, 202, 202}, {201, 201, 201}, {200, 200, 200}, {199, 199, 199}, {199, 199, 199},
   {198, 198, 198}, {197, 197, 197}, {196, 196, 196}, {195, 195, 195}, {194, 194, 194},
   {193, 193, 193}, {192, 192, 192}, {192, 190, 190}, {193, 187, 187}, {194, 184, 184},
   {195, 181, 181}, {195, 179, 179}, {196, 176, 176}, {197, 173, 173}, {198, 170, 170},
   {199, 167, 167}, {200, 164, 164}, {201, 161, 161}, {202, 159, 159}, {203, 156, 156},
   {204, 153, 153}, {205, 150, 150}, {206, 147, 147}, {207, 144, 144}, {208, 141, 141},
   {209, 138, 138}, {210, 136, 136}, {211, 133, 133}, {212, 130, 130}, {213, 127, 127},
   {214, 124, 124}, {215, 121, 121}, {216, 118, 118}, {217, 115, 115}, {217, 113, 113},
   {218, 110, 110}, {219, 107, 107}, {220, 104, 104}, {221, 101, 101}, {222,  98,  98},
   {223,  95,  95}, {224,  92,  92}, {225,  90,  90}, {226,  87,  87}, {227,  84,  84},
   {228,  81,  81}, {229,  78,  78}, {230,  75,  75}, {231,  72,  72}, {232,  69,  69},
   {233,  67,  67}, {234,  64,  64}, {235,  61,  61}, {236,  58,  58}, {237,  55,  55},
   {238,  52,  52}, {239,  49,  49}, {239,  47,  47}, {240,  44,  44}, {241,  41,  41},
   {242,  38,  38}, {243,  35,  35}, {244,  32,  32}, {245,  29,  29}, {246,  26,  26},
   {247,  24,  24}, {248,  21,  21}, {249,  18,  18}, {250,  15,  15}, {251,  12,  12},
   {252,   9,   9}, {253,   6,   6}, {254,   3,   3}, {255,   1,   1}, {255,   3,   0},
   {255,   7,   0}, {255,  11,   0}, {255,  15,   0}, {255,  18,   0}, {255,  22,   0},
   {255,  26,   0}, {255,  30,   0}, {255,  34,   0}, {255,  38,   0}, {255,  41,   0},
   {255,  45,   0}, {255,  49,   0}, {255,  53,   0}, {255,  57,   0}, {255,  60,   0},
   {255,  64,   0}, {255,  68,   0}, {255,  72,   0}, {255,  76,   0}, {255,  80,   0},
   {255,  83,   0}, {255,  87,   0}, {255,  91,   0}, {255,  95,   0}, {255,  99,   0},
   {255, 103,   0}, {255, 106,   0}, {255, 110,   0}, {255, 114,   0}, {255, 118,   0},
   {255, 122,   0}, {255, 126,   0}, {255, 129,   0}, {255, 133,   0}, {255, 137,   0},
   {255, 141,   0}, {255, 145,   0}, {255, 149,   0}, {255, 152,   0}, {255, 156,   0},
   {255, 160,   0}, {255, 164,   0}, {255, 168,   0}, {255, 172,   0}, {255, 175,   0},
   {255, 179,   0}, {255, 183,   0}, {255, 187,   0}, {255, 191,   0}, {255, 195,   0},
   {255, 198,   0}, {255, 202,   0}, {255, 206,   0}, {255, 210,   0}, {255, 214,   0},
   {255, 217,   0}, {255, 221,   0}, {255, 225,   0}, {255, 229,   0}, {255, 233,   0},
   {255, 237,   0}, {255, 240,   0}, {255, 244,   0}, {255, 248,   0}, {255, 252,   0},
   {254, 255,   0}, {250, 255,   0}, {247, 255,   0}, {243, 255,   0}, {239, 255,   0},
   {235, 255,   0}, {231, 255,   0}, {227, 255,   0}, {224, 255,   0}, {220, 255,   0},
   {216, 255,   0}, {212, 255,   0}, {208, 255,   0}, {204, 255,   0}, {201, 255,   0},
   {197, 255,   0}, {193, 255,   0}, {189, 255,   0}, {185, 255,   0}, {181, 255,   0},
   {178, 255,   0}, {174, 255,   0}, {170, 255,   0}, {166, 255,   0}, {162, 255,   0},
   {159, 255,   0}, {155, 255,   0}, {151, 255,   0}, {147, 255,   0}, {143, 255,   0},
   {139, 255,   0}, {136, 255,   0}, {132, 255,   0}, {128, 255,   0}, {124, 255,   0},
   {120, 255,   0}, {116, 255,   0}, {113, 255,   0}, {109, 255,   0}, {105, 255,   0},
   {101, 255,   0}, { 97, 255,   0}, { 93, 255,   0}, { 90, 255,   0}, { 86, 255,   0},
   { 82, 255,   0}, { 78, 255,   0}, { 74, 255,   0}, { 70, 255,   0}, { 67, 255,   0},
   { 63, 255,   0}, { 59, 255,   0}, { 55, 255,   0}, { 51, 255,   0}, { 47, 255,   0},
   { 44, 255,   0}, { 40, 255,   0}, { 36, 255,   0}, { 32, 255,   0}, { 28, 255,   0},
   { 25, 255,   0}, { 21, 255,   0}, { 17, 255,   0}, { 13, 255,   0}, {  9, 255,   0},
   {  5, 255,   0}, {  2, 255,   0}, {  0, 255,   2}, {  0, 255,   6}, {  0, 255,  10},
   {  0, 255,  14}, {  0, 255,  18}, {  0, 255,  21}, {  0, 255,  25}, {  0, 255,  29},
   {  0, 255,  33}, {  0, 255,  37}, {  0, 255,  41}, {  0, 255,  44}, {  0, 255,  48},
   {  0, 255,  52}, {  0, 255,  56}, {  0, 255,  60}, {  0, 255,  64}, {  0, 255,  67},
   {  0, 255,  71}, {  0, 255,  75}, {  0, 255,  79}, {  0, 255,  83}, {  0, 255,  87},
   {  0, 255,  90}, {  0, 255,  94}, {  0, 255,  98}, {  0, 255, 102}, {  0, 255, 106},
   {  0, 255, 110}, {  0, 255, 113}, {  0, 255, 117}, {  0, 255, 121}, {  0, 255, 125},
   {  0, 255, 129}, {  0, 255, 132}, {  0, 255, 136}, {  0, 255, 140}, {  0, 255, 144},
   {  0, 255, 148}, {  0, 255, 152}, {  0, 255, 155}, {  0, 255, 159}, {  0, 255, 163},
   {  0, 255, 167}, {  0, 255, 171}, {  0, 255, 175}, {  0, 255, 178}, {  0, 255, 182},
   {  0, 255, 186}, {  0, 255, 190}, {  0, 255, 194}, {  0, 255, 198}, {  0, 255, 201},
   {  0, 255, 205}, {  0, 255, 209}, {  0, 255, 213}, {  0, 255, 217}, {  0, 255, 221},
   {  0, 255, 224}, {  0, 255, 228}, {  0, 255, 232}, {  0, 255, 236}, {  0, 255, 240},
   {  0, 255, 244}, {  0, 255, 247}, {  0, 255, 251}, {  0, 255, 255}, {  0, 251, 255},
   {  0, 247, 255}, {  0, 244, 255}, {  0, 240, 255}, {  0, 236, 255}, {  0, 232, 255},
   {  0, 228, 255}, {  0, 224, 255}, {  0, 221, 255}, {  0, 217, 255}, {  0, 213, 255},
   {  0, 209, 255}, {  0, 205, 255}, {  0, 201, 255}, {  0, 198, 255}, {  0, 194, 255},
   {  0, 190, 255}, {  0, 186, 255}, {  0, 182, 255}, {  0, 178, 255}, {  0, 175, 255},
   {  0, 171, 255}, {  0, 167, 255}, {  0, 163, 255}, {  0, 159, 255}, {  0, 155, 255},
   {  0, 152, 255}, {  0, 148, 255}, {  0, 144, 255}, {  0, 140, 255}, {  0, 136, 255},
   {  0, 132, 255}, {  0, 129, 255}, {  0, 125, 255}, {  0, 121, 255}, {  0, 117, 255},
   {  0, 113, 255}, {  0, 110, 255}, {  0, 106, 255}, {  0, 102, 255}, {  0,  98, 255},
   {  0,  94, 255}, {  0,  90, 255}, {  0,  87, 255}, {  0,  83, 255}, {  0,  79, 255},
   {  0,  75, 255}, {  0,  71, 255}, {  0,  67, 255}, {  0,  64, 255}, {  0,  60, 255},
   {  0,  56, 255}, {  0,  52, 255}, {  0,  48, 255}, {  0,  44, 255}, {  0,  41, 255},
   {  0,  37, 255}, {  0,  33, 255}, {  0,  29, 255}, {  0,  25, 255}, {  0,  21, 255},
   {  0,  18, 255}, {  0,  14, 255}, {  0,  10, 255}, {  0,   6, 255}, {  0,   2, 255},
   {  2,   0, 255}, {  5,   0, 255}, {  9,   0, 255}, { 13,   0, 255}, { 17,   0, 255},
   { 21,   0, 255}, { 25,   0, 255}, { 28,   0, 255}, { 32,   0, 255}, { 36,   0, 255},
   { 40,   0, 255}, { 44,   0, 255}, { 47,   0, 255}, { 51,   0, 255}, { 55,   0, 255},
   { 59,   0, 255}, { 63,   0, 255}, { 67,   0, 255}, { 70,   0, 255}, { 74,   0, 255},
   { 78,   0, 255}, { 82,   0, 255}, { 86,   0, 255}, { 90,   0, 255}, { 93,   0, 255},
   { 97,   0, 255}, {101,   0, 255}, {105,   0, 255}, {109,   0, 255}, {113,   0, 255},
   {116,   0, 255}, {120,   0, 255}, {124,   0, 255}, {128,   0, 255}, {132,   0, 255},
   {136,   0, 255}, {139,   0, 255}, {143,   0, 255}, {147,   0, 255}, {151,   0, 255},
   {155,   0, 255}, {159,   0, 255}, {162,   0, 255}, {166,   0, 255}, {170,   0, 255},
   {174,   0, 255}, {178,   0, 255}, {181,   0, 255}, {185,   0, 255}, {189,   0, 255},
   {193,   0, 255}, {197,   0, 255}, {201,   0, 255}, {204,   0, 255}, {208,   0, 255},
   {212,   0, 255}, {216,   0, 255}, {220,   0, 255}, {224,   0, 255}, {227,   0, 255},
   {231,   0, 255}, {235,   0, 255}, {239,   0, 255}, {243,   0, 255}, {247,   0, 255},
   {250,   0, 255}, {254,   0, 255}, {252,   0, 252}, {248,   0, 248}, {244,   0, 244},
   {240,   0, 240}, {237,   0, 237}, {233,   0, 233}, {229,   0, 229}, {225,   0, 225},
   {221,   0, 221}, {217,   0, 217}, {214,   0, 214}, {210,   0, 210}, {206,   0, 206},
   {202,   0, 202}, {198,   0, 198}, {195,   0, 195}, {191,   0, 191}, {187,   0, 187},
   {183,   0, 183}, {179,   0, 179}, {175,   0, 175}, {172,   0, 172}, {168,   0, 168},
   {164,   0, 164}, {160,   0, 160}, {156,   0, 156}, {152,   0, 152}, {149,   0, 149},
   {145,   0, 145}, {141,   0, 141}, {137,   0, 137}, {133,   0, 133}, {129,   0, 129},
   {126,   0, 126}, {122,   0, 122}, {118,   0, 118}, {114,   0, 114}, {110,   0, 110},
   {106,   0, 106}, {103,   0, 103}, { 99,   0,  99}, { 95,   0,  95}, { 91,   0,  91},
   { 87,   0,  87}, { 83,   0,  83}, { 80,   0,  80}, { 76,   0,  76}, { 72,   0,  72},
   { 68,   0,  68}, { 64,   0,  64}, { 60,   0,  60}, { 57,   0,  57}, { 53,   0,  53},
   { 49,   0,  49}, { 45,   0,  45}, { 41,   0,  41}, { 38,   0,  38}, { 34,   0,  34},
   { 30,   0,  30}, { 26,   0,  26}, { 22,   0,  22}, { 18,   0,  18}, { 15,   0,  15},
   { 11,   0,  11}, {  7,   0,   7}, {  3,   0,   3}, {  0,   0,   0}, {  2,   2,   2},
   {  4,   4,   4}, {  6,   6,   6}, {  8,   8,   8}, { 10,  10,  10}, { 12,  12,  12},
   { 14,  14,  14}, { 16,  16,  16}, { 18,  18,  18}, { 20,  20,  20}, { 21,  21,  21},
   { 23,  23,  23}, { 25,  25,  25}, { 27,  27,  27}, { 29,  29,  29}, { 31,  31,  31},
   { 33,  33,  33}, { 35,  35,  35}, { 37,  37,  37}, { 39,  39,  39}, { 41,  41,  41},
   { 43,  43,  43}, { 44,  44,  44}, { 46,  46,  46}, { 48,  48,  48}, { 50,  50,  50},
   { 52,  52,  52}, { 54,  54,  54}, { 56,  56,  56}, { 58,  58,  58}, { 60,  60,  60},
   { 62,  62,  62}, { 64,  64,  64}, { 65,  65,  65}, { 67,  67,  67}, { 69,  69,  69},
   { 71,  71,  71}, { 73,  73,  73}, { 75,  75,  75}, { 77,  77,  77}, { 79,  79,  79},
   { 81,  81,  81}, { 83,  83,  83}, { 85,  85,  85}, { 87,  87,  87}, { 88,  88,  88},
   { 90,  90,  90}, { 92,  92,  92}, { 94,  94,  94}, { 96,  96,  96}, { 98,  98,  98},
   {100, 100, 100}, {102, 102, 102}, {104, 104, 104}, {106, 106, 106}, {108, 108, 108},
   {110, 110, 110}, {111, 111, 111}, {113, 113, 113}, {115, 115, 115}, {117, 117, 117},
   {119, 119, 119}, {121, 121, 121}, {123, 123, 123}, {125, 125, 125}, {127, 127, 127},
   {128, 126, 126}, {128, 124, 124}, {128, 123, 123}, {128, 121, 121}, {128, 119, 119},
   {128, 117, 117}, {128, 115, 115}, {128, 113, 113}, {128, 111, 111}, {128, 109, 109},
   {128, 107, 107}, {128, 105, 105}, {128, 103, 103}, {128, 101, 101}, {128, 100, 100},
   {128,  98,  98}, {128,  96,  96}, {128,  94,  94}, {128,  92,  92}, {128,  90,  90},
   {128,  88,  88}, {128,  86,  86}, {128,  84,  84}, {128,  82,  82}, {128,  80,  80},
   {128,  78,  78}, {128,  77,  77}, {128,  75,  75}, {128,  73,  73}, {128,  71,  71},
   {128,  69,  69}, {128,  67,  67}, {128,  65,  65}, {128,  63,  63}, {128,  61,  61},
   {128,  59,  59}, {128,  57,  57}, {128,  56,  56}, {128,  54,  54}, {128,  52,  52},
   {128,  50,  50}, {128,  48,  48}, {128,  46,  46}, {128,  44,  44}, {128,  42,  42},
   {128,  40,  40}, {128,  38,  38}, {128,  36,  36}, {128,  34,  34}, {128,  33,  33},
   {128,  31,  31}, {128,  29,  29}, {128,  27,  27}, {128,  25,  25}, {128,  23,  23},
   {128,  21,  21}, {128,  19,  19}, {128,  17,  17}, {128,  15,  15}, {128,  13,  13},
   {128,  11,  11}, {128,  10,  10}, {128,   8,   8}, {128,   6,   6}, {128,   4,   4},
   {128,   2,   2}, {128,   0,   0}, {128,   2,   0}, {128,   4,   0}, {128,   6,   0},
   {128,   8,   0}, {128,  10,   0}, {128,  11,   0}, {128,  13,   0}, {128,  15,   0},
   {128,  17,   0}, {128,  19,   0}, {128,  21,   0}, {128,  23,   0}, {128,  25,   0},
   {128,  27,   0}, {128,  29,   0}, {128,  31,   0}, {128,  33,   0}, {128,  34,   0},
   {128,  36,   0}, {128,  38,   0}, {128,  40,   0}, {128,  42,   0}, {128,  44,   0},
   {128,  46,   0}, {128,  48,   0}, {128,  50,   0}, {128,  52,   0}, {128,  54,   0},
   {128,  56,   0}, {128,  57,   0}, {128,  59,   0}, {128,  61,   0}, {128,  63,   0},
   {128,  65,   0}, {128,  67,   0}, {128,  69,   0}, {128,  71,   0}, {128,  73,   0},
   {128,  75,   0}, {128,  77,   0}, {128,  78,   0}, {128,  80,   0}, {128,  82,   0},
   {128,  84,   0}, {128,  86,   0}, {128,  88,   0}, {128,  90,   0}, {128,  92,   0},
   {128,  94,   0}, {128,  96,   0}, {128,  98,   0}, {128, 100,   0}, {128, 101,   0},
   {128, 103,   0}, {128, 105,   0}, {128, 107,   0}, {128, 109,   0}, {128, 111,   0},
   {128, 113,   0}, {128, 115,   0}, {128, 117,   0}, {128, 119,   0}, {128, 121,   0},
   {128, 123,   0}, {128, 124,   0}, {128, 126,   0}, {127, 128,   0}, {125, 128,   0},
   {123, 128,   0}, {121, 128,   0}, {119, 128,   0}, {117, 128,   0}, {115, 128,   0},
   {113, 128,   0}, {111, 128,   0}, {110, 128,   0}, {108, 128,   0}, {106, 128,   0},
   {104, 128,   0}, {102, 128,   0}, {100, 128,   0}, { 98, 128,   0}, { 96, 128,   0},
   { 94, 128,   0}, { 92, 128,   0}, { 90, 128,   0}, { 88, 128,   0}, { 87, 128,   0},
   { 85, 128,   0}, { 83, 128,   0}, { 81, 128,   0}, { 79, 128,   0}, { 77, 128,   0},
   { 75, 128,   0}, { 73, 128,   0}, { 71, 128,   0}, { 69, 128,   0}, { 67, 128,   0},
   { 65, 128,   0}, { 64, 128,   0}, { 62, 128,   0}, { 60, 128,   0}, { 58, 128,   0},
   { 56, 128,   0}, { 54, 128,   0}, { 52, 128,   0}, { 50, 128,   0}, { 48, 128,   0},
   { 46, 128,   0}, { 44, 128,   0}, { 43, 128,   0}, { 41, 128,   0}, { 39, 128,   0},
   { 37, 128,   0}, { 35, 128,   0}, { 33, 128,   0}, { 31, 128,   0}, { 29, 128,   0},
   { 27, 128,   0}, { 25, 128,   0}, { 23, 128,   0}, { 21, 128,   0}, { 20, 128,   0},
   { 18, 128,   0}, { 16, 128,   0}, { 14, 128,   0}, { 12, 128,   0}, { 10, 128,   0},
   {  8, 128,   0}, {  6, 128,   0}, {  4, 128,   0}, {  2, 128,   0}, {  0, 128,   0},
   {  0, 128,   2}, {  0, 128,   3}, {  0, 128,   5}, {  0, 128,   7}, {  0, 128,   9},
   {  0, 128,  11}, {  0, 128,  13}, {  0, 128,  15}, {  0, 128,  17}, {  0, 128,  19},
   {  0, 128,  21}, {  0, 128,  23}, {  0, 128,  25}, {  0, 128,  26}, {  0, 128,  28},
   {  0, 128,  30}, {  0, 128,  32}, {  0, 128,  34}, {  0, 128,  36}, {  0, 128,  38},
   {  0, 128,  40}, {  0, 128,  42}, {  0, 128,  44}, {  0, 128,  46}, {  0, 128,  47},
   {  0, 128,  49}, {  0, 128,  51}, {  0, 128,  53}, {  0, 128,  55}, {  0, 128,  57},
   {  0, 128,  59}, {  0, 128,  61}, {  0, 128,  63}, {  0, 128,  65}, {  0, 128,  67},
   {  0, 128,  69}, {  0, 128,  70}, {  0, 128,  72}, {  0, 128,  74}, {  0, 128,  76},
   {  0, 128,  78}, {  0, 128,  80}, {  0, 128,  82}, {  0, 128,  84}, {  0, 128,  86},
   {  0, 128,  88}, {  0, 128,  90}, {  0, 128,  92}, {  0, 128,  93}, {  0, 128,  95},
   {  0, 128,  97}, {  0, 128,  99}, {  0, 128, 101}, {  0, 128, 103}, {  0, 128, 105},
   {  0, 128, 107}, {  0, 128, 109}, {  0, 128, 111}, {  0, 128, 113}, {  0, 128, 114},
   {  0, 128, 116}, {  0, 128, 118}, {  0, 128, 120}, {  0, 128, 122}, {  0, 128, 124},
   {  0, 128, 126}, {  0, 127, 128}, {  0, 125, 128}, {  0, 123, 128}, {  0, 121, 128},
   {  0, 119, 128}, {  0, 118, 128}, {  0, 116, 128}, {  0, 114, 128}, {  0, 112, 128},
   {  0, 110, 128}, {  0, 108, 128}, {  0, 106, 128}, {  0, 104, 128}, {  0, 102, 128},
   {  0, 100, 128}, {  0,  98, 128}, {  0,  96, 128}, {  0,  95, 128}, {  0,  93, 128},
   {  0,  91, 128}, {  0,  89, 128}, {  0,  87, 128}, {  0,  85, 128}, {  0,  83, 128},
   {  0,  81, 128}, {  0,  79, 128}, {  0,  77, 128}, {  0,  75, 128}, {  0,  74, 128},
   {  0,  72, 128}, {  0,  70, 128}, {  0,  68, 128}, {  0,  66, 128}, {  0,  64, 128},
   {  0,  62, 128}, {  0,  60, 128}, {  0,  58, 128}, {  0,  56, 128}, {  0,  54, 128},
   {  0,  52, 128}, {  0,  51, 128}, {  0,  49, 128}, {  0,  47, 128}, {  0,  45, 128},
   {  0,  43, 128}, {  0,  41, 128}, {  0,  39, 128}, {  0,  37, 128}, {  0,  35, 128},
   {  0,  33, 128}, {  0,  31, 128}, {  0,  29, 128}, {  0,  28, 128}, {  0,  26, 128},
   {  0,  24, 128}, {  0,  22, 128}, {  0,  20, 128}, {  0,  18, 128}, {  0,  16, 128},
   {  0,  14, 128}, {  0,  12, 128}, {  0,  10, 128}, {  0,   8, 128}, {  0,   7, 128},
   {  0,   5, 128}, {  0,   3, 128}, {  0,   1, 128}, {  1,   0, 128}, {  3,   0, 128},
   {  5,   0, 128}, {  7,   0, 128}, {  9,   0, 128}, { 11,   0, 128}, { 13,   0, 128},
   { 15,   0, 128}, { 16,   0, 128}, { 18,   0, 128}, { 20,   0, 128}, { 22,   0, 128},
   { 24,   0, 128}, { 26,   0, 128}, { 28,   0, 128}, { 30,   0, 128}, { 32,   0, 128},
   { 34,   0, 128}, { 36,   0, 128}, { 38,   0, 128}, { 39,   0, 128}, { 41,   0, 128},
   { 43,   0, 128}, { 45,   0, 128}, { 47,   0, 128}, { 49,   0, 128}, { 51,   0, 128},
   { 53,   0, 128}, { 55,   0, 128}, { 57,   0, 128}, { 59,   0, 128}, { 60,   0, 128},
   { 62,   0, 128}, { 64,   0, 128}, { 66,   0, 128}, { 68,   0, 128}, { 70,   0, 128},
   { 72,   0, 128}, { 74,   0, 128}, { 76,   0, 128}, { 78,   0, 128}, { 80,   0, 128},
   { 82,   0, 128}, { 83,   0, 128}, { 85,   0, 128}, { 87,   0, 128}, { 89,   0, 128},
   { 91,   0, 128}, { 93,   0, 128}, { 95,   0, 128}, { 97,   0, 128}, { 99,   0, 128},
   {101,   0, 128}, {103,   0, 128}, {105,   0, 128}, {106,   0, 128}, {108,   0, 128},
   {110,   0, 128}, {112,   0, 128}, {114,   0, 128}, {116,   0, 128}, {118,   0, 128},
   {120,   0, 128}, {122,   0, 128}, {124,   0, 128}, {126,   0, 128}, {128,   0, 128}
};

const color yarg_colormap[1000] = {
   {  0,   0,   0}, {  0,   0,   0}, {  1,   1,   1}, {  1,   1,   1}, {  1,   1,   1},
   {  1,   1,   1}, {  2,   2,   2}, {  2,   2,   2}, {  2,   2,   2}, {  2,   2,   2},
   {  3,   3,   3}, {  3,   3,   3}, {  3,   3,   3}, {  3,   3,   3}, {  4,   4,   4},
   {  4,   4,   4}, {  4,   4,   4}, {  4,   4,   4}, {  5,   5,   5}, {  5,   5,   5},
   {  5,   5,   5}, {  5,   5,   5}, {  6,   6,   6}, {  6,   6,   6}, {  6,   6,   6},
   {  6,   6,   6}, {  7,   7,   7}, {  7,   7,   7}, {  7,   7,   7}, {  7,   7,   7},
   {  8,   8,   8}, {  8,   8,   8}, {  8,   8,   8}, {  8,   8,   8}, {  9,   9,   9},
   {  9,   9,   9}, {  9,   9,   9}, {  9,   9,   9}, { 10,  10,  10}, { 10,  10,  10},
   { 10,  10,  10}, { 10,  10,  10}, { 11,  11,  11}, { 11,  11,  11}, { 11,  11,  11},
   { 11,  11,  11}, { 12,  12,  12}, { 12,  12,  12}, { 12,  12,  12}, { 13,  13,  13},
   { 13,  13,  13}, { 13,  13,  13}, { 13,  13,  13}, { 14,  14,  14}, { 14,  14,  14},
   { 14,  14,  14}, { 14,  14,  14}, { 15,  15,  15}, { 15,  15,  15}, { 15,  15,  15},
   { 15,  15,  15}, { 16,  16,  16}, { 16,  16,  16}, { 16,  16,  16}, { 16,  16,  16},
   { 17,  17,  17}, { 17,  17,  17}, { 17,  17,  17}, { 17,  17,  17}, { 18,  18,  18},
   { 18,  18,  18}, { 18,  18,  18}, { 18,  18,  18}, { 19,  19,  19}, { 19,  19,  19},
   { 19,  19,  19}, { 19,  19,  19}, { 20,  20,  20}, { 20,  20,  20}, { 20,  20,  20},
   { 20,  20,  20}, { 21,  21,  21}, { 21,  21,  21}, { 21,  21,  21}, { 21,  21,  21},
   { 22,  22,  22}, { 22,  22,  22}, { 22,  22,  22}, { 22,  22,  22}, { 23,  23,  23},
   { 23,  23,  23}, { 23,  23,  23}, { 23,  23,  23}, { 24,  24,  24}, { 24,  24,  24},
   { 24,  24,  24}, { 25,  25,  25}, { 25,  25,  25}, { 25,  25,  25}, { 25,  25,  25},
   { 26,  26,  26}, { 26,  26,  26}, { 26,  26,  26}, { 26,  26,  26}, { 27,  27,  27},
   { 27,  27,  27}, { 27,  27,  27}, { 27,  27,  27}, { 28,  28,  28}, { 28,  28,  28},
   { 28,  28,  28}, { 28,  28,  28}, { 29,  29,  29}, { 29,  29,  29}, { 29,  29,  29},
   { 29,  29,  29}, { 30,  30,  30}, { 30,  30,  30}, { 30,  30,  30}, { 30,  30,  30},
   { 31,  31,  31}, { 31,  31,  31}, { 31,  31,  31}, { 31,  31,  31}, { 32,  32,  32},
   { 32,  32,  32}, { 32,  32,  32}, { 32,  32,  32}, { 33,  33,  33}, { 33,  33,  33},
   { 33,  33,  33}, { 33,  33,  33}, { 34,  34,  34}, { 34,  34,  34}, { 34,  34,  34},
   { 34,  34,  34}, { 35,  35,  35}, { 35,  35,  35}, { 35,  35,  35}, { 35,  35,  35},
   { 36,  36,  36}, { 36,  36,  36}, { 36,  36,  36}, { 37,  37,  37}, { 37,  37,  37},
   { 37,  37,  37}, { 37,  37,  37}, { 38,  38,  38}, { 38,  38,  38}, { 38,  38,  38},
   { 38,  38,  38}, { 39,  39,  39}, { 39,  39,  39}, { 39,  39,  39}, { 39,  39,  39},
   { 40,  40,  40}, { 40,  40,  40}, { 40,  40,  40}, { 40,  40,  40}, { 41,  41,  41},
   { 41,  41,  41}, { 41,  41,  41}, { 41,  41,  41}, { 42,  42,  42}, { 42,  42,  42},
   { 42,  42,  42}, { 42,  42,  42}, { 43,  43,  43}, { 43,  43,  43}, { 43,  43,  43},
   { 43,  43,  43}, { 44,  44,  44}, { 44,  44,  44}, { 44,  44,  44}, { 44,  44,  44},
   { 45,  45,  45}, { 45,  45,  45}, { 45,  45,  45}, { 45,  45,  45}, { 46,  46,  46},
   { 46,  46,  46}, { 46,  46,  46}, { 46,  46,  46}, { 47,  47,  47}, { 47,  47,  47},
   { 47,  47,  47}, { 47,  47,  47}, { 48,  48,  48}, { 48,  48,  48}, { 48,  48,  48},
   { 48,  48,  48}, { 49,  49,  49}, { 49,  49,  49}, { 49,  49,  49}, { 50,  50,  50},
   { 50,  50,  50}, { 50,  50,  50}, { 50,  50,  50}, { 51,  51,  51}, { 51,  51,  51},
   { 51,  51,  51}, { 51,  51,  51}, { 52,  52,  52}, { 52,  52,  52}, { 52,  52,  52},
   { 52,  52,  52}, { 53,  53,  53}, { 53,  53,  53}, { 53,  53,  53}, { 53,  53,  53},
   { 54,  54,  54}, { 54,  54,  54}, { 54,  54,  54}, { 54,  54,  54}, { 55,  55,  55},
   { 55,  55,  55}, { 55,  55,  55}, { 55,  55,  55}, { 56,  56,  56}, { 56,  56,  56},
   { 56,  56,  56}, { 56,  56,  56}, { 57,  57,  57}, { 57,  57,  57}, { 57,  57,  57},
   { 57,  57,  57}, { 58,  58,  58}, { 58,  58,  58}, { 58,  58,  58}, { 58,  58,  58},
   { 59,  59,  59}, { 59,  59,  59}, { 59,  59,  59}, { 59,  59,  59}, { 60,  60,  60},
   { 60,  60,  60}, { 60,  60,  60}, { 60,  60,  60}, { 61,  61,  61}, { 61,  61,  61},
   { 61,  61,  61}, { 62,  62,  62}, { 62,  62,  62}, { 62,  62,  62}, { 62,  62,  62},
   { 63,  63,  63}, { 63,  63,  63}, { 63,  63,  63}, { 63,  63,  63}, { 64,  64,  64},
   { 64,  64,  64}, { 64,  64,  64}, { 64,  64,  64}, { 65,  65,  65}, { 65,  65,  65},
   { 65,  65,  65}, { 65,  65,  65}, { 66,  66,  66}, { 66,  66,  66}, { 66,  66,  66},
   { 66,  66,  66}, { 67,  67,  67}, { 67,  67,  67}, { 67,  67,  67}, { 67,  67,  67},
   { 68,  68,  68}, { 68,  68,  68}, { 68,  68,  68}, { 68,  68,  68}, { 69,  69,  69},
   { 69,  69,  69}, { 69,  69,  69}, { 69,  69,  69}, { 70,  70,  70}, { 70,  70,  70},
   { 70,  70,  70}, { 70,  70,  70}, { 71,  71,  71}, { 71,  71,  71}, { 71,  71,  71},
   { 71,  71,  71}, { 72,  72,  72}, { 72,  72,  72}, { 72,  72,  72}, { 72,  72,  72},
   { 73,  73,  73}, { 73,  73,  73}, { 73,  73,  73}, { 74,  74,  74}, { 74,  74,  74},
   { 74,  74,  74}, { 74,  74,  74}, { 75,  75,  75}, { 75,  75,  75}, { 75,  75,  75},
   { 75,  75,  75}, { 76,  76,  76}, { 76,  76,  76}, { 76,  76,  76}, { 76,  76,  76},
   { 77,  77,  77}, { 77,  77,  77}, { 77,  77,  77}, { 77,  77,  77}, { 78,  78,  78},
   { 78,  78,  78}, { 78,  78,  78}, { 78,  78,  78}, { 79,  79,  79}, { 79,  79,  79},
   { 79,  79,  79}, { 79,  79,  79}, { 80,  80,  80}, { 80,  80,  80}, { 80,  80,  80},
   { 80,  80,  80}, { 81,  81,  81}, { 81,  81,  81}, { 81,  81,  81}, { 81,  81,  81},
   { 82,  82,  82}, { 82,  82,  82}, { 82,  82,  82}, { 82,  82,  82}, { 83,  83,  83},
   { 83,  83,  83}, { 83,  83,  83}, { 83,  83,  83}, { 84,  84,  84}, { 84,  84,  84},
   { 84,  84,  84}, { 84,  84,  84}, { 85,  85,  85}, { 85,  85,  85}, { 85,  85,  85},
   { 86,  86,  86}, { 86,  86,  86}, { 86,  86,  86}, { 86,  86,  86}, { 87,  87,  87},
   { 87,  87,  87}, { 87,  87,  87}, { 87,  87,  87}, { 88,  88,  88}, { 88,  88,  88},
   { 88,  88,  88}, { 88,  88,  88}, { 89,  89,  89}, { 89,  89,  89}, { 89,  89,  89},
   { 89,  89,  89}, { 90,  90,  90}, { 90,  90,  90}, { 90,  90,  90}, { 90,  90,  90},
   { 91,  91,  91}, { 91,  91,  91}, { 91,  91,  91}, { 91,  91,  91}, { 92,  92,  92},
   { 92,  92,  92}, { 92,  92,  92}, { 92,  92,  92}, { 93,  93,  93}, { 93,  93,  93},
   { 93,  93,  93}, { 93,  93,  93}, { 94,  94,  94}, { 94,  94,  94}, { 94,  94,  94},
   { 94,  94,  94}, { 95,  95,  95}, { 95,  95,  95}, { 95,  95,  95}, { 95,  95,  95},
   { 96,  96,  96}, { 96,  96,  96}, { 96,  96,  96}, { 96,  96,  96}, { 97,  97,  97},
   { 97,  97,  97}, { 97,  97,  97}, { 98,  98,  98}, { 98,  98,  98}, { 98,  98,  98},
   { 98,  98,  98}, { 99,  99,  99}, { 99,  99,  99}, { 99,  99,  99}, { 99,  99,  99},
   {100, 100, 100}, {100, 100, 100}, {100, 100, 100}, {100, 100, 100}, {101, 101, 101},
   {101, 101, 101}, {101, 101, 101}, {101, 101, 101}, {102, 102, 102}, {102, 102, 102},
   {102, 102, 102}, {102, 102, 102}, {103, 103, 103}, {103, 103, 103}, {103, 103, 103},
   {103, 103, 103}, {104, 104, 104}, {104, 104, 104}, {104, 104, 104}, {104, 104, 104},
   {105, 105, 105}, {105, 105, 105}, {105, 105, 105}, {105, 105, 105}, {106, 106, 106},
   {106, 106, 106}, {106, 106, 106}, {106, 106, 106}, {107, 107, 107}, {107, 107, 107},
   {107, 107, 107}, {107, 107, 107}, {108, 108, 108}, {108, 108, 108}, {108, 108, 108},
   {108, 108, 108}, {109, 109, 109}, {109, 109, 109}, {109, 109, 109}, {110, 110, 110},
   {110, 110, 110}, {110, 110, 110}, {110, 110, 110}, {111, 111, 111}, {111, 111, 111},
   {111, 111, 111}, {111, 111, 111}, {112, 112, 112}, {112, 112, 112}, {112, 112, 112},
   {112, 112, 112}, {113, 113, 113}, {113, 113, 113}, {113, 113, 113}, {113, 113, 113},
   {114, 114, 114}, {114, 114, 114}, {114, 114, 114}, {114, 114, 114}, {115, 115, 115},
   {115, 115, 115}, {115, 115, 115}, {115, 115, 115}, {116, 116, 116}, {116, 116, 116},
   {116, 116, 116}, {116, 116, 116}, {117, 117, 117}, {117, 117, 117}, {117, 117, 117},
   {117, 117, 117}, {118, 118, 118}, {118, 118, 118}, {118, 118, 118}, {118, 118, 118},
   {119, 119, 119}, {119, 119, 119}, {119, 119, 119}, {119, 119, 119}, {120, 120, 120},
   {120, 120, 120}, {120, 120, 120}, {120, 120, 120}, {121, 121, 121}, {121, 121, 121},
   {121, 121, 121}, {122, 122, 122}, {122, 122, 122}, {122, 122, 122}, {122, 122, 122},
   {123, 123, 123}, {123, 123, 123}, {123, 123, 123}, {123, 123, 123}, {124, 124, 124},
   {124, 124, 124}, {124, 124, 124}, {124, 124, 124}, {125, 125, 125}, {125, 125, 125},
   {125, 125, 125}, {125, 125, 125}, {126, 126, 126}, {126, 126, 126}, {126, 126, 126},
   {126, 126, 126}, {127, 127, 127}, {127, 127, 127}, {127, 127, 127}, {127, 127, 127},
   {128, 128, 128}, {128, 128, 128}, {128, 128, 128}, {128, 128, 128}, {129, 129, 129},
   {129, 129, 129}, {129, 129, 129}, {129, 129, 129}, {130, 130, 130}, {130, 130, 130},
   {130, 130, 130}, {130, 130, 130}, {131, 131, 131}, {131, 131, 131}, {131, 131, 131},
   {131, 131, 131}, {132, 132, 132}, {132, 132, 132}, {132, 132, 132}, {132, 132, 132},
   {133, 133, 133}, {133, 133, 133}, {133, 133, 133}, {133, 133, 133}, {134, 134, 134},
   {134, 134, 134}, {134, 134, 134}, {135, 135, 135}, {135, 135, 135}, {135, 135, 135},
   {135, 135, 135}, {136, 136, 136}, {136, 136, 136}, {136, 136, 136}, {136, 136, 136},
   {137, 137, 137}, {137, 137, 137}, {137, 137, 137}, {137, 137, 137}, {138, 138, 138},
   {138, 138, 138}, {138, 138, 138}, {138, 138, 138}, {139, 139, 139}, {139, 139, 139},
   {139, 139, 139}, {139, 139, 139}, {140, 140, 140}, {140, 140, 140}, {140, 140, 140},
   {140, 140, 140}, {141, 141, 141}, {141, 141, 141}, {141, 141, 141}, {141, 141, 141},
   {142, 142, 142}, {142, 142, 142}, {142, 142, 142}, {142, 142, 142}, {143, 143, 143},
   {143, 143, 143}, {143, 143, 143}, {143, 143, 143}, {144, 144, 144}, {144, 144, 144},
   {144, 144, 144}, {144, 144, 144}, {145, 145, 145}, {145, 145, 145}, {145, 145, 145},
   {145, 145, 145}, {146, 146, 146}, {146, 146, 146}, {146, 146, 146}, {147, 147, 147},
   {147, 147, 147}, {147, 147, 147}, {147, 147, 147}, {148, 148, 148}, {148, 148, 148},
   {148, 148, 148}, {148, 148, 148}, {149, 149, 149}, {149, 149, 149}, {149, 149, 149},
   {149, 149, 149}, {150, 150, 150}, {150, 150, 150}, {150, 150, 150}, {150, 150, 150},
   {151, 151, 151}, {151, 151, 151}, {151, 151, 151}, {151, 151, 151}, {152, 152, 152},
   {152, 152, 152}, {152, 152, 152}, {152, 152, 152}, {153, 153, 153}, {153, 153, 153},
   {153, 153, 153}, {153, 153, 153}, {154, 154, 154}, {154, 154, 154}, {154, 154, 154},
   {154, 154, 154}, {155, 155, 155}, {155, 155, 155}, {155, 155, 155}, {155, 155, 155},
   {156, 156, 156}, {156, 156, 156}, {156, 156, 156}, {156, 156, 156}, {157, 157, 157},
   {157, 157, 157}, {157, 157, 157}, {157, 157, 157}, {158, 158, 158}, {158, 158, 158},
   {158, 158, 158}, {159, 159, 159}, {159, 159, 159}, {159, 159, 159}, {159, 159, 159},
   {160, 160, 160}, {160, 160, 160}, {160, 160, 160}, {160, 160, 160}, {161, 161, 161},
   {161, 161, 161}, {161, 161, 161}, {161, 161, 161}, {162, 162, 162}, {162, 162, 162},
   {162, 162, 162}, {162, 162, 162}, {163, 163, 163}, {163, 163, 163}, {163, 163, 163},
   {163, 163, 163}, {164, 164, 164}, {164, 164, 164}, {164, 164, 164}, {164, 164, 164},
   {165, 165, 165}, {165, 165, 165}, {165, 165, 165}, {165, 165, 165}, {166, 166, 166},
   {166, 166, 166}, {166, 166, 166}, {166, 166, 166}, {167, 167, 167}, {167, 167, 167},
   {167, 167, 167}, {167, 167, 167}, {168, 168, 168}, {168, 168, 168}, {168, 168, 168},
   {168, 168, 168}, {169, 169, 169}, {169, 169, 169}, {169, 169, 169}, {169, 169, 169},
   {170, 170, 170}, {170, 170, 170}, {170, 170, 170}, {171, 171, 171}, {171, 171, 171},
   {171, 171, 171}, {171, 171, 171}, {172, 172, 172}, {172, 172, 172}, {172, 172, 172},
   {172, 172, 172}, {173, 173, 173}, {173, 173, 173}, {173, 173, 173}, {173, 173, 173},
   {174, 174, 174}, {174, 174, 174}, {174, 174, 174}, {174, 174, 174}, {175, 175, 175},
   {175, 175, 175}, {175, 175, 175}, {175, 175, 175}, {176, 176, 176}, {176, 176, 176},
   {176, 176, 176}, {176, 176, 176}, {177, 177, 177}, {177, 177, 177}, {177, 177, 177},
   {177, 177, 177}, {178, 178, 178}, {178, 178, 178}, {178, 178, 178}, {178, 178, 178},
   {179, 179, 179}, {179, 179, 179}, {179, 179, 179}, {179, 179, 179}, {180, 180, 180},
   {180, 180, 180}, {180, 180, 180}, {180, 180, 180}, {181, 181, 181}, {181, 181, 181},
   {181, 181, 181}, {181, 181, 181}, {182, 182, 182}, {182, 182, 182}, {182, 182, 182},
   {183, 183, 183}, {183, 183, 183}, {183, 183, 183}, {183, 183, 183}, {184, 184, 184},
   {184, 184, 184}, {184, 184, 184}, {184, 184, 184}, {185, 185, 185}, {185, 185, 185},
   {185, 185, 185}, {185, 185, 185}, {186, 186, 186}, {186, 186, 186}, {186, 186, 186},
   {186, 186, 186}, {187, 187, 187}, {187, 187, 187}, {187, 187, 187}, {187, 187, 187},
   {188, 188, 188}, {188, 188, 188}, {188, 188, 188}, {188, 188, 188}, {189, 189, 189},
   {189, 189, 189}, {189, 189, 189}, {189, 189, 189}, {190, 190, 190}, {190, 190, 190},
   {190, 190, 190}, {190, 190, 190}, {191, 191, 191}, {191, 191, 191}, {191, 191, 191},
   {191, 191, 191}, {192, 192, 192}, {192, 192, 192}, {192, 192, 192}, {192, 192, 192},
   {193, 193, 193}, {193, 193, 193}, {193, 193, 193}, {193, 193, 193}, {194, 194, 194},
   {194, 194, 194}, {194, 194, 194}, {195, 195, 195}, {195, 195, 195}, {195, 195, 195},
   {195, 195, 195}, {196, 196, 196}, {196, 196, 196}, {196, 196, 196}, {196, 196, 196},
   {197, 197, 197}, {197, 197, 197}, {197, 197, 197}, {197, 197, 197}, {198, 198, 198},
   {198, 198, 198}, {198, 198, 198}, {198, 198, 198}, {199, 199, 199}, {199, 199, 199},
   {199, 199, 199}, {199, 199, 199}, {200, 200, 200}, {200, 200, 200}, {200, 200, 200},
   {200, 200, 200}, {201, 201, 201}, {201, 201, 201}, {201, 201, 201}, {201, 201, 201},
   {202, 202, 202}, {202, 202, 202}, {202, 202, 202}, {202, 202, 202}, {203, 203, 203},
   {203, 203, 203}, {203, 203, 203}, {203, 203, 203}, {204, 204, 204}, {204, 204, 204},
   {204, 204, 204}, {204, 204, 204}, {205, 205, 205}, {205, 205, 205}, {205, 205, 205},
   {205, 205, 205}, {206, 206, 206}, {206, 206, 206}, {206, 206, 206}, {207, 207, 207},
   {207, 207, 207}, {207, 207, 207}, {207, 207, 207}, {208, 208, 208}, {208, 208, 208},
   {208, 208, 208}, {208, 208, 208}, {209, 209, 209}, {209, 209, 209}, {209, 209, 209},
   {209, 209, 209}, {210, 210, 210}, {210, 210, 210}, {210, 210, 210}, {210, 210, 210},
   {211, 211, 211}, {211, 211, 211}, {211, 211, 211}, {211, 211, 211}, {212, 212, 212},
   {212, 212, 212}, {212, 212, 212}, {212, 212, 212}, {213, 213, 213}, {213, 213, 213},
   {213, 213, 213}, {213, 213, 213}, {214, 214, 214}, {214, 214, 214}, {214, 214, 214},
   {214, 214, 214}, {215, 215, 215}, {215, 215, 215}, {215, 215, 215}, {215, 215, 215},
   {216, 216, 216}, {216, 216, 216}, {216, 216, 216}, {216, 216, 216}, {217, 217, 217},
   {217, 217, 217}, {217, 217, 217}, {217, 217, 217}, {218, 218, 218}, {218, 218, 218},
   {218, 218, 218}, {218, 218, 218}, {219, 219, 219}, {219, 219, 219}, {219, 219, 219},
   {220, 220, 220}, {220, 220, 220}, {220, 220, 220}, {220, 220, 220}, {221, 221, 221},
   {221, 221, 221}, {221, 221, 221}, {221, 221, 221}, {222, 222, 222}, {222, 222, 222},
   {222, 222, 222}, {222, 222, 222}, {223, 223, 223}, {223, 223, 223}, {223, 223, 223},
   {223, 223, 223}, {224, 224, 224}, {224, 224, 224}, {224, 224, 224}, {224, 224, 224},
   {225, 225, 225}, {225, 225, 225}, {225, 225, 225}, {225, 225, 225}, {226, 226, 226},
   {226, 226, 226}, {226, 226, 226}, {226, 226, 226}, {227, 227, 227}, {227, 227, 227},
   {227, 227, 227}, {227, 227, 227}, {228, 228, 228}, {228, 228, 228}, {228, 228, 228},
   {228, 228, 228}, {229, 229, 229}, {229, 229, 229}, {229, 229, 229}, {229, 229, 229},
   {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {230, 230, 230}, {231, 231, 231},
   {231, 231, 231}, {231, 231, 231}, {232, 232, 232}, {232, 232, 232}, {232, 232, 232},
   {232, 232, 232}, {233, 233, 233}, {233, 233, 233}, {233, 233, 233}, {233, 233, 233},
   {234, 234, 234}, {234, 234, 234}, {234, 234, 234}, {234, 234, 234}, {235, 235, 235},
   {235, 235, 235}, {235, 235, 235}, {235, 235, 235}, {236, 236, 236}, {236, 236, 236},
   {236, 236, 236}, {236, 236, 236}, {237, 237, 237}, {237, 237, 237}, {237, 237, 237},
   {237, 237, 237}, {238, 238, 238}, {238, 238, 238}, {238, 238, 238}, {238, 238, 238},
   {239, 239, 239}, {239, 239, 239}, {239, 239, 239}, {239, 239, 239}, {240, 240, 240},
   {240, 240, 240}, {240, 240, 240}, {240, 240, 240}, {241, 241, 241}, {241, 241, 241},
   {241, 241, 241}, {241, 241, 241}, {242, 242, 242}, {242, 242, 242}, {242, 242, 242},
   {242, 242, 242}, {243, 243, 243}, {243, 243, 243}, {243, 243, 243}, {244, 244, 244},
   {244, 244, 244}, {244, 244, 244}, {244, 244, 244}, {245, 245, 245}, {245, 245, 245},
   {245, 245, 245}, {245, 245, 245}, {246, 246, 246}, {246, 246, 246}, {246, 246, 246},
   {246, 246, 246}, {247, 247, 247}, {247, 247, 247}, {247, 247, 247}, {247, 247, 247},
   {248, 248, 248}, {248, 248, 248}, {248, 248, 248}, {248, 248, 248}, {249, 249, 249},
   {249, 249, 249}, {249, 249, 249}, {249, 249, 249}, {250, 250, 250}, {250, 250, 250},
   {250, 250, 250}, {250, 250, 250}, {251, 251, 251}, {251, 251, 251}, {251, 251, 251},
   {251, 251, 251}, {252, 252, 252}, {252, 252, 252}, {252, 252, 252}, {252, 252, 252},
   {253, 253, 253}, {253, 253, 253}, {253, 253, 253}, {253, 253, 253}, {254, 254, 254},
   {254, 254, 254}, {254, 254, 254}, {254, 254, 254}, {255, 255, 255}, {255, 255, 255}
};

// FONTS =================================================================================================================================

/****************************************************************
Font data in array format. [charWidth x charHeight length array]
std::vector is sorted by ASCII codes from 32 (space) to  126 (~)

parameters:
	font[0][0] : characters Width
	font[0][1] : characters Height

example of data storage ('#' are the printed pixels of the char)
	--#--    00100
	-#-#- -> 01010 -> 00100 01010 01100 -> 001000101001100
	-##--    01100

Copyright (c) Andrea Marchi 2021
****************************************************************/


std::vector<std::string> ZX_Spectrum_16x16 = {
	{16, 16}, // width and height of character in pixels {charWidth, charHeight}
	"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------", // <space>
	"--------------------------------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00----------------------------------------------00--------------00----------------------------------------", // !
	"------------------------------------00----00--------00----00--------00----00--------00----00--------------------------------------------------------------------------------------------------------------------------------------------------------------------", // "
	"------------------------------------00----00--------00----00------000000000000----000000000000------00----00--------00----00--------00----00--------00----00------000000000000----000000000000------00----00--------00----00------------------------------------", // #
	"----------------------------------------00--------------00----------0000000000------0000000000------00--00----------00--00----------0000000000------0000000000----------00--00----------00--00------0000000000------0000000000----------00--------------00------", // $
	"----------------------------------0000------00----0000------00----0000----00------0000----00------------00--------------00------------00--------------00------------00----0000------00----0000----00------0000----00------0000----------------------------------", // %
	"--------------------------------------00--------------00------------00--00----------00--00------------00--------------00------------00--00--00------00--00--00----00------00------00------00--------000000--00------000000--00----------------------------------", // &
	"----------------------------------------00--------------00------------00--------------00------------------------------------------------------------------------------------------------------------------------------------------------------------------------", // '
	"------------------------------------------00--------------00------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00----------------00--------------00------------------------------------", // (
	"------------------------------------00--------------00----------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00------------00--------------00------------------------------------------", // )
	"----------------------------------------------------------------------00--00----------00--00------------00--------------00----------0000000000------0000000000----------00--------------00------------00--00----------00--00------------------------------------", // *
	"------------------------------------------------------------------------00--------------00--------------00--------------00----------0000000000------0000000000----------00--------------00--------------00--------------00--------------------------------------", // +
	"------------------------------------------------------------------------------------------------------------------------------------------------------------------------00--------------00--------------00--------------00------------00--------------00--------", // ,
	"------------------------------------------------------------------------------------------------------------------------------------0000000000------0000000000--------------------------------------------------------------------------------------------------", // -
	"----------------------------------------------------------------------------------------------------------------------------------------------------------------------0000------------0000------------0000------------0000--------------------------------------", // .
	"----------------------------------------------------------------------------00--------------00------------00--------------00------------00--------------00------------00--------------00------------00--------------00------------------------------------------", // /
	"------------------------------------00000000--------00000000------00------0000----00------0000----00----00--00----00----00--00----00--00----00----00--00----00----0000------00----0000------00------00000000--------00000000------------------------------------", // 0
	"--------------------------------------0000------------0000----------00--00----------00--00--------------00--------------00--------------00--------------00--------------00--------------00----------0000000000------0000000000----------------------------------", // 1
	"------------------------------------00000000--------00000000------00--------00----00--------00--------------00--------------00------00000000--------00000000------00--------------00--------------000000000000----000000000000----------------------------------", // 2
	"------------------------------------00000000--------00000000------00--------00----00--------00----------0000------------0000----------------00--------------00----00--------00----00--------00------00000000--------00000000------------------------------------", // 3
	"----------------------------------------00--------------00------------0000------------0000----------00--00----------00--00--------00----00--------00----00--------000000000000----000000000000----------00--------------00--------------------------------------", // 4
	"----------------------------------000000000000----000000000000----00--------------00--------------0000000000------0000000000----------------00--------------00----00--------00----00--------00------00000000--------00000000------------------------------------", // 5
	"------------------------------------00000000--------00000000------00--------------00--------------0000000000------0000000000------00--------00----00--------00----00--------00----00--------00------00000000--------00000000------------------------------------", // 6
	"----------------------------------000000000000----000000000000--------------00--------------00------------00--------------00------------00--------------00------------00--------------00--------------00--------------00----------------------------------------", // 7
	"------------------------------------00000000--------00000000------00--------00----00--------00------00000000--------00000000------00--------00----00--------00----00--------00----00--------00------00000000--------00000000------------------------------------", // 8
	"------------------------------------00000000--------00000000------00--------00----00--------00----00--------00----00--------00------0000000000------0000000000--------------00--------------00------00000000--------00000000------------------------------------", // 9
	"------------------------------------------------------------------------------------------------------00--------------00------------------------------------------------------------------------------00--------------00----------------------------------------", // :
	"----------------------------------------------------------------------00--------------00------------------------------------------------------------------------------00--------------00--------------00--------------00------------00--------------00----------", // ;
	"--------------------------------------------------------------------------00--------------00------------00--------------00------------00--------------00----------------00--------------00----------------00--------------00------------------------------------", // <
	"----------------------------------------------------------------------------------------------------0000000000------0000000000--------------------------------------0000000000------0000000000------------------------------------------------------------------", // =
	"----------------------------------------------------------------------00--------------00----------------00--------------00----------------00--------------00------------00--------------00------------00--------------00----------------------------------------", // >
	"------------------------------------00000000--------00000000------00--------00----00--------00------------00--------------00------------00--------------00----------------------------------------------00--------------00--------------------------------------", // ?
	"------------------------------------00000000--------00000000------00----00--00----00----00--00----00--00--0000----00--00--0000----00--00000000----00--00000000----00--------------00----------------00000000--------00000000------------------------------------", // @
	"------------------------------------00000000--------00000000------00--------00----00--------00----00--------00----00--------00----000000000000----000000000000----00--------00----00--------00----00--------00----00--------00----------------------------------", // A
	"----------------------------------0000000000------0000000000------00--------00----00--------00----0000000000------0000000000------00--------00----00--------00----00--------00----00--------00----0000000000------0000000000------------------------------------", // B
	"------------------------------------00000000--------00000000------00--------00----00--------00----00--------------00--------------00--------------00--------------00--------00----00--------00------00000000--------00000000------------------------------------", // C
	"----------------------------------00000000--------00000000--------00------00------00------00------00--------00----00--------00----00--------00----00--------00----00------00------00------00------00000000--------00000000--------------------------------------", // D
	"----------------------------------000000000000----000000000000----00--------------00--------------0000000000------0000000000------00--------------00--------------00--------------00--------------000000000000----000000000000----------------------------------", // E
	"----------------------------------000000000000----000000000000----00--------------00--------------0000000000------0000000000------00--------------00--------------00--------------00--------------00--------------00--------------------------------------------", // F
	"------------------------------------00000000--------00000000------00--------00----00--------00----00--------------00--------------00----000000----00----000000----00--------00----00--------00------00000000--------00000000------------------------------------", // G
	"----------------------------------00--------00----00--------00----00--------00----00--------00----000000000000----000000000000----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----------------------------------", // H
	"------------------------------------0000000000------0000000000----------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00----------0000000000------0000000000----------------------------------", // I
	"--------------------------------------------00--------------00--------------00--------------00--------------00--------------00----00--------00----00--------00----00--------00----00--------00------00000000--------00000000------------------------------------", // J
	"----------------------------------00------00------00------00------00----00--------00----00--------000000----------000000----------00----00--------00----00--------00------00------00------00------00--------00----00--------00----------------------------------", // K
	"----------------------------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------000000000000----000000000000----------------------------------", // L
	"----------------------------------00--------00----00--------00----0000----0000----0000----0000----00--0000--00----00--0000--00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----------------------------------", // M
	"----------------------------------00--------00----00--------00----0000------00----0000------00----00--00----00----00--00----00----00----00--00----00----00--00----00------0000----00------0000----00--------00----00--------00----------------------------------", // N
	"------------------------------------00000000--------00000000------00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00------00000000--------00000000------------------------------------", // O
	"----------------------------------0000000000------0000000000------00--------00----00--------00----00--------00----00--------00----0000000000------0000000000------00--------------00--------------00--------------00--------------------------------------------", // P
	"------------------------------------00000000--------00000000------00--------00----00--------00----00--------00----00--------00----00--00----00----00--00----00----00----00--00----00----00--00------00000000--------00000000------------------------------------", // Q
	"----------------------------------0000000000------0000000000------00--------00----00--------00----00--------00----00--------00----0000000000------0000000000------00------00------00------00------00--------00----00--------00----------------------------------", // R
	"------------------------------------00000000--------00000000------00--------------00----------------00000000--------00000000----------------00--------------00----00--------00----00--------00------00000000--------00000000------------------------------------", // S
	"--------------------------------00000000000000--00000000000000--------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00----------------------------------------", // T
	"----------------------------------00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00------00000000--------00000000------------------------------------", // U
	"----------------------------------00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00------00----00--------00----00----------0000------------0000--------------------------------------", // V
	"----------------------------------00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--------00----00--0000--00----00--0000--00------00----00--------00----00------------------------------------", // W
	"----------------------------------00--------00----00--------00------00----00--------00----00----------0000------------0000------------0000------------0000----------00----00--------00----00------00--------00----00--------00----------------------------------", // X
	"--------------------------------00----------00--00----------00----00------00------00------00--------00--00----------00--00------------00--------------00--------------00--------------00--------------00--------------00----------------------------------------", // Y
	"----------------------------------000000000000----000000000000------------00--------------00------------00--------------00------------00--------------00------------00--------------00------------000000000000----000000000000----------------------------------", // Z
	"----------------------------------------000000----------000000----------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------000000----------000000----------------------------------", // [
	"------------------------------------------------------------------00--------------00----------------00--------------00----------------00--------------00----------------00--------------00----------------00--------------00------------------------------------", // '\'
	"----------------------------------000000----------000000--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00----------000000----------000000----------------------------------------", // ]
	"--------------------------------------00--------------00------------00--00----------00--00--------00------00------00------00------------------------------------------------------------------------------------------------------------------------------------", // ^
	"--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------00000000000000000000000000000000", // _
	"--------------------------------------00--------------00----------------00--------------00----------------------------------------------------------------------------------------------------------------------------------------------------------------------", // `
	"---------------------------------------------------------------------00000-----------00000----------------00--------------00--------00000000--------00000000------00------00------00------00--------00000000--------00000000------------------------------------", // a
	"------------------------------------00--------------00--------------00--------------00--------------00000000--------00000000--------00------00------00------00------00------00------00------00------00000000--------00000000------------------------------------", // b
	"----------------------------------------------------------------------000000----------000000--------00--------------00--------------00--------------00--------------00--------------00----------------000000----------000000------------------------------------", // c
	"------------------------------------------00--------------00--------------00--------------00--------00000000--------00000000------00------00------00------00------00------00------00------00--------00000000--------00000000------------------------------------", // d
	"------------------------------------------------------------------------------------000000--------00------00------00------00------00000000--------00000000--------00--------------00----------------00000000--------00000000------------------------------------", // d
	"----------------------------------------0000------------0000----------00--------------00--------------0000------------0000------------00--------------00--------------00--------------00--------------00--------------00----------------------------------------", // f
	"--------------------------------------------------------------------00000000--------00000000------00------00------00------00------00------00------00------00--------00000000--------00000000--------------00--------------00--------000000----------000000------", // g
	"----------------------------------00--------------00--------------00--------------00--------------00000000--------00000000--------00------00------00------00------00------00------00------00------00------00------00------00------------------------------------", // h
	"--------------------------------------00--------------00--------------------------------------------0000------------0000--------------00--------------00--------------00--------------00------------000000----------000000--------------------------------------", // i
	"------------------------------------------00--------------00----------------------------------------------00--------------00--------------00--------------00--------------00--------------00--------00----00--------00----00----------0000------------0000------", // j
	"------------------------------------00--------------00--------------00--00----------00--00----------0000------------0000------------0000------------0000------------00--00----------00--00----------00----00--------00----00------------------------------------", // k
	"--------------------------------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00----------------0000------------0000------------------------------------", // l
	"------------------------------------------------------------------0000--00--------0000--00--------00--00--00------00--00--00------00--00--00------00--00--00------00--00--00------00--00--00------00--00--00------00--00--00------------------------------------", // m
	"------------------------------------------------------------------00000000--------00000000--------00------00------00------00------00------00------00------00------00------00------00------00------00------00------00------00------------------------------------", // n
	"--------------------------------------------------------------------000000----------000000--------00------00------00------00------00------00------00------00------00------00------00------00--------000000----------000000--------------------------------------", // o
	"------------------------------------------------------------------00000000--------00000000--------00------00------00------00------00------00------00------00------00000000--------00000000--------00--------------00--------------00--------------00------------", // p
	"--------------------------------------------------------------------00000000--------00000000------00------00------00------00------00------00------00------00--------00000000--------00000000--------------00--------------00--------------0000------------0000--", // q
	"----------------------------------------------------------------------000000----------000000--------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00------------------------------------------", // r
	"--------------------------------------------------------------------000000----------000000--------00--------------00----------------000000----------000000----------------00--------------00------00000000--------00000000--------------------------------------", // s
	"--------------------------------------00--------------00------------000000----------000000------------00--------------00--------------00--------------00--------------00--------------00----------------0000------------0000------------------------------------", // t
	"------------------------------------------------------------------00------00------00------00------00------00------00------00------00------00------00------00------00------00------00------00--------000000----------000000--------------------------------------", // u
	"------------------------------------------------------------------00------00------00------00------00------00------00------00--------00--00----------00--00----------00--00----------00--00------------00--------------00----------------------------------------", // v
	"------------------------------------------------------------------00------00------00------00------00--00--00------00--00--00------00--00--00------00--00--00------00--00--00------00--00--00--------00--00----------00--00--------------------------------------", // w
	"------------------------------------------------------------------00------00------00------00--------00--00----------00--00------------00--------------00------------00--00----------00--00--------00------00------00------00------------------------------------", // x
	"------------------------------------------------------------------00------00------00------00------00------00------00------00------00------00------00------00--------00000000--------00000000--------------00--------------00--------000000----------000000------", // y
	"------------------------------------------------------------------0000000000------0000000000------------00--------------00------------00--------------00------------00--------------00------------0000000000------0000000000------------------------------------", // z
	"----------------------------------------000000----------000000----------00--------------00----------0000------------0000----------------00--------------00--------------00--------------00--------------000000----------000000----------------------------------", // {
	"----------------------------------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------00--------------------------------------", // |
	"----------------------------------000000----------000000--------------00--------------00----------------0000------------0000----------00--------------00--------------00--------------00----------000000----------000000----------------------------------------", // }
	"--------------------------------------00--00----------00--00--------00--00----------00--00----------------------------------------------------------------------------------------------------------------------------------------------------------------------"  // ~
};
/****************************************************************
Font data in array format. [charWidth x charHeight length array]
std::vector is sorted by ASCII codes from 32 (space) to  126 (~)

parameters:
	font[0][0] : characters Width
	font[0][1] : characters Height

example of data storage ('#' are the printed pixels of the char)
	--#--    00100
	-#-#- -> 01010 -> 00100 01010 01100 -> 001000101001100
	-##--    01100

Copyright (c) Andrea Marchi 2021
****************************************************************/


std::vector<std::string> IBM_EGA_8x14 = {
	{8, 14}, // width and height of character in pixels {charWidth, charHeight}
	"----------------------------------------------------------------------------------------------------------------", // <space>
	"-------------------00------00-----0000----0000----0000-----00------00--------------00------00-------------------", // !
	"---------00--00--00--00--00--00---0--0--------------------------------------------------------------------------", // "
	"-----------------00-00---00-00--0000000--00-00---00-00---00-00--0000000--00-00---00-00--------------------------", // #
	"---00------00----00000--00---00-00----0-00-------00000-------00-0----00-00---00--00000-----00------00-----------", // $
	"--------------------------------00----0-00---00-----00-----00-----00-----00--00-00---00-------------------------", // %
	"------------------000----00-00---00-00----000----000-00-00-000--00--00--00--00---000-00-------------------------", // &
	"----------00------00------00-----00-----------------------------------------------------------------------------", // '
	"--------------------00-----00-----00------00------00------00------00-------00-------00--------------------------", // (
	"------------------00-------00-------00------00------00------00------00-----00-----00----------------------------", // )
	"---------------------------------00--00---0000--00000000--0000---00--00-----------------------------------------", // *
	"-----------------------------------00------00----000000----00------00-------------------------------------------", // +
	"-------------------------------------------------------------------00------00------00-----00--------------------", // ,
	"------------------------------------------------0000000---------------------------------------------------------", // -
	"---------------------------------------------------------------------------00------00---------------------------", // .
	"----------------------0------00-----00-----00-----00-----00-----00------0---------------------------------------", // /
	"-----------------00000--00---00-00--000-00-0000-0000-00-000--00-00---00-00---00--00000--------------------------", // 0
	"-------------------00-----000----0000------00------00------00------00------00----000000-------------------------", // 1
	"-----------------00000--00---00------00-----00-----00-----00-----00-----00---00-0000000-------------------------", // 2
	"-----------------00000--00---00------00------00---0000-------00------00-00---00--00000--------------------------", // 3
	"--------------------00-----000----0000---00-00--00--00--0000000-----00------00-----0000-------------------------", // 4
	"----------------0000000-00------00------00------000000-------00------00-00---00--00000--------------------------", // 5
	"------------------000----00-----00------00------000000--00---00-00---00-00---00--00000--------------------------", // 6
	"----------------0000000-00---00------00-----00-----00-----00------00------00------00----------------------------", // 7
	"-----------------00000--00---00-00---00-00---00--00000--00---00-00---00-00---00--00000--------------------------", // 8
	"-----------------00000--00---00-00---00-00---00--000000------00------00-----00---0000---------------------------", // 9
	"---------------------------00------00------------------------------00------00-----------------------------------", // :
	"---------------------------00------00------------------------------00------00-----00----------------------------", // ;
	"---------------------00-----00-----00-----00-----00-------00-------00-------00-------00-------------------------", // <
	"-----------------------------------------000000------------------000000-----------------------------------------", // =
	"-----------------00-------00-------00-------00-------00-----00-----00-----00-----00-----------------------------", // >
	"-----------------00000--00---00-00---00-----00-----00------00--------------00------00---------------------------", // ?
	"-----------------00000--00---00-00---00-00-0000-00-0000-00-0000-00-000--00-------00000--------------------------", // @
	"-------------------0------000----00-00--00---00-00---00-0000000-00---00-00---00-00---00-------------------------", // A
	"----------------000000---00--00--00--00--00--00--00000---00--00--00--00--00--00-000000--------------------------", // B
	"------------------0000---00--00-00----0-00------00------00------00----0--00--00---0000--------------------------", // C
	"----------------00000----00-00---00--00--00--00--00--00--00--00--00--00--00-00--00000---------------------------", // D
	"----------------0000000--00--00--00---0--00-0----0000----00-0----00---0--00--00-0000000-------------------------", // E
	"----------------0000000--00--00--00---0--00-0----0000----00-0----00------00-----0000----------------------------", // F
	"------------------0000---00--00-00----0-00------00------00-0000-00---00--00--00---000-0-------------------------", // G
	"----------------00---00-00---00-00---00-00---00-0000000-00---00-00---00-00---00-00---00-------------------------", // H
	"------------------0000-----00------00------00------00------00------00------00-----0000--------------------------", // I
	"-------------------0000-----00------00------00------00------00--00--00--00--00---0000---------------------------", // J
	"----------------000--00--00--00--00-00---00-00---0000----00-00---00-00---00--00-000--00-------------------------", // K
	"----------------0000-----00------00------00------00------00------00---0--00--00-0000000-------------------------", // L
	"----------------00---00-000-000-0000000-0000000-00-0-00-00---00-00---00-00---00-00---00-------------------------", // M
	"----------------00---00-000--00-0000-00-0000000-00-0000-00--000-00---00-00---00-00---00-------------------------", // N
	"------------------000----00-00--00---00-00---00-00---00-00---00-00---00--00-00----000---------------------------", // O
	"----------------000000---00--00--00--00--00--00--00000---00------00------00-----0000----------------------------", // P
	"-----------------00000--00---00-00---00-00---00-00---00-00-0-00-00-0000--00000------00------000-----------------", // Q
	"----------------000000---00--00--00--00--00--00--00000---00-00---00--00--00--00-000--00-------------------------", // R
	"-----------------00000--00---00-00---00--00-------000-------00--00---00-00---00--00000--------------------------", // S
	"-----------------000000--000000--0-00-0----00------00------00------00------00-----0000--------------------------", // T
	"----------------00---00-00---00-00---00-00---00-00---00-00---00-00---00-00---00--00000--------------------------", // U
	"----------------00---00-00---00-00---00-00---00-00---00-00---00--00-00----000------0----------------------------", // V
	"------------------------00---00-00---00-00---00-00---00-00---00-00-0-00-0000000--00-00--------------------------", // W
	"----------------00---00-00---00--00-00----000-----000-----000----00-00--00---00-00---00-------------------------", // X
	"-----------------00--00--00--00--00--00--00--00---0000-----00------00------00-----0000--------------------------", // Y
	"----------------0000000-00---00-0---00-----00-----00-----00-----00----0-00---00-0000000-------------------------", // Z
	"------------------0000----00------00------00------00------00------00------00------0000--------------------------", // [
	"----------------0-------00------000------000------000------000------000------00-------0-------------------------", // '\'
	"------------------0000------00------00------00------00------00------00------00----0000--------------------------", // ]
	"---0------000----00-00--00---00---------------------------------------------------------------------------------", // ^
	"------------------------------------------------------------------------------------------------00000000--------", // _
	"--00------00-------00-------------------------------------------------------------------------------------------", // `
	"-----------------------------------------0000-------00---00000--00--00--00--00---000-00-------------------------", // a
	"----------------000------00------00------0000----00-00---00--00--00--00--00--00--00000--------------------------", // b
	"-----------------------------------------00000--00---00-00------00------00---00--00000--------------------------", // c
	"-------------------000------00------00----0000---00-00--00--00--00--00--00--00---000-00-------------------------", // d
	"-----------------------------------------00000--00---00-0000000-00------00---00--00000--------------------------", // d
	"------------------000----00-00---00--0---00-----0000-----00------00------00-----0000----------------------------", // f
	"-----------------------------------------000-00-00--00--00--00--00--00---00000------00--00--00---0000-----------", // g
	"----------------000------00------00------00-00---000-00--00--00--00--00--00--00-000--00-------------------------", // h
	"-------------------00------00-------------000------00------00------00------00-----0000--------------------------", // i
	"---------------------00------00-------------000------00------00------00------00--00--00--00--00---0000----------", // j
	"----------------000------00------00------00--00--00-00---0000----00-00---00--00-000--00-------------------------", // k
	"------------------000------00------00------00------00------00------00------00-----0000--------------------------", // l
	"----------------------------------------000-00--0000000-00-0-00-00-0-00-00-0-00-00---00-------------------------", // m
	"----------------------------------------00-000---00--00--00--00--00--00--00--00--00--00-------------------------", // n
	"-----------------------------------------00000--00---00-00---00-00---00-00---00--00000--------------------------", // o
	"----------------------------------------00-000---00--00--00--00--00--00--00000---00------00-----0000------------", // p
	"-----------------------------------------000-00-00--00--00--00--00--00---00000------00------00-----0000---------", // q
	"----------------------------------------00-000---000-00--00--00--00------00-----0000----------------------------", // r
	"-----------------------------------------00000--00---00--000-------000--00---00--00000--------------------------", // s
	"-------------------0------00------00----000000----00------00------00------00-00----000--------------------------", // t
	"----------------------------------------00--00--00--00--00--00--00--00--00--00---000-00-------------------------", // u
	"-----------------------------------------00--00--00--00--00--00--00--00---0000-----00---------------------------", // v
	"----------------------------------------00---00-00---00-00-0-00-00-0-00-0000000--00-00--------------------------", // w
	"----------------------------------------00---00--00-00----000-----000----00-00--00---00-------------------------", // x
	"----------------------------------------00---00-00---00-00---00-00---00--000000------00-----00--00000-----------", // y
	"----------------------------------------0000000-00--00-----00-----00-----00--00-0000000-------------------------", // z
	"--------------------000----00------00------00----000-------00------00------00-------000-------------------------", // {
	"-------------------00------00------00------00--------------00------00------00------00---------------------------", // |
	"-----------------000-------00------00------00-------000----00------00------00----000----------------------------", // }
	"-----------------000-00-00-000----------------------------------------------------------------------------------"  // ~
};
/****************************************************************
Font data in array format. [charWidth x charHeight length array]
std::vector is sorted by ASCII codes from 32 (space) to  126 (~)

parameters:
	font[0][0] : characters Width
	font[0][1] : characters Height

example of data storage ('#' are the printed pixels of the char)
	--#--    00100
	-#-#- -> 01010 -> 00100 01010 01100 -> 001000101001100
	-##--    01100

Copyright (c) Andrea Marchi 2021
****************************************************************/


std::vector<std::string> Portfolio_6x8 = { // licensed by Atari (c)
	{6, 8}, // width and height of character in pixels {charWidth, charHeight}
	"------------------------------------------------", // <space>
	"--0-----0-----0-----0-----0-----------0---------", // !
	"-0-0---0-0---0-0--------------------------------", // "
	"-0-0---0-0--00000--0-0--00000--0-0---0-0--------", // #
	"--0----0000-0------000------0-0000----0---------", // $
	"00----00--0----0----0----0----0--00----00-------", // %
	"-00---0--0--0-0----0----0-0-0-0--0---00-0-------", // &
	"--00-----0----0---------------------------------", // '
	"---0----0----0-----0-----0------0------0--------", // (
	"-0------0------0-----0-----0----0----0----------", // )
	"--------0---0-0-0--000--0-0-0---0---------------", // *
	"--------0-----0---00000---0-----0---------------", // +
	"--------------------------00-----0----0---------", // ,
	"------------------00000-------------------------", // -
	"--------------------------------00----00--------", // .
	"----------0----0----0----0----0-----------------", // /
	"-000--0---0-0--00-0-0-0-00--0-0---0--000--------", // 0
	"--0----00-----0-----0-----0-----0----000--------", // 1
	"-000--0---0-----0----0----0----0----00000-------", // 2
	"00000----0----0------0------0-0---0--000--------", // 3
	"---0----00---0-0--0--0--00000----0-----0--------", // 4
	"00000-0-----0000------0-----0-0---0--000--------", // 5
	"--00---0----0-----0000--0---0-0---0--000--------", // 6
	"00000-----0----0----0-----0-----0-----0---------", // 7
	"-000--0---0-0---0--000--0---0-0---0--000--------", // 8
	"-000--0---0-0---0--0000-----0----0---00---------", // 9
	"--------00----00----------00----00--------------", // :
	"--------00----00----------00-----0----0---------", // ;
	"----0----0----0----0------0------0------0-------", // <
	"------------00000-------00000-------------------", // =
	"0------0------0------0----0----0----0-----------", // >
	"-000--0---0-----0----0----0-----------0---------", // ?
	"-000--0---0-0--00-0-0-0-0--0--0------000--------", // @
	"-000--0---0-0---0-0---0-00000-0---0-0---0-------", // A
	"0000--0---0-0---0-0000--0---0-0---0-0000--------", // B
	"-000--0---0-0-----0-----0-----0---0--000--------", // C
	"000---0--0--0---0-0---0-0---0-0--0--000---------", // D
	"00000-0-----0-----0000--0-----0-----00000-------", // E
	"00000-0-----0-----0000--0-----0-----0-----------", // F
	"-000--0---0-0-----0-----0-000-0---0--0000-------", // G
	"0---0-0---0-0---0-00000-0---0-0---0-0---0-------", // H
	"-000----0-----0-----0-----0-----0----000--------", // I
	"--000----0-----0-----0-----0--0--0---00---------", // J
	"0---0-0--0--0-0---00----0-0---0--0--0---0-------", // K
	"0-----0-----0-----0-----0-----0-----00000-------", // L
	"0---0-00-00-0-0-0-0-0-0-0---0-0---0-0---0-------", // M
	"0---0-0---0-00--0-0-0-0-0--00-0---0-0---0-------", // N
	"-000--0---0-0---0-0---0-0---0-0---0--000--------", // O
	"0000--0---0-0---0-0000--0-----0-----0-----------", // P
	"-000--0---0-0---0-0---0-0-0-0-0--0---00-0-------", // Q
	"0000--0---0-0---0-0000--0-0---0--0--0---0-------", // R
	"-000--0---0-0------000------0-0---0--000--------", // S
	"00000---0-----0-----0-----0-----0-----0---------", // T
	"0---0-0---0-0---0-0---0-0---0-0---0--000--------", // U
	"0---0-0---0-0---0-0---0-0---0--0-0----0---------", // V
	"0---0-0---0-0---0-0-0-0-0-0-0-00-00-0---0-------", // W
	"0---0-0---0--0-0----0----0-0--0---0-0---0-------", // X
	"0---0-0---0-0---0--000----0-----0-----0---------", // Y
	"00000-----0----0----0----0----0-----00000-------", // Z
	"-000---0-----0-----0-----0-----0-----000--------", // [
	"------0------0------0------0------0-------------", // '\'
	"-000-----0-----0-----0-----0-----0---000--------", // ]
	"--0----0-0--0---0-------------------------------", // ^
	"------------------------------------------000000", // _
	"-00----0------0---------------------------------", // `
	"-------------000------0--0000-0---0--0000-------", // a
	"0-----0-----0-00--00--0-0---0-0---0-0000--------", // b
	"-------------000--0-----0-----0---0--000--------", // c
	"----0-----0--00-0-0--00-0---0-0---0--0000-------", // d
	"-------------000--0---0-00000-0------0000-------", // d
	"--00---0--0--0----0000---0-----0-----0----------", // f
	"-------------0000-0---0--0000-----0--000--------", // g
	"0-----0-----0-00--00--0-0---0-0---0-0---0-------", // h
	"--0----------00-----0-----0-----0----000--------", // i
	"---0----------00-----0-----0--0--0---00---------", // j
	"0-----0-----0---0-0--0--0-0---00-0--0---0-------", // k
	"-00-----0-----0-----0-----0-----0----000--------", // l
	"------------00-0--0-0-0-0-0-0-0---0-0---0-------", // m
	"------------0-00--00--0-0---0-0---0-0---0-------", // n
	"-------------000--0---0-0---0-0---0--000--------", // o
	"------------0000--0---0-0000--0-----0-----------", // p
	"-------------0000-0---0--0000-----0-----0-------", // q
	"------------0-00--00--0-0-----0-----0-----------", // r
	"-------------0000-0------000------0-0000--------", // s
	"-0-----0----0000---0-----0-----0--0---00--------", // t
	"------------0---0-0---0-0---0-0--00--00-0-------", // u
	"------------0---0-0---0-0---0--0-0----0---------", // v
	"------------0---0-0---0-0-0-0-0-0-0--0-0--------", // w
	"------------0---0--0-0----0----0-0--0---0-------", // x
	"------------0---0-0---0--0000-----0-0000--------", // y
	"------------00000----0----0----0----00000-------", // z
	"---00---0-----0----00-----0-----0------00-------", // {
	"--0-----0-----0-----------0-----0-----0---------", // |
	"00------0-----0-----00----0-----0---00----------", // }
	"-00-0-0-00--------------------------------------"  // ~
};
/****************************************************************
Font data in array format. [charWidth x charHeight length array]
std::vector is sorted by ASCII codes from 32 (space) to  126 (~)

parameters:
	font[0][0] : characters Width
	font[0][1] : characters Height

example of data storage ('#' are the printed pixels of the char)
	--#--    --0--
	-#-#- -> -0-0- -> --0-- -0-0- -00-- -> --0---0-0--00--
	-##--    -00--

Copyright (c) Andrea Marchi 2021
****************************************************************/


std::vector<std::string> Morse_font = {
	{8, 16}, // width and height of character in pixels {charWidth, charHeight}
	"--------------------------------------------------------------------------------------------------------------------------------", // <space>
	"-------------------00-----0000----0000----0000-----00------00------00--------------00------00-----------------------------------", // !
	"---------00--00--00--00--00--00---0--0------------------------------------------------------------------------------------------", // "
	"-------------------------00-00---00-00--0000000--00-00---00-00---00-00--0000000--00-00---00-00----------------------------------", // #
	"---00------00----00000--00---00-00----0-00-------00000-------00------00-0----00-00---00--00000-----00------00-------------------", // $
	"--------------------------------00----0-00---00-----00-----00-----00-----00-----00---00-0----00---------------------------------", // %
	"------------------000----00-00---00-00----000----000-00-00-000--00--00--00--00--00--00---000-00---------------------------------", // &
	"----------00------00------00-----00---------------------------------------------------------------------------------------------", // '
	"--------------------00-----00-----00------00------00------00------00------00-------00-------00----------------------------------", // (
	"------------------00-------00-------00------00------00------00------00------00-----00-----00------------------------------------", // )
	"-----------------------------------------00--00---0000--00000000--0000---00--00-------------------------------------------------", // *
	"-------------------------------------------00------00----000000----00------00---------------------------------------------------", // +
	"---------------------------------------------------------------------------00------00------00-----00----------------------------", // ,
	"--------------------------------------------------------0000000-----------------------------------------------------------------", // -
	"-----------------------------------------------------------------------------------00------00-----------------------------------", // .
	"--------------------------------------0------00-----00-----00-----00-----00-----00------0---------------------------------------", // /
	"--------0000000-0000000---------0000000-0000000---------0000000-0000000---------0000000-0000000---------0000000-0000000---------", // 0
	"----------000-----000-----------0000000-0000000---------0000000-0000000---------0000000-0000000---------0000000-0000000---------", // 1
	"----------000-----000-------------000-----000-----------0000000-0000000---------0000000-0000000---------0000000-0000000---------", // 2
	"----------000-----000-------------000-----000-------------000-----000-----------0000000-0000000---------0000000-0000000---------", // 3
	"----------000-----000-------------000-----000-------------000-----000-------------000-----000-----------0000000-0000000---------", // 4
	"----------000-----000-------------000-----000-------------000-----000-------------000-----000-------------000-----000-----------", // 5
	"--------0000000-0000000-----------000-----000-------------000-----000-------------000-----000-------------000-----000-----------", // 6
	"--------0000000-0000000---------0000000-0000000-----------000-----000-------------000-----000-------------000-----000-----------", // 7
	"--------0000000-0000000---------0000000-0000000---------0000000-0000000-----------000-----000-------------000-----000-----------", // 8
	"--------0000000-0000000---------0000000-0000000---------0000000-0000000---------0000000-0000000-----------000-----000-----------", // 9
	"-----------------------------------00------00------------------------------00------00-------------------------------------------", // :
	"-----------------------------------00------00------------------------------00------00-----00------------------------------------", // ;
	"-----------------------------00-----00-----00-----00-----00-------00-------00-------00-------00---------------------------------", // <
	"-----------------------------------------000000------------------000000---------------------------------------------------------", // =
	"-------------------------00-------00-------00-------00-------00-----00-----00-----00-----00-------------------------------------", // >
	"-----------------00000--00---00-00---00-----00-----00------00------00--------------00------00-----------------------------------", // ?
	"-------------------------00000--00---00-00---00-00-0000-00-0000-00-0000-00-000--00-------00000----------------------------------", // @
	"----------000-----000-----------0000000-0000000---------------------------------------------------------------------------------", // A
	"--------0000000-0000000-----------000-----000-------------000-----000-------------000-----000-----------------------------------", // B
	"--------0000000-0000000-----------000-----000-----------0000000-0000000-----------000-----000-----------------------------------", // C
	"--------0000000-0000000-----------000-----000-------------000-----000-----------------------------------------------------------", // D
	"----------000-----000-----------------------------------------------------------------------------------------------------------", // E
	"----------000-----000-------------000-----000-----------0000000-0000000-----------000-----000-----------------------------------", // F
	"--------0000000-0000000---------0000000-0000000-----------000-----000-----------------------------------------------------------", // G
	"------------------000-----000-------------000-----000-------------000-----000-------------000-----000---------------------------", // H
	"----------000-----000-------------000-----000-----------------------------------------------------------------------------------", // I
	"----------000-----000-----------0000000-0000000---------0000000-0000000---------0000000-0000000---------------------------------", // J
	"--------0000000-0000000-----------000-----000-----------0000000-0000000---------------------------------------------------------", // K
	"----------000-----000-----------0000000-0000000-----------000-----000-------------000-----000-----------------------------------", // L
	"--------0000000-0000000---------0000000-0000000---------------------------------------------------------------------------------", // M
	"--------0000000-0000000-----------000-----000-----------------------------------------------------------------------------------", // N
	"--------0000000-0000000---------0000000-0000000---------0000000-0000000---------------------------------------------------------", // O
	"----------000-----000-----------0000000-0000000---------0000000-0000000-----------000-----000-----------------------------------", // P
	"--------0000000-0000000---------0000000-0000000-----------000-----000-----------0000000-0000000---------------------------------", // Q
	"----------000-----000-----------0000000-0000000-----------000-----000-----------------------------------------------------------", // R
	"----------000-----000-------------000-----000-------------000-----000-----------------------------------------------------------", // S
	"--------0000000-0000000---------------------------------------------------------------------------------------------------------", // T
	"----------000-----000-------------000-----000-----------0000000-0000000---------------------------------------------------------", // U
	"----------000-----000-------------000-----000-------------000-----000-----------0000000-0000000---------------------------------", // V
	"----------000-----000-----------0000000-0000000---------0000000-0000000---------------------------------------------------------", // W
	"--------0000000-0000000-----------000-----000-------------000-----000-----------0000000-0000000---------------------------------", // X
	"--------0000000-0000000-----------000-----000-----------0000000-0000000---------0000000-0000000---------------------------------", // Y
	"--------0000000-0000000---------0000000-0000000-----------000-----000-------------000-----000-----------------------------------", // Z
	"------------------0000----00------00------00------00------00------00------00------00------0000----------------------------------", // [
	"------------------------0-------00------000------000------000------000------000------00-------0---------------------------------", // '\'
	"------------------0000------00------00------00------00------00------00------00------00----0000----------------------------------", // ]
	"---0------000----00-00--00---00-------------------------------------------------------------------------------------------------", // ^
	"--------------------------------------------------------------------------------------------------------00000000----------------", // _
	"--00------00-------00-----------------------------------------------------------------------------------------------------------", // `
	"----------000-----000-----------0000000-0000000---------------------------------------------------------------------------------", // a
	"--------0000000-0000000-----------000-----000-------------000-----000-------------000-----000-----------------------------------", // b
	"--------0000000-0000000-----------000-----000-----------0000000-0000000-----------000-----000-----------------------------------", // c
	"--------0000000-0000000-----------000-----000-------------000-----000-----------------------------------------------------------", // d
	"----------000-----000-----------------------------------------------------------------------------------------------------------", // d
	"----------000-----000-------------000-----000-----------0000000-0000000-----------000-----000-----------------------------------", // f
	"--------0000000-0000000---------0000000-0000000-----------000-----000-----------------------------------------------------------", // g
	"----------000-----000-------------000-----000-------------000-----000-------------000-----000-----------------------------------", // h
	"----------000-----000-------------000-----000-----------------------------------------------------------------------------------", // i
	"----------000-----000-----------0000000-0000000---------0000000-0000000---------0000000-0000000---------------------------------", // j
	"--------0000000-0000000-----------000-----000-----------0000000-0000000-----------000-----000-----------------------------------", // k
	"----------000-----000-----------0000000-0000000-----------000-----000-------------000-----000-----------------------------------", // l
	"--------0000000-0000000---------0000000-0000000---------------------------------------------------------------------------------", // m
	"--------0000000-0000000-----------000-----000-----------------------------------------------------------------------------------", // n
	"--------0000000-0000000---------0000000-0000000---------0000000-0000000---------------------------------------------------------", // o
	"----------000-----000-----------0000000-0000000---------0000000-0000000-----------000-----000-----------------------------------", // p
	"--------0000000-0000000---------0000000-0000000-----------000-----000-----------0000000-0000000---------------------------------", // q
	"----------000-----000-----------0000000-0000000-----------000-----000-----------------------------------------------------------", // r
	"----------000-----000-------------000-----000-------------000-----000-----------------------------------------------------------", // s
	"--------0000000-0000000---------------------------------------------------------------------------------------------------------", // t
	"----------000-----000-------------000-----000-----------0000000-0000000---------------------------------------------------------", // u
	"----------000-----000-------------000-----000-------------000-----000-----------0000000-0000000---------------------------------", // v
	"----------000-----000-----------0000000-0000000---------0000000-0000000---------------------------------------------------------", // w
	"--------0000000-0000000-----------000-----000-------------000-----000-----------0000000-0000000---------------------------------", // x
	"--------0000000-0000000-----------000-----000-----------0000000-0000000---------0000000-0000000---------------------------------", // y
	"--------0000000-0000000---------0000000-0000000-----------000-----000-------------000-----000-----------------------------------", // z
	"--------------------000----00------00------00----000-------00------00------00------00-------000---------------------------------", // {
	"-------------------00------00------00------00--------------00------00------00------00------00-----------------------------------", // |
	"-----------------000-------00------00------00-------000----00------00------00------00----000------------------------------------", // }
	"-----------------000-00-00-000--------------------------------------------------------------------------------------------------"  // ~
};
/****************************************************************
Font data in array format. [charWidth x charHeight length array]
std::vector is sorted by ASCII codes from 32 (space) to  126 (~)

parameters:
	font[0][0] : characters Width
	font[0][1] : characters Height

example of data storage ('#' are the printed pixels of the char)
	--#--    --0--
	-#-#- -> -0-0- -> --0-- -0-0- -00-- -> --0---0-0--00--
	-##--    -00--

Copyright (c) Andrea Marchi 2021
****************************************************************/


std::vector<std::string> ISO_font = {
	{8, 16}, // width and height of character in pixels {charWidth, charHeight}
	"--------------------------------------------------------------------------------------------------------------------------------", // <space>
	"-------------------0-------0-------0-------0-------0-------0-------0---------------0-------0------------------------------------", // !
	"---------0--0----0--0----0--0----0--0-------------------------------------------------------------------------------------------", // "
	"-------------------------0---0---0---0--0000000--0---0---0---0---0---0--0000000--0---0---0---0----------------------------------", // #
	"-----------0-------0-----00000--0--0--0-0--0----0--------00000--------0----0--0-0--0--0--00000-----0-------0--------------------", // $
	"-------------------------00-----0000--0--00--0------0------0------0------0--00--0--0000-----00----------------------------------", // %
	"------------------00-----0--0----0--0----0--0-----00--0--0--0-0-0----0--0----0--0----0---0000-0---------------------------------", // &
	"-----------0-------0-------0------0---------------------------------------------------------------------------------------------", // '
	"--------------------0------0------0-------0-------0-------0-------0-------0--------0--------0-----------------------------------", // (
	"------------------0--------0--------0-------0-------0-------0-------0-------0------0------0-------------------------------------", // )
	"-----------------------------------0-----0-0-0----000---0000000---000----0-0-0-----0--------------------------------------------", // *
	"-----------------------------------0-------0-------0----0000000----0-------0-------0--------------------------------------------", // +
	"-----------------------------------------------------------------------------------0-------0-------0------0---------------------", // ,
	"--------------------------------------------------------0000000-----------------------------------------------------------------", // -
	"----------------------------------------------------------------------------------00------00------------------------------------", // .
	"----------------------------------------------0------0------0------0------0------0------0---------------------------------------", // /
	"------------------000----0---0--0-----0-0-----0-0--0--0-0--0--0-0-----0-0-----0--0---0----000-----------------------------------", // 0
	"-------------------0------00-----0-0-------0-------0-------0-------0-------0-------0-----00000----------------------------------", // 1
	"-----------------00000--0-----0-------0-------0------0------0------0------0------0------0000000---------------------------------", // 2
	"-----------------00000--0-----0-------0-------0---0000--------0-------0-------0-0-----0--00000----------------------------------", // 3
	"---------------------0------00-----0-0----0--0---0---0--0----0--0000000------0-------0-------0----------------------------------", // 4
	"----------------0000000-0-------0-------0-------000000--0-----0-------0-------0-0-----0--00000----------------------------------", // 5
	"-----------------00000--0-------0-------0-------000000--0-----0-0-----0-0-----0-0-----0--00000----------------------------------", // 6
	"----------------0000000-------0-------0------0------0------0-------0-------0-------0-------0------------------------------------", // 7
	"-----------------00000--0-----0-0-----0--0---0---00000--0-----0-0-----0-0-----0-0-----0--00000----------------------------------", // 8
	"-----------------00000--0-----0-0-----0-0-----0-0-----0--000000-------0-------0-------0--00000----------------------------------", // 9
	"-------------------------------------------0-------0-------------------------------0-------0------------------------------------", // :
	"-------------------------------------------0-------0-------------------------------0-------0-------0------0---------------------", // ;
	"-----------------------------0------0------0------0------0--------0--------0--------0--------0----------------------------------", // <
	"----------------------------------------0000000-----------------0000000---------------------------------------------------------", // =
	"-------------------------0--------0--------0--------0--------0------0------0------0------0--------------------------------------", // >
	"-----------------00000--0-----0-0-----0-------0-----00-----0-------0---------------0-------0------------------------------------", // ?
	"-----------------00000--0-----0-0-----0-0-000-0-0-0-0-0-0-0-0-0-0--000--0-------0--------00000----------------------------------", // @
	"------------------000----0---0--0-----0-0-----0-0-----0-0000000-0-----0-0-----0-0-----0-0-----0---------------------------------", // A
	"----------------000000--0-----0-0-----0-0-----0-000000--0-----0-0-----0-0-----0-0-----0-000000----------------------------------", // B
	"-----------------00000--0-----0-0-------0-------0-------0-------0-------0-----0-0-----0--00000----------------------------------", // C
	"----------------00000---0----0--0-----0-0-----0-0-----0-0-----0-0-----0-0-----0-0----0--00000-----------------------------------", // D
	"----------------0000000-0-------0-------0-------000000--0-------0-------0-------0-------0000000---------------------------------", // E
	"----------------0000000-0-------0-------0-------000000--0-------0-------0-------0-------0---------------------------------------", // F
	"-----------------00000--0-----0-0-------0-------0--0000-0-----0-0-----0-0-----0-0-----0--000000---------------------------------", // G
	"----------------0-----0-0-----0-0-----0-0-----0-0000000-0-----0-0-----0-0-----0-0-----0-0-----0---------------------------------", // H
	"-----------------00000-----0-------0-------0-------0-------0-------0-------0-------0-----00000----------------------------------", // I
	"------------------00000------0-------0-------0-------0-------0-------0-------0--0----0---0000-----------------------------------", // J
	"----------------0----0--0---0---0--0----0-0-----00------0-0-----0--0----0---0---0----0--0-----0---------------------------------", // K
	"----------------0-------0-------0-------0-------0-------0-------0-------0-------0-------0000000---------------------------------", // L
	"----------------0-----0-00---00-0-0-0-0-0--0--0-0--0--0-0-----0-0-----0-0-----0-0-----0-0-----0---------------------------------", // M
	"----------------0-----0-00----0-0-0---0-0--0--0-0---0-0-0----00-0-----0-0-----0-0-----0-0-----0---------------------------------", // N
	"-----------------00000--0-----0-0-----0-0-----0-0-----0-0-----0-0-----0-0-----0-0-----0--00000----------------------------------", // O
	"----------------000000--0-----0-0-----0-0-----0-000000--0-------0-------0-------0-------0---------------------------------------", // P
	"-----------------00000--0-----0-0-----0-0-----0-0-----0-0-----0-0--0--0-0--0--0-0--0--0--00000------0--------0------------------", // Q
	"----------------000000--0-----0-0-----0-0-----0-000000--0----0--0-----0-0-----0-0-----0-0-----0---------------------------------", // R
	"-----------------00000--0-----0-0--------00-------00--------00--------0-0-----0-0-----0--00000----------------------------------", // S
	"----------------0000000----0-------0-------0-------0-------0-------0-------0-------0-------0------------------------------------", // T
	"----------------0-----0-0-----0-0-----0-0-----0-0-----0-0-----0-0-----0-0-----0-0-----0--00000----------------------------------", // U
	"----------------0-----0-0-----0-0-----0-0-----0-0-----0-0-----0-0-----0--0---0----0-0------0------------------------------------", // V
	"----------------0-----0-0-----0-0-----0-0-----0-0-----0-0--0--0-0--0--0-0--0--0-0-0-0-0--0---0----------------------------------", // W
	"----------------0-----0-0-----0--0---0----0-0------0------0-0----0---0--0-----0-0-----0-0-----0---------------------------------", // X
	"----------------0-----0-0-----0-0-----0--0---0----0-0------0-------0-------0-------0-------0------------------------------------", // Y
	"----------------0000000-------0------0------0------0------0------0------0-------0-------0000000---------------------------------", // Z
	"------------------000-----0-------0-------0-------0-------0-------0-------0-------0-------000-----------------------------------", // [
	"----------------------------------------0--------0--------0--------0--------0--------0--------0---------------------------------", // '\'
	"------------------000-------0-------0-------0-------0-------0-------0-------0-------0-----000-----------------------------------", // ]
	"---0------0-0----0---0--0-----0-------------------------------------------------------------------------------------------------", // ^
	"--------------------------------------------------------------------------------------------------------0000000-----------------", // _
	"----------0-------0-------0--------0--------------------------------------------------------------------------------------------", // `
	"-----------------------------------------0000--------0-------0---00000--0----0--0----0---000000---------------------------------", // a
	"----------------0-------0-------0-------0-0000--00----0-0-----0-0-----0-0-----0-0-----0-000000----------------------------------", // b
	"-----------------------------------------00000--0-----0-0-------0-------0-------0-----0--00000----------------------------------", // c
	"----------------------0-------0-------0--0000-0-0----00-0-----0-0-----0-0-----0-0-----0--000000---------------------------------", // d
	"-----------------------------------------00000--0-----0-0-----0-0000000-0-------0--------00000----------------------------------", // d
	"-------------------000----0-------0-----00000-----0-------0-------0-------0-------0-------0-------------------------------------", // f
	"-----------------------------------------000000-0-----0-0-----0-0-----0-0-----0-0----00--0000-0-------0-------0--00000----------", // g
	"----------------0-------0-------0-------0-0000--00----0-0-----0-0-----0-0-----0-0-----0-0-----0---------------------------------", // h
	"-------------------0-------0-------------000-------0-------0-------0-------0-------0-----00000----------------------------------", // i
	"--------------------0-------0-------------000-------0-------0-------0-------0-------0-------0-------0-------0----000------------", // j
	"----------------0-------0-------0-------0----00-0--00---000-----0--0----0---0---0----0--0-----0---------------------------------", // k
	"-----------------00-------0-------0-------0-------0-------0-------0-------0-------0--------000----------------------------------", // l
	"----------------------------------------000-00--0--0--0-0--0--0-0--0--0-0--0--0-0--0--0-0--0--0---------------------------------", // m
	"----------------------------------------0-0000--00----0-0-----0-0-----0-0-----0-0-----0-0-----0---------------------------------", // n
	"-----------------------------------------00000--0-----0-0-----0-0-----0-0-----0-0-----0--00000----------------------------------", // o
	"----------------------------------------0-0000--00----0-0-----0-0-----0-0-----0-00----0-0-0000--0-------0-------0---------------", // p
	"-----------------------------------------000000-0-----0-0-----0-0-----0-0-----0-0----00--0000-0-------0-------0-------0---------", // q
	"----------------------------------------0-0000--00----0-0-------0-------0-------0-------0---------------------------------------", // r
	"-----------------------------------------000000-0-------0--------00000--------0-------0-000000----------------------------------", // s
	"--------------------------0-------0-----000000----0-------0-------0-------0-------0--------000----------------------------------", // t
	"----------------------------------------0-----0-0-----0-0-----0-0-----0-0-----0-0----00--0000-0---------------------------------", // u
	"----------------------------------------0-----0-0-----0-0-----0-0-----0--0---0----0-0------0------------------------------------", // v
	"----------------------------------------0-----0-0-----0-0-----0-0--0--0-0--0--0-0--0--0--00-00----------------------------------", // w
	"----------------------------------------0-----0--0---0----0-0------0------0-0----0---0--0-----0---------------------------------", // x
	"----------------------------------------0-----0-0-----0-0-----0-0-----0-0-----0-0----00--0000-0-------0-------0--00000----------", // y
	"----------------------------------------0000000------0------0------0------0------0------0000000---------------------------------", // z
	"--------------------00-----0-------0-------0-----00--------0-------0-------0-------0--------00----------------------------------", // {
	"-----------0-------0-------0-------0-------0-------0-------0-------0-------0-------0-------0-------0-------0-------0------------", // |
	"-----------------00--------0-------0-------0--------00-----0-------0-------0-------0-----00-------------------------------------", // }
	"-----------------00---0-0--0--0-0---00------------------------------------------------------------------------------------------"  // ~
};
/****************************************************************
Font data in array format. [charWidth x charHeight length array]
std::vector is sorted by ASCII codes from 32 (space) to  126 (~)

parameters:
	font[0][0] : characters Width
	font[0][1] : characters Height

example of data storage ('#' are the printed pixels of the char)
	--#--    --0--
	-#-#- -> -0-0- -> --0-- -0-0- -00-- -> --0---0-0--00--
	-##--    -00--

Copyright (c) Andrea Marchi 2021
****************************************************************/


std::vector<std::string> PS2THIN4 = {
	{8, 16}, // width and height of character in pixels {charWidth, charHeight}
	"--------------------------------------------------------------------------------------------------------------------------------", // <space>
	"-------------------0-------0-------0-------0-------0-------0-------0---------------0-------0------------------------------------", // !
	"------------------0--0----0--0----0--0----0--0----------------------------------------------------------------------------------", // "
	"-------------------------0---0---0---0--0000000--0---0---0---0---0---0--0000000--0---0---0---0----------------------------------", // #
	"-------------------0-----00000--0--0--0-0--0----0--0-----00000-----0--0----0--0-0--0--0--00000-----0----------------------------", // $
	"-------------------------00---0-0--0--0--00--0------0------0------0------0--00--0--0--0-0---00----------------------------------", // %
	"--------------------------000----0---0---0---0----000----0-0--0-0---00--0----0--0----0---0000-0---------------------------------", // &
	"------------------00-------0-------0------0-------------------------------------------------------------------------------------", // '
	"--------------------0------0------0-------0-------0-------0-------0-------0--------0--------0-----------------------------------", // (
	"------------------0--------0--------0-------0-------0-------0-------0-------0------0------0-------------------------------------", // )
	"---------------------------0-----0-0-0----000---0000000---000----0-0-0-----0----------------------------------------------------", // *
	"---------------------------0-------0-------0----0000000----0-------0-------0----------------------------------------------------", // +
	"----------------------------------------------------------------------------------00------00-------0------0---------------------", // ,
	"-------------------------------------------------000000-------------------------------------------------------------------------", // -
	"-----------------------------------------------------------------------------------00------00-----------------------------------", // .
	"---------------------0-------0------0-------0------0-------0------0-------0------0-------0--------------------------------------", // /
	"------------------0000---0----0--0----0--0---00--0--0-0--0-0--0--00---0--0----0--0----0---0000----------------------------------", // 0
	"---------------------0------00-----0-0----0--0-------0-------0-------0-------0-------0-------0----------------------------------", // 1
	"------------------0000---0----0-------0-------0------0------0------0------0------0-------000000---------------------------------", // 2
	"------------------0000---0----0-------0-------0----000--------0-------0-------0--0----0---0000----------------------------------", // 3
	"---------------------0------00-----0-0----0--0---0---0--0----0--0000000------0-------0-------0----------------------------------", // 4
	"-----------------000000--0-------0-------0-------00000--------0-------0-------0--0----0---0000----------------------------------", // 5
	"------------------0000---0-------0-------0-------00000---0----0--0----0--0----0--0----0---0000----------------------------------", // 6
	"-----------------000000-------0-------0------0------0------0-------0-------0-------0-------0------------------------------------", // 7
	"------------------000----0---0--0-----0--0---0----000----0---0--0-----0-0-----0--0---0----000-----------------------------------", // 8
	"------------------0000---0----0--0----0--0----0--0----0---00000-------0-------0-------0---0000----------------------------------", // 9
	"-----------------------------------00------00----------------------00------00---------------------------------------------------", // :
	"---------------------------------------------------00------00----------------------00------00-------0------0--------------------", // ;
	"-----------------------------0------0------0------0------0--------0--------0--------0--------0----------------------------------", // <
	"-----------------------------------------000000------------------000000---------------------------------------------------------", // =
	"-------------------------0--------0--------0--------0--------0------0------0------0------0--------------------------------------", // >
	"------------------0000---0----0-------0-------0------0------0------0---------------0-------0------------------------------------", // ?
	"-------------------------00000--0-----0-0-000-0-0-0-0-0-0-0-0-0-0-0-0-0-0--000--0--------00000----------------------------------", // @
	"-------------------00-----0--0---0----0--0----0--0----0--000000--0----0--0----0--0----0--0----0---------------------------------", // A
	"-----------------00000---0----0--0----0--0----0--00000---0----0--0----0--0----0--0----0--00000----------------------------------", // B
	"------------------0000---0----0--0-------0-------0-------0-------0-------0-------0----0---0000----------------------------------", // C
	"-----------------0000----0---0---0----0--0----0--0----0--0----0--0----0--0----0--0---0---0000-----------------------------------", // D
	"-----------------000000--0-------0-------0-------00000---0-------0-------0-------0-------000000---------------------------------", // E
	"-----------------000000--0-------0-------0-------00000---0-------0-------0-------0-------0--------------------------------------", // F
	"------------------00000--0-------0-------0-------0--000--0----0--0----0--0----0--0----0---00000---------------------------------", // G
	"-----------------0----0--0----0--0----0--0----0--000000--0----0--0----0--0----0--0----0--0----0---------------------------------", // H
	"-------------------000------0-------0-------0-------0-------0-------0-------0-------0------000----------------------------------", // I
	"----------------------0-------0-------0-------0-------0-------0-------0--0----0--0----0---0000----------------------------------", // J
	"-----------------0----0--0---0---0--0----0-0-----00------00------0-0-----0--0----0---0---0----0---------------------------------", // K
	"-----------------0-------0-------0-------0-------0-------0-------0-------0-------0-------000000---------------------------------", // L
	"----------------0-----0-00---00-0-0-0-0-0--0--0-0-----0-0-----0-0-----0-0-----0-0-----0-0-----0---------------------------------", // M
	"----------------0-----0-00----0-0-0---0-0-0---0-0--0--0-0--0--0-0---0-0-0---0-0-0----00-0-----0---------------------------------", // N
	"------------------0000---0----0--0----0--0----0--0----0--0----0--0----0--0----0--0----0---0000----------------------------------", // O
	"-----------------00000---0----0--0----0--0----0--0----0--00000---0-------0-------0-------0--------------------------------------", // P
	"------------------0000---0----0--0----0--0----0--0----0--0----0--0----0--0-0--0--0--0-0---0000-------0--------0-----------------", // Q
	"-----------------00000---0----0--0----0--0----0--0----0--00000---0--0----0---0---0----0--0----0---------------------------------", // R
	"------------------00000--0-------0-------0--------0000--------0-------0-------0-------0--00000----------------------------------", // S
	"----------------0000000----0-------0-------0-------0-------0-------0-------0-------0-------0------------------------------------", // T
	"-----------------0----0--0----0--0----0--0----0--0----0--0----0--0----0--0----0--0----0---0000----------------------------------", // U
	"----------------0-----0-0-----0-0-----0-0-----0-0-----0-0-----0-0-----0--0---0----0-0------0------------------------------------", // V
	"----------------0-----0-0-----0-0-----0-0-----0-0-----0-0-----0-0--0--0-0-0-0-0-00---00-0-----0---------------------------------", // W
	"----------------0-----0-0-----0--0---0----0-0------0-------0------0-0----0---0--0-----0-0-----0---------------------------------", // X
	"----------------0-----0-0-----0-0-----0--0---0----0-0------0-------0-------0-------0-------0------------------------------------", // Y
	"----------------0000000-------0-------0------0------0------0------0------0------0-------0000000---------------------------------", // Z
	"------------------0000----0-------0-------0-------0-------0-------0-------0-------0-------0000----------------------------------", // [
	"-----------------0-------0--------0-------0--------0-------0--------0-------0--------0-------0----------------------------------", // '\'
	"------------------0000-------0-------0-------0-------0-------0-------0-------0-------0----0000----------------------------------", // ]
	"-------------------0------0-0----0---0------------------------------------------------------------------------------------------", // ^
	"------------------------------------------------------------------------------------------------00000000------------------------", // _
	"-------------------0-------0--------0-------------------------------------------------------------------------------------------", // `
	"------------------------------------------0000--------0-------0---00000--0----0--0----0---00000---------------------------------", // a
	"-----------------0-------0-------0-------00000---0----0--0----0--0----0--0----0--0----0--00000----------------------------------", // b
	"------------------------------------------00000--0-------0-------0-------0-------0--------00000---------------------------------", // c
	"----------------------0-------0-------0---00000--0----0--0----0--0----0--0----0--0----0---00000---------------------------------", // d
	"------------------------------------------0000---0----0--0----0--000000--0-------0--------00000---------------------------------", // d
	"--------------------000----0-------0-----000000----0-------0-------0-------0-------0-------0------------------------------------", // f
	"------------------------------------------0000---0----0--0----0--0----0--0----0--0----0---00000-------0-------0---0000----------", // g
	"-----------------0-------0-------0-------00000---0----0--0----0--0----0--0----0--0----0--0----0---------------------------------", // h
	"--------------------0-------0-------------000-------0-------0-------0-------0-------0-----00000---------------------------------", // i
	"---------------------0-------0-------------000-------0-------0-------0-------0-------0-------0-------0-------0---0000-----------", // j
	"-----------------0-------0-------0-------0----0--0---0---0--0----000-----0--0----0---0---0----0---------------------------------", // k
	"------------------000-------0-------0-------0-------0-------0-------0-------0-------0-----00000---------------------------------", // l
	"----------------------------------------000-00--0--0--0-0--0--0-0--0--0-0--0--0-0--0--0-0--0--0---------------------------------", // m
	"-----------------------------------------00000---0----0--0----0--0----0--0----0--0----0--0----0---------------------------------", // n
	"------------------------------------------0000---0----0--0----0--0----0--0----0--0----0---0000----------------------------------", // o
	"-----------------------------------------00000---0----0--0----0--0----0--0----0--0----0--00000---0-------0-------0--------------", // p
	"------------------------------------------00000--0----0--0----0--0----0--0----0--0----0---00000-------0-------0-------0---------", // q
	"-----------------------------------------0-0000--00------0-------0-------0-------0-------0--------------------------------------", // r
	"------------------------------------------00000--0-------0--------0000--------0-------0--00000----------------------------------", // s
	"---------------------------0-------0-----000000----0-------0-------0-------0-------0--------000---------------------------------", // t
	"-----------------------------------------0----0--0----0--0----0--0----0--0----0--0----0---00000---------------------------------", // u
	"-----------------------------------------0----0--0----0--0----0--0----0--0----0---0--0-----00-----------------------------------", // v
	"----------------------------------------0--0--0-0--0--0-0--0--0-0--0--0-0--0--0-0--0--0--00-00----------------------------------", // w
	"-----------------------------------------0----0--0----0---0--0-----00-----0--0---0----0--0----0---------------------------------", // x
	"-----------------------------------------0----0--0----0--0----0--0----0--0----0--0----0---00000-------0-------0---0000----------", // y
	"-----------------------------------------000000------0------0------0------0------0-------000000---------------------------------", // z
	"--------------------00-----0-------0-------0------0-------0--------0-------0-------0--------00----------------------------------", // {
	"-----------0-------0-------0-------0-------0-----------------------0-------0-------0-------0-------0----------------------------", // |
	"------------------00--------0-------0-------0--------0-------0------0-------0-------0-----00------------------------------------", // }
	"-------------------------------------------------00---0-0--0--0-0---00----------------------------------------------------------"  // ~
};


#endif // MRC_IMAGE_HPP

/*****************************************************
funzioni che potrei aggiungere:
 - path : come i path degli SVG, stesso formato.
 - fill : flood_fill da un punto dato un colore, e veder da solo i margini
 - dash : disegna una linea tratteggiata con pattern definibili da una stringa
 
*****************************************************/
