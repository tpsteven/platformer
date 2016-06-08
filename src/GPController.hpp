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
	int max=6;
    GPController();  // create a GPController object that contains pin list and input list
	void pollController();
private:
	GPIOClass pins[max];
	void initGPIO (GPIOClass pins[max]);
	class Button{
	public:
		string button;
		int buttonUpDown; // 0=up, 1=down
		void Button(string buttonName, int buttonState);
		void set_button(string buttonName);
		void set_buttonUpDown(int buttonState);
		int get_button();
		int get_buttonUpDown();
	};
public:
	std::list<Button> inputList;
};

#endif