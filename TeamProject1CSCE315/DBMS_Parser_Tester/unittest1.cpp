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
			/*DataManager dataManager = DataManager();
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

			Assert::IsTrue(solution == dmlParser.splitProgram(toSplit));*/
		}
		TEST_METHOD(TestSplitCreate)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit = "CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);";
			vector<string> solution = {
				"CREATE TABLE",
				"animals",
				"(",
				"name",
				"VARCHAR(20)",
				"kind",
				"VARCHAR(8)",
				"years",
				"INTEGER",
				")",
				"PRIMARY KEY",
				"(",
				"name",
				"kind",
				")"
			};
			vector<string> result = dmlParser.split(toSplit);

			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(solution[i], result[i]);
			}
			Assert::AreEqual(result.size(), solution.size());			
		}
		TEST_METHOD(TestSplitInsert)
		{
			DataManager dataManager = DataManager();
			DMLParser parser = DMLParser(&dataManager);

			string input = "INSERT INTO tools VALUES FROM (\"blow torch\", 3, 25);";
			vector<string> result = parser.split(input);
			vector<string> solution = { 
				"INSERT INTO",
				"tools",
				"VALUES FROM",
				"(", 
				"blow torch",
				"3", 
				"25", 
				")"
			};

			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(solution[i], result[i]);
			}
			Assert::AreEqual(result.size(), solution.size());
		}
		TEST_METHOD(TestSplitSelect)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit = "dinosaur_players <- select (team == \"Dinosaurs\") baseball_players;";
			vector<string> solution = {
				"dinosaur_players",
				"<-",
				"select",
				"(",
				"team",
				"==",
				"Dinosaurs",
				")",
				"baseball_players"
			};
			vector<string> result = dmlParser.split(toSplit);

			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(solution[i], result[i]);
			}
			Assert::AreEqual(result.size(), solution.size());
		}
		TEST_METHOD(TestSplitShow)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit = "SHOW dinosaur_players;";
			vector<string> solution = {
				"SHOW",
				"dinosaur_players"
			};
			vector<string> result = dmlParser.split(toSplit);

			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(solution[i], result[i]);
			}
			Assert::AreEqual(result.size(), solution.size());
		}
		TEST_METHOD(TestSplitRename)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit = "dots_to_points <- rename (x2, y2, z2) dots;";
			vector<string> solution = {
				"dots_to_points",
				"<-",
				"rename",
				"(",
				"x2",
				"y2",
				"z2",
				")",
				"dots"
			};
			vector<string> result = dmlParser.split(toSplit);

			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(solution[i], result[i]);
			}
			Assert::AreEqual(result.size(), solution.size());
		}
		TEST_METHOD(TestSplitProject)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit = "bad_query <- project (t) points;";
			vector<string> solution = {
				"bad_query",
				"<-",
				"project",
				"(",
				"t",
				")",
				"points"
			};
			vector<string> result = dmlParser.split(toSplit);

			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(solution[i], result[i]);
			}
			Assert::AreEqual(result.size(), solution.size());
		}
		TEST_METHOD(TestSplitUpdate)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit = "UPDATE dots SET x1 = 0 WHERE x1 < 0;";
			vector<string> solution = {
				"UPDATE",
				"dots",
				"SET",
				"x1",
				"=",
				"0",
				"WHERE",
				"x1",
				"<",
				"0"
			};
			vector<string> result = dmlParser.split(toSplit);

			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(solution[i], result[i]);
			}
			Assert::AreEqual(result.size(), solution.size());
		}
		TEST_METHOD(TestSplitExit)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit = "EXIT;";
			vector<string> solution = {
				"EXIT"
			};
			vector<string> result = dmlParser.split(toSplit);

			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(solution[i], result[i]);
			}
			Assert::AreEqual(result.size(), solution.size());
		}
		TEST_METHOD(TestSplitOp)
		{
			DataManager dataManager = DataManager();
			DMLParser dmlParser = DMLParser(&dataManager);

			string toSplit = "set_diff_test <- product_test - shape_color;";
			vector<string> solution = {
				"set_diff_test",
				"<-",
				"product_test",
				"-",
				"shape_color"
			};
			vector<string> result = dmlParser.split(toSplit);

			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(solution[i], result[i]);
			}
			Assert::AreEqual(result.size(), solution.size());
		}
	};
}