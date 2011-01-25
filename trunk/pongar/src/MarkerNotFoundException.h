#pragma once
#include <exception>

class MarkerNotFoundException :
	public std::exception
{
public:
	MarkerNotFoundException(void);
	~MarkerNotFoundException(void);
};

