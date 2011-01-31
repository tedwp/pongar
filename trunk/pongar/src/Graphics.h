#pragma once
#include "conf.h"
#include "PlayingField.h"

class Graphics
{
public:
	static Graphics& getInstance(void);
	void init(int argc, char* argv[]);
	void start(void);

	unsigned char m_bkgnd[CAM_WIDTH*CAM_HEIGHT*3];

private:
	Graphics(void);
	Graphics(const Graphics&);
	~Graphics(void);
	static Graphics& m_instance;


	static void render();
	void doRender(void);

	static void resize( int w, int h) ;
	void doResize( int w, int h) ;

	static void idle(void);

	void prepareForDisplay(void);
	void redrawDisplay(void);

	void arrayToCvMat(float* transform, CvMat* mat);
};

