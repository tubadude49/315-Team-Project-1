// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
// CSCE 315

#include <iostream>
#include <fstream>
#include "DBMSAPI.h"

vector<string> splitAtSpace(string splitThis);

string displayMainMenu();
void insertValue(string relation, vector<string> values, DBMSAPI* dbms);
void initRelations(DBMSAPI* dbms);
void saveRelations(DBMSAPI* dbms);
void showRelation(string relation, DBMSAPI* dbms);

int main() {
	DBMSAPI dbms;

	initRelations(&dbms);
	cout << displayMainMenu() << endl << endl;
	
	while (1) {
		string in;
		getline(cin, in);

		vector<string> tokens = splitAtSpace(in);

		if (tokens[0] == "help") {
			cout << displayMainMenu() << endl;
		}
		else if (tokens[0] == "EXIT") {
			saveRelations(&dbms);
			exit(0);
		}
		else if (tokens[0] == "ADD") {
			if (tokens[1] == "CAR") {
				vector<string> values;
				for (int i = 2; i < tokens.size(); i++) {
					values.push_back(tokens[i]);
				}
				insertValue("cars", values, &dbms);
			}
			else if (tokens[1] == "DEALERSHIP") {
				vector<string> values;
				for (int i = 2; i < tokens.size(); i++) {
					values.push_back(tokens[i]);
				}
				insertValue("dealerships", values, &dbms);
			}
		}
		else if (tokens[0] == "LIST") {
			/*vector<string> values;
			for (int i = 1; i < tokens.size(); i++) {
				values.push_back(tokens[i]);
			}*/
		}
		else if (tokens[0] == "SELL") {

		}
		else if (tokens[0] == "CARS") {
			showRelation("cars", &dbms);
		}
		else if (tokens[0] == "DEALERSHIPS") {
			showRelation("dealerships", &dbms);
		}

	}
}

vector<string> splitAtSpace(string splitThis) {
	vector<string> split;
	int lastPos = 0;
	int pos = splitThis.find(" ", lastPos);
	while (pos > 0 && pos < splitThis.size()) {
		split.push_back(splitThis.substr(lastPos, pos-lastPos));
		lastPos = pos + 1;
		pos = splitThis.find(" ", lastPos);
	}
	split.push_back(splitThis.substr(lastPos, splitThis.size()-lastPos));

	return split;
}

string displayMainMenu() {
	return
		"USE THE FOLLOWING COMMANDS TO PERFORM AN ACTION:\n\n"
		"CARS - Show the cars loaded into the database.\n"
		"DEALERSHIPS - Show the dealerships loaded into the database.\n"
		"LISTINGS - Show the listings loaded into the database.\n"
		"SALES - Show the sales loaded into the database.\n"
		"ADD CAR <Values>.\n"
		"ADD DEALERSHIP <Values>.\n"
		"LIST <Car_VIN>.\n"
		"SELL <Car_VIN>.\n"
		"SEARCH <Relation_Name> <Condition(s)> - Search a relation bounded by a conditional statement.\n"
		"EXIT - Exit the program.\n";
}

void insertValue(string relation, vector<string> values, DBMSAPI* dbms) {
	string request = "INSERT INTO " + relation + " VALUES FROM ";
	for (int i = 0; i<values.size(); i++) {
		request += values[i];
		if (i + 1 < values.size()) { request += ", "; }
	}
	//cout << "Sent req: " << request << endl;
	dbms->execute(request);
}

void initRelations(DBMSAPI* dbms) {
	string request = "OPEN dealerships;OPEN cars;OPEN listings;OPEN sales;";
	dbms->execute(request);
}

void saveRelations(DBMSAPI* dbms) {
	string request = "WRITE dealerships;WRITE cars;WRITE listings;WRITE sales;";
	dbms->execute(request);
}

void showRelation(string relation, DBMSAPI* dbms) {
	string request = "SHOW " + relation + ";";
	dbms->execute(request);
}