#include <cmath>


double fade(double t) { return t*t*t*(t*(t*6.0-15.0)+10.0); } // una specie di sigmoide per rendere il calcolo delle distanze più realistiche

double lerp(double t, double a, double b) { return a + t *(b-a); } // interpolazione lineare tra a e b

double grad(int hash, double x, double y){
	int h = hash & 15; // Convert low 4 bits of hash code
	double u = h<8 ? x : y; // into 12 gradient directions
	double v = h<4 ? y : h==12 || h==14 ? x : z;
	return ((h&1)==0 ? u : -u ) + ((h&2)==0 ? v : -v );
}


double noise(double x, double y)
{
	// punto della griglia per cui X<x e Y<y più vicino a (x,y):
	int x0 = (int) floor(x) & 255;
	int y0 = (int) floor(y) & 255;
	
	double dx = x - floor(x); // Find relative x,y
	double dy = y - floor(y); // of point in cube
	
	// Compute fade curves for each of x,y:
	double u = fade(dx);
	double v = fade(dy);
	
	// Hash coordinates of the 4 cube corners:
	int p0 = perm[X  ] + Y  ;
	int p1 = perm[X+1] + Y  ;
	int p2 = perm[X+1] + Y+1;
	int p3 = perm[X  ] + Y+1;
	
	// add blended results from 4 corners of cube:
	return lerp(w, lerp(v, lerp(u, grad(p[AA],x,y), grad(p[BA],x-1,y,z)),
	       lerp (u , grad ( p [ AB ] , x , y -1 , z ) , grad ( p [ BB ] , x -1 , y -1 , z ) ) ) ,
	       lerp (v , lerp (u , grad ( p [ AA +1] , x , y , z -1 ) , grad ( p [ BA +1] , x -1 , y , z -1 ) ) ,
	       lerp (u , grad ( p [ AB +1] , x , y -1 , z -1 ) , grad ( p [ BB +1] , x -1 , y -1 , z -1 ) ) ) ) ;
}





int p[] = new int[512];
int permutation[] = {151 ,160 ,137 ,91 ,90 ,15 , // ... remaining 506 entries omitted
};

static { for (int i =0; i < 256 ; i ++)
p [256+ i ] = p [ i ] = permutation [ i ]; }
}

