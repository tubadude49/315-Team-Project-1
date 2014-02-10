// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include <iostream>

#include "DBMSAPI.h"

void renameTest();
void unionTest();
void crossTest();

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
	vector<string> dataAttrNames1 = { "Index", "Words" , "Num", "!"};
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(40)","INTEGER","VARCHAR(1)" };
	vector<string> testTuple1 = { "23", "This is a test string","19","?" };
	vector<string> testTuple2 = { "15", "This is also a test string","8","!" };
	vector<vector<string>> testTuples;
	string primaryKey = "test1";

	vector<string> newDataAttrNames1 = { "new_test", "new_name" };

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
	dataManager.insert(dataRelatName1, testTuple1);
	dataManager.insert(dataRelatName1, testTuple2);
	//dataManager.rename(dataRelatName1, dataAttrNames1, newDataAttrNames1);
	dataManager.show(dataRelatName1, cout);

	renameTest();
	unionTest();
	crossTest();
	string tester;
	cin >> tester;

}

void renameTest() {
	DataManager dataManager = DataManager();

	string dataRelatName1 = "Tester";
	vector<string> dataAttrNames1 = { "test", "name" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
	vector<string> testTuple1 = { "test1", "Thomas" };
	vector<string> testTuple2 = { "test2", "Colin" };
	vector<vector<string>> testTuples;
	string primaryKey = "test1";

	string newDataName = "Tester_Renamed";
	vector<string> newDataAttrNames1 = { "new_test", "new_name" };

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
	dataManager.insert(dataRelatName1, testTuple1);
	dataManager.insert(dataRelatName1, testTuple2);
	dataManager.rename(dataRelatName1, newDataName, newDataAttrNames1);
	dataManager.show(dataRelatName1, cout);
	dataManager.show(newDataName, cout);

}

void unionTest() {
	DataManager dataManager = DataManager();

	string dataRelatName1 = "Tester1";
	vector<string> dataAttrNames1 = { "test", "name" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
	vector<string> testTuple1_1 = { "test1", "Thomas" };
	vector<string> testTuple1_2 = { "test2", "Colin" };
	vector<vector<string>> testTuples;
	string primaryKey1 = "test1";

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey1);
	dataManager.insert(dataRelatName1, testTuple1_1);
	dataManager.insert(dataRelatName1, testTuple1_2);

	string dataRelatName2 = "Tester2";
	vector<string> dataAttrNames2 = { "test", "name" };
	vector<string> dataAttrTypes2 = { "INTEGER", "VARCHAR(20)" };
	vector<string> testTuple2_1 = { "test1", "Thomas" };
	vector<string> testTuple2_2 = { "test2", "Jack" };
	string primaryKey2 = "test1";

	dataManager.create(dataRelatName2, dataAttrNames2, dataAttrTypes2, primaryKey2);
	dataManager.insert(dataRelatName2, testTuple2_1);
	dataManager.insert(dataRelatName2, testTuple2_2);

	string unionRelatName = "Unionize";
	vector<string> unionAttrNames = { "test", "name" };
	vector<string> unionAttrTypes = { "INTEGER", "VARCHAR(20)" };
	vector<string> unionTuples = { "test1", "Thomas" };
	vector<vector<string>> unionTupleResults = { unionTuples };

	dataManager.setUnion(dataRelatName1, dataRelatName2, unionRelatName);

	cout << dataManager.testRelation(unionRelatName, unionAttrNames, unionAttrTypes, unionTupleResults) << endl;

	dataManager.show(unionRelatName, cout);
}

void crossTest()
{
	DataManager dataManager = DataManager();

	string dataRelatName1 = "Greek";
	vector<string> dataAttrNames1 = { "Alpha", "Beta","Gamma" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(5)","VARCHAR(18)"};
	vector<vector<string>> tupleSet1 = { { "1", ".exe", "Commentary" }, { "2", "and", "three-halves" } };
	string primaryKey1 = "Beta";

	string dataRelatName2 = "Phoenetic";
	vector<string> dataAttrNames2 = { "Bravo", "Charlie" , "Echo", "Foxtrot"};
	vector<string> dataAttrTypes2 = { "VARCHAR(10)", "INTEGER", "VARCHAR(4)","INTEGER" };
	vector<vector<string>> tupleSet2 = { { "Directive", "363", "this", "1010" }, { "Anyway", "21", "yes", "0" }, { "none", "1", "two", "3" } };
	string primaryKey2 = "Echo";

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey1);
	dataManager.insert(dataRelatName1, tupleSet1[0]);
	dataManager.insert(dataRelatName1, tupleSet1[1]);

	dataManager.create(dataRelatName2, dataAttrNames2, dataAttrTypes2, primaryKey2);
	dataManager.insert(dataRelatName2, tupleSet2[0]);
	dataManager.insert(dataRelatName2, tupleSet2[1]);
	dataManager.insert(dataRelatName2, tupleSet2[2]);

	dataManager.show(dataRelatName1, cout);
	dataManager.show(dataRelatName2, cout);

	string crossName = dataManager.crossProduct(dataRelatName1, dataRelatName2);
	dataManager.show(crossName, cout);
}