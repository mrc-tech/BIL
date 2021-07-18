/**************************************
writes a bitmap file with all the shades of gray
possible using 24-bit per pixel bitmp file
**************************************/

#include "../include/writeBMP.h"

using namespace std;



int main()
{
	int width  = 256;
	int height = 256;
	int data[width*height];
	
	int count = 0;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			data[i*width + j] = count++;
			if(count>255) count = 0;
		}
	}
	
	writeBMP("writeBMP.bmp", width,height, data);
	
	return 0;
}
