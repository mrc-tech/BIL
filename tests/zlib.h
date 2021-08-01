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


static unsigned int stbiw__crc32(unsigned char *buffer, int len)
{
   static unsigned int crc_table[256] =
   {
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
   int i;
   for (i=0; i < len; ++i)
      crc = (crc >> 8) ^ crc_table[buffer[i] ^ (crc & 0xff)];
   return ~crc;
}

static void stbiw__wpcrc(unsigned char **data, int len)
{
   unsigned int crc = stbiw__crc32(*data - len - 4, len+4);
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
			s1 %= 65521; s2 %= 65521;
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


#endif
