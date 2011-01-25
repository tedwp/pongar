#include "Game.h"

Game::Game(void) 
{
}

Game::~Game( void )
{
}

void Game::init( void )
{
	m_graphics.init();
	m_capture.init();
	
}

void Game::idle( void )
{
}

void Game::cleanup( void )
{
}