#include <iostream>

#include "crc32.h"
#include "Adler32.h"

using namespace std;



vector<unsigned char> str2bin(string str)
{
	// converte una stringa in un vettore di bytes
	vector<unsigned char> res;
	for(int i=0; i<str.length(); i++) res.push_back(str[i]);
	return res;
}



int main(int argc, char **argv)
{
	string str = "pollo";

	if(argc > 1){
		str = "";
		for(auto i=1; i<argc-1; i++)
			str = str + argv[i] + " "; 
		str += argv[argc-1]; // aggiunge l'uiltimo senza spazio alla fine
	}
	
	cout << "String = " << str << endl;
	printf("CRC32B  : %08X \n",   crc32(str2bin(str)));
	printf("ADLER32 : %08X \n", Adler32(str2bin(str)));
	
	return 0;
}
