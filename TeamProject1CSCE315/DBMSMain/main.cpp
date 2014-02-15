// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
// CSCE 315

#include <iostream>
#include <fstream>
#include "DBMSAPI.h"

int main() {

	string testName = "TestFile";

	DataManager dataManager = DataManager();
	DMLParser dmlParser = DMLParser(&dataManager);

	for (int i = 0; i < 5; i++) {
		stringstream ss;
		ss << (i+1);
		string tName = testName + ss.str() + ".txt";
		ifstream in(tName.c_str());
		string program;
		string tmp;
		while (getline(in, tmp)) {
			program += tmp;
		}
		in.close();

		cout << "DMLParser responds with: " << dmlParser.parseProgram(program) << " for " << tName << endl;
	}	

	string tester;
	cin >> tester;
}
