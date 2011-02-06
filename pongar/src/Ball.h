#pragma once
#include "conf.h"
#include "Color.h"
#include "PlayingField.h"
#include "ColoredObject.h"
#include "UI.h"
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
	void resumeAfterPause(void);

	void enableActionSpeedIncrease(void);
	void disableActionSpeedIncrease(void);
	void enableActionSpeedDecrease(void);
	void disableActionSpeedDecrease(void);

	void enableActionSizeIncrease(void);
	void disableActionSizeIncrease(void);
	void enableActionSizeDecrease(void);
	void disableActionSizeDecrease(void);
	
	void updateMovement(void);
	void disableAllActions(void);
	float getRadius(void);
	
	
	static const int ONFIELD = 1;
	static const int LEFTOUT = 2;
	static const int RIGHTOUT = 3;

private:
	float m_x;
	float m_y;
	float m_radius;
	int m_state;
	PlayingField* m_playingField;
	float m_speed;
	float m_angle;
	bool m_actionSizeIncreaseEnabled;
	bool m_actionSizeDecreaseEnabled;

	std::pair<float, float> m_direction;
	
	bool m_actionSpeedIncreaseEnabled;
	bool m_actionSpeedDecreaseEnabled;

	void drawCircle(float r);
	long int m_lastUpdate;
};


