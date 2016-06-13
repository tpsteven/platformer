#include "Log.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

////////////////////////////////////////////////////////////////////////////
// Static Member Initialization
////////////////////////////////////////////////////////////////////////////
Log* Log::globalInstance = nullptr;

////////////////////////////////////////////////////////////////////////////
// Constructors/Destructor
////////////////////////////////////////////////////////////////////////////

Log::Log()
{
	// Intentionally empty
}

Log::~Log()
{
	// Intentionally empty
}

////////////////////////////////////////////////////////////////////////////
// Public static functions
////////////////////////////////////////////////////////////////////////////

void
Log::finalize(string fileName)
{
	ofstream file;
	file.open(fileName, ios::out | ios::trunc);
	
	if (file.is_open()) {
		if (globalInstance != nullptr) {
			while (!globalInstance->messages.empty()) {
				file << globalInstance->messages.front();
				globalInstance->messages.pop();
			}
		}
		else {
			file << "" << endl;
			cout << "No messages to write; file cleared." << endl;
		}
		
		file.close();
	}
	else {
		cout << "Failed to write to file " << fileName << endl;
	}
	
	if (globalInstance != nullptr) {
		delete globalInstance;
	}
}

Log*
Log::instance()
{
	if (globalInstance == nullptr) {
		globalInstance = new Log();
	}
	
	return globalInstance;
}

////////////////////////////////////////////////////////////////////////////
// Public utility functions (defined in Log.cpp)
////////////////////////////////////////////////////////////////////////////

void
Log::debug(const char* function, const char* message)
{
	tempStream << "[DEBUG] " << function << " - " << message << endl;
	messages.push(tempStream.str());
	cout << tempStream.str();
	
	tempStream.str("");
	tempStream.clear();
}

void 
Log::debug(const char* function, stringstream& message)
{
	tempStream << "[DEBUG] " << function << " - " << message.str() << endl;
	messages.push(tempStream.str());
	cout << tempStream.str();
	
	message.str("");
	message.clear();
	
	tempStream.str("");
	tempStream.clear();
}

void
Log::error(const char* function, const char* message)
{
	tempStream << "[ERROR] " << function << " - " << message << endl;
	messages.push(tempStream.str());
	cout << tempStream.str();
	
	tempStream.str("");
	tempStream.clear();
}

void 
Log::error(const char* function, stringstream& message)
{
	tempStream << "[ERROR] " << function << " - " << message.str() << endl;
	messages.push(tempStream.str());
	cout << tempStream.str();
	
	message.str("");
	message.clear();
	
	tempStream.str("");
	tempStream.clear();
}

void
Log::warning(const char* function, const char* message)
{
	tempStream << "[WARNING] " << function << " - " << message << endl;
	messages.push(tempStream.str());
	cout << tempStream.str();
	
	tempStream.str("");
	tempStream.clear();
}

void 
Log::warning(const char* function, stringstream& message)
{
	tempStream << "[WARNING] " << function << " - " << message.str() << endl;
	messages.push(tempStream.str());
	cout << tempStream.str();
	
	message.str("");
	message.clear();
	
	tempStream.str("");
	tempStream.clear();
}
