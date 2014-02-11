// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
// CSCE 315

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DBMS_Engine_Tester
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestShellSort)
		{
			DataManager dataManager = DataManager();
			vector<string> arraytest = { "Best", "Thomas", "At", "Jack", "Reverse" };
			vector<string> arrayout = { "At", "Best", "Jack", "Reverse", "Thomas" };

			dataManager.shellSort(arraytest);

			for (int i = 0; i < arraytest.size(); i++) {
				Assert::AreEqual(arraytest[i], arrayout[i]);
			}

		}
		TEST_METHOD(TestSetRename)
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "Tester";
			vector<string> dataAttrNames1 = { "test", "name" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
			vector<string> testTuple1 = { "1", "Thomas" };
			vector<string> testTuple2 = { "2", "Colin" };
			vector<vector<string>> testTuples = { testTuple1, testTuple2 };
			string primaryKey = "test";

			string newDataName = "Test_Renamed";
			vector<string> newDataAttrNames1 = { "new_test", "new_name" };

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
			dataManager.insert(dataRelatName1, testTuple1);
			dataManager.insert(dataRelatName1, testTuple2);

			dataManager.rename(dataRelatName1, newDataName, newDataAttrNames1);

			Assert::IsTrue(dataManager.testRelation(dataRelatName1, dataAttrNames1, dataAttrTypes1, testTuples));
			Assert::IsTrue(dataManager.testRelation(newDataName, newDataAttrNames1, dataAttrTypes1, testTuples));
		}
		TEST_METHOD(TestSetUnion)
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "Tester1";
			vector<string> dataAttrNames1 = { "test", "name" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
			vector<string> testTuple1_1 = { "1", "Thomas" };
			vector<string> testTuple1_2 = { "2", "Colin" };
			vector<vector<string>> testTuples;
			string primaryKey1 = "test";

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey1);
			dataManager.insert(dataRelatName1, testTuple1_1);
			dataManager.insert(dataRelatName1, testTuple1_2);

			string dataRelatName2 = "Tester2";
			vector<string> dataAttrNames2 = { "test", "name" };
			vector<string> dataAttrTypes2 = { "INTEGER", "VARCHAR(20)" };
			vector<string> testTuple2_1 = { "1", "Thomas" };
			vector<string> testTuple2_2 = { "2", "Jack" };
			string primaryKey2 = "test";

			dataManager.create(dataRelatName2, dataAttrNames2, dataAttrTypes2, primaryKey2);
			dataManager.insert(dataRelatName2, testTuple2_1);
			dataManager.insert(dataRelatName2, testTuple2_2);

			string unionRelatName = "Test_SetUnion";
			vector<string> unionAttrNames = { "test", "name" };
			vector<string> unionAttrTypes = { "INTEGER", "VARCHAR(20)" };
			vector<string> unionTuples = { "1", "Thomas" };
			vector<vector<string>> unionTupleResults = { unionTuples };

			dataManager.setUnion(dataRelatName1, dataRelatName2, unionRelatName);

			Assert::IsTrue(dataManager.testRelation(unionRelatName, unionAttrNames, unionAttrTypes, unionTupleResults));

		}
		TEST_METHOD(TestSetDifference)
			//Author: Josh Tutt
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "TesterA";
			vector<string> dataAttrNames1 = { "test", "name" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
			vector<string> testTuple1 = { "1", "Thomas" };
			vector<string> testTuple2 = { "2", "Colin" };
			vector<string> testTuple3 = { "3", "Josh" };

			vector<vector<string>> solutionTuples(0);
			solutionTuples.push_back(testTuple2);
			solutionTuples.push_back(testTuple3);

			string primaryKey = "test";

			vector<string> newDataAttrNames1 = { "new_test", "new_name" };

			//Create relation A
			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
			dataManager.insert(dataRelatName1, testTuple1);
			dataManager.insert(dataRelatName1, testTuple2);
			dataManager.insert(dataRelatName1, testTuple3);

			//Create relation B
			string dataRelatName2 = "TesterB";
			vector<string> testTuple4 = { "4", "Frankie" };
			dataManager.create(dataRelatName2, dataAttrNames1, dataAttrTypes1, primaryKey);
			dataManager.insert(dataRelatName2, testTuple1);
			dataManager.insert(dataRelatName2, testTuple4);

			string newName = "Test_SetDifference";

			//Compute the difference, A - B
			dataManager.setDifference(dataRelatName1, dataRelatName2, newName);

			Assert::IsTrue(dataManager.testRelation(newName, dataAttrNames1, dataAttrTypes1, solutionTuples));
		}
		TEST_METHOD(TestSelect)
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "Tester1";
			vector<string> dataAttrNames1 = { "test", "name" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
			vector<string> testTuple1_1 = { "1", "Thomas" };
			vector<string> testTuple1_2 = { "2", "Thomas" };
			vector<string> testTuple1_3 = { "3", "Colin" };
			vector<vector<string>> testTuples;
			string primaryKey1 = "test";

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey1);
			dataManager.insert(dataRelatName1, testTuple1_1);
			dataManager.insert(dataRelatName1, testTuple1_2);
			dataManager.insert(dataRelatName1, testTuple1_3);

			vector<string> booleanArgs = { "test", ">", "0", "&&", "name", "==", "Thomas" };
			string newName = "Test_Select";
			vector<vector<string>> result = { testTuple1_1, testTuple1_2 };

			dataManager.select(dataRelatName1, newName, booleanArgs);

			Assert::IsTrue(dataManager.testRelation(newName, dataAttrNames1, dataAttrTypes1, result));
		}
		TEST_METHOD(TestCross)
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "Greek";
			vector<string> dataAttrNames1 = { "Alpha", "Beta", "Gamma" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(5)", "VARCHAR(18)" };
			vector<vector<string>> tupleSet1 = { { "1", ".exe", "Commentary" }, { "2", "and", "three-halves" } };
			string primaryKey1 = "Beta";

			string dataRelatName2 = "Phoenetic";
			vector<string> dataAttrNames2 = { "Bravo", "Charlie", "Echo", "Foxtrot" };
			vector<string> dataAttrTypes2 = { "VARCHAR(10)", "INTEGER", "VARCHAR(4)", "INTEGER" };
			vector<vector<string>> tupleSet2 = { { "Directive", "363", "this", "1010" }, { "Anyway", "21", "yes", "0" }, { "none", "1", "two", "3" } };
			string primaryKey2 = "Echo";

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey1);
			dataManager.insert(dataRelatName1, tupleSet1[0]);
			dataManager.insert(dataRelatName1, tupleSet1[1]);

			dataManager.create(dataRelatName2, dataAttrNames2, dataAttrTypes2, primaryKey2);
			dataManager.insert(dataRelatName2, tupleSet2[0]);
			dataManager.insert(dataRelatName2, tupleSet2[1]);
			dataManager.insert(dataRelatName2, tupleSet2[2]);

			string crossName = dataRelatName1 + " X " + dataRelatName2;
			dataManager.crossProduct(dataRelatName1, dataRelatName2, crossName);

			vector<vector<string>> solutionTuples;
			vector<string> crossedNames = { "{Alpha X Bravo}", "{Alpha X Charlie}", "{Alpha X Echo}", "{Alpha X Foxtrot}", "{Beta X Bravo}", "{Beta X Charlie}", "{Beta X Echo}", "{Beta X Foxtrot}", "{Gamma X Bravo}", "{Gamma X Charlie}", "{Gamma X Echo}", "{Gamma X Foxtrot}" };
			vector<string> crossedTypes = { "(INTEGER X VARCHAR(10))", "(INTEGER X INTEGER)", "(INTEGER X VARCHAR(4))", "(INTEGER X INTEGER)", "(VARCHAR(5) X VARCHAR(10))", "(VARCHAR(5) X INTEGER)", "(VARCHAR(5) X VARCHAR(4))", "(VARCHAR(5) X INTEGER)", "(VARCHAR(18) X VARCHAR(10))", "(VARCHAR(18) X INTEGER)", "(VARCHAR(18) X VARCHAR(4))", "(VARCHAR(18) X INTEGER)" };
			Assert::IsTrue(dataManager.testRelation(crossName, crossedNames, crossedTypes, solutionTuples));
		}
		TEST_METHOD(TestSetProject)
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "TheBigOne";
			vector<string> dataAttrNames1 = { "bangFactor", "name" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
			vector<string> testTuple1 = { "1", "Jason" };
			vector<string> testTuple2 = { "2", "SomeOtherGuy" };
			string primaryKey = "test";

			string newDataName = "Test_Renamed";
			vector<string> newDataAttrNames1 = { "name" };
			vector<string> newDataAttrTypes1 = { "VARCHAR(20)" };
			vector<vector<string>> solution = { vector<string> { "Jason" }, vector<string> { "SomeOtherGuy" } };

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
			dataManager.insert(dataRelatName1, testTuple1);
			dataManager.insert(dataRelatName1, testTuple2);
			dataManager.project(dataRelatName1, newDataName, newDataAttrNames1);

			Assert::IsTrue(dataManager.testRelation(newDataName, newDataAttrNames1, newDataAttrTypes1, solution));
		}
		TEST_METHOD(TestNaturalJoin)
			//Author: Josh Tutt
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "TesterA";
			vector<string> dataAttrNames1 = { "test", "name" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
			vector<string> testTuple1 = { "1", "Thomas" };
			vector<string> testTuple2 = { "2", "Colin" };
			vector<string> testTuple3 = { "3", "Josh" };
			vector<string> testTuple9 = { "4", "Thomas" };


			vector<string> solutionAttrNames = { "test", "name", "number", "dog?" };
			vector<string> solutionAttrTypes = { "INTEGER", "VARCHAR(20)", "INTEGER", "VARCHAR(5)" };
			vector<vector<string>> solutionTuples;

			// Uncomment to enable testing for correct tuples
			solutionTuples = {
				vector<string> { "1", "Thomas", "1", "true" },
				vector<string> { "2", "Colin", "2", "true" },
				vector<string> { "3", "Josh", "3", "false" },
				vector<string> { "3", "Josh", "5", "NULL" },
				vector<string> { "4", "Thomas", "1", "true" }
			};

			string primaryKey = "test";

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
			vector<string> dataAttrTypes2 = { "VARCHAR(20)", "INTEGER", "VARCHAR(5)" };
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

			Assert::IsTrue(dataManager.testRelation(newName, solutionAttrNames, solutionAttrTypes, solutionTuples));
		}

	};
}
