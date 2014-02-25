// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
// CSCE 315

#include <iostream>
#include <fstream>
#include "DBMSAPI.h"

/* Split a given string into a vector<string>. Split at space. */
vector<string> splitAtSpace(string splitThis);

/* Return the string constant that holds the main menu display. */
string displayMainMenu();
/* Construct a request to the dbms api to insert a value into a relation. */
void insertValue(string relation, vector<string> values, DBMSAPI* dbms);
/* use the OPEN command to read from files to open the databases. */
void initRelations(DBMSAPI* dbms);
/* use the WRITE command to save to files the opened databases. 
Used at the end of program execution. */
void saveRelations(DBMSAPI* dbms);
/* send a SHOW command to the dbms api to print out a relation. */
void showRelation(string relation, DBMSAPI* dbms);

int main() {
	DBMSAPI dbms;	//Declare the dbms api

	initRelations(&dbms);
	cout << displayMainMenu() << endl;

	int searchResults = 0;
	
	while (1) {		//input loop
		string in;
		getline(cin, in);

		vector<string> tokens = splitAtSpace(in);	//split the command given by spaces

		if (tokens[0] == "HELP") {				//process HELP
			cout << displayMainMenu() << endl;
		}
		else if (tokens[0] == "EXIT") {			//process EXIT
			saveRelations(&dbms);
			exit(0);
		}
		else if (tokens[0] == "ADD") {			
			if (tokens[1] == "CAR") {			//process ADD CAR
				vector<string> values;
				for (int i = 2; i < tokens.size(); i++) {
					values.push_back(tokens[i]);
				}
				insertValue("cars", values, &dbms);
				cout << "Addition approved!" << endl << endl;
			}
			else if (tokens[1] == "DEALERSHIP") {//process ADD DEALERSHIP
				vector<string> values;
				for (int i = 2; i < tokens.size(); i++) {
					values.push_back(tokens[i]);
				}
				insertValue("dealerships", values, &dbms);
				cout << "Addition approved!" << endl << endl;
			}
		}
		else if (tokens[0] == "LIST") {			//process LIST
			if (tokens.size() != 5) {
				cout << "Invalid number of arguments!" << endl << endl;
			}
			else {
				//Price INTEGER, Date_Listed VARCHAR(20), VIN VARCHAR(17), Dealership_Name VARCHAR(30)
				string listing = "INSERT INTO listings VALUES FROM (" + tokens[1] + ", " + tokens[2] + ", " + tokens[3] + ", " + tokens[4] + ");";
				dbms.execute(listing);
				cout << "Listing approved!" << endl << endl;
			}
		}
		else if (tokens[0] == "UNLIST") {		//process UNLIST
			if (tokens.size() != 2) {
				cout << "Invalid number of arguments!" << endl << endl;
			}
			else {
				string removeListing = "DELETE FROM listings WHERE (VIN == \"" + tokens[1] + "\");";
				dbms.execute(removeListing);
			}
		}
		else if (tokens[0] == "SELL") {			//process SELL
			if (tokens.size() != 6) {
				cout << "Invalid number of arguments!" << endl << endl;
			}
			else {
				//Price INTEGER, Date_Sold VARCHAR(20), VIN VARCHAR(17), Dealership_Name VARCHAR(30), Buyer_Name VARCHAR(50)
				string sale = "INSERT INTO sales VALUES FROM (" + tokens[1] + ", " + tokens[2] + ", " + tokens[3] + ", " + tokens[4] + ", " + tokens[5] + ");";
				dbms.execute(sale);
				string removeListing = "DELETE FROM listings WHERE (VIN == \"" + tokens[3] + "\");";
				dbms.execute(removeListing);
				string removeInventory = "DELETE FROM cars WHERE (VIN == \"" + tokens[3] + "\");";
				dbms.execute(removeInventory);
				cout << "Sale approved!" << endl << endl;
			}			
		}
		else if (tokens[0] == "INVENTORY") {	//process INVENTORY
			showRelation("cars", &dbms);
		}
		else if (tokens[0] == "DEALERSHIPS") {	//process DEALERSHIPS
			showRelation("dealerships", &dbms);
		}
		else if (tokens[0] == "LISTINGS") {		//process LISTINGS
			showRelation("listings", &dbms);
		}
		else if (tokens[0] == "SALES") {		//process SALES
			showRelation("sales", &dbms);
		}
		else if (tokens[0] == "SEARCH") {		//process SEARCH
			searchResults++;
			stringstream ss;
			ss << searchResults;
			string query = "searchresult" + ss.str() + " <- select (";
			for (int i = 2; i < tokens.size(); i++) {
				query += tokens[i];
				if (i + 1 < tokens.size()) { query += " "; }
			}
			query += ") " + tokens[1] + ";";
			dbms.execute(query);			
			string showResult = "SHOW searchresult" + ss.str() + ";";
			dbms.execute(showResult);
			cout << endl;
		}
		else if (tokens[0] == "UPDATE") {
			if (tokens[1] == "LISTING") {		//process UPDATE LISTING
				if (tokens.size() != 4) {
					cout << "Invalid number of arguments!" << endl;
				}
				else {
					string updateListings = "UPDATE listings SET Price = " + tokens[3] + " WHERE VIN == " + tokens[2] + ";";
					dbms.execute(updateListings);
				}				
			}
		}
		else if (tokens[0] == "CROSS") {		//process CROSS
			string cross = "crossResult <- dealerships * cars;";
			dbms.execute(cross);
			string showCross = "SHOW crossResult;";
			dbms.execute(showCross);
			string closeCross = "CLOSE crossResult;";
			dbms.execute(closeCross);
		}
		else if (tokens[0] == "PROJECT") {		//process PROJECT
			string project = "projectResult <- project (";
			for (int i = 2; i < tokens.size(); i++) {
				project += tokens[i];
				if (i + 1 < tokens.size()) { project += ", "; }
			}
			project += ") " + tokens[1] + ";";
			dbms.execute(project);
			string showProject = "SHOW projectResult;";
			dbms.execute(showProject);
			string closeProject = "CLOSE projectResult;";
			dbms.execute(closeProject);
		}
		else if (tokens[0] == "UNION") {		
			if (tokens[1] == "SEARCHES") {		//process UNION SEARCHES
				string unionQuery = "unionResult <- " + tokens[2] + " + " + tokens[3] + ";";
				dbms.execute(unionQuery);
				string showUnion = "SHOW unionResult;";
				dbms.execute(showUnion);
				string closeUnion = "CLOSE unionResult;";
				dbms.execute(closeUnion);
			}
		}
		else if (tokens[0] == "DIFFERENCE") {	
			if (tokens[1] == "SEARCHES") {		//process DIFFERENCE SEARCHES
				string diffQuery = "diffResult <- " + tokens[2] + " - " + tokens[3] + ";";
				dbms.execute(diffQuery);
				string showDiff = "SHOW diffResult;";
				dbms.execute(showDiff);
				string closeDiff = "CLOSE diffResult;";
				dbms.execute(closeDiff);
			}
		}
		else {									//process unknown command
			cout << "Unknown command!" << endl << endl;
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
		"INVENTORY - Show the inventory of cars loaded into the database.\n"
		"DEALERSHIPS - Show the dealerships loaded into the database.\n"
		"LISTINGS - Show the listings loaded into the database.\n"
		"SALES - Show the sales loaded into the database.\n"
		"ADD CAR <Color> <VIN> <Make> <Model> <Year> <Condition> <MSRP>.\n"
		"UPDATE LISTING <VIN> <Price>.\n"
		"ADD DEALERSHIP <Dealership_Name> <City> <State>.\n"
		"LIST <Price> <Date_Listed> <VIN> <Dealership_Name>.\n"
		"UNLIST <VIN>.\n"
		"SELL <Price> <Date_Sold> <VIN> <Dealership_Name> <Buyer_Name>.\n"
		"SEARCH <Relation Name> <Condition(s)> - Search a relation bounded by a conditional statement.\n"
		"CROSS - Obtain the cross product of the inventory of cars and dealerships.\n"
		"PROJECT <Relation Name> <Attribute(s)> - Project attributes onto a resultant.\n"
		"UNION SEARCHES <Search Name> <Search Name>.\n"
		"DIFFERENCE SEARCHES <Search Name> <Search Name>.\n"
		"HELP - Display this menu again\n"
		"EXIT - Exit the program.\n";
}

void insertValue(string relation, vector<string> values, DBMSAPI* dbms) {
	string request = "INSERT INTO " + relation + " VALUES FROM ";
	for (int i = 0; i<values.size(); i++) {
		request += values[i];
		if (i + 1 < values.size()) { request += ", "; }
	}
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