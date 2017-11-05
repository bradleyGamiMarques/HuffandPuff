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
	ifstream fin(name_of_file, ios::binary);
	// Read the file name;
	fin.read((char*)&length_of_filename, sizeof(length_of_filename));
	char* filename = new char[length_of_filename + 1];
	fin.read(filename, length_of_filename);
	filename[length_of_filename] = '\0';
	// Read the Huffman Table
	fin.read((char*)&number_of_huffman_entries, sizeof number_of_huffman_entries);
	HuffTableEntry* huffman_tree = new HuffTableEntry[number_of_huffman_entries];
}