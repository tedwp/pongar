#pragma once
#include "conf.h"
#include "Color.h"
#include "PlayingField.h"
class Ball
{
public:
	Ball(void);
	~Ball(void);
	void render(PlayingField* playingField);

private:
	Color m_color;
	double m_x;
	double m_y;
};

