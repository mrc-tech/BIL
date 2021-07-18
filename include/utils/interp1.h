#ifndef MRC_INTERP1
#define MRC_INTERP1

#include <iostream>
#include <vector>



template<typename T>
double interp1(std::vector<T> x, std::vector<T> y, T val)
{
	T res;
	
	if(x.size() != y.size()){
		std::cout << "ERRORE!: Il vettore x e il vettore y devono avere la stessa dimensione" << std::endl;
		return 0;
	}
	
	if(x[x.size()-1] < val){
		std::cout << "ERRORE!: non riesce ad estrapolare per val > x[end]" << std::endl;
		return 0;
	}
	
	int i=0; //indice della x minore (x_(i+1) è quella maggiore)
	//cerca il range delle x in cui ricade "val":
	while(x[i+1] < val) i++;
	//fa l'interpolazione lineare:
	res = y[i] + (y[i+1]-y[i])/(x[i+1]-x[i]) * (val-x[i]);
	
	return res;
}



template<typename T>
std::vector<T> interp1(std::vector<T> x, std::vector<T> y, std::vector<T> vals)
{
	std::vector<T> res;
	
	for(int i=0;i<vals.size();i++) res.push_back(interp1(x,y, vals[i]));
	
	return res;
}




#endif
