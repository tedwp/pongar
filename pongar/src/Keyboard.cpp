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

void Keyboard::pressKey(int key, int x, int y)
{
	getInstance().doPressKey(key, x, y);
}
void Keyboard::doPressKey(int key, int x, int y) {

	switch (key)
	{
		case GLUT_KEY_F1:
			Graphics::getInstance().fullScreenSwitch();
			break;

		/*case GLUT_KEY_F6: 
			// return to default window
			Graphics::getInstance().fullScreenLeave();
			break;*/
	}
}