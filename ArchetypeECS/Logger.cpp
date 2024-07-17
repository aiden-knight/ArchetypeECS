#include "Logger.h"
#include <iostream>

namespace Logger
{
	using namespace std;

	void Break()
	{
		cout << endl;
	}

	void Log(std::string message)
	{
		cout << "Log: " << message << endl;
	}

	void Warning(std::string message)
	{
		cout << "WARNING: " << message << endl;
	}
}
