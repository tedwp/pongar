#pragma once
#include "Marker.h"
#include "PlayingField.h"
#include "Color.h"

class PlayingField;

class Paddle
{
public:
	Paddle(void);
	~Paddle(void);

	Color getColor(void);
	void setColor(float r, float g, float b, float a);
	void setColor(Color& c);
	float getYPosition(void);
	int getScore(void);
	void updatePositionFromMarker(void);
	void setMarker(Marker* marker);
	void render(void);
	bool isLeft(void);
	void setLeft(bool isLeft);

	void setPlayingField(PlayingField* playingField);

private:
	float m_yPosition;
	bool m_isLeft;
	int m_score;
	Color m_color;
	Marker* m_marker;
	PlayingField* m_playingField;
	void arrayToCvMat(float* transform, CvMat* mat);
};

