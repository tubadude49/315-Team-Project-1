#include "Attribute.h"

Attribute::Attribute(){
}

Attribute::Attribute(string& attributeName, string& attributeType) {
	name = attributeName;
	type = attributeType;
}

string& const Attribute::getName()
{
	return name;
}

string& const Attribute::getType()
{
	return type;
}