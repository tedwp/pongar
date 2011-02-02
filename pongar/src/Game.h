#pragma once
#include "conf.h"
#include <vector>
#include "Marker.h"
#include "Capture.h"
#include "Graphics.h"
#include "PlayingField.h"
#include "Ball.h"

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
	void registerMarkers(void);
	static void timer(int value);

	static std::vector<Marker*> getMarkers(void);
	static Marker* getMarkerById(int id);
	static Marker* getMarkerByPurpose(int purpose);
	void end(void);
	bool isMarkerPresent(int purpose);

	//IplImage* m_markerImage;

	static const int PURPOSE_PADDLE1 = 1;
	static const int PURPOSE_PADDLE2 = 2;
	static const int PURPOSE_PLAYINGFIELD = 3;
	static const int PURPOSE_PAUSE = 4;
	static const int PURPOSE_REINIT = 5;
	static const int PURPOSE_RESTARTGAME = 6;
	
	static const int PURPOSE_ACTION_INCREASESIZE_PADDLE1 = 101;
	static const int PURPOSE_ACTION_INCREASESIZE_PADDLE2 = 102;

	static const int PURPOSE_ACTION_DECREASESIZE_PADDLE1 = 103;
	static const int PURPOSE_ACTION_DECREASESIZE_PADDLE2 = 104;
	
	static const int PURPOSE_ACTION_INCREASESPEED_BALL = 105;
	static const int PURPOSE_ACTION_INCREASESPEED_PADDLE1 = 106;
	static const int PURPOSE_ACTION_INCREASESPEED_PADDLE2 = 107;
	
	static const int PURPOSE_ACTION_DECREASESPEED_BALL = 108;
	static const int PURPOSE_ACTION_DECREASESPEED_PADDLE1 = 109;
	static const int PURPOSE_ACTION_DECREASESPEED_PADDLE2 = 110;
	
	static const int STAGE_BEAMERCALIBRATION = 1;
	static const int STAGE_STARTUP = 2;
	static const int STAGE_INITIALIZATION = 3;
	static const int STAGE_RUNNING = 4;
	static const int STAGE_PAUSE = 5;
	static const int STAGE_WON_LEFT = 6;
	static const int STAGE_WON_RIGHT = 7;

private:
	static Game& m_instance;
	std::vector<Marker*> m_markers;
	int m_gameStage;
	long int timerStart;

	Game(void);
	Game(const Game&);
	~Game(void);
	void registerMarker(int id, int purpose);
	void updateMarkerOffsets(void);
	void arrayToCvMat(float* transform, CvMat* mat);
	void performInitialization(void);
	bool isActionMarkerPresent(void);
	void performStageBeamerCalibration(void);
	void performStageStartup(void);
	void performStageInitialization(void);
	void performStageRunning(void);
	void performStagePause(void);
	void performStageWon(bool isLeft);
	void performRestart(void);
};


