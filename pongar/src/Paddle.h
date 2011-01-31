#pragma once
#include "Marker.h"
#include "PlayingField.h"
#include "Color.h"
class Paddle
{
public:
	Paddle(void);
	~Paddle(void);

	Color getColor(void);
	void setColor(int r, int g, int b);
	void setColor(Color c);
	float getYPosition(void);
	int getScore(void);
	void updatePositionFromMarker(void);
	void setMarker(Marker* marker);
	void render(PlayingField* playingField );
	bool isLeft(void);
	void setLeft(bool isLeft);

private:
	float m_yPosition;
	float* m_historyYPosition[SMOOTHING_HISTORYSIZE];
	double* m_timestampsYPosition[SMOOTHING_HISTORYSIZE];
	int m_historyLength;
	bool m_isLeft;
	int m_score;
	Color m_color;
	Marker* m_marker;
};

