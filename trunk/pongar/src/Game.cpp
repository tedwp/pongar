#include "Game.h"
#include <iostream>
#include "UI.h"

using namespace std;

Game::Game(void) 
{
	//m_markerImage = NULL;
}
Game::~Game(void)
{
	for(unsigned i = 0; i < m_markers.size(); i++)
	{
		delete(m_markers[i]);
	}
	m_markers.clear();
}

Game& Game::getInstance(void)
{
	static Game m_instance;
	return m_instance;
}

void Game::init( int argc, char* argv[] )
{
	getInstance().registerMarkers();
		
	
	m_gameStage = STAGE_BEAMERCALIBRATION;
	timerStart = getTimeSinceStart();
	
	Capture::getInstance().init();
	Graphics::getInstance().init(argc, argv);
	
	
}
void Game::registerMarkers(void)
{
	registerMarker(2884, PURPOSE_PADDLE1); // 1
	registerMarker(626, PURPOSE_PADDLE2);  // 2
	registerMarker(90, PURPOSE_PLAYINGFIELD);
}
void Game::start(void)
{
	Graphics::getInstance().start();
}


void Game::idle( void )
{
	for(unsigned i = 0; i < m_markers.size(); i++)
		m_markers[i]->clearHasPositionChanged();
	Capture::getInstance().updateMarkerPositions();
	/**
	Hier kommt die Spiellogik rein
	Diese Methode wird von Anfang an immer wieder aufgerufen.
	Um verschiedene Spielphasen abzubilden, wäre ein Property m_gameStage sinnvoll,
	z.B. m_gameStage = Game.STAGE_INITIALIZATION oder m_gameStage = Game.STAGE_PAUSE oder ..._RUNNING usw.

	- Die Zeit muss gemessen werden und es muss klar sein, wie lange der letzte Aufruf dieser Methode zurück liegt (~ FPS)
	  Wichtig für Animationen und andere Sachen (z.B. 3 Sekunden Spielfeldkalibrierung oÄ)

	 Grundidee:
	 - Steuerung des Spiels durch Karten. Jeweils einem PURPOSE zugeordnet (siehe Game::init())
	 - Zugriff auf Paddle-Marker mit Hilfe von Game::getMarkerByPurpose();
	 - Jedes Paddle-Objekt hat ein Marker-Property, mit dem es seine Position aktualisieren kann.
	 **/

	
//	if(getMarkerByPurpose(PURPOSE_REINIT) != NULL && getMarkerByPurpose(PURPOSE_REINIT)->isVisible())
	if(isMarkerPresent(PURPOSE_REINIT))
	{
		// In the eyes of the players Restarts the whole application. Asks for all markers again
		/*
		do reinit stuff 
		*/
		m_gameStage = STAGE_STARTUP;
	}
	switch(m_gameStage)
	{
		case STAGE_BEAMERCALIBRATION:
			performStageBeamerCalibration();
		break;

		case STAGE_STARTUP:
			performStageStartup();
		break;
		
		case STAGE_INITIALIZATION:
			performStageInitialization();
			break;
		
		case STAGE_RUNNING:
			performStageRunning();
			break;
			
		case STAGE_PAUSE:
			performStagePause();
			break;
		case STAGE_WON_LEFT:
			performStageWon(true);
			break;
		case STAGE_WON_RIGHT:
			performStageWon(false);
			break;
		default:
		break;
	}

	
}
void Game::performStageBeamerCalibration(void)
{
	UI::getInstance().showHeading("STAGE_BEAMERCALIBRATION");
	//Graphics::showImage("marker.jpg");
	//Track the marker and estimate the canvas' pose

	// load the image
    /*if(m_markerImage == NULL)
	{
		m_markerImage = cvLoadImage("marker.jpg", CV_LOAD_IMAGE_COLOR);
	}*/
	/*
    	/* create a window * /
		cvNamedWindow( "image", CV_WINDOW_FULLSCREEN);
		/* display the image * /
		cvShowImage( "image", img );*/
	PlayingField::getInstance().setColor(1.0f, 1.0f, 1.0f, 1.0f);
	setGameStage(STAGE_STARTUP);
}

