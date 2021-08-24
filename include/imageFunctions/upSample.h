#ifndef MRC_UPSAMPLE
#define MRC_UPSAMPLE

#include "../Image.hpp" // include the Image class definition

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
