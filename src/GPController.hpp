#ifdef RPI

#ifndef GP_CONTROLLER_H
#define GP_CONTROLLER_H

#include <string>
#include <list>
using namespace std;

#include <linux/spi/spidev.h>
#include "mcp3008Spi.hpp"

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
	GPIOClass* pins[4];
	void initGPIO ();
	mcp3008Spi a2d;

	class Button{
	public:
		char button;
		int buttonUpDown; // 0=up, 1=down
		Button(char buttonName, int buttonState);
		void set_button(char buttonName);
		void set_buttonUpDown(int buttonState);
		char get_button();
		int get_buttonUpDown();
	};
	
public:
	std::list<Button> inputList;
};

#endif
#endif

