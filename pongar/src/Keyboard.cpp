#include "Keyboard.h"
Keyboard::Keyboard(void) 
{
}

Keyboard::~Keyboard(void)
{
	
}

Keyboard& Keyboard::getInstance(void)
{
	static Keyboard m_instance;
	return m_instance;
}


void Keyboard::pressKey(unsigned char key, int x, int y)
{
	getInstance().doPressKey(key, x, y);
}
void Keyboard::doPressKey(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 0x2b: // "+"
			Capture::getInstance().increaseThreshold();
			break;

		case 0x2d: // "-"
			Capture::getInstance().decreaseThreshold();
			break;

		case 0x2a: // "*"
			Capture::getInstance().increaseThresholdBW();
			break;
		
		case 0x2f: // "/"
			Capture::getInstance().decreaseThresholdBW();
			break;

		case 0x66:
			Graphics::getInstance().fullScreenSwitch();
			break;
		
		case 27:
			Game::getInstance().end();
			break;
	}
}


void Keyboard::pressSpecialKey(int key, int x, int y)
{
	getInstance().doPressSpecialKey(key, x, y);
}
void Keyboard::doPressSpecialKey(int key, int x, int y) {

	switch (key)
	{
		case GLUT_KEY_F1:
			Graphics::getInstance().fullScreenSwitch();
			break;
	}
}