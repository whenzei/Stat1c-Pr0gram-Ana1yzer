#include "pql_parser.h"
#include "declaration.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

PqlParser::PqlParser(string content) {
	if (content != "") Parse(content);
	else cout << "Query cannot be empty!" << endl;
}

void PqlParser::Parse(string content) { 
	if (content == "") {
		cout << "Query cannot be empty." << endl;
		return;
	}

	vector<string> statements;

	size_t last = 0;
    size_t next = 0;
    while ((next = content.find(';', last)) != string::npos) {
        statements.push_back(content.substr(last, next - last));
        last = next + 1;
    }

	if (statements.size() == 0) {
      cout << "Invalid query syntax. Possibly missing semicolon." << endl;
      return;
	}

	query = Query();

	for (vector<string>::const_iterator i = statements.begin(); i != statements.end(); ++i) {
		ParseStatement(*i);
    }
}

void PqlParser::ParseStatement(string statement) { 
	if (statement == "") {
		cout << "Invalid query syntax." << endl;
		return;
	}

	vector<string> tokens;

	std::stringstream stream(statement);
	string token;
	while (std::getline(stream, token, ' ')) {
		if (token != "") tokens.push_back(token);
	}

	if (tokens.size() == 0 || tokens.size() > 2) {
        cout << "Invalid query syntax." << endl;
        return;
    }

	string keyword = tokens[0];
	if (keyword == "Select") {
		query.SetVarName(tokens[1]);
	} else if (keyword == "assign") {
          Declaration declaration(kAssign, tokens[1]);
          query.AddDeclaration(declaration);
	} else {
          cout << "Invalid query keyword." << endl;    
	}
}