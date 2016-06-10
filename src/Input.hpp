#ifndef INPUT_HPP
#define INPUT_HPP

#include <queue>
using namespace std;

#include "Types.hpp"

class Input {
public:
	Input();
	~Input();
	
	void clearEvents();
	int  pollEvent(ButtonEvent& event);
	void pushEvent(const ButtonEvent& event);
	void pushEvent(const Button& button, const ButtonState& buttonState);
	void reset();
	
	bool getA()     const { return a; }
	bool getB()     const { return b; }
	bool getDown()  const { return down; }
	bool getLeft()  const { return left; }
	bool getRight() const { return right; }
	bool getUp()    const { return up; }
	bool getX()     const { return x; }
	bool getY()     const { return y; }
	
	bool getAlt()   const { return alt; }
private:
	bool a     = false;
	bool b     = false;
	bool down  = false;
	bool left  = false;
	bool right = false;
	bool up    = false;	
	bool x     = false;
	bool y     = false;
	
	bool alt   = false;

	queue<ButtonEvent> events;
};

#endif
