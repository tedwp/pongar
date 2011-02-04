//Blendet die Konsole unter Windows von Anfang an aus
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") 

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
	//Blendet die Konsole unter allen Systemen kurz nach dem Start aus
	if(HIDE_CONSOLE)
		FreeConsole();
	Game::getInstance().init(argc, argv);
	Game::getInstance().start();
    return 0;
}
