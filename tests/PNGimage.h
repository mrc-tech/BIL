#ifndef MRC_PNG_IMAGE
#define MRC_PNG_IMAGE

#include "BasicImage.h"
#include "zlib.h"





class PNGimage : public BasicImage
{
	public:
		using BasicImage::BasicImage; // inherit constructors (C++11)
		
		// save and load bitmap files
		void save_file(std::string fileName, int stride_bytes);
		void load_file(std::string fileName);
		
		
	private:
		//auxiliary functions:
		void encode_png_line(int y, int stride_bytes, int filter_type, signed char *line_buffer);
};


// CONSTANTS =================================================================================================================================



// DEFINITIONS ================================================================================================================================



//STBIWDEF unsigned char *stbi_write_png_to_mem(
//const unsigned char *pixels, // DATA
//int stride_bytes, // CHE COS'E`???
//int x, 			// width
//int y,			// height
//int n, 			// bytes_per_pixel
//int *out_len)

void PNGimage::save_file(std::string fileName, int stride_bytes)
{
	std::ofstream file(fileName, std::ios::binary);
	
	int stbi_write_png_compression_level = 8; // NON CAMBIA????
	int stbi_write_force_png_filter = -1; // A COSA SERVE?????
	int force_filter = stbi_write_force_png_filter;
	int ctype[5] = { -1, 0, 4, 2, 6 };
	unsigned char sig[8] = { 0x89,'P','N','G',0x0D,0x0A,0x1A,0x0A }; // header starting combination
	unsigned char *out,*o, *filt, *zlib;
	signed char *line_buffer;
	int zlen;
	
	if(stride_bytes == 0) stride_bytes = bytes_per_row;
	if(force_filter >= 5) force_filter = -1;
	
	filt = new unsigned char[(bytes_per_row+1) * _height]; if (!filt) { file.close(); return;}
	line_buffer = new signed char[bytes_per_row]; if (!line_buffer) { delete [] filt; file.close(); return; }
	for(int j=0; j<_height; j++){
		int filter_type;
		if(force_filter > -1){
			filter_type = force_filter;
			encode_png_line(j, stride_bytes, force_filter, line_buffer);
		}else{ // Estimate the best filter by running through all of them:
			int best_filter = 0, best_filter_val = 0x7FFFFFFF, est;
			for(filter_type = 0; filter_type < 5; filter_type++){
				encode_png_line(j, stride_bytes, force_filter, line_buffer);
				// Estimate the entropy of the line using this filter; the less, the better.
				est = 0;
				for(int i=0; i<bytes_per_row; i++) est += abs((signed char) line_buffer[i]);
				if(est < best_filter_val){
					best_filter_val = est;
					best_filter = filter_type;
				}
			}
			if(filter_type != best_filter) {  // If the last iteration already got us the best filter, don't redo it
				encode_png_line(j, stride_bytes, force_filter, line_buffer);
				filter_type = best_filter;
			}
		}
		// when we get here, filter_type contains the filter type, and line_buffer contains the data
		filt[j*(bytes_per_row+1)] = (unsigned char) filter_type;
		memmove(filt+j*(bytes_per_row+1)+1, line_buffer, bytes_per_row);
	}
	delete [] line_buffer;
	zlib = zlib_compress(filt, _height*(bytes_per_row+1), &zlen, stbi_write_png_compression_level);
	delete [] filt;
	if(!zlib) { file.close(); return; }
	
	// each tag requires 12 bytes of overhead
	out = new unsigned char[8 + 12+13 + 12+zlen + 12];
	if (!out) { file.close(); return; }
//	*out_len = 8 + 12+13 + 12+zlen + 12;
	
	o=out;
//	memmove(o,sig,8); o+= 8;
	write_to_stream(file, sig);
	stbiw__wp32(o, 13); // header length
		write_to_stream(file, *o); o=out;
//	stbiw__wptag(o, "IHDR");
	write_to_stream(file, "IHDR");
	stbiw__wp32(o, _width);
		write_to_stream(file, *o); o=out;
	stbiw__wp32(o, _height);
		write_to_stream(file, *o); o=out;
//	*o++ = 8;
	write_to_stream(file, 8);
//	*o++ = (unsigned char)(ctype[bytes_per_pixel] & 0xFF);
	write_to_stream(file, (unsigned char)(ctype[bytes_per_pixel] & 0xFF));
//	*o++ = 0;
//	*o++ = 0;
//	*o++ = 0;
	write_to_stream(file, (unsigned char)0);
	write_to_stream(file, (unsigned char)0);
	write_to_stream(file, (unsigned char)0);
	stbiw__wpcrc(&o,13);
		write_to_stream(file, *o); o=out;
	
	stbiw__wp32(o, zlen);
		write_to_stream(file, *o); o=out;
//	stbiw__wptag(o, "IDAT");
	write_to_stream(file, "IDAT");
	memmove(o, zlib, zlen); o += zlen;
		write_to_stream(file, *o); o=out;
	delete [] zlib;
	stbiw__wpcrc(&o, zlen);
		write_to_stream(file, *o); o=out;
	
	stbiw__wp32(o,0);
		write_to_stream(file, *o); o=out;
//	stbiw__wptag(o, "IEND");
	write_to_stream(file, "IEND");
	stbiw__wpcrc(&o,0);
		write_to_stream(file, *o); o=out;
	
//	STBIW_ASSERT(o == out + *out_len);
	
	file.close();
}


