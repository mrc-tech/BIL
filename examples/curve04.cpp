#include <fstream>

#include "../include/Image.hpp"

using namespace std;


int main()
{
	Image img(800,800);
	
	img.penColor(50,50,50);
	ifstream file("curve04.txt");
	string str;
	int y = 0;
	while(getline(file,str)){
		img.text(0, y, str, PS2THIN4);
		y += 16;
	}
	file.close();

	
	img.penColor(255,255,255);
//	img.penWidth(3);
	
	
	int N = 20;
	int Delta = 400/N;
	for(int x=0; x<=400; x+=Delta){
		img.line(x  , 400  , 400  , 400-x);
		img.line(x  , 400  , 400  , 400+x);
		img.line(400, x    , 400+x, 400  );
		img.line(400, 800-x, 400+x, 400  );
	}
	img.penColor(100,100,100);
	img.circ(0  ,0  ,400);
	img.circ(800,0  ,400);
	img.circ(0  ,800,400);
	img.circ(800,800,400); 
	
	img.save_png("curve04.png");
}
