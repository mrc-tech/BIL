#include <iostream>
#include <vector>
#include <string>
#include <bitset>

//#include "Huffman.h"

#define MAX_BITS 256 //max bits of Huffman tree

using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////// to be moved on "Huffman.h"
#ifndef MRC_HUFFMAN
#define MRC_HUFFMAN

#include <algorithm>
#include <vector>
#include <string>
//#include <bitset>


// TEMPORANEO, PER IL DEBUG //
#include <iostream>         //
using namespace std;        //
//////////////////////////////


struct node {
	unsigned char data; // only for leaf
	node* left;  // 0
	node* right; // 1
	
	node(){
		// node constructor
		left  = nullptr;
		right = nullptr;
	}
	node(unsigned char _data){
		// leaf constructor
		data  = _data;
		left  = nullptr;
		right = nullptr;
	}
};


class HuffmanTree
{
	node* root; //root of binary tree (bottom-up construction)
	vector<unsigned char> symbol;
	vector<unsigned char> length; // number of bits of symbol codes
	
	public:
		HuffmanTree() { root = nullptr; }
		HuffmanTree(std::vector<unsigned char> data) { init(data); root = nullptr; }
		~HuffmanTree();
		
		void init(std::vector<unsigned char> data);
		void init_str(std::string str) { std::vector<unsigned char> data; for(int i=0;i<str.size();i++) data.push_back(str[i]); init(data); }
		
	private:
		// auxiliary functions:
		node* add_leaf(node* left, node* right);
		void destroy_tree(node* n);
		template<typename A,typename B> void sort_by_vector(std::vector<A>& occurr, std::vector<B>& data);
		
};


// IMPLEMENTARTION =======================================================================================================


HuffmanTree::~HuffmanTree()
{
	if(root) destroy_tree(root);
}


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
	
	
node* temp = add_leaf(new node('A'), new node('B'));

cout << temp->left->data << endl;
cout << temp->right->data << endl;

temp = add_leaf(temp, new node('C'));
cout << temp->left->left->data << endl;
cout << temp->left->right->data << endl;
cout << temp->right->data << endl;
	
	
	sort_by_vector(occurr, symbol); // sort occurr and symbol by occurrence vector MI SA CHE NON SERVE, PERCH[ ALLA FINE P IMPORTANTE ANCHE LA SOMMA SUI NODI DELL'ALBERO
	
	for(int i=0;i<symbol.size();i++) cout << symbol[i] << " "; cout << endl;
	for(int i=0;i<occurr.size();i++) cout << occurr[i] << " "; cout << endl;
	
}





// AUXILIARY FUNCTIONS =====================================================================================================


node* HuffmanTree::add_leaf(node* left, node* right)
{
	node* n = new node();
	n->left  = left;
	n->right = right;
	
	return n;
}


void HuffmanTree::destroy_tree(node* n)
{
	if(n != nullptr){
		destroy_tree(n->left);
		destroy_tree(n->right);
		delete n;
	}
}


template<typename A,typename B>
void HuffmanTree::sort_by_vector(std::vector<A>& score, std::vector<B>& data)
{
	// Sort data and score vectors based on score vector
	vector< pair<int, unsigned char> > vec; // vector of pairs (sort by first member)
	for(int i=0; i<data.size(); i++) vec.push_back(make_pair(score[i], data[i])); // populate vector of pairs
	sort(vec.begin(), vec.end());
	for(int i=0; i<score.size(); i++) score[i] = vec[i].first;
	for(int i=0; i<data.size(); i++)  data[i]  = vec[i].second;
}




#endif
///////////////////////////////////////////////////////////////////////////// End move





struct HuffmanTreeStruct {
	vector<unsigned char> symbol;
	vector<unsigned short> len;
};

void print_Huffman_tree(HuffmanTreeStruct t)
{
	if((t.symbol.size() != t.len.size()) || (t.symbol.size() == 0)) return;
	// RIPENSARE L'ALGORITMO USANDO I "bitset" NEL CALCOLO DEI CODICI
	// Count the number of codes for each code length. Let bl_count[N] be the number of codes of length N:
	int N = t.symbol.size();
	vector<unsigned short> bl_count; // Binary Length count
	for(int n=0; n<MAX_BITS; n++){
		int total = 0; //total elements of length n
		for(int i=0; i<N; i++) if(t.len[i] == n) total++;
		bl_count.push_back(total);
	}
//	for(int i=0;i<bl_count.size();i++) cout << bl_count[i] << " "; cout << endl;
	// Find the numerical value of the smallest code for each code length:
	int code = 0;
	bl_count[0] = 0;
	vector<unsigned short> next_code(MAX_BITS,0); //initialize of length MAX_BITS and value 0
	for(int bits=0; bits<MAX_BITS; bits++){
		code = (code + bl_count[bits]) << 1;
		next_code[bits+1] = code;
	}
//	for(int i=0;i<next_code.size();i++) cout << next_code[i] << " "; cout << endl;
	// Calculate codes:
	vector<unsigned char> Huffman_code(N,0);
	for(int i=0; i<N; i++){
		if(t.len[i] != 0){
			Huffman_code[i] = next_code[t.len[i]];
			next_code[t.len[i]]++;
		}
	}
//	for(int i=0;i<Huffman_code.size();i++) cout << bitset<8>(Huffman_code[i]) << " "; cout << endl;
	
	
	for(int i=0; i<N; i++){
		cout << t.symbol[i] << " :  " << t.len[i] << "\t";
		bitset<MAX_BITS> temp = Huffman_code[i];
		for(int j=t.len[i]-1; j>=0; j--) cout << temp[j]; //prints only the length bits
		cout << endl;
	}
}



int main()
{	
	HuffmanTreeStruct tree;
	tree.symbol.push_back('A'); tree.len.push_back(3);
	tree.symbol.push_back('B'); tree.len.push_back(3);
	tree.symbol.push_back('C'); tree.len.push_back(3);
	tree.symbol.push_back('D'); tree.len.push_back(3);
	tree.symbol.push_back('E'); tree.len.push_back(3);
	tree.symbol.push_back('F'); tree.len.push_back(2);
	tree.symbol.push_back('G'); tree.len.push_back(4);
	tree.symbol.push_back('H'); tree.len.push_back(4);
	print_Huffman_tree(tree);
	
	string str = "polletto";
	
	HuffmanTree t;
	t.init({'A','B','C','C','B','C','C','D'});
//	t.init_str(str);
	
	
	return 0;
}
