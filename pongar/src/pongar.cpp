//schtest
#include "conf.h"
#include "Game.h"

#include <iostream>
#include <iomanip>

#include "GL/glut.h"


using namespace std;



int main(int argc, char* argv[]) 
{
	Game::getInstance().init(argc, argv);

    // start the action

	Game::getInstance().start();

    return 0;
}
