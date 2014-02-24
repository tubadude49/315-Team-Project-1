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
	cout << displayMainMenu() << endl;
	
	while (1) {
		string in;
		getline(cin, in);

		vector<string> tokens = splitAtSpace(in);

		if (tokens[0] == "HELP") {
			cout << displayMainMenu() << endl;
		}
		else if (tokens[0] == "EXIT") {
			//saveRelations(&dbms);
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
			if (tokens.size() != 5) {
				cout << "Invalid number of arguments!" << endl;
			}
			else {
				//Price INTEGER, Date_Listed VARCHAR(20), VIN VARCHAR(17), Dealership_Name VARCHAR(30)
				string listing = "INSERT INTO listings VALUES FROM (" + tokens[1] + ", " + tokens[2] + ", " + tokens[3] + ", " + tokens[4] + ");";
				dbms.execute(listing);
				cout << "Listing approved!" << endl << endl;
			}
		}
		else if (tokens[0] == "UNLIST") {
			if (tokens.size() != 2) {
				cout << "Invalid number of arguments!" << endl;
			}
			else {
				string removeListing = "DELETE FROM listings WHERE (VIN == \"" + tokens[1] + "\");";
				dbms.execute(removeListing);
			}
		}
		else if (tokens[0] == "SELL") {
			if (tokens.size() != 6) {
				cout << "Invalid number of arguments!" << endl;
			}
			else {
				//Price INTEGER, Date_Sold VARCHAR(20), VIN VARCHAR(17), Dealership_Name VARCHAR(30), Buyer_Name VARCHAR(50)
				string sale = "INSERT INTO sales VALUES FROM (" + tokens[1] + ", " + tokens[2] + ", " + tokens[3] + ", " + tokens[4] + ", " + tokens[5] + ");";
				dbms.execute(sale);
				string removeListing = "DELETE FROM listings WHERE (VIN == \"" + tokens[3] + "\");";
				dbms.execute(removeListing);
				string removeInventory = "DELETE FROM cars WHERE (VIN == \"" + tokens[3] + "\");";
				dbms.execute(removeInventory);
				cout << "Sale approved!" << endl;
			}			
		}
		else if (tokens[0] == "INVENTORY") {
			showRelation("cars", &dbms);
		}
		else if (tokens[0] == "DEALERSHIPS") {
			showRelation("dealerships", &dbms);
		}
		else if (tokens[0] == "LISTINGS") {
			showRelation("listings", &dbms);
		}
		else if (tokens[0] == "SALES") {
			showRelation("sales", &dbms);
		}
		else if (tokens[0] == "SEARCH") {
			string query = "searchresult <- select (";
			for (int i = 2; i < tokens.size(); i++) {
				query += tokens[i];
				if (i + 1 < tokens.size()) { query += " "; }
			}
			query += ") " + tokens[1] + ";";
			dbms.execute(query);			
			string showResult = "SHOW searchresult;";
			dbms.execute(showResult);
		}
		else if (tokens[0] == "UPDATE") {
			if (tokens[1] == "LISTING") {
				if (tokens.size() != 4) {
					cout << "Invalid number of arguments!" << endl;
				}
				else {
					string updateListings = "UPDATE listings SET Price = " + tokens[3] + " WHERE VIN == " + tokens[2] + ";";
					dbms.execute(updateListings);
				}				
			}
		}
		else if (tokens[0] == "CROSS") {
			string cross = "crossResult <- dealerships * cars;";
			dbms.execute(cross);
			string showCross = "SHOW crossResult;";
			dbms.execute(showCross);
			string closeCross = "CLOSE crossResult;";
			dbms.execute(closeCross);
		}
		else if (tokens[0] == "PROJECT") {
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
		"SEARCH <Relation_Name> <Condition(s)> - Search a relation bounded by a conditional statement.\n"
		"CROSS.\n"
		"HELP - Display this menu again\n"
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