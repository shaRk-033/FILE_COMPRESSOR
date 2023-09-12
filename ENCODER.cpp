#include<cstring>
#include<vector>
#include<iostream>
#include<iomanip>
#include<queue>
#include<fstream>
using namespace std;
vector< pair<char, string> > characterMap;
string s = "";
vector<int> encoded_text;
vector<string> code;
class BitwiseWrite
{
	private:
		char buffer;
		int bits;
		ostream &out;

	public:
	    BitwiseWrite(ostream &os): out(os), bits(0), buffer(0)
		{}
	
	void flush()  				  // writes the buffer into the file
	{
		out.put(buffer);    
		out.flush();
		buffer = 0;bits = 0;
	}
	void writeBit(int i )
	{   unsigned char mask = 1;
		if(i==-1)
		{
			flush();
		}
		if(bits == 8)  			//waits until the buffer contains 8 bits of data
		{
			flush();
		}
		if(i==1) 				//if the input is 1 then the mask is shifted 7 - bits to the left
		{						//and the buffer value is ORed with the mask to place the input bit i in the correct position
			mask = mask << (7 - bits);
			buffer |= mask;

		}
		//if the bit is 0 the mask is shifted 7 - bits to the left and the mask is negated 
		if (i == 0)
	    {						
		mask = mask << (7 - bits);
		mask = ~mask;
		buffer = buffer & mask;
	    }
		bits++;
	}

	 
};
//node struct is created to contain left and right pointers so as to create a binary tree
struct Huffman_node {

	int freq;

	long long int asci_let;

	Huffman_node *left;
	Huffman_node *right;
	Huffman_node()
	{
		left = right = nullptr;
	}
	Huffman_node(int freq, int asci_let)
	{
		left = nullptr;
		right = nullptr;
		this->freq = freq;
		this->asci_let = asci_let;
	}

};
struct compare
{
	bool operator()( Huffman_node* l,  Huffman_node* r) { return l->freq > r->freq; }
};
// A Min Heap is created 
priority_queue< Huffman_node*, vector<Huffman_node*>, compare> min_Heap;

void frequency_calculator(int freq[], string file_name)
{
	char c;
	ifstream fin;
	fin.open(file_name);
	char ch;
	if (!fin.is_open())
	{
		cout << "The given file doesnt exist";exit(1);
	}
	while (fin.get(c))
	{
		if (c >= 32 && c < 257)
		{
			characterMap.push_back(make_pair(c, ""));
		}

		int y = int(c);
		freq[y]++;//count3++;
	}
	ch = char(129);
	characterMap.push_back(make_pair(ch,""));
	cout<<characterMap.size();
	fin.close();
}
void findingAscii(int freq[], vector<int>&ascii_table, vector<int> &ogfreq)
{
	
	for (int i = 32; i < 254; i++)
	{
		if (freq[i] != 0)
		{
			// cout<<freq[i]<<"\n";
			ascii_table.push_back((i));
			ogfreq.push_back(freq[i]);
		}
	}
	ascii_table.push_back(129);
	ogfreq.push_back(1);
}
void printFreqTable(vector<int>&ascii_table, vector<int> &ogfreq)
{
	string s = "Space";
	char c;
	cout << "\t\tCHARACTER\t\t" << "\t\tFREQUENCY\t\t\n";
	// cout<<ogfreq.size()<<"\t"<<ascii_table.size();
	for (int i = 0; i < ogfreq.size(); i++)
	{
		c = ascii_table[i];

		if (c == ' ') {
			cout << "\t\t" << s << "\t\t" << ogfreq[i] << "\t\t\n";
		}

		else {
			cout << "\t" << ascii_table[i] << "\t" << ogfreq[i] << "\t\n";
		}

	}
}

