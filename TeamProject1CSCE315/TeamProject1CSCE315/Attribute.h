// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
// CSCE 315

#pragma once

#include <string>

using namespace std;

class Attribute
{
private:
	string name;
	string type;
public:
	Attribute();
	Attribute(string& attributeName, string& attributeType);

	string& const getName();
	string& const getType();
};
