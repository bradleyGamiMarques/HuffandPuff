#include<cmath>
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
	char* filename = new char[length_of_filename + 1];
	fin.read(filename, length_of_filename);
	filename[length_of_filename] = '\0';
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

	//create output file
	ofstream fout(filename, ios::out | ios::binary);

	//decode the data
	//read in data, start at root of tree, if bit is 0 go left, if 1 go right, 
	//if get to glyph write it out and start back at root of tree for next bit

	string large_decoded_string = "";
	unsigned char inputbyte;
	short bitpos = 0;
	int tablepos = 0;
	bool foundEOF = false;

	fin.read((char*) &inputbyte, sizeof(inputbyte));

	while (!foundEOF) {
		if (huffman_tree[tablepos].glyph == -1) {
			if (inputbyte & (short)pow(2.0, bitpos)) {
				//it's a 1, go right
				tablepos = huffman_tree[tablepos].right;
			}
			else {
				//it's a 0, go left
				tablepos = huffman_tree[tablepos].left;
			}

			//go to the next byte
			bitpos++;
			if (bitpos == 8) {
				//read in a new byte
				fin.read((char*) &inputbyte, sizeof(inputbyte));
				bitpos = 0;
			}
		}
		else if (huffman_tree[tablepos].glyph == 256) {
			//found the end of the file, stop looping, write out the big string
			foundEOF = true;
			//fout.write((char *) &large_decoded_string, large_decoded_string.size());
			fout.write(large_decoded_string.c_str(), large_decoded_string.size());
		}
		else {
			//it's a glyph, append it to the large string and write out when done.
			//if (huffman_tree[tablepos].glyph != 10) {
			//	fout << (char)huffman_tree[tablepos].glyph;
			//}
			large_decoded_string += (unsigned char)huffman_tree[tablepos].glyph;
			tablepos = 0;
		}	
	}

	//close the files
	fin.close();
	fout.close();

	//deallocate dynamic memory
	delete filename;
	delete huffman_tree;
}
