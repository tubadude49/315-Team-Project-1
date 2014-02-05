#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DBMS_Engine_Tester
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			DataManager dataManager = DataManager();

			string dataRelatName1 = "Tester";
			vector<string> dataAttrNames1 = { "test", "name" };
			vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };
			unsigned int primaryKeyAt = 1;

			dataManager.create(dataRelatName1, dataAttrNames1, dataAttrTypes1, primaryKeyAt);
			cout << dataManager.show(dataRelatName1) << endl;

			// Intentionally fail
			Assert::IsTrue(false);
		}

	};
}