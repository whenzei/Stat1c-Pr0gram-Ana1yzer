#include "util.h"

#include <fstream>

using std::ifstream;
using std::istreambuf_iterator;

bool Util::IsValidFile(string filepath) {
	ifstream infile(filepath);
	return infile.good();
}

string Util::ReadContentFromFile(string filepath) {
	if (!IsValidFile(filepath)) {
		std::cout << "File not found! Content is set to empty string" << std::endl;
		return string();
	}
	// start reading file
	ifstream ifs(filepath);
	return string((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));
}
