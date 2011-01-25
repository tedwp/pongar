#pragma once
#include <vector>

#include "Marker.h"
#include "Capture.h"
#include "Graphics.h"

class Game
{
public:
	static Game& getInstance(void);

	void init( int argc, char* argv[] );
	void idle( void );
	void cleanup( void );
	void start(void);

	static std::vector<Marker*> getMarkers(void);
	static Marker getMarkerById(int id);

private:
	Game(void);
	Game(const Game&);
	~Game(void);
	static Game& m_instance;
	std::vector<Marker*> m_markers;
};

