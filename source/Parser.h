
#pragma once

#include <vector>

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

/**
 * The parser class for the front end component
 * Used to parse file contents and pass the parsed content to the PKB component
 */
class Parser {
	std::string content;
public:
	/* Constructor */
	Parser(std::string filepath);

	///* Destructor */
	//~Parser();

	/**
	 * Reads the content of the file at the location specified by the file path
	 * and returns the file contents if available
	 * @param filepath the filepath of the file to be parsed
	 * @returns file content as a string if file exists, or empty string otherwise
	 */
	std::string readContentFromFile(std::string filepath);

	void parse();

private:
	/**
	 * Checks if file can be located at given file path
	 * @param filepath the filepath of the file to be located
	 * @returns true if file exists, false otherwise
	*/
	bool isValidFile(std::string filepath);
};
	
#endif // !SPA_PARSER_H
