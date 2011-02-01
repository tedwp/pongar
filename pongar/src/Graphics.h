#pragma once
#include "conf.h"
#include <sstream>
#include "PlayingField.h"
#include "Capture.h"
#include "Game.h"
#include "GL/glut.h"
#include "Keyboard.h"
#include <iostream>


class Graphics
{
public:
	static Graphics& getInstance(void);
	void init(int argc, char* argv[]);
	void start(void);
	void fullScreenEnter(void);
	void fullScreenLeave(void);
	void fullScreenSwitch(void);
	void showString(char string[], float r, float g, float b, int cx, int y);

	unsigned char m_bkgnd[CAM_WIDTH*CAM_HEIGHT*3];

private:
	
	bool isInFullScreen;
	static Graphics& m_instance;

	Graphics(void);
	Graphics(const Graphics&);
	~Graphics(void);

	
	static void render();
	void doRender(void);

	static void resize( int w, int h) ;
	void doResize( int w, int h) ;

	static void idle(void);

	void prepareForDisplay(void);
	void redrawDisplay(void);
	void Graphics::drawStuffOnTop(void);

	void Graphics::arrayToCvMat(float* transform, CvMat* mat);
	
};

