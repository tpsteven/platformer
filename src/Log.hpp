#ifndef LOG_HPP
#define LOG_HPP

#include <sstream>
using namespace std;

class Log {
public:
	static void error(const char* function, const char* message);
	static void error(const char* function, stringstream& message);
	static void warning(const char* function, const char* message);
	static void warning(const char* function, stringstream& message);
};

#endif

