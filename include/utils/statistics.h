#include <vector>



double massimo(std::vector<double> x)
{
	double Max = x[0];
	for(int i=0;i<x.size();i++){
		if(x[i] > Max) Max = x[i];
	}
	return Max;
}



double minimo(std::vector<double> x)
{
	double Min = x[0];
	for(int i=0;i<x.size();i++){
		if(x[i] < Min) Min = x[i];
	}
	return Min;
}


double media(std::vector<double> x)
{
	double sum = 0;
	int N = x.size();
	for(int i=0;i<N;i++) sum += x[i];
	return sum / N;
}
