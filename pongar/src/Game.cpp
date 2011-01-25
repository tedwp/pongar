#include "Game.h"
#include "Graphics.h"
#include <vector>

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
		
	Capture::getInstance().init();
	Graphics::getInstance().init(argc, argv);
	
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

void Game::idle( void )
{
	Capture::getInstance().updateMarkerPositions();
}

void Game::cleanup( void )
{
}