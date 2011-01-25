#include "Game.h"
#include "Graphics.h"
Game::Game(void) 
{
}


Game::~Game(void)
{
	if( m_instance != NULL )
		delete( m_instance );
}

Game* Game::getInstance(void)
{
	if(m_instance == NULL)
	{
		m_instance = new Game();
	}
	return m_instance;

}

void Game::init( void )
{
	Capture::getInstance()->init();
	Graphics::getInstance()->init();
	
}
void start(void)
{
	Graphics::getInstance()->start();
}

void Game::idle( void )
{
}

void Game::cleanup( void )
{
}