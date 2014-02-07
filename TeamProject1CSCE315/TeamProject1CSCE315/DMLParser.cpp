// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "DMLParser.h"

#include <iostream>

/* 	Main implementation of the DMLParser.
All functionality of the parser is here.
*/
void DMLParser::parse(string &line, string &response) {
	//split keywords and variables based on terminal characters (i.e.) ' ', '&', '<', '*', ',', etc.
	//should we use vector<string> or string[]? <---------------------------------------------------
	//we need a split function either way^	
	cout << "[DMLParser]: " << line << endl;
}

DMLParser::DMLParser(DataManager* dataManager) {
	DMLParser::dataManager = dataManager;
}

DMLParser::~DMLParser() {
}

void DMLParser::parseProgram(string &program, string &response) {
	//split the lines of the program based on semicolons (;)
	//loop through for loop the lines and parse one at a time
	//for(;;) {
	string nuller = "";
	parse(nuller, response);
	//}
	response = "Completed!";
	// please don't run the above
}

static string terminalChars = " ,&<>=!()"; //TODO
static string removeChars = " ";
//static vector<char> terminalChars = { ',', '&', '<', '>', '=', '!', '(', ')' };
//static vector<char> removeChars = { ' ' };

vector<string> DMLParser::split(string &input){
	vector<string> splitString;
	size_t found = input.find_first_of(terminalChars);
	while (found != string::npos)
	{		
		input[found] = '*';
		found = input.find_first_of(removeChars, found + 1);
	}
	stringstream reader(input);
	string tmp;
	while (getline(reader, tmp, '*')){
		if (tmp != "") {
			splitString.push_back(tmp);
		}
	}
	return splitString;
}
