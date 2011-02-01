#include "Game.h"

using namespace std;

Game::Game(void) 
{
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
	getInstance().registerMarker(2884, PURPOSE_PADDLE1);
	getInstance().registerMarker(626, PURPOSE_PADDLE2);
	getInstance().registerMarker(90, PURPOSE_PLAYINGFIELD);
		
	
	m_gameStage = STAGE_STARTUP;
	timerStart = getTimeSinceStart();
	
	Capture::getInstance().init();
	Graphics::getInstance().init(argc, argv);
	
	
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
void Game::start(void)
{
	Graphics::getInstance().start();
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

void Game::idle( void )
{
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
		default:
		break;
	}

	
}
void Game::performStageStartup(void)
{
	if(isMarkerPresent(PURPOSE_PLAYINGFIELD))
	{
		if(timerStart - getTimeSinceStart() > STARTUP_DURATION)
		{
			//TODO here: Fix playing field
			m_gameStage = STAGE_INITIALIZATION;
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
	if(isMarkerPresent(PURPOSE_PADDLE1) && isMarkerPresent(PURPOSE_PADDLE2) )
	{
		if(timerStart - getTimeSinceStart() > INITIALIZATION_DURATION)
		{
			m_gameStage = STAGE_RUNNING;
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
	if(isMarkerPresent(PURPOSE_PAUSE))
		m_gameStage = STAGE_PAUSE;
			
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
		if(isMarkerPresent(PURPOSE_ACTION_INCREASESPEED_GAME))
		{
		}
		if(isMarkerPresent(PURPOSE_ACTION_INCREASESPEED_PADDLE1))
		{
		}
		if(isMarkerPresent(PURPOSE_ACTION_INCREASESPEED_PADDLE2))
		{
		}
		if(isMarkerPresent(PURPOSE_ACTION_DECREASESPEED_GAME))
		{
		}
		if(isMarkerPresent(PURPOSE_ACTION_DECREASESPEED_PADDLE1))
		{
		}
		if(isMarkerPresent(PURPOSE_ACTION_DECREASESPEED_PADDLE2))
		{
		}
	}
}
void Game::performStagePause(void)
{
	if(isMarkerPresent(PURPOSE_PAUSE))
		m_gameStage = STAGE_RUNNING;
			
	if(isMarkerPresent(PURPOSE_RESTARTGAME))
	{
		/*
		TODO do restart stuff
		*/
		m_gameStage = STAGE_RUNNING;
	}
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
//#define markerPresent(a) false
void Game::end(void)
{
	exit(0);
}
void Game::cleanup( void )
{
	Capture::getInstance().cleanup();
}