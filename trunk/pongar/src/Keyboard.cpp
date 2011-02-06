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

		case 0x66: // "f"
			Graphics::getInstance().fullScreenSwitch();
			break;

		case 0x61: // "a"
			Graphics::getInstance().moveCamera( 0.0f, CAM_CALIB_STEP, 0.f );
			break;

		case 0x64: // "d"
			Graphics::getInstance().moveCamera( 0.f, -CAM_CALIB_STEP, 0.f );
			break;
		
		case 0x73: // "s"
			Graphics::getInstance().moveCamera( CAM_CALIB_STEP, 0.f, 0.f );
			break;
		
		case 0x77: // "w"
			Graphics::getInstance().moveCamera( -CAM_CALIB_STEP, 0.f, 0.f );
			break;
		
		case 0x71: // "q"
			Graphics::getInstance().moveCamera( 0.f, 0.f, -CAM_CALIB_STEP );
			break;
		
		case 0x65: // "e"
			Graphics::getInstance().moveCamera( 0.f, 0.f, CAM_CALIB_STEP );
			break;

		case 0x79: // "y"
			Graphics::getInstance().changeFov( VCAM_FOV_CALIB_STEP );
			break;
		
		case 0x78: // "x"
			Graphics::getInstance().changeFov( -VCAM_FOV_CALIB_STEP );
			break;

		case 27: // Escape
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