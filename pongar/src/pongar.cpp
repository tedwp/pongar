//schtest
#include "conf.h"
#include "Game.h"

int main(int argc, char* argv[]) 
{
	Game::getInstance().init(argc, argv);
	Game::getInstance().start();
    return 0;
}
