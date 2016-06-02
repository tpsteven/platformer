#include "Log.hpp"

#include <iostream>
#include <sstream>
using namespace std;

void
Log::error(const char* function, const char* message)
{
	cout << "[ERROR] " << function << " - " << message << endl;
}

void 
Log::error(const char* function, stringstream& message)
{
	cout << "[ERROR] " << function << " - " << message.str() << endl;
	message.str("");
	message.clear();
}

void
Log::warning(const char* function, const char* message)
{
	cout << "[WARNING] " << function << " - " << message << endl;
}

void
Log::warning(const char* function, stringstream& message)
{
	cout << "[WARNING] " << function << " - " << message.str() << endl;
	message.str("");
	message.clear();
}
