#pragma once
#include "Marker.h"

class Paddle
{
public:
	Paddle(void);
	~Paddle(void);
	void render();

private:
	Marker* marker;
};

