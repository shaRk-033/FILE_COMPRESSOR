#include <iostream>
#include <fstream>
#include<stdlib.h>
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;
string output;
int count = 0;
unsigned long int total_traversal = 0;
class BitwiseRead {
private:
	char buf;
	int nbits;
	istream& in;
public:
	BitwiseRead(istream & is);
	void fill();

	int readBit();
};

BitwiseRead::BitwiseRead(istream & is) : in(is) {
	buf = 0;
	nbits = 8;
}



void BitwiseRead::fill()
{
	buf = in.get();
	nbits = 0;
}

int BitwiseRead::readBit()
{
	if (nbits == 8) {
		fill();
	}

	unsigned char mask = 1;
	mask = mask << (7 - nbits);
	mask = mask & buf;
	nbits++;
	if (mask == 0) {
		return 0;
	}
	else {
		return 1;
	}
}
class Huffman
{
private:
	class node
	{
	public:
		char data;
		node * left, * right;

		node(char item)
		{
			data = item;
			left = right = 0;
		}
	};
	typedef node * nodePointer;
public:
	Huffman();
	void buildDecodingTree(ifstream & codeIn);
	void insert(char ch, string code);
	char decode(BitwiseRead &in);
	void printTree(ostream & out, nodePointer root, int indent);
	void displayDecodingTree(ostream & out);
private:
	nodePointer root;
};
inline Huffman::Huffman()
{
	root = new node('*');
}
inline void Huffman::displayDecodingTree(ostream & out)
{
	printTree(out, root, 0);
}
void Huffman::buildDecodingTree(ifstream & codeIn)
{
	char ch;
	string code;
	for (;;)
	{
		if ( codeIn.eof() ) return;
		codeIn >> ch >> code;
		cout << "\n" << ch << " = " << code;
		if (ch == '_')
		{
			ch = ' ';
		}
		insert(ch, code);
	}
}


void Huffman::insert(char ch, string code)
{
	Huffman::nodePointer p = root;
	for (int i = 0; i < code.length(); i++)
	{
		switch (code[i])
		{
		case '0' :
			if (p->left == 0)
				p->left = new Huffman::node('*');
			p = p->left;
			break;
		case '1' :
			if (p->right == 0)
				p->right = new Huffman::node('*');
			p = p->right;
			break;
		default:
			cerr << "*** Illegal character in code ***\n";
			exit(1);
		}
	}
	p->data = ch;
}


char Huffman::decode(BitwiseRead &in)             //To decode from huffman tree
{
	Huffman::nodePointer p;
	p = root;
	int bit;
	while (true)
	{
		bit = in.readBit();
		//count++;
		if (bit == 1)
		{
			p = p -> right;
		}
		if (bit == 0)
		{
			p = p -> left;
		}
		if (p->right == NULL || p->left == NULL)
		{
			break;
		}
	}
	total_traversal++;
	return (p->data);
}

void Huffman::printTree(ostream & out, Huffman::nodePointer root, int indent) //To print huffman tree
{
	if (root != 0)
	{
		printTree(out, root->right, indent + 4);
		out << setw(indent) << " " << root->data << endl;
		printTree(out, root->left, indent + 4);
	}
}

string str;


int main()
{
	int total_bits_written;
	int current_bits = 0;
	char data;
	char filename[32];
	cout << "\n\t====================================";
	cout << "\n\t\t Text File Decompressor\n";
	cout << "\t====================================";
	cout << "\n\nEnter name of code file: ";
	cin >> filename;
	ifstream codestream(filename);
	if (!codestream.is_open())
	{
		cout << "Error !!! Cannot open code file.\n";
		exit(1);
	}
	Huffman h;
	h.buildDecodingTree(codestream);
	cout << "Here is the Huffman decoding tree:\n";
	h.displayDecodingTree(cout);
	cout << endl;
	cout << "\nEnter Name of Compressed file : ";
	cin >> filename;
	ifstream in;
	ofstream fout;
	fout.open("Decompressed.txt");
	in.open(filename, ios::binary);
	BitwiseRead os(in);
	if (!in.is_open())
	{
		cout << "Error !!! Cannot open Compressed file.\n";
		exit(1);
	}
	cout << "\nProcessing..... Plz Wait";
	char ch3;
	ch3 = char(129);
	while (true)
	{

		data = h.decode(os);
		if ( data == ch3)
		{
			break;
		}
		current_bits++;
		fout << data;
	}
	cout << "\nDecompression Successful !!!\n";
	cout << "\nTotal Number of Times Tree Traversed : " << total_traversal;
	int l;
	cout << "\nEnter any key to exit : ";
	cin >> l;
	return (0);
}
