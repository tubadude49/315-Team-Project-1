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

			vector<string> newDataAttrNames1 = { "new_test", "new_name" };

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKey);
			dataManager.rename(dataRelatName1, dataAttrNames1, newDataAttrNames1);

			Assert::IsTrue(dataManager.testRelation(dataRelatName1, newDataAttrNames1, dataAttrTypes1, testTuples));
		}

	};
}