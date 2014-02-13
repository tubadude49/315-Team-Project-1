// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "DMLParser.h"

#include <iostream>
#include <algorithm>

/* 	Main implementation of the DMLParser.
All functionality of the parser is here.
*/

/*
Parse Response Codes

0x0: Program returns without any non-0 responses.
0x1: Program encountered EXIT Command
0x2: parse(line) encountered an assignment which did not include "<-" as its second argument
0x3: invalid operator sent
0x4: missing keywords "VALUES" and "FROM" in "INSERT INTO"
0x5: missing ; at end of statement
0x6: missing opening "("

0xE: not yet implemented
*/
int DMLParser::parse(string &line) {
	//add try/catch for bounds checking (std::invalid_argument)
	vector<string> tokens = split(line);
	
	if (tokens[0] == "EXIT") { // IMPL
		return 0x1;
	}
	else if (tokens[0] == "UPDATE") {
		return 0xE;
	}
	else if (tokens[0] == "SHOW") {
		return 0xE;
	}
	else if (tokens[0] == "WRITE") {
		string outputRelation = tokens[1];
		return 0xE;
	}
	else if (tokens[0] == "OPEN") {
		string outputRelation = tokens[1];
		return 0xE;		
	}
	else if ((tokens[0] == "CREATE") && (tokens[1] == "TABLE")) { // IMPL
		//CREATE TABLE
		string outputRelation = tokens[2];
		if (tokens[3] == "(") {
			vector<string> tuple;
			for (int i = 4; tokens[i] != ")"; i++) {
				tuple.push_back(tokens[i]);
			}
			dataManager->insert(outputRelation, tuple);
			dataManager->addBuildCmd(outputRelation, line);
		}
		else {
			return 0x6;
		}
	}
	else if ((tokens[0] == "INSERT") && (tokens[1] == "INTO")) {
		string outputRelation = tokens[2];
		if ((tokens[3] == "VALUES") && (tokens[4] == "FROM")) {
			if (tokens[5] == "(") {
				for (int i = 6; tokens[i] != ")"; i++) {
					
				}
			}
		}
		else {
			return 0x4;
		}
		return 0xE;
	}
	else if ((tokens[0] == "DELETE") && (tokens[1] == "FROM")) {

	}
	else {
		string outputRelation = tokens[0];
		if (tokens[1] != "<-") return 0x2;
		if (tokens[2] == "project") {
			//PROJECT
			return 0xE;
		}
		else if (tokens[2] == "rename") {
			//RENAME
			return 0xE;
		}
		else if (tokens[2] == "select") {
			//SELECT
			return 0xE;
		}
		else {
			string firstRelation = tokens[2];
			string op = tokens[3];
			string secondRelation = tokens[4];
			if(op == "+") {
				dataManager->setUnion(firstRelation, secondRelation, outputRelation);
				dataManager->addBuildCmd(outputRelation, line);
				if (tokens[5] != ";") return 0x5;
			}
			else if(op == "-") {
				dataManager->setDifference(firstRelation, secondRelation, outputRelation);
				dataManager->addBuildCmd(outputRelation, line);
				if (tokens[5] != ";") return 0x5;
			}
			else if(op == "*") {
				dataManager->crossProduct(firstRelation, secondRelation, outputRelation);
				dataManager->addBuildCmd(outputRelation, line);
				if (tokens[5] != ";") return 0x5;
			}
			else if(op == "JOIN") {
				dataManager->naturalJoin(firstRelation, secondRelation, outputRelation);
				dataManager->addBuildCmd(outputRelation, line);
				if (tokens[5] != ";") return 0x5;
			}
			else {
				return 0x3;
			}
		}
	}
	
	
	cout << "[DMLParser]: " << line << endl;
}

DMLParser::DMLParser(DataManager* dataManager) {
	DMLParser::dataManager = dataManager;
}

DMLParser::~DMLParser() {
}

int DMLParser::parseProgram(string &programs) {
	vector<string> program = splitProgram(programs);
	for(string line : program) {
		int error;
		if (error = parse(line)) {
			return error;
		}
	}
	return 0x0;
}

/*assumptions: takes in a command string such as "SHOW animals;\nWRITE animals;\nOPEN animals;", and 
produces a vector<string> that contains { "SHOW animals", "WRITE animals", "OPEN animals" }*/
vector<string> DMLParser::splitProgram(string &input) {
	vector<string> commands;
	stringstream copyOfInput(input);
	string reader;

	while (!copyOfInput.eof()) {
		getline(copyOfInput, reader);
		commands.push_back(reader);
	}

	for (int i = 0; i < commands.size(); i++) {
		reader = commands[i];
		commands.push_back(reader.substr(0, reader.size() - 1));
	}

	return commands;
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
