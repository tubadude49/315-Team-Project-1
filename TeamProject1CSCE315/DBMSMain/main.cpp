// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include <iostream>

#include "DBMSAPI.h"

int main() {

	DataManager dataManager = DataManager();

	string dataRelatName1 = "Tester";
	vector<string> dataAttrNames1 = { "test", "name" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
	vector<string> testTuple1 = { "test1", "Thomas" };
	vector<string> testTuple2 = { "test2", "Colin" };
	unsigned int primaryKeyAt = 1;

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKeyAt);
	dataManager.insert(dataRelatName1, testTuple1);
	dataManager.insert(dataRelatName1, testTuple2);
	//cout << dataManager.show(dataRelatName1) << endl;
	dataManager.show(dataRelatName1);

	string tester;
	cin >> tester;

}