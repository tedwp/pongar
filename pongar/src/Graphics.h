#pragma once
#include "temp.h"

class Graphics
{
public:
	static Graphics& getInstance(void);
	void init(void);
	void start(void);

private:
	Graphics(void);
	Graphics(const Graphics&);
	~Graphics(void);
	static Graphics& m_instance;


	static void render();
	void doRender(void);

	static void resize( int w, int h) ;
	void doResize( int w, int h) ;

	void prepareForDisplay(void);
	void redrawDisplay(void);
	void Graphics::cleanup(void);
	
};

