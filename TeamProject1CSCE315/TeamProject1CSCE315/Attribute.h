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
