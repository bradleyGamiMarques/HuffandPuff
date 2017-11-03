#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::hex;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::pow;
using std::setfill;
using std::setw;
using std::string;
using std::swap;
using std::uppercase;

// Functions taken from: https://stackoverflow.com/questions/25706991/appending-a-file-extension-in-a-c-progam
bool EndsWith(string const &, string const);
void AppendFileExtension(string & str, string const suffix);
// Function taken from: https://stackoverflow.com/questions/6417817/easy-way-to-remove-extension-from-a-filename
string RemoveFileExtension(const string& filename);

struct HuffTableEntry {
	int glyph;
	int left;
	int right;
	int frequency;
};

void BubbleSort(int, HuffTableEntry[]);
void CreateHuffmanTable(HuffTableEntry[], int);
void GetCodes(HuffTableEntry frequency_table[], HuffTableEntry node, string& code, string codes[]);
int MarkM(HuffTableEntry[], const int, const int);
void Reheap(HuffTableEntry[], int, int);

void main() {
	int length_of_file_name;
	string input_file_name;
	string output_file_name;
	cout << "Please enter a file name: ";
	getline(cin, input_file_name);
	output_file_name = input_file_name;
	length_of_file_name = input_file_name.length();
	ifstream fin(input_file_name, ios::binary);
	// Coach said that we can assume that the file exists. For that reason
	// I haven't written the code to check if the file does not exist.
	AppendFileExtension(output_file_name, ".huf");
	ofstream out(output_file_name, ios::binary | ios::out);

	//Create a frequency table
	long long glyph_frequency[257] = {0};
	char input[1];
	while (fin.read(input, 1)) {
		glyph_frequency[input[0]]++;
	}
	glyph_frequency[256] = 1; // End of file frequency.

	// Do the sorting stuff, and get the huffman table
	HuffTableEntry frequency_table[513];
	int index = 0;
	for (int i = 0; i < 257; i++) {
		if (glyph_frequency[i] != 0) {
			frequency_table[index].glyph = i;
			frequency_table[index].frequency = glyph_frequency[i];
			frequency_table[index].left = -1;
			frequency_table[index].right = -1;
			index++;
		}
	}
	int length = index;
	// Sort the frequency table.
	BubbleSort(length, frequency_table);

	//create the huffmantable
	CreateHuffmanTable(frequency_table, length);

	//generate Huffman codes
	string bitcodes[257] = { "" };
	string code = "";
	GetCodes(frequency_table, frequency_table[0], code, bitcodes);

	//write the header data (length of filename, filename, #huffman entries, huffman table)
	int numEntries = (length * 2) - 1;

	out.write((char *) &length_of_file_name, sizeof(length_of_file_name));
	out.write(input_file_name.c_str(), length_of_file_name);
	out.write((char *) &numEntries, sizeof(numEntries));

	for (int i = 0; i < numEntries; i++) {
		out.write((char *) &frequency_table[i].glyph, sizeof(frequency_table[i].glyph));
		out.write((char *) &frequency_table[i].left, sizeof(frequency_table[i].left));
		out.write((char *) &frequency_table[i].right, sizeof(frequency_table[i].right));
	}

	/// TODO: encode the data. and write it out

	//move back to the beginning of input file
	fin.clear();
	fin.seekg(0, fin.beg);

	char inputByte[1];
	int bitstringLength;
	int count = 0;
	unsigned char output_byte ='\0';
	while (fin.read(inputByte, 1)) {
		string bitstring = bitcodes[inputByte[0]];
		//encode the byte, write it out one byte at a time
		bitstringLength = bitstring.length();
		for (int i = 0; i < bitstringLength; i++) {
			if (bitstring[i] == '1') {
				output_byte = output_byte | (int)pow(2.0, count);
			}
			count++;
			if (count == 8) {
				//write the byte out to file
				//out.write((char *) &output_byte, 1);
				out << output_byte;
				//reset the byte
				output_byte = '\0';
				count = 0;
			}
		}
		//output value for testing
		cout << "right to left encoding: " << endl;
		cout << hex << setfill('0') << setw(2) << uppercase << int(output_byte) << endl;
	}

	//close the files
	fin.close();
	out.close();
}

inline bool EndsWith(string const & str, string const file_extension) {
	bool flag = false;
	if (str.size() < file_extension.size()) {
		return flag;
	}
	return 0 == str.compare(str.size() - file_extension.size(),
		file_extension.size(),
		file_extension,
		0,
		file_extension.size());
}

/* Name: AppendFileExtension
Inputs: input_file_name - A string passed by reference that contains the
the name of the file including the file extension.
file_extension - The file extension that you want to search for.

Description: A function that uses the the two helper functions EndsWith and,
RemoveFileExtension to check if the input_file_name contains the file_extension
that was passed into the function. If it does not then RemoveFileExtension is
called. Finally input_file_name has the file_extension you originally passed in
appended to the end of the string.
 */
inline void AppendFileExtension(string & input_file_name, string const file_extension)
{
	if (!EndsWith(input_file_name, file_extension)) {
		input_file_name = RemoveFileExtension(input_file_name);
		input_file_name += ".huf";
	}
}

inline string RemoveFileExtension(const string & filename)
{
	size_t last_dot = filename.find_last_of(".");
	if (last_dot == std::string::npos) {
		return filename;
	}
	return filename.substr(0, last_dot);
}

