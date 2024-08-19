#include <vector>
#include <cstdlib> // rand
#include <cmath>


class vec2
{
	public:
		vec2(double x, double y) { this->x = x; this->y = y; }
		
		double x, y;
};

double dot(vec2 u, vec2 v){
	return u.x*v.x + u.y*v.y;
}

void Shuffle(std::vector<int> &arrayToShuffle) {
	for(size_t e=arrayToShuffle.size()-1; e>0; e--){
		int index = round(rand() % arrayToShuffle.size());
		
		int temp = arrayToShuffle[e]; // swap...
		arrayToShuffle[e] = arrayToShuffle[index];
		arrayToShuffle[index] = temp;
	}
}

std::vector<int> MakePermutation()
{
	// crea un'array di permutazioni a caso
	std::vector<int> permutation;
	for(int i = 0; i < 256; i++) permutation.push_back(i);

	Shuffle(permutation);
	
	for(int i = 0; i < 256; i++) permutation.push_back(permutation[i]);
	
	return permutation;
}
std::vector<int> Permutation = MakePermutation();

vec2 GetConstantVector(int v) {
	// v is the value from the permutation table
	int h = v & 3;
	if(h == 0)      return vec2( 1.0,  1.0);
	else if(h == 1) return vec2(-1.0,  1.0);
	else if(h == 2) return vec2(-1.0, -1.0);
	else            return vec2( 1.0, -1.0);
}

double fade(double t) { return ((6*t - 15)*t + 10)*t*t*t; }

double lerp(double t, double a1, double a2) { return a1 + t*(a2-a1); }

double Noise2D(double x, double y)
{
	int X = (int)floor(x) % 255;
	int Y = (int)floor(y) % 255;
	
	double xf = x-floor(x);
	double yf = y-floor(y);
	
	vec2 topRight   (xf-1.0, yf-1.0);
	vec2 topLeft    (xf    , yf-1.0);
	vec2 bottomRight(xf-1.0, yf    );
	vec2 bottomLeft (xf    , yf    );
	
	// Select a value from the permutation array for each of the 4 corners
	int valueTopRight    = Permutation[Permutation[X+1]+Y+1];
	int valueTopLeft     = Permutation[Permutation[X  ]+Y+1];
	int valueBottomRight = Permutation[Permutation[X+1]+Y  ];
	int valueBottomLeft  = Permutation[Permutation[X  ]+Y  ];
	
	double dotTopRight    = dot(topRight   , GetConstantVector(valueTopRight));
	double dotTopLeft     = dot(topLeft    , GetConstantVector(valueTopLeft));
	double dotBottomRight = dot(bottomRight, GetConstantVector(valueBottomRight));
	double dotBottomLeft  = dot(bottomLeft , GetConstantVector(valueBottomLeft));
	
	double u = fade(xf);
	double v = fade(yf);
	
	return lerp(u,
		lerp(v, dotBottomLeft,  dotTopLeft),
		lerp(v, dotBottomRight, dotTopRight)
	);
}