void formHuffmanCodes(Huffman_node * root, string s)
{
	if(!root)
	{
		return;
	}
	else
	{
		if (root->asci_let != 254 && root->asci_let != 0x2DB && !root->left && !root->right)

		{
			char c = char(root->asci_let);
			if(root->asci_let == ' ')
			{
				code.push_back(string("_").append(" ").append(s));

			}
			else
			{
				string x(1,c);
			    code.push_back(x + " " +  s );	
			}
			for(int i = 0; i<characterMap.size(); i++)
			{
				if(characterMap[i].first == c)
				{
					characterMap[i].second = s;
				}
			}
		}
	}
	formHuffmanCodes(root->left,s+"0");
	formHuffmanCodes(root->right,s+"1");
}
void Huffman_encoding_tree(vector<int> ascii_table, vector<int> ogfreq)
{
   Huffman_node *left,*right,*top1;
   for(int i = 0;i<ogfreq.size();i++)
   {
	   min_Heap.push(new Huffman_node(ogfreq[i],ascii_table[i]));
   }
   while(min_Heap.size()>1)
   {
	   left = min_Heap.top();
	   min_Heap.pop();

	   right = min_Heap.top();
	   min_Heap.pop();
	   
	   top1 = new Huffman_node(left->freq+right->freq,char(254));
	   

	   top1->left = left;
	   top1->right = right;
	   min_Heap.push(top1);
   }
   formHuffmanCodes(min_Heap.top(),s);
}
void print_Huffman_Tree(ostream &os, Huffman_node* root, int indent)
{
	if(root!=nullptr)
	{
		print_Huffman_Tree(os, root->left, indent+4);
		os<<setw(indent)<<" "<<(char)root->asci_let<<endl;
		print_Huffman_Tree(os, root->right, indent+4);
	}
}
int total_bits_written()
{
	string temp;
	int total_bits = 0;
	for (int i = 0; i < characterMap.size(); i++)
	{
		temp = characterMap[i].second;
		for (int j = 0; j < temp.length(); j++)
		{
			total_bits++;
		}
	}
	return total_bits;
}
int ct = 0;
void cal_compressionRatio()
{
	long double csize = 0;
	long double size = 0;
	long double cratio;
	long long lsize = 0;
	long long lcsize = 0;
	char ch;
	int x;
	x = characterMap.size();
	x = x - 1;
	lsize = x;
	lcsize = encoded_text.size() / 8;
	size = x * 8;
	size = size / 8;
	cout << "\nActual Size of File  = " << lsize << " Bytes";
	csize = encoded_text.size() / 8;
	cout << "\nSize of Compressed File = " << lcsize << " Bytes";
	cratio = (1-(csize / size)) * 100;
	cout << "\nCompression Rate achieved : " << cratio << "% !";
}
void forming_encoded_text()
{
	for(int i = 0;i < characterMap.size(); i++)
	{
		string x = characterMap[i].second;
		for(int j = 0; j < x.length(); j++)
		{
			if(x[j] == '0')
			{
				encoded_text.push_back(0);
			}
			else if(x[j] == '1')
			{
				encoded_text.push_back(1);
			}
		}
	}
	encoded_text.push_back(-1);
}
void write_code_file()
{
	ofstream oi;
	oi.open("codes.txt");
	for(int i = 0;i < code.size(); i++)
	{
		
		oi << code[i] << endl;
	}
	oi.close();
}

int main()
{
	int freq[256];
	for(int i = 0;i<256;i++)freq[i]=0;
	string file_name;
	vector<int> ogfreq;
	vector<int> ascii_table;

	cout<<"                 WELCOME TO OUR APPLICATION                  \n";
	cout<<"               ________ ____  ____ ___ _____                 \n";
	cout<<"              |__  /_ _|  _ ||  _ |_ _|_   _|                \n";
	cout<<"                / / | || |_) | |_) | |  | |                  \n";
	cout<<"               / /_ | ||  __/|  __/| |  | |                  \n";
	cout<<"              /____|___|_|   |_|  |___| |_|                  \n";


	cout << "PLEASE ENTER THE FILENAME OF THE ENCODED FILE,MAKE SURE ITS IN THE CURRENT DIRECTORY";

	cin >> file_name;

    frequency_calculator(freq,file_name);

	findingAscii(freq,ascii_table,ogfreq);

	printFreqTable(ascii_table,ogfreq);	

	Huffman_encoding_tree(ascii_table,ogfreq);
	
	write_code_file();

	forming_encoded_text();

	print_Huffman_Tree(cout,min_Heap.top(), 0);

	ofstream oi;
	int n = file_name.length();
	file_name[n-1] = 'p';
	file_name[n-2] = 'm';
	file_name[n-3] = 'c';

	oi.open(file_name, ios::binary);

    BitwiseWrite file_writer(oi);
	

	for(int i = 0; i<encoded_text.size(); i++)
	{
        int x = encoded_text[i];
        
		file_writer.writeBit(x);

	}

	cal_compressionRatio();                //To calculate compression ratio


    cout<<"COMPRESSION SUCCESSFUL";

	return 0;
	
}