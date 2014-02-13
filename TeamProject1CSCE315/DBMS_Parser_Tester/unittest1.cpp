#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DBMS_Parser_Tester
{		
	TEST_CLASS(UnitTest1)
	{
	public:		
		TEST_METHOD(TestSplitProgram)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit =
				"CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);"
				"INSERT INTO animals VALUES FROM(\"Joe\", \"cat\", 4);"
				"INSERT INTO animals VALUES FROM(\"Spot\", \"dog\", 10);"
				"INSERT INTO animals VALUES FROM(\"Snoopy\", \"dog\", 3);"
				"INSERT INTO animals VALUES FROM(\"Tweety\", \"bird\", 1);"
				"INSERT INTO animals VALUES FROM(\"Joe\", \"bird\", 2); ";
			vector<string> solution = { "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);",
				"INSERT INTO animals VALUES FROM(\"Joe\", \"cat\", 4);",
				"INSERT INTO animals VALUES FROM(\"Spot\", \"dog\", 10);",
				"INSERT INTO animals VALUES FROM(\"Snoopy\", \"dog\", 3);",
				"INSERT INTO animals VALUES FROM(\"Tweety\", \"bird\", 1);",
				"INSERT INTO animals VALUES FROM(\"Joe\", \"bird\", 2);" };

			Assert::AreEqual(solution, dmlParser.splitProgram(toSplit));
		}
		TEST_METHOD(TestSplit)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);";
			vector<string> solution = {
				"CREATE TABLE",
				"animals",
				"(",
				"name",
				"VARCHAR(20)"
				"kind",
				"VARCHAR(8)"
				"years"
				"INTEGER"
				")",
				"PRIMARY KEY",
				"(",
				"name",
				"kind",
				")",
				";"
			};

			Assert::AreEqual(solution, dmlParser.split(toSplit));
		}
	};
}