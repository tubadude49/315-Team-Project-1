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
0x4: missing keywords "VALUES FROM" in "INSERT INTO"
0x5: Unknown command, argument
0x6: missing opening "("
0x7: missing keyword "SET" in "UPDATE"
0x8: missing keyword "WHERE" in "UPDATE"
0x9: missing keyword "PRIMARY KEY" in "CREATE TABLE"
0xA: missing keyword "WHERE" in "DELETE FROM"

0xB: std::invalid_argument was thrown by DMLParser::parse

0xE: not yet implemented
*/
int DMLParser::parse(string &line) {
	//add try/catch for bounds checking (std::invalid_argument)
	vector<string> tokens = split(line);
	
	if (tokens[0] == "EXIT") {
		return 0x1;
	}
	else if (tokens[0] == "UPDATE") {
		string outputRelation = tokens[1];
		if (tokens[2] == "SET") {
			vector<string> setArgs;
			for (int i = 3; i < 6; i++) {
				setArgs.push_back(tokens[i]);
			}			
			if (tokens[6] == "WHERE") {
				vector<string> booleanArgs;
				for (int i = 7; i < 10; i++) {
					booleanArgs.push_back(tokens[i]);
				}
				dataManager->update(outputRelation, setArgs, booleanArgs);
			}
			else {
				return 0x8;
			}
		}
		else {
			return 0x7;
		}
	}
	else if (tokens[0] == "SHOW") {
		string outputRelation = tokens[1];
		dataManager->show(outputRelation, cout);
	}
	else if (tokens[0] == "WRITE") {
		string outputRelation = tokens[1];
		dataManager->relationToFile(outputRelation);		
	}
	else if (tokens[0] == "OPEN") {
		string outputRelation = tokens[1];
		parseProgram(dataManager->relationFromFile(outputRelation));
	}
	else if (tokens[0] == "CREATE TABLE") {
		string outputRelation = tokens[1];
		if (tokens[2] == "(") {
			vector<string> attrNames;
			vector<string> attrTypes;
			int resumeAt;
			for (int i = 3; tokens[i] != ")"; i+=2) {
				attrNames.push_back(tokens[i]);
				attrTypes.push_back(tokens[i + 1]);
				resumeAt = i + 3;
			}
			string primaryKey;
			if (tokens[resumeAt] == "PRIMARY KEY") {
				primaryKey = tokens[resumeAt + 2];
			}
			else {
				return 0x9;
			}
			dataManager->create(outputRelation, attrNames, attrTypes, primaryKey);
		}
		else {
			return 0x6;
		}
	}
	else if (tokens[0] == "INSERT INTO") {
		string outputRelation = tokens[1];
		if (tokens[2] == "VALUES FROM") {
			if (tokens[3] == "(") {
				vector<string> values;
				for (int i = 4; tokens[i] != ")"; i++) {
					values.push_back(tokens[i]);
				}
				dataManager->insert(outputRelation, values);
			}
			else if (tokens[3] == "RELATION") {
				string o = parseComplex(tokens, 5, outputRelation);
			}
			else {
				return 0x6;
			}
		}
		else {
			return 0x4;
		}
	}
	else if (tokens[0] == "DELETE FROM") {
		string outputRelation = tokens[1];
		if (tokens[2] == "WHERE") {
			vector<string> booleanArgs;
			if (tokens[3] == "(") {
				for (int i = 3; tokens[i] != ")" && i < tokens.size(); i++) {
					booleanArgs.push_back(tokens[i]);
				}
			}
			else {
				for (int i = 3; tokens[i] != ";" && i < tokens.size(); i++) {
					booleanArgs.push_back(tokens[i]);
				}				
			}
			dataManager->del(outputRelation, booleanArgs); // this will not work
		}
		else {
			return 0xA;
		}
	}
	else if (tokens[1] == "<-") {
		string outputRelation = tokens[0];
		if (tokens[2] == "project" || tokens[2] == "rename" || tokens[2] == "select") {
			string o = parseComplex(tokens, 2, outputRelation);
		}	
		else {
			string firstRelation = tokens[2];
			string op = tokens[3];
			string secondRelation = tokens[4];
			//cout << firstRelation << " " << op << " " << secondRelation << " = " << outputRelation << endl;
			if(op == "+") {
				dataManager->setUnion(firstRelation, secondRelation, outputRelation);
			}
			else if (op == "-") {				
				dataManager->setDifference(firstRelation, secondRelation, outputRelation);
			}
			else if(op == "*") {
				dataManager->crossProduct(firstRelation, secondRelation, outputRelation);
			}
			else if(op == "JOIN") {
				dataManager->naturalJoin(firstRelation, secondRelation, outputRelation);
			}
			else {
				return 0x3;
			}
		}
	}
	else {
		return 0x5;
	}
		
	//cout << "[DMLParser]: " << line << endl;
	return 0x0;
}

