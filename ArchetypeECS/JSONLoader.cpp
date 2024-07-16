#include "JSONLoader.h"
#include <fstream>
#include <iostream>

namespace JSONLoader
{
	json Load(const char* path)
	{
		json jsonFile;
		std::ifstream fileOpen(path);
		if (!fileOpen.good()) return json();

		jsonFile = json::parse(fileOpen);
		return jsonFile;
	}
}