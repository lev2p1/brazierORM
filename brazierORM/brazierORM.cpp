// brazierORM.cpp: определяет точку входа для приложения.
//

#include "brazierORM.h"

using namespace brazier;

int main()
{
	setlocale(LC_ALL, "Russian");
	try {
		Database db("localhost", "5432", "test", "test", "test");
	}
	catch (const std::exception& e) {
		Logger::log(std::string("Exception: ") + e.what(), "ERROR");
	}

	return 0;
}