void BubbleSort(int n, HuffTableEntry huff_table[])
{
	bool did_swap = true;
	int num_pairs = n;
	HuffTableEntry temp;
	while (did_swap) {
		num_pairs--;
		did_swap = false;
		for (int index = 0; index < num_pairs; index++) {
			if (huff_table[index].frequency > huff_table[index + 1].frequency) {
				temp = huff_table[index];
				huff_table[index] = huff_table[index + 1];
				huff_table[index + 1] = temp;
				did_swap = true;
			}
		}
	}
}

void CreateHuffmanTable(HuffTableEntry frequency_table[], int length)
{
	int h = length - 1;
	int f = length;
	int m;

	do {
		// Choose the smaller frequency out of slots 1 and 2.
		if (h >= 2) {
			m = MarkM(frequency_table, 1, 2);
		}
		// Choose the left one if slot 2 is equal to the heap or if slot 2 past the heap.
		else if (h >= 1) {
			m = 1;
		}

		// Move m to f.
		swap(frequency_table[m], frequency_table[f]);

		if (m < h) {
			// Move h to m.
			swap(frequency_table[m], frequency_table[h]);
		}
		// Reheap?
		Reheap(frequency_table, m, h);

		// Move slot 0 to h.
		swap(frequency_table[0], frequency_table[h]);

		// Create a new frequency node
		frequency_table[0].glyph = -1;
		frequency_table[0].frequency = frequency_table[h].frequency + frequency_table[f].frequency;
		frequency_table[0].left = h;
		frequency_table[0].right = f;

		// Reheap?
		Reheap(frequency_table, 0, h);

		// Move h left.
		h--;
		// Move f right.
		f++;
	} while (f != 2 * length - 1); // You will have (2* number of entries in table) - 1 merge nodes.

}

inline int MarkM(HuffTableEntry table[], const int slot_one, const int slot_two)
{
	if (table[slot_one].frequency <= table[slot_two].frequency) {
		return slot_one;
	}
	else {
		return slot_two;
	}
}

void Reheap(HuffTableEntry frequency_table[], int m, int h)
{
	// A min heap is when the parent is less than or equal to its children.
	int head = (m - 1) / 2;
	int left_child = (m * 2) + 1;
	int right_child = (m * 2) + 2;
	// Check to see if the marked node m is >= h || if h == 1 This is our base case
	if (m >= h || h == 1) {
		// Do nothing! We're done!
		return;
	}
	// The goal of this statement is to get the largest frequency node into slot 0.
	if (frequency_table[head].frequency > frequency_table[m].frequency) {
		swap(frequency_table[head], frequency_table[m]);
		// We need to reheap because the largest node is not at slot 0.
		if (m != 0) {
			Reheap(frequency_table, head, h);
		}
	}
	else {
		// If not a min heap...
		// Check to see if the right child is not past the heap.
		// This is one of the ways we check to see if we need to stop.
		if (right_child < h) {
			// If we go into this statement we do not have a min heap.
			// Because both of the children are greater than the parent.
			if (frequency_table[m].frequency > frequency_table[left_child].frequency &&
				frequency_table[m].frequency > frequency_table[right_child].frequency) {
				// Compare the left child to the right child.
				if (frequency_table[left_child].frequency <= frequency_table[right_child].frequency) {
					// Swap the parent with the left_child because it was
					// smaller than the right_child
					swap(frequency_table[m], frequency_table[left_child]);
					Reheap(frequency_table, left_child, h);
				}
				else {
					// Swap the parent with the right_child.
					swap(frequency_table[m], frequency_table[right_child]);
					Reheap(frequency_table, right_child, h);
				}
			}
			// If we go into this statement we do not have a min heap.
			// Because one of the children are greater than the parent.
			else if (frequency_table[m].frequency > frequency_table[left_child].frequency ||
				frequency_table[m].frequency > frequency_table[right_child].frequency) {
				// Figure out which one of the children is the smallest.
				// We have a function which does that for us MarkM().
				int minimum = MarkM(frequency_table, left_child, right_child);
				swap(frequency_table[m], frequency_table[minimum]);
				Reheap(frequency_table, minimum, h);
			}
		}
		// If the right child was past the heap we should check and see if
		// the left child is past the heap.
		else if (left_child < h) {
			if (frequency_table[m].frequency > frequency_table[left_child].frequency) {
				swap(frequency_table[m], frequency_table[left_child]);
			}
		}
	}
}

inline void GetCodes(HuffTableEntry frequency_table[], HuffTableEntry node, string& code, string bitcodes[]) {
	//depth first search, recursive

	if (node.glyph != -1) {
		//it's a frequency node, save the code into the array of bitcodes
		bitcodes[node.glyph] = code;
	}
	else {
		//it's a merge node
		//add a '1' to code string if go right, and add a '0' if go left
		if (node.left != -1) {
			code.append("0");
			GetCodes(frequency_table, frequency_table[node.left], code, bitcodes);
			code.erase(code.size() - 1, 1);
		}
		if (node.right != -1) {
			code.append("1");
			GetCodes(frequency_table, frequency_table[node.right], code, bitcodes);
			code.erase(code.size() - 1, 1);
		}
	}
}
