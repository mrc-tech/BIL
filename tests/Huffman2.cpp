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
#include <queue>
#include <unordered_map>
//#include <bitset>


// TEMPORANEO, PER IL DEBUG //
#include <iostream>         //
using namespace std;        //
//////////////////////////////


// Tree node
struct node {
	unsigned char data; // only for leaf
	int freq;
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

// Comparison object to be used to order the heap
struct comp {
	bool operator()(node* l, node* r) { return l->freq > r->freq; } // highest priority item has lowest frequency
};


class HuffmanTree
{
	node* tree_root; //root of binary tree (bottom-up construction)
	std::vector<unsigned char> data_input; // input data
	vector<unsigned char> symbol;
	vector<unsigned char> length; // number of bits of symbol codes
	
	public:
		HuffmanTree() { tree_root = nullptr; }
		HuffmanTree(std::vector<unsigned char> data) { init(data); tree_root = nullptr; }
		~HuffmanTree();
		
		void init(std::vector<unsigned char> data);
		void init_str(std::string str) { std::vector<unsigned char> data; for(int i=0;i<str.size();i++) data.push_back(str[i]); init(data); }
		
//		std::vector<bitset<1>> encode();
		std::string encode();
		std::vector<unsigned char> decode(std::string str);
		
		
	private:
		// auxiliary functions:
		node* getNode(unsigned char data, int freq, node* left, node* right);
		void destroy_tree(node* n);
//		template<typename A,typename B> void sort_by_vector(std::vector<A>& occurr, std::vector<B>& data);
		void encode_tree(node* root, string str, std::unordered_map<unsigned char, string> &huffmanCode);
		void decode_tree(std::vector<unsigned char> &res, node* root, int &index, string str);
		
};


// IMPLEMENTARTION =======================================================================================================


HuffmanTree::~HuffmanTree()
{
	if(tree_root) destroy_tree(tree_root);
}


void HuffmanTree::init(std::vector<unsigned char> data)
{
	// count frequency of appearance of each character and store it in a map
	unordered_map<unsigned char, int> freq;
	for (unsigned char x: data) freq[x]++;

	// Create a priority queue to store live nodes of Huffman tree;
	priority_queue<node*, vector<node*>, comp> pq;

	// Create a leaf node for each character and add it to the priority queue.
	for (auto pair: freq) pq.push(getNode(pair.first, pair.second, nullptr, nullptr));

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority (lowest frequency) from the queue
		node *left	= pq.top();	pq.pop();
		node *right	= pq.top();	pq.pop();

		// Create a new internal node with these two nodes as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	this->tree_root = pq.top();
	
	data_input = data;

	

	
	
}



std::string HuffmanTree::encode()
{
	std::vector<bitset<1>> res;
	
	// traverse the Huffman Tree and store Huffman Codes in a map. Also prints them
	unordered_map<unsigned char, string> huffmanCode;
	encode_tree(this->tree_root, "", huffmanCode);

//	cout << "Huffman Codes are :\n" << '\n';
//	for(auto pair: huffmanCode) cout << pair.first << " " << pair.second << '\n';

	// build encoded string (string made of 0 and 1)
	string str = "";
	for(unsigned char x: data_input) str += huffmanCode[x];

//	cout << "\nEncoded string is :\n" << str << '\n';
	
	for(char c: str) if(c == '0') res.push_back(0); else res.push_back(1); // converts into a bitset vector
	
	return str;
//	return res;
}


std::vector<unsigned char> HuffmanTree::decode(std::string str)
{
	// decode from encoded string of 0 and ones to the original data
	
	std::vector<unsigned char> res;
	
	// traverse the Huffman Tree again and this time decode the encoded string
	int index = -1;
	while (index < (int)str.size() - 2) {
		decode_tree(res, this->tree_root, index, str);
	}
	
	return res;
}





// AUXILIARY FUNCTIONS =====================================================================================================


// Function to allocate a new tree node
node* HuffmanTree::getNode(unsigned char data, int freq, node* left, node* right)
{
	node* n = new node();

	n->data = data;
	n->freq = freq;
	n->left = left;
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




// traverse the Huffman Tree and store Huffman Codes in a map.
void HuffmanTree::encode_tree(node* root, string str, unordered_map<unsigned char, string> &huffmanCode)
{
	if (root == nullptr) return;

	// found a leaf node
	if (!root->left && !root->right) huffmanCode[root->data] = str;

	encode_tree(root->left, str + "0", huffmanCode);
	encode_tree(root->right, str + "1", huffmanCode);
}


// traverse the Huffman Tree and decode the encoded string
void HuffmanTree::decode_tree(std::vector<unsigned char> &res, node* root, int &index, string str)
{
	if(root == nullptr) return;

	// found a leaf node
	if(!root->left && !root->right){
		res.push_back(root->data);
		return;
	}

	index++;

	if (str[index] == '0') decode_tree(res, root->left, index, str); else decode_tree(res, root->right, index, str);
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
//	t.init({'A','B','C','C','B','C','C','D'});
	t.init_str(str);
	
	string encoded = t.encode();
	
	cout << "Encoded data: " << encoded << endl;
	
	auto x = t.decode(encoded);
	
	cout << "Decoded data: ";
	for(auto i: x) cout << i; cout << endl;
	
	
	
	
	return 0;
}

