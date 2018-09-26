#pragma once

#ifndef SPA_UTIL_H
#define SPA_UTIL_H

#include <iostream>
using std::string;

class Util {
public: 
	static bool IsValidFile(string filepath);
	static string ReadContentFromFile(string filepath);
};
#endif // !SPA_UTIL_H
