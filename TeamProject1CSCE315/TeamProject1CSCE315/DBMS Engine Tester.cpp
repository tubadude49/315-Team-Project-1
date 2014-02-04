#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "DataManager.h"

int main() {
	DataManager* dataManager = new DataManager();

	string dataRelatName1 = "Tester";
	vector<string> dataAttrNames1 = { "test", "name" };
	vector<string> dataAttrTypes1 = { "INTEGER", "VARCHAR(20)" };

	dataManager->create(dataRelatName1, dataAttrNames1, dataAttrTypes1);
	cout << dataManager->show(dataRelatName1) << endl;

	string pause;
	cin >> pause;
}