#pragma once
#include "Marker.h"
#include "PlayingField.h"
class Paddle
{
public:
	Paddle(void);
	~Paddle(void);

	int* getColor(void);
	float getYPosition(void);
	int getScore(void);
	void updatePositionFromMarker(void);
	void setMarker(Marker* marker);
	void render(PlayingField* playingField );
	bool isLeft(void);
	void setLeft(bool isLeft);

private:
	float m_yPosition;
	bool m_isLeft;
	int m_score;
	int m_color[3];
	Marker* m_marker;
};

