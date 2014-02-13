// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "DMLParser.h"

#include <iostream>
#include <algorithm>

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
	if ((input.size() == 0) || (input.at(input.size() - 1) != ';'))
	{
		cerr << "Invalid command. Missing ;\n";
		return splitString;
	}
	input = input.substr(0, input.size() - 1);//Remove semicolon
	std::replace(input.begin(),input.end(),',', ' ');
	stringstream reader(input);

	while (reader)//No eof or errors
	{
		string next = "";
		reader >> next;
		if (next.size() <= 0)
		{
			break;
		}

		if ((next.size() > 1) && next.find("(") == 0)//Seperate open paren from following data
		{
			splitString.push_back("(");
			next = next.substr(1);
		}
		if (next.find("\"") == 0)//Objects enclosed in quotes are one string 
		{
			next = next.substr(1);
			while (reader.peek() != 34)
			{
				next += reader.get();
			}
			reader.get();
		}

		if (next.size() > 1 && next.find("V") != 0 && next.find(")") == next.size() - 1)//Seperate ) if not VARCHAR
		{
			next = next.substr(0, next.size() - 1);
			splitString.push_back(next);
			splitString.push_back(")");
			continue;
		}

		splitString.push_back(next);
		if (next == "INSERT" || next == "CREATE" || next == "VALUES" || next == "PRIMARY")//INSERT INTO,CREATE TABLE, VALUES FROM, & PRIMARY KEY are one token
		{
			reader >> next;
			splitString[splitString.size() - 1] += " " + next;
		}
	}
	return splitString;
}
