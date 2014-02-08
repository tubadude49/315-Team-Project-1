// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include <iostream>

#include "DBMSAPI.h"

int main() {

	/*DataManager dataManager = DataManager();

	string dataRelatName1 = "Tester";
	vector<string> dataAttrNames1 = { "test", "name" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
	vector<string> testTuple1 = { "test1", "Thomas" };
	vector<string> testTuple2 = { "test2", "Colin" };
	string primaryKey = "test1";

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
	dataManager.insert(dataRelatName1, testTuple1);
	dataManager.insert(dataRelatName1, testTuple2);	
	dataManager.show(dataRelatName1, cout);*/

	DataManager dataManager = DataManager();

	string dataRelatName1 = "Tester";
	vector<string> dataAttrNames1 = { "test", "name" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
	vector<string> testTuple1 = { "test1", "Thomas" };
	vector<string> testTuple2 = { "test2", "Colin" };
	vector<vector<string>> testTuples;
	string primaryKey = "test1";

	vector<string> newDataAttrNames1 = { "new_test", "new_name" };

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
	//dataManager.rename(dataRelatName1, dataAttrNames1, newDataAttrNames1);
	dataManager.show(dataRelatName1, cout);

	string tester;
	cin >> tester;

}