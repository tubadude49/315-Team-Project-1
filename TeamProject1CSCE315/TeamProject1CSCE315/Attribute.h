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
	string getName();
	string getType();
};
