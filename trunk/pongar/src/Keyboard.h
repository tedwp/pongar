#pragma once
#include "Graphics.h"
#include "GL\glut.h"

class Keyboard
{
public:
	static Keyboard& getInstance(void);
	static void pressSpecialKey(int key, int x, int y);
	static void pressKey(unsigned char key, int x, int y);
	
private:
	static Keyboard& m_instance;

	Keyboard(void);
	Keyboard(const Keyboard&);
	~Keyboard(void);
	void doPressSpecialKey(int key, int x, int y);
	void doPressKey(unsigned char key, int x, int y);
};

