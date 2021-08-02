#ifndef MRC_PNG_IMAGE
#define MRC_PNG_IMAGE

#include "BasicImage.h"
#include "zlib.h"

#include <iostream> //TEMPORANEO, PER IL DEBUG
//#include <boost/crc> NON FUNZIONA (non ce l'ho)


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





void PNGimage::save_file(std::string fileName, int stride_bytes)
{
	std::ofstream file(fileName, std::ios::binary);
	
	// 1) writes file header:
	unsigned char sig[8] = { 0x89,'P','N','G',0x0D,0x0A,0x1A,0x0A }; // header starting combination (8-byte signature)
	write_to_stream(file, sig);
	
	// 2) data "chunks":
	
	// 2.1) Image Header chunk (13 data bytes total):
	std::vector<unsigned char> temp;
	temp.push_back((_width &0xFF000000)>>24); temp.push_back((_width &0x00FF0000)>>16); temp.push_back((_width &0x0000FF00)>>8); temp.push_back((_width &0x000000FF)); //image Width  (Big-Endian)
	temp.push_back((_height&0xFF000000)>>24); temp.push_back((_height&0x00FF0000)>>16); temp.push_back((_height&0x0000FF00)>>8); temp.push_back((_height&0x000000FF)); //image Height (Big-Endian)
	temp.push_back(8); // bit depth (1 byte, values 1, 2, 4, 8, or 16)
	temp.push_back(2); // color type (1 byte, values 0, 2, 3, 4, or 6)
	temp.push_back(0); // compression method (1 byte, value 0)
	temp.push_back(0); // filter method (1 byte, value 0)
	temp.push_back(0); // interlace method (1 byte, values 0 "no interlace" or 1 "Adam7 interlace")
	
	write_chunk(file, temp, "IHDR");
	
	// 2.2) Data chunks:
	temp.clear();
	write_chunk(file, {0x08, 0xD7, 0x63, 0xF8, 0xCF, 0xC0, 0x00, 0x00, 0x03, 0x01, 0x01, 0x00}, "IDAT"); //hardcoded red single pixel
	
	
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




#endif
