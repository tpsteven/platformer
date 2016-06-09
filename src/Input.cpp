#include "Input.hpp"

#include <cassert>
using namespace std;

Input::Input()
{
	// Intentionally empty
}

Input::~Input()
{
	// Intentionally empty
}

void
Input::clearEvents()
{
	while (!events.empty()) {
		events.pop();
	}
}

int
Input::pollEvent(ButtonEvent& event)
{
	assert(events.size() > 0);
	
	event = events.front();
	events.pop();
	
	return events.size();
}

void
Input::pushEvent(const ButtonEvent& event)
{
	pushEvent(event.button, event.state);
}

void
Input::pushEvent(const Button& button, const ButtonState& buttonState)
{
	// Add event to the queue of new states for this frame
	events.emplace(button, buttonState);
	
	// Update the overall state of the buttons
	switch (button) {
		case Button::A:
			a = (buttonState == ButtonState::Pressed);
			break;
		
		case Button::B:
			b = (buttonState == ButtonState::Pressed);
			break;
			
		case Button::Down:
			down = (buttonState == ButtonState::Pressed);
			break;
			
		case Button::Left:
			left = (buttonState == ButtonState::Pressed);
			break;
			
		case Button::Right:
			right = (buttonState == ButtonState::Pressed);
			break;
		
		case Button::Up:
			up = (buttonState == ButtonState::Pressed);
			break;
			
		case Button::X:
			x = (buttonState == ButtonState::Pressed);
			break;
			
		case Button::Y:
			y = (buttonState == ButtonState::Pressed);
			break;
	}
}

void
Input::reset()
{
	clearEvents();
	
	a     = false;
	b     = false;
	down  = false;
	left  = false;
	right = false;
	up    = false;	
	x     = false;
	y     = false;
}