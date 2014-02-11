// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
// CSCE 315

#include <iostream>

#include "DBMSAPI.h"

void renameTest();
void projectTest();
void unionTest();
void crossTest();
void selectTest();
void naturalJoinTester();

int main() {
	renameTest();
	projectTest();
	unionTest();
	crossTest();
	selectTest();
	naturalJoinTester();

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

	string newDataName = "Test_Renamed";
	vector<string> newDataAttrNames1 = { "new_test", "new_name" };

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
	dataManager.insert(dataRelatName1, testTuple1);
	dataManager.insert(dataRelatName1, testTuple2);
	dataManager.rename(dataRelatName1, newDataName, newDataAttrNames1);
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

	string unionRelatName = "Test_Union";
	vector<string> unionAttrNames = { "test", "name" };
	vector<string> unionAttrTypes = { "INTEGER", "VARCHAR(20)" };
	vector<string> unionTuples = { "test1", "Thomas" };
	vector<vector<string>> unionTupleResults = { unionTuples };

	dataManager.setUnion(dataRelatName1, dataRelatName2, unionRelatName);
	
	dataManager.show(unionRelatName, cout);
}


void naturalJoinTester(){
	DataManager dataManager = DataManager();

	string dataRelatName1 = "TesterA";
	vector<string> dataAttrNames1 = { "test", "name" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
	vector<string> testTuple1 = { "test1", "Thomas" };
	vector<string> testTuple2 = { "test2", "Colin" };
	vector<string> testTuple3 = { "test3", "Josh" };
	vector<string> testTuple9 = { "test4", "Thomas" };


	vector<string> solutionAttrNames = { "test", "name", "number", "dog?" };
	vector<string> solutionAttrTypes = { "INTEGER", "VARCHAR(20)", "INTEGER", "BOOL" };
	vector<vector<string>> solutionTuples;

	// Uncomment to enable testing for correct tuples
	solutionTuples = {
		vector<string> { "test1", "Thomas", "1", "true" },
		vector<string> { "test2", "Colin", "2", "true" },
		vector<string> { "test3", "Josh", "3", "false" },
		vector<string> { "test3", "Josh", "5", "NULL" },
		vector<string> { "test4", "Thomas", "1", "true" }
	};

	string primaryKey = "test1";

	vector<string> newDataAttrNames1 = { "new_test", "new_name" };

	//Create relation A
	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
	dataManager.insert(dataRelatName1, testTuple1);
	dataManager.insert(dataRelatName1, testTuple2);
	dataManager.insert(dataRelatName1, testTuple3);
	dataManager.insert(dataRelatName1, testTuple9);

	//Create relation B
	string dataRelatName2 = "TesterB";
	vector<string> dataAttrNames2 = { "name", "number", "dog?" };
	vector<string> dataAttrTypes2 = { "VARCHAR(20)", "INTEGER", "BOOL" };
	vector<string> testTuple4 = { "Thomas", "1", "true" };
	vector<string> testTuple5 = { "Colin", "2", "true" };
	vector<string> testTuple6 = { "Josh", "3", "false" };
	vector<string> testTuple7 = { "Jason", "4", "false" };
	vector<string> testTuple8 = { "Josh", "5", "NULL" };

	dataManager.create(dataRelatName2, dataAttrNames2, dataAttrTypes2, primaryKey);
	dataManager.insert(dataRelatName2, testTuple4);
	dataManager.insert(dataRelatName2, testTuple5);
	dataManager.insert(dataRelatName2, testTuple6);
	dataManager.insert(dataRelatName2, testTuple7);
	dataManager.insert(dataRelatName2, testTuple8);

	//Compute the Natural Join, A |><| B
	string newName = "Test_NaturalJoin";

	dataManager.naturalJoin(dataRelatName1, dataRelatName2, newName);
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

	string crossName = "crossproduction";

	dataManager.crossProduct(dataRelatName1, dataRelatName2, crossName);
	dataManager.show(crossName, cout);
}

void selectTest() {
	DataManager dataManager = DataManager();

	string dataRelatName1 = "Tester1";
	vector<string> dataAttrNames1 = { "test", "name" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
	vector<string> testTuple1_1 = { "1", "Thomas" };
	vector<string> testTuple1_2 = { "2", "Thomas" };
	vector<string> testTuple1_3 = { "3", "Colin" };
	vector<vector<string>> testTuples;
	string primaryKey1 = "test1";

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey1);
	dataManager.insert(dataRelatName1, testTuple1_1);
	dataManager.insert(dataRelatName1, testTuple1_2);
	dataManager.insert(dataRelatName1, testTuple1_3);

	vector<string> booleanArgs = { "test", ">", "0", "&&", "name", "==", "Thomas" };
	string newName = "Test_Select";
	vector<vector<string>> result = { testTuple1_1, testTuple1_2 };

	dataManager.select(dataRelatName1, newName, booleanArgs);
	
	dataManager.show(newName, cout);
}

void projectTest() {
	DataManager dataManager = DataManager();

	string dataRelatName1 = "TheBigOne";
	vector<string> dataAttrNames1 = { "bangFactor", "name" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
	vector<string> testTuple1 = { "1", "Jason" };
	vector<string> testTuple2 = { "2", "SomeOtherGuy" };
	string primaryKey = "test1";

	string newDataName = "Test_Project";
	vector<string> newDataAttrNames1 = { "name" };
	vector<string> newDataAttrTypes1 = { "VARCHAR(20)" };
	vector<vector<string>> solution = { vector<string> { "Jason" }, vector<string> { "SomeOtherGuy" } };

	dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
	dataManager.insert(dataRelatName1, testTuple1);
	dataManager.insert(dataRelatName1, testTuple2);
	dataManager.project(dataRelatName1, newDataName, newDataAttrNames1);

	dataManager.show(newDataName, cout);
}