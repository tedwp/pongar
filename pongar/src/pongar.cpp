//schtest
#include "conf.h"
#include "Game.h"
#include <string>
#include <stdio.h>
#include <sstream>

std::string toString(int a)
{
	std::stringstream tmp;
	tmp << a;
	return tmp.str();
}
std::string toString(long a)
{
	return toString((int) a);
}
std::string toString(double a)
{
	return toString((int) a);
}

int main(int argc, char* argv[]) 
{
	Game::getInstance().init(argc, argv);
	Game::getInstance().start();
    return 0;
}
