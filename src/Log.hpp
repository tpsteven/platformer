#ifndef LOG_HPP
#define LOG_HPP

#include <queue>
#include <sstream>
#include <string>
using namespace std;

class Log {
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors/Destructor (defined in Log.cpp)
	////////////////////////////////////////////////////////////////////////////
	Log();
	~Log();

	////////////////////////////////////////////////////////////////////////////
	// Public static functions (defined in Log.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Write the current contents of the log to the specified file, then delete
	 * the global instance.
	 */
	static void finalize(string fileName);
	
	/**
	 * Return a pointer to the global Log instance. If null, create one first.
	 */
	static Log* instance();
	
	////////////////////////////////////////////////////////////////////////////
	// Public utility functions (defined in Log.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Print an debug string to the console and specify its function of origin.
	 */
	void debug(const char* function, const char* message);
	
	/**
	 * Print the contents of a debug stringstream to the console and specify its
	 * function of origin, then clears the stringstream.
	 */
	void debug(const char* function, stringstream& message);
	
	/**
	 * Print an error string to the console and specify its function of origin.
	 */
	void error(const char* function, const char* message);
	
	/**
	 * Print the contents of a error stringstream to the console and specify its
	 * function of origin, then clears the stringstream.
	 */
	void error(const char* function, stringstream& message);
	
	/**
	 * Print a warning string to the console and specify its function of origin.
	 */
	void warning(const char* function, const char* message);
	
	/**
	 * Print the contents of a warning stringstream to the console and specify
	 * its function of origin, then clears the stringstream.
	 */
	void warning(const char* function, stringstream& message);
	
private:
	static Log*   globalInstance;
	
	queue<string> messages;
	stringstream  tempStream;
};

#endif