void Game::performStageStartup(void)
{
	UI::getInstance().showHeading("STAGE_STARTUP");
	UI::getInstance().showInstruction("Bitte Spielfeldmarker platzieren.");
	if(isMarkerPresent(PURPOSE_PLAYINGFIELD))
	{
		UI::getInstance().showPercentageString("Spielfeldmarker initialisieren: ", getTimeSinceStart() - timerStart, STARTUP_DURATION);
		PlayingField::getInstance().setCorrespondingMarker(getMarkerByPurpose(PURPOSE_PLAYINGFIELD));
		if(getTimeSinceStart() - timerStart> STARTUP_DURATION)
		{
			PlayingField::getInstance().setColor(1.0f, 1.0f, 1.0f, 1.0f);
			//TODO remove line above

			PlayingField::getInstance().getCorrespondingMarker()->lock();
			setGameStage(STAGE_INITIALIZATION);
		}
	}
	else
	{
		timerStart = getTimeSinceStart();
	}
	/*
	Do nothing except, wait until playing field is positioned
	*/
}
void Game::performStageInitialization(void)
{
	UI::getInstance().showHeading("STAGE_INITIALIZATION");
	UI::getInstance().showInstruction("Bitte Paddlemarker platzieren.");
	if(isMarkerPresent(PURPOSE_PADDLE1) && isMarkerPresent(PURPOSE_PADDLE2) )
	{
		UI::getInstance().showPercentageString("Paddlemarker initialisieren: ", getTimeSinceStart() - timerStart, INITIALIZATION_DURATION);
		if(getTimeSinceStart() - timerStart > INITIALIZATION_DURATION)
		{
			Paddle* leftPaddle = PlayingField::getInstance().spawnPaddle(true);
			leftPaddle->setMarker(getMarkerByPurpose(PURPOSE_PADDLE1));
			leftPaddle->setColor(0.5f, 0.5f, 0.5f, 1.0f);

			Paddle* rightPaddle = PlayingField::getInstance().spawnPaddle(false);
			rightPaddle->setMarker(getMarkerByPurpose(PURPOSE_PADDLE2));
			rightPaddle->setColor(0.0f, 1.0f, 0.0f, 1.0f);
			
			Ball* ball = PlayingField::getInstance().spawnBall();
			ball->setColor(0.0f, 0.0f, 1.0f, 1.0f);

			setGameStage(STAGE_RUNNING);
		}
	}
	else
	{
		timerStart = getTimeSinceStart();
	}
	// Nicht zu verwechseln mit init()! Hier wird das Spiel aus Sicht der Benutzers initialisiert, alles technische geschieht hingegen in init()
}
void Game::performStageRunning(void)
{
	UI::getInstance().showHeading("STAGE_RUNNING");

	PlayingField::getInstance().updatePaddlePositions();
	PlayingField::getInstance().render();
	Ball* ball = PlayingField::getInstance().getBall();
	if(ball->getState() != Ball::ONFIELD)
	{
		if(ball->getState() == Ball::LEFTOUT)
			PlayingField::getInstance().getRightPaddle()->increaseScore();

		else if(ball->getState() != Ball::RIGHTOUT)
			PlayingField::getInstance().getLeftPaddle()->increaseScore();

		if(PlayingField::getInstance().getLeftPaddle()->getScore() > MAX_POINTS_PER_ROUND)
			setGameStage(STAGE_WON_LEFT);

		if(PlayingField::getInstance().getRightPaddle()->getScore() > MAX_POINTS_PER_ROUND)
			setGameStage(STAGE_WON_RIGHT);
	}

	if(isMarkerPresent(PURPOSE_PAUSE))
	{
		setGameStage(STAGE_PAUSE);
	}
	else
	{
		UI::getInstance().showScores();
		//TODO Idee haben: Aktuell müssen alle actions erst deaktiviert werden
		
		ball->disableActionSpeedIncrease();
		ball->disableActionSpeedDecrease();


		if(isActionMarkerPresent())
		{
			if(isMarkerPresent(PURPOSE_ACTION_INCREASESIZE_PADDLE1))
			{
			}
			if(isMarkerPresent(PURPOSE_ACTION_INCREASESIZE_PADDLE2))
			{
			}
			if(isMarkerPresent(PURPOSE_ACTION_DECREASESIZE_PADDLE1))
			{
			}
			if(isMarkerPresent(PURPOSE_ACTION_DECREASESIZE_PADDLE2))
			{
			}
			if(isMarkerPresent(PURPOSE_ACTION_INCREASESPEED_BALL))
			{
				ball->enableActionSpeedIncrease();
			}
			if(isMarkerPresent(PURPOSE_ACTION_INCREASESPEED_PADDLE1))
			{
			}
			if(isMarkerPresent(PURPOSE_ACTION_INCREASESPEED_PADDLE2))
			{
			}
			if(isMarkerPresent(PURPOSE_ACTION_DECREASESPEED_BALL))
			{
				ball->enableActionSpeedDecrease();
			}
			if(isMarkerPresent(PURPOSE_ACTION_DECREASESPEED_PADDLE1))
			{
			}
			if(isMarkerPresent(PURPOSE_ACTION_DECREASESPEED_PADDLE2))
			{
			}
		}
	}
}
void Game::performStagePause(void)
{
	UI::getInstance().showHeading( "STAGE_PAUSE");
	if(!isMarkerPresent(PURPOSE_PAUSE))
		setGameStage(STAGE_RUNNING);
			
	if(isMarkerPresent(PURPOSE_RESTARTGAME))
	{
		performRestart();
		setGameStage(STAGE_RUNNING);
	}
}
void Game::performStageWon(bool isLeft)
{
	UI::getInstance().showHeading( "GAME_OVER");
	if(isLeft)
		UI::getInstance().showInstruction("Links hat gewonnen");
	else
		UI::getInstance().showInstruction("rechts hat gewonnen");
	if(isMarkerPresent(PURPOSE_RESTARTGAME))
	{
		performRestart();
		setGameStage(STAGE_RUNNING);
	}
}
void Game::performRestart(void)
{
	PlayingField::getInstance().getBall()->reset();
	PlayingField::getInstance().getLeftPaddle()->reset();
	PlayingField::getInstance().getRightPaddle()->reset();
}
bool Game::isActionMarkerPresent(void)
{
	for(unsigned i = 0; i < getInstance().m_markers.size(); i++)
	{
		int p = (getInstance().m_markers[i])->getPurpose();
		if(	p > 100 && p < 200)
			return true;
	}
	return false;
}
bool Game::isMarkerPresent(int purpose)
{
	Marker* m = getMarkerByPurpose(purpose);
	return m != NULL && m->isVisible();
}
void Game::cleanup( void )
{
	Capture::getInstance().cleanup();
}
vector<Marker*> Game::getMarkers(void)
{
	return getInstance().m_markers;
}
Marker* Game::getMarkerById(int id)
{
	for(unsigned i = 0; i < getInstance().m_markers.size(); i++)
	{
		if(	(getInstance().m_markers[i])->getId() == id)
		{
			return getInstance().m_markers[i];
		}
	}
	return NULL;
}
Marker* Game::getMarkerByPurpose(int purpose)
{
	for(unsigned i = 0; i < getInstance().m_markers.size(); i++)
	{
		if(	(getInstance().m_markers[i])->getPurpose() == purpose)
		{
			return getInstance().m_markers[i];
		}
	}
	return NULL;
}
int Game::getGameStage()
{
	return m_gameStage;
}
void Game::setGameStage(int gameStage)
{
	m_gameStage = gameStage;
}
void Game::registerMarker(int id, int purpose)
{
	Marker* m = new Marker(id);
	m->setPurpose(purpose);
	m_markers.push_back(m);
}

void Game::end(void)
{
	Graphics::getInstance().cleanup();
	Capture::getInstance().cleanup();
	exit(0);
}