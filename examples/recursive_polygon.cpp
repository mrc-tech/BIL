#include <vector>
#include <cmath>

#include "../include/Image.hpp"

#define PI 3.141592653589793
#define N_POLY 100 //number of recursive polygons

using namespace std;


int main()
{
	Image img(800,800);
	
	int N = 3; //number of verticies of polygon
	double R = 400; //initial radius (polar coord)
	double theta = -PI/2; //initial angle (polar coord)
	double delta_theta = PI/50; //updating angle
	double phi = (double) ((N-2)*PI) / N; //figure internal angles between verticies (all equal: regular polygon)
	
	img.penColor(255,255,255);
	vector<int> coord;
	for(auto i=0; i<=N_POLY; i++){
		//create polygon coordinates (from polar to cartesian):
		coord.clear(); //reset coordinates vector
		for(auto j=0; j<N; j++){
			coord.push_back(R*cos(theta) + 400); //save x coordinate (from polar to cartesian)
			coord.push_back(R*sin(theta) + 400); //save y coordinate (from polar to cartesian)
			theta += 2*PI / N; //next point
		}
		//plot closed polygon:
		img.poly(coord, true);
		//update polar coordinates:
		theta += delta_theta;
		R = R / (sin(delta_theta)*(1./tan(phi/2.)) + cos(delta_theta));
	}
	
	img.save_png("recursive_polygon.png");
	
	
	return 0;
}
