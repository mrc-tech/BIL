#include <iostream>
#include <vector>
#include <string>
#include <bitset>

#define MAX_BITS 256 //max bits of Huffman tree

using namespace std;


struct HuffmanTree {
	vector<unsigned char> symbol;
	vector<unsigned short> len;
};

void print_Huffman_tree(HuffmanTree t)
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
	HuffmanTree tree;
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
	
	
	
	return 0;
}
