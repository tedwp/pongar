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
	
	int getState(void);
	void reset(void);

	void enableActionSpeedIncrease(void);
	void disableActionSpeedIncrease(void);
	void enableActionSpeedDecrease(void);
	void disableActionSpeedDecrease(void);
	
	
	static const int ONFIELD = 1;
	static const int LEFTOUT = 2;
	static const int RIGHTOUT = 3;

private:
	Color m_color;
	float m_x;
	float m_y;
	int m_state;

	PlayingField* m_playingField;
	float m_speed;
	float m_angle;
	
	bool m_actionSpeedIncreaseEnabled;
	bool m_actionSpeedDecreaseEnabled;

	void drawCircle(float r);
	void updateMovement(void);
};

