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


	};
}