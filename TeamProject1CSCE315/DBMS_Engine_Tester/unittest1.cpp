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
		TEST_METHOD(TestSplit)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string test_s = "(SPLIT && TEST==YES)";
			vector<string> out = dmlParser.split(test_s);
			vector<string> arrayout = { "(", "SPLIT", "&&", "TEST", "==", "YES", ")" };

			for (int i = 0; i < arrayout.size(); i++) {
				Assert::AreEqual(arrayout[i], out[i]);
			}

		}
		TEST_METHOD(TestSetRename)
		{
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

			Assert::IsTrue(dataManager.testRelation(unionRelatName, unionAttrNames, unionAttrTypes, unionTupleResults));

		}
		TEST_METHOD(TestSetDifference)
			//Author: Josh Tutt
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "TesterA";
			vector<string> dataAttrNames1 = { "test", "name" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
			vector<string> testTuple1 = { "test1", "Thomas" };
			vector<string> testTuple2 = { "test2", "Colin" };
			vector<string> testTuple3 = { "test3", "Josh" };

			vector<vector<string>> solutionTuples(0);
			solutionTuples.push_back(testTuple2);
			solutionTuples.push_back(testTuple3);

			string primaryKey = "test1";

			vector<string> newDataAttrNames1 = { "new_test", "new_name" };

			//Create relation A
			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
			dataManager.insert(dataRelatName1, testTuple1);
			dataManager.insert(dataRelatName1, testTuple2);
			dataManager.insert(dataRelatName1, testTuple3);

			//Create relation B
			string dataRelatName2 = "TesterB";
			vector<string> testTuple4 = { "test4", "Frankie" };
			dataManager.create(dataRelatName2, dataAttrNames1, dataAttrTypes1, primaryKey);
			dataManager.insert(dataRelatName2, testTuple1);
			dataManager.insert(dataRelatName2, testTuple4);

			//Compute the difference, A - B
			dataManager.setDifference(dataRelatName1, dataRelatName2);

			string newName = "Difference: \"" + dataRelatName1 + "\" - \"" + dataRelatName2 + "\"";

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
			string primaryKey1 = "test1";

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey1);
			dataManager.insert(dataRelatName1, testTuple1_1);
			dataManager.insert(dataRelatName1, testTuple1_2);
			dataManager.insert(dataRelatName1, testTuple1_3);

			vector<string> booleanArgs = { "name", "==", "Thomas" };
			string newName = "newTest";
			vector<vector<string>> result = { testTuple1_1, testTuple1_2 };

			dataManager.select(dataRelatName1, newName, booleanArgs);

			Assert::IsTrue(dataManager.testRelation(newName, dataAttrNames1, dataAttrTypes1, result));
		}
		TEST_METHOD(testCross)
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "Left";
			vector<string> dataAttrNames1 = { "L1","L2"};
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(5)"};
			vector<vector<string>> tupleSet1 = { { "1", ".exe"}, { "2", "and" } };
			string primaryKey1 = "L2";

			string dataRelatName2 = "Right";
			vector<string> dataAttrNames2 = { "R1", "R2",};
			vector<string> dataAttrTypes2 = { "VARCHAR(10)", "INTEGER"};
			vector<vector<string>> tupleSet2 = { { "Directive", "363"} };
			string primaryKey2 = "R2";

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey1);
			dataManager.insert(dataRelatName1, tupleSet1[0]);
			dataManager.insert(dataRelatName1, tupleSet1[1]);

			dataManager.create(dataRelatName2, dataAttrNames2, dataAttrTypes2, primaryKey2);
			dataManager.insert(dataRelatName2, tupleSet2[0]);
			dataManager.insert(dataRelatName2, tupleSet2[1]);
			dataManager.insert(dataRelatName2, tupleSet2[2]);

			string crossName = dataRelatName1 + " X " + dataRelatName2;
			dataManager.crossProduct(dataRelatName1, dataRelatName2,crossName);
			
			
			vector<string> crossedNames = { "{L1 X R1}", "{L1 X R2}", "{L1 X R3}", "{L2 X R1}", "{L2 X R2}", "{L2 X R3}" };
			vector<string> crossedTypes = { "(INTEGER X VARCHAR(10))", "(INTEGER X INTEGER)", "(VARCHAR(5) X VARCHAR(10))", "(VARCHAR(5) X INTEGER)" };
			vector<vector<string>> solutionTuples;
			solutionTuples.push_back({ "1~Directive", "1~363", "2~Directive", "2~363" });
			solutionTuples.push_back({ ".exe~Directive", ".exe~363", "and~Directive", "and~363" });

			Assert::IsTrue(dataManager.testRelation(crossName, crossedNames, crossedTypes, solutionTuples));
		}
		TEST_METHOD(testSetProject)
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "TheBigOne";
			vector<string> dataAttrNames1 = { "bangFactor", "name" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
			vector<string> testTuple1 = { "test1", "Jason" };
			vector<string> testTuple2 = { "test2", "SomeOtherGuy" };
			vector<vector<string>> testTuples;
			string primaryKey = "test1";

			string newDataName = "Tester_Renamed";
			vector<string> newDataAttrNames1 = { "new_test", "new_name" };

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
			dataManager.project(dataRelatName1, newDataName, newDataAttrNames1);

			Assert::IsTrue(dataManager.testRelation(dataRelatName1, dataAttrNames1, dataAttrTypes1, testTuples));
			Assert::IsTrue(dataManager.testRelation(newDataName, newDataAttrNames1, dataAttrTypes1, testTuples));
		}
		TEST_METHOD(TestNaturalJoin)
			//Author: Josh Tutt
		{
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
			string newName = "NaturalJoin";

			dataManager.naturalJoin(dataRelatName1, dataRelatName2, newName);

			Assert::IsTrue(dataManager.testRelation(newName, solutionAttrNames, solutionAttrTypes, solutionTuples));
		}

	};
}
