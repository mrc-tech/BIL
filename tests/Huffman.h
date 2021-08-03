#ifndef MRC_HUFFMAN
#define MRC_HUFFMAN

#include <vector>
#include <string>
//#include <bitset>


// TEMPORANEO, PER IL DEBUG //
#include <iostream>         //
using namespace std;        //
//////////////////////////////


class HuffmanTree
{
	vector<unsigned char> symbol;
	vector<unsigned char> length; // number of bits of symbol codes
	
	public:
		HuffmanTree() {}
		HuffmanTree(std::vector<unsigned char> data) { init(data); }
		
		void init(std::vector<unsigned char> data);
		
	private:
		
		
};


// IMPLEMENTARTION =======================================================================================================


void HuffmanTree::init(std::vector<unsigned char> data)
{
	std::vector<int> occurr; //number of occurrencies per symbol
	for(int i=0; i<data.size(); i++){
		bool found = false;
		for(int j=0; j<symbol.size(); j++){
			if(symbol[j] == data[i]){
				found = true;
				if(j>=occurr.size()) occurr.resize(j+1); //allocate memory if not enough
				occurr[j]++;
			}
		}
		if(!found){
			symbol.push_back(data[i]); //populate symbol vector
			if(symbol.size()-1>=occurr.size()) occurr.resize(symbol.size()); //allocate memory if not enough
			occurr[symbol.size()-1]++; //save occurrence = 1
		}
	}
	
	
	for(int i=0;i<symbol.size();i++) cout << symbol[i] << " "; cout << endl;
	for(int i=0;i<occurr.size();i++) cout << occurr[i] << " "; cout << endl;
	
	
}





// AUXILIARY FUNCTIONS =====================================================================================================







#endif