string DMLParser::parseComplex(vector<string> tokens, int startAt, string destRelation) {
	/*for (int i = startAt; i < tokens.size(); i++) {
		cout << tokens[i] << ",";
	}
	cout << endl;*/
	if (tokens[startAt] == "(") {
		if (tokens[startAt + 1] == "project" || tokens[startAt + 1] == "select" || tokens[startAt + 1] == "rename") {
			string operation = tokens[startAt + 1];
			vector<string> args;
			int startIncr = 0;
			for (int i = startAt + 3; tokens[i] != ")" && i < tokens.size(); i++) {
				args.push_back(tokens[i]);
				startIncr = i + 2;
			}
			string source = parseComplex(tokens, startIncr, destRelation);
			if (operation == "project") {
				dataManager->project(source, destRelation, args);
			}
			else if (operation == "select") {
				dataManager->select(source, destRelation, args);
			}
			else {
				dataManager->rename(source, destRelation, args);
			}
			return destRelation;
		}
		else {
			string firstRelation = tokens[startAt + 1];
			string op = tokens[startAt + 2];
			string secondRelation = tokens[startAt + 3];
			if (op == "+") {
				dataManager->setUnion(firstRelation, secondRelation, destRelation);
			}
			else if (op == "-") {
				return 0x0;
				dataManager->setDifference(firstRelation, secondRelation, destRelation);
			}
			else if (op == "*") {
				dataManager->crossProduct(firstRelation, secondRelation, destRelation);
			}
			else if (op == "JOIN") {
				dataManager->naturalJoin(firstRelation, secondRelation, destRelation);
			}
			return destRelation;
		}
	}
	else if (tokens[startAt] == "project" || tokens[startAt] == "select" || tokens[startAt] == "rename") {
		startAt--;
		string operation = tokens[startAt + 1];
		vector<string> args;
		int startIncr = 0;
		for (int i = startAt + 3; tokens[i] != ")" && i < tokens.size(); i++) {
			args.push_back(tokens[i]);
			startIncr = i + 2;
		}
		string source = parseComplex(tokens, startIncr, destRelation);
		if (operation == "project") {
			dataManager->project(source, destRelation, args);
		}
		else if (operation == "select") {
			dataManager->select(source, destRelation, args);
		}
		else {
			dataManager->rename(source, destRelation, args);
		}
		return destRelation;
	}
	else {
		return tokens[startAt];
	}
	
}

DMLParser::DMLParser(DataManager* dataManager) {
	DMLParser::dataManager = dataManager;
}

DMLParser::~DMLParser() {
}

int DMLParser::parseProgram(string &programs) {
	vector<string> program = splitProgram(programs);
	try {
		for (string line : program) {
			int error;
			if (error = parse(line)) {
				return error;
			}
		}
	}
	catch (std::invalid_argument) {
		return 0xA;
	}
	return 0x0;
}

vector<string> DMLParser::splitProgram(string &input) {
	stringstream reader(input);
	vector<string> splitString;
	while (reader)
	{
		string next = "";
		while (isspace(reader.peek()))
		{
			reader.get();
		}
		while (reader && reader.peek() != ';')
		{
			next += reader.get();
		}
		char c = reader.get();
		if (c == ';')
		{
			next += c;
		}
		else
		{
			break;
		}
		
		splitString.push_back(next);
	}
	return splitString;
}

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
			int lastQ = next.find_last_of("\"");
			if (lastQ != 0)//no spaces
			{
				splitString.push_back(next.substr(1, lastQ - 1));
				string remainder = next.substr(lastQ + 1);
				if (remainder != "")
				{
					splitString.push_back(remainder);
				}
				continue;
			}

			next = next.substr(1);
			while (reader.peek() != 34)
			{
				next += reader.get();
			}
			reader.get();
		}

		if (next.size() > 1 && next.find_last_of(")") == next.size() - 1)//Seperate ) if not VARCHAR
		{
			if (next.find("V") != 0)
			{
				next = next.substr(0, next.size() - 1);
			}
			else if (next.find(")") == next.find_last_of(")"))
			{
				splitString.push_back(next);
				continue;
			}
			else
			{
				next = next.substr(0, next.find(")"));
			}
			splitString.push_back(next);
			splitString.push_back(")");
			continue;
		}
		
		splitString.push_back(next);
		if (next == "INSERT" || next == "CREATE" || next == "VALUES" || next == "PRIMARY" || next == "DELETE")//INSERT INTO,CREATE TABLE, VALUES FROM, & PRIMARY KEY are one token
		{
			reader >> next;
			splitString[splitString.size() - 1] += " " + next;
		}
	}
	return splitString;
}
