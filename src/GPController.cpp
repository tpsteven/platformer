#ifdef RPI

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
	: a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8)
{

    initGPIO();

}
/*
*	***** ARDUINO LAYOUT *****
*	5V input: bottom row left stack pin5(from left) //may need 3.3 for ADC pin4
*	Xstick: A0, need adc for left and right (uses potentiometer)
*			bottom row right stack pin1(from left)
*			connect to GPIO pins: mosi_10,miso_09,sclk_11
*	Ystick: A1, need adc for up and down (uses potentiometer)
*			bottom row right stack pin2(from left)
*	SW1-SW4: D2-5, top row right stack pins3-6(from left)gpiopins(12,16,20,21)
*/

void GPController::initGPIO (){
    string inputstate;
	//Digital buttons : SW1-SW4 

    pins[0] =  new GPIOClass("12"); //create new GPIO object to be attached to  GPIO12
    pins[1] =  new GPIOClass("16"); //create new GPIO object to be attached to  GPIO16
    pins[2] =  new GPIOClass("20"); //create new GPIO object to be attached to  GPIO20
    pins[3] =  new GPIOClass("21"); //create new GPIO object to be attached to  GPIO21

    for(int i=0;i<4;i++){
    	pins[i]->export_gpio();
    }
    cout << " GPIO pins exported" << endl;

    for(int i=0;i<4;i++){
    	pins[i]->setdir_gpio("in");
    }
    inputList[0] = new Button('Y', 0,0);
    inputList[1] = new Button('X', 0,0);
    inputList[2] = new Button('A', 0,0);
    inputList[3] = new Button('B', 0,0);
    //add in joystick L/R
    inputList[4] = new Button('L',0,0);
    inputList[5] = new Button('R',0,0);
    cout << " Set GPIO pin directions" << endl;	
}

void GPController::pollController(){

    string bstate_str;
    int bstate;
    int a2dVal = 0;
    int a2dChannel = 0;
    for(int i=0;i<6;i++){ 
	if(i>3){
		//analog stick:
        	data[0] = 1;  //  first byte transmitted -> start bit
        	data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
        	data[2] = 0; // third byte transmitted....don't care

        	a2d.spiWriteRead(data, sizeof(data) );

        	a2dVal = 0;
                a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
                a2dVal |=  (data[2] & 0xff);

		if(i==4){ //Left
			if(a2dVal>682)
				bstate=1;
			else
				bstate=0;
		}
		else{
			if(a2dVal<340)
				bstate=1; 
			else
				bstate=0; 
 		}
	}
	else {
		if (pins[i]->getval_gpio(bstate_str) == -1) {
			cout << "Failed to read pin " << pins[i]->get_gpionum() << endl;
		}
	
		if (bstate_str.compare("0") == 0) {
			bstate = 1;
		}
		else if (bstate_str.compare("1") == 0) {
			bstate = 0;
		}
		else {
			cout << "bstate = 2" << endl;
			bstate = 2;
		}
	}

	if(bstate!=inputList[i]->get_buttonUpDown()){
		inputList[i]->set_buttonUpDown(bstate);
		inputList[i]->set_buttonEvent(0);
	}
	else{
		inputList[i]->set_buttonEvent(1);
	}
    }	
}

GPController::Button::Button(char buttonName, int buttonState, int buttonE){
	this->button = buttonName;
	this->buttonUpDown=buttonState;
	this->buttonEvent=buttonE;
}

void GPController::Button::set_button(char buttonName){
	button = buttonName;
}

void GPController::Button::set_buttonUpDown(int buttonState){
	buttonUpDown = buttonState;
}

void GPController::Button::set_buttonEvent(int buttonE){
	buttonEvent = buttonE;
}

char GPController::Button::get_button(){
	return button;
}

int GPController::Button::get_buttonUpDown(){
	return buttonUpDown;
}

int GPController::Button::get_buttonEvent(){
	return buttonEvent;
}
#endif
