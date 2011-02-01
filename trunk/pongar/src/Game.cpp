#include "Game.h"

using namespace std;

#define markerPresent(a) ( getMarkerByPurpose(a) != NULL && getMarkerByPurpose(a)->isVisible())
//#define markerPresent(a) false

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
	if(markerPresent(PURPOSE_REINIT))
	{
		/*
		do restart stuff
		*/
		m_gameStage = STAGE_STARTUP;
	}
	
	switch(m_gameStage)
	{
		case STAGE_STARTUP:
			if(markerPresent(PURPOSE_PLAYINGFIELD))
			{
				if(timerStart - getTimeSinceStart() > STARTUP_DURATION)
				{
					m_gameStage = STAGE_INITIALIZATION;
					break;
				}
			}
			else
			{
				timerStart = getTimeSinceStart();
			}
			/*
			Do nothing except, wait until playing field is positioned
			*/
		break;
		
		case STAGE_INITIALIZATION:
			if(markerPresent(PURPOSE_PADDLE1) && markerPresent(PURPOSE_PADDLE2) )
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
			//performInitialization();
			break;
		
		case STAGE_RUNNING:
			if(markerPresent(PURPOSE_PAUSE))
				m_gameStage = STAGE_PAUSE;
			
			//performRunning();
			break;

		case STAGE_PAUSE:
			if(markerPresent(PURPOSE_PAUSE))
				m_gameStage = STAGE_RUNNING;
			
			if(markerPresent(PURPOSE_RESTARTGAME))
			{
				/*
				do restart stuff
				*/
				m_gameStage = STAGE_RUNNING;
			}
			break;
		default:
		break;
	}

	
}

void Game::end(void)
{
	exit(0);
}
void Game::cleanup( void )
{
	Capture::getInstance().cleanup();
}