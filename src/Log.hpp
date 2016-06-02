#ifndef LOG_HPP
#define LOG_HPP

#include <sstream>
using namespace std;

class Log {
public:
	////////////////////////////////////////////////////////////////////////////
	// Public static functions (defined in Log.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Print an error string to the console and specify its function of origin.
	 */
	static void error(const char* function, const char* message);
	
	/**
	 * Print the contents of a error stringstream to the console and specify its
	 * function of origin, then clears the stringstream.
	 */
	static void error(const char* function, stringstream& message);
	
	/**
	 * Print a warning string to the console and specify its function of origin.
	 */
	static void warning(const char* function, const char* message);
	
	/**
	 * Print the contents of a warning stringstream to the console and specify
	 * its function of origin, then clears the stringstream.
	 */
	static void warning(const char* function, stringstream& message);
};

#endif

