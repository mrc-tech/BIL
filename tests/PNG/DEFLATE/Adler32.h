#ifndef MRC_ALDER32
#define MRC_ADLER32

#include <vector>

#define ADLER32_BASE 65521 // largest prime smaller than 65536


uint32_t Adler32_update(uint32_t adler, std::vector<unsigned char> data)
{
	// update a 32 bit (4 byte) Adler-32 checksum due to new data
	// update funcion used in DEFLATE algorithm for hashing only specific data
	uint32_t s1 =  adler        & 0xFFFF; // least significant 16 bit
	uint32_t s2 = (adler >> 16) & 0xFFFF; // most significant 16 bit
	for(int i=0; i<data.size(); i++){
		s1 = (s1 + data[i]) % ADLER32_BASE;
		s2 = (s2 + s1)      % ADLER32_BASE;
	}
	return (s2 << 16) + s1;
}


uint32_t Adler32(std::vector<unsigned char> data)
{
	// return a 32 bit (4 byte) Adler-32 checksum
	return Adler32_update(1L, data); // The Adler-32 checksum should be initialized to 1.
}

     
#endif
