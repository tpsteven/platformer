#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>

#include "GPIOClass.hpp"
#include "GPController.hpp"

using namespace std;

GPController::GPController() //initialize gpio, populate gpiolist and inputlist 
{

    initGPIO(pins[]);
	
}
/*
*	***** ARDUINO LAYOUT *****
*	5V input: bottom row left stack pin5(from left) //may need 3.3 for ADC pin4
*	Xstick: A0, need adc for left and right (uses potentiometer)
*	Ystick: A1, need adc for up and down (uses potentiometer)
*	SW1-SW4: D2-5, gpiopins(12,16,20,21)
*/

void GPController::initGPIO (GPIOClass pins[max]){ 
    string inputstate;
	//Digital buttons : SW1-SW4 
//    GPIOClass* gpio12 = new GPIOClass("12"); //create new GPIO object to be attached to  GPIO12
//    GPIOClass* gpio16 = new GPIOClass("16"); //create new GPIO object to be attached to  GPIO16
//    GPIOClass* gpio20 = new GPIOClass("20"); //create new GPIO object to be attached to  GPIO20
//    GPIOClass* gpio21 = new GPIOClass("21"); //create new GPIO object to be attached to  GPIO21
    pins[0] =  new GPIOClass("12"); //create new GPIO object to be attached to  GPIO12
    pins[1] =  new GPIOClass("16"); //create new GPIO object to be attached to  GPIO16
    pins[2] =  new GPIOClass("20"); //create new GPIO object to be attached to  GPIO20
    pins[3] =  new GPIOClass("21"); //create new GPIO object to be attached to  GPIO21
    //Analog joystick : X , Y
	
//     gpio12->export_gpio(); //export GPIO12
//     gpio16->export_gpio(); //export GPIO16
//	   gpio20->export_gpio(); //export GPIO20
//     gpio21->export_gpio(); //export GPIO21
    for(int i=0;i<max;i++){
    	pins[i].export_gpio();
    }
    cout << " GPIO pins exported" << endl;

    // gpio12->setdir_gpio("in"); //GPIO12 set to input
    // gpio16->setdir_gpio("in"); // GPIO16 set to input
	// gpio20->setdir_gpio("in"); //GPIO20 set to input
    // gpio21->setdir_gpio("in"); // GPIO21 set to input
    for(int i=0;i<max;i++){
    	pins[i].setdir_gpio("in");
    }

    cout << " Set GPIO pin directions" << endl;	
}

void GPController::pollController(){
	//Button temp = new Button;
	int bstate;
	for(int i=0;i<max;i++){
		bstate = pins[i].get_gpionum();
		switch(i){								 //yquit,x,ajump,b
			case 0 ://temp.Button('Ypin',0);
					inputList.push_back(Button('Yquit',bstate)); //exit game?
					//inputList.push_back(temp);
					break;
			case 1 :inputList.push_back(Button('Xpin',bstate));
					break;
			case 2 :inputList.push_back(Button('Ajump',bstate)); //jump
					break;
			case 3 :inputList.push_back(Button('Bpin',bstate));

			//analog stick:


		}
	}
}

void GPController::Button::Button(string buttonName, int buttonState){
	this->button = buttonName;
	this->buttonUpDown=buttonState;
}

void GPController::Button::set_button(string buttonName){
	button = buttonName;
}

void GPController::Button::set_buttonUpDown(int buttonState){
	buttonUpDown = buttonState;
}

int GPController::Button::get_button(){
	return button;
}

int GPController::Button::get_buttonUpDown(){
	return buttonUpDown;
}