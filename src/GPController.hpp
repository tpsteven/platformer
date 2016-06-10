#ifndef GP_CONTROLLER_H
#define GP_CONTROLLER_H

#include <string>
#include <list>

using namespace std;
/* GPIO Controller
 * Purpose: Initialize buttons and control input signals
 */
class GPController
{
public:
	//static int max=6;
    GPController();  // create a GPController object that contains pin list and input list
	void pollController();
private:
	GPIOClass* pins[6];
	void initGPIO ();
	class Button{
	public:
		string button;
		string buttonUpDown; // 0=up, 1=down
		Button(string buttonName, string buttonState);
		void set_button(string buttonName);
		void set_buttonUpDown(string buttonState);
		string get_button();
		string get_buttonUpDown();
	};
public:
	std::list<Button> inputList;
};

#endif