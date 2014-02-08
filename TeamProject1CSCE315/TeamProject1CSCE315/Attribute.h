#pragma once

#include <string>

using namespace std;

class Attribute
{
private:
	string name;
	string type;
public:
	Attribute(string& attributeName, string& attributeType);

	void setName(string name);

	string& const getName();
	string& const getType();
};
