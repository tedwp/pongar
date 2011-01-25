#pragma once
#include "Capture.h"
#include "Graphics.h"

class Game
{
public:
	Game(void);
	~Game(void);

	void init( void );
	void idle( void );
	void cleanup( void );
private:
	Graphics m_graphics;
	Capture m_capture
};

