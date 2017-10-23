#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::map;
using std::pair;
using std::string;
using std::vector;

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
int MarkM(HuffTableEntry[], const int, const int);

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


	/// TODO: Create a frequency table
	// type char is the glyph and type int is the frequency of the glyph.
	map<char, int> frequency_map;
	string input;
	getline(fin, input);
	for(char c: input) {
		auto iter = frequency_map.find(c);
		if (iter == frequency_map.end()) {
			// Didn't find the char in the map so we need to insert it into the map.
			frequency_map.insert(pair<char, int>(c,1));
		}
		else {
			// We found it so increment it in the map frequency_table
			frequency_map[c]++;
		}
	}
	
	/// TODO: Do the sorting stuff, and get the huffman table
	HuffTableEntry frequency_table[513];
	int index = 0;
	for (map<char, int>::iterator iter = frequency_map.begin(); iter != frequency_map.end(); ++iter) {
		frequency_table[index].glyph = iter->first;
		frequency_table[index].left = -1;
		frequency_table[index].right = -1;
		frequency_table[index].frequency = iter->second;
		index++;
	}
	// Add in the end of file character.
	frequency_table[index].glyph = 256;
	frequency_table[index].left = -1;
	frequency_table[index].right = -1;
	frequency_table[index].frequency = 1;

	// Sort the frequency table.
	BubbleSort(index + 1, frequency_table);
	/// TODO: Write all the header stuff to the file, and encode the data.
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
		// Mark m.
		if (h >= 2) {
			m = MarkM(frequency_table, 1, 2);
		}
		else if (h >= 1) {
			m = 1; // If we've reached this point always choose the left.
		}
		// Move frequency_table[m] to frequency_table[f].
		frequency_table[m] = frequency_table[f];
		// Move frequency_table[h] to frequency_table[m].
		frequency_table[h] = frequency_table[m];
		// Reheap?

		// Move frequency_table[0] to frequency_table[h].
		frequency_table[h] = frequency_table[0];
		// Create a merge frequency node at frequency_table[0].

		// Reheap?

		// Move H left and F right.
		h--;
		f++;
	} while (f != 2 * length - 1);
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

