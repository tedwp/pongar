#pragma once
#include "Capture.h"
#include "Graphics.h"

class Game
{
public:
	static Game& getInstance(void);

	void init( void );
	void idle( void );
	void cleanup( void );
	void start(void);

private:
	Game(void);
	Game(const Game&);
	~Game(void);
	static Game& m_instance;
};

