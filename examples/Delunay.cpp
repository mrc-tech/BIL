#include <algorithm> //for sorting algorithm
#include <vector>
#include <cmath>
#include <cstdlib> //for rand()
#include <ctime> //for time()

#include "../include/Image.hpp"

#define NPOINTS 100

using namespace std;


int main()
{
	srand((unsigned)time(0)); //set RNG
	
	Image img(800, 600); //create image
	int X[NPOINTS], Y[NPOINTS]; //coordinates of points
	
	//initialize points coordinates:
	for(int i=0; i<NPOINTS; i++){
		X[i] = rand() % img.width();
		Y[i] = rand() % img.height();
	}
	
	for(int y=0; y<img.height(); y++)
		for(int x=0; x<img.width(); x++){
			//calculate distances between current point (x,y) and points (X[i],Y[i])
			vector<double> lengths; //vector of distances between all (X[i],Y[i]) points
			for(int i=0; i<NPOINTS; i++){
				lengths.push_back(sqrt((double)pow(x-X[i],2)+(double)pow(y-Y[i],2)));
			}
			sort(lengths.begin(), lengths.end()); //sort from nearest to most far
			if(abs(lengths[0]-lengths[1]) < 2) img.set_pixel(x,y, {55,55,55});
			lengths.clear(); //reset the vector (computationally expensive for memory menagement)
		}
	
	img.penColor(255,255,255);
	img.penWidth(3);
	for(int i=0; i<NPOINTS; i++) img.drawPoint(X[i], Y[i]);
	
	img.save_bmp("Delunay.bmp");
	
	return 0;
}
