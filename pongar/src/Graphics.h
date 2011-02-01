#pragma once
#include "conf.h"
#include "PlayingField.h"
#include "Keyboard.h"
#include <string>

class Graphics
{
public:
	unsigned char m_bkgnd[CAM_WIDTH*CAM_HEIGHT*3];

	static Graphics& getInstance(void);
	void init(int argc, char* argv[]);
	void start(void);

	void fullScreenEnter(void);
	void fullScreenLeave(void);
	void fullScreenSwitch(void);
	void showString(std::string& str, float r, float g, float b, int cx, int y);
	std::string m_currentString;

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
	void drawStuffOnTop(void);

	void arrayToCvMat(float* transform, CvMat* mat);

};