void PNGimage::load_file(std::string fileName)
{
	
}











// AUXILIARY FUNCTIONS ================================================================================================================================


// @OPTIMIZE: provide an option that always forces left-predict or paeth predict
void PNGimage::encode_png_line(int y, int stride_bytes, int filter_type, signed char *line_buffer)
{
	unsigned char * pixels = new unsigned char[data.size()];
	for(int i=0; i<data.size(); i++) pixels[i] = data[i];
	static int stbi__flip_vertically_on_write = 0;
   static int mapping[]  = { 0,1,2,3,4 };
   static int firstmap[] = { 0,1,0,5,6 };
   int *mymap = (y != 0) ? mapping : firstmap;
   int i;
   int type = mymap[filter_type];
   unsigned char *z = pixels + stride_bytes * (stbi__flip_vertically_on_write ? _height-1-y : y);
//   unsigned char *z = (unsigned char*)data[stride_bytes * (stbi__flip_vertically_on_write ? _height-1-y : y)]; // NON FUNZIONA
   int signed_stride = stbi__flip_vertically_on_write ? -stride_bytes : stride_bytes;

   if (type==0) {
      memcpy(line_buffer, z, bytes_per_row);
      return;
   }

   // first loop isn't optimized since it's just one pixel
   for (i = 0; i < bytes_per_pixel; ++i) {
      switch (type) {
         case 1: line_buffer[i] = z[i]; break;
         case 2: line_buffer[i] = z[i] - z[i-signed_stride]; break;
         case 3: line_buffer[i] = z[i] - (z[i-signed_stride]>>1); break;
         case 4: line_buffer[i] = (signed char) (z[i] - stbiw__paeth(0,z[i-signed_stride],0)); break;
         case 5: line_buffer[i] = z[i]; break;
         case 6: line_buffer[i] = z[i]; break;
      }
   }
   switch (type) {
      case 1: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - z[i-bytes_per_pixel]; break;
      case 2: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - z[i-signed_stride]; break;
      case 3: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - ((z[i-bytes_per_pixel] + z[i-signed_stride])>>1); break;
      case 4: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - stbiw__paeth(z[i-bytes_per_pixel], z[i-signed_stride], z[i-signed_stride-bytes_per_pixel]); break;
      case 5: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - (z[i-bytes_per_pixel]>>1); break;
      case 6: for (i=bytes_per_pixel; i < bytes_per_row; ++i) line_buffer[i] = z[i] - stbiw__paeth(z[i-bytes_per_pixel], 0,0); break;
   }
}








#endif
