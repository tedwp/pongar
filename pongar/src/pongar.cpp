//Blendet die Konsole unter Windows von Anfang an aus
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") 

#include "conf.h"
#include "Game.h"
#include <string>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;


std::string toString(int a)
{
	std::stringstream tmp;
	tmp << a;
	return tmp.str();
}
std::string toString(long a)
{
	std::stringstream tmp;
	tmp << setprecision(3);
	tmp << a;
	return tmp.str();
}
std::string toString(float a)
{
	std::stringstream tmp;
	tmp << setprecision(3);
	tmp << a;
	return tmp.str();
}
std::string toString(double a)
{
	std::stringstream tmp;
	tmp << setprecision(3);
	tmp << a;
	return tmp.str();
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
