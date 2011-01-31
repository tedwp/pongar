#pragma once
#include "Graphics.h"
#include "GL\glut.h"

class Keyboard
{
public:
	static Keyboard& getInstance(void);
	static void pressKey(int key, int x, int y);
	
private:
	static Keyboard& m_instance;

	Keyboard(void);
	Keyboard(const Keyboard&);
	~Keyboard(void);
	void doPressKey(int key, int x, int y);
};

