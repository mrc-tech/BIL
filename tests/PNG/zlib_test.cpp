#include <iostream>

#include "zlib.h"

int main()
{
	std::string s = "pollo";
	
	unsigned char *dati;
	unsigned char *temp;
	int zlibsize = 0;
	
	dati = (unsigned char*)malloc(s.length());
	for(size_t i=0; i<s.length(); i++) dati[i] = s[i];
	
	temp = zlib_compress(dati, s.length(), &zlibsize, 5);
	
//	for(size_t i=0; i<zlibsize; i++) std::cout << std::hex << (int)temp[i] << " "; std::cout << std::endl;
	for(size_t i=0; i<zlibsize; i++) printf("%02X ",temp[i]); std::cout << std::endl;
	
	free(dati);
	free(temp);
	
	
	return 0;
}
