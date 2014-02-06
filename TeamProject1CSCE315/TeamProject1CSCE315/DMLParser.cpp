// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "DMLParser.h"

#include <iostream>

/* 	Main implementation of the DMLParser.
All functionality of the parser is here.
*/
void DMLParser::parse(string &line, string &response) {
	//I was unsure which of the 2 inputs were actually the "command line". Replace "input" with either "line" or "response"
	//this code has been tested in a controlled environment, given a command such as "Parse<<This==String&&Please", will
	//produce "parsedString", the vector, containing { "Parse", "This", "String", "Please" } 
	string output;
	size_t found = input.find_first_of(" ,&<="); //READ THE ABOVE COMMENT BEFORE CHANGING THIS ERROR
	while (found != string::npos)
	{
		input[found] = '*';
		found = input.find_first_of(" ,&<=", found + 1);
	}
	stringstream reader(input); //READ THE ABOVE COMMENT BEFORE CHANGING THIS ERROR
	while (getline(reader, output, '*')){
		if (output == "") {
		}
		else{
			parsedString.push_back(output);
		}

	}

	for (int i = 0; i < parsedString.size(); i++) {
		cout << parsedString[i] << "\n";
	}

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