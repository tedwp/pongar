#pragma once
#include "conf.h"
#include "Color.h"
#include "PlayingField.h"
#include "ColoredObject.h"

class PlayingField;

class Ball : public ColoredObject
{
public:
	Ball(void);
	~Ball(void);
	void render();
	void setPlayingField(PlayingField* playingField);

private:
	Color m_color;
	double m_x;
	double m_y;
	PlayingField* m_playingField;
	float speed;
	float angle;
	std::pair<float, float> vector;
	std::pair<float, float> position;
	

	void drawCircle(float r);
};

