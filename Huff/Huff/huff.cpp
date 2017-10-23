#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using std::cin;
using std::cout;
using std::ifstream;
using std::ios;
using std::map;
using std::string;
using std::vector;

// Functions taken from: https://stackoverflow.com/questions/25706991/appending-a-file-extension-in-a-c-progam
bool EndsWith(string const &, string const);
void AppendFileExtension(string & str, string const suffix);

// Function taken from: https://stackoverflow.com/questions/6417817/easy-way-to-remove-extension-from-a-filename
string RemoveFileExtension(const string& filename);

void BubbleSort(int, HuffTableEntry[]);
struct HuffTableEntry {
	int glyph;
	int left; 
	int right;
	int frequency;
};
void main() {
	int length_of_file_name;
	string input_file_name;
	string output_file_name;
	cout << "Please enter a file name: ";
	getline(cin, input_file_name);
	output_file_name = input_file_name;
	length_of_file_name = input_file_name.length();
	ifstream fin(input_file_name, ios::binary);
	// Coach said that we can assume that the file exists. For that reason I
	// haven't written the code to check if the file does not exist.
	AppendFileExtension(output_file_name, ".huf");


	/// TODO: Create a frequency table
	// char is glyph and int is the frequency
	map<char, int> frequency_table;
	char buffer[8192];
	// MAYBE fin.read(buffer,);
	/// TODO: Do the sorting stuff, and get the huffman table
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
			if (huff_table[index].frequency < huff_table[index + 1].frequency) {
				temp = huff_table[index];
				huff_table[index] = huff_table[index + 1];
				huff_table[index + 1] = temp;
				did_swap = true;
			}
		}
	}
}

