#pragma once
#include "conf.h"
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
	int getGameStage();
	void setGameStage(int stage);

	static void timer(int value);

	static std::vector<Marker*> getMarkers(void);
	static Marker* getMarkerById(int id);
	static Marker* getMarkerByPurpose(int purpose);
	void end(void);

	static const int PURPOSE_PADDLE1 = 1;
	static const int PURPOSE_PADDLE2 = 2;
	static const int PURPOSE_PLAYINGFIELD = 3;
	static const int PURPOSE_PAUSE = 4;
	static const int PURPOSE_REINIT = 5;
	static const int PURPOSE_RESTARTGAME = 6;

	static const int STAGE_STARTUP = 1;
	static const int STAGE_INITIALIZATION = 2;
	static const int STAGE_RUNNING = 3;
	static const int STAGE_PAUSE = 4;
	static const int STAGE_OVER = 5;

private:
	static Game& m_instance;
	std::vector<Marker*> m_markers;
	int m_gameStage;

	Game(void);
	Game(const Game&);
	~Game(void);
	void Game::registerMarker(int id, int purpose);
	void updateMarkerOffsets(void);
	void arrayToCvMat(float* transform, CvMat* mat);
	void performInitialization(void);

	long int timerStart;
};


