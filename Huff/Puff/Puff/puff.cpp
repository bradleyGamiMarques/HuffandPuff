#include<fstream>
#include<iostream>
#include<string>
using std::cin;
using std::cout;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;

struct HuffTableEntry {
	int glyph;
	int left;
	int right;
};

void main() {
	int length_of_filename = 0;
	int number_of_huffman_entries = 0;
	string name_of_file = "";
	cout << "File name: ";
	cin >> name_of_file;
	ifstream fin(name_of_file, ios::in | ios::binary);
	// Read the file name length
	fin.read((char*)&length_of_filename, sizeof(length_of_filename));
	//read the filename
	string filename;
	fin.read((char *)&filename, length_of_filename);
	// Read the # of Huffman Table entries
	fin.read((char*)&number_of_huffman_entries, sizeof number_of_huffman_entries);
	HuffTableEntry* huffman_tree = new HuffTableEntry[number_of_huffman_entries];

	//read the entries
	for (int i = 0; i < number_of_huffman_entries; i++) {
		HuffTableEntry entry;
		fin.read((char *)&entry, sizeof(entry));
		huffman_tree[i].glyph = entry.glyph;
		huffman_tree[i].left = entry.left;
		huffman_tree[i].right = entry.right;
	}

	//decode the data

}