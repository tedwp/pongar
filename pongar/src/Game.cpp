#include "Game.h"
#include "Graphics.h"
Game::Game(void) 
{
}


Game::~Game(void)
{
}

Game& Game::getInstance(void)
{
	static Game m_instance;
	return m_instance;

}

void Game::init( void )
{
	Capture::getInstance()->init();
	Graphics::getInstance().init();
	
}
void Game::start(void)
{
	Graphics::getInstance().start();
}

void Game::idle( void )
{
}

void Game::cleanup( void )
{
}