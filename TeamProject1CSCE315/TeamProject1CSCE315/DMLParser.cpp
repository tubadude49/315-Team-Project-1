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
void DMLParser::parseInput(string &input){
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
}