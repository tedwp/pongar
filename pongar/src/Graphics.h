#pragma once
#include "conf.h"
#include "PlayingField.h"
#include "Keyboard.h"
#include "Color.h"
#include <string>

struct vertex
{
	float x, y, z;
};

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
	void showString(std::string str, Color& c, int cx, int y);

	void setCamPosition( vertex& pos );
	vertex& getCamPosition( void );
	void moveCamera( float x, float y , float z );
	void changeFov( float step );

	static void transposeMatrix(float* src, float* dst);
	void cleanup(void);

	

private:
	bool m_isInFullScreen;
	bool m_isInGameMode;
	static Graphics& m_instance;
	int m_mainWindow;
	vertex m_vcamPosition;
	float m_vcamFov, m_vcamAspect, m_vcamNear, m_vcamFar;

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

	void arrayToCvMat(float* transform, CvMat* mat);
	void prepare2D(void);
	void finish2D(void);
};

