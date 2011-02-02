#pragma once
#include "Marker.h"
#include "PlayingField.h"
#include "Color.h"
#include "ColoredObject.h"

class PlayingField;

class Paddle : public ColoredObject
{
public:
	Paddle(void);
	~Paddle(void);

	float getYPosition(void);
	float getYRenderPosition(void);
	void updatePositionFromMarker(void);
	void setMarker(Marker* marker);
	void render(void);
	bool isLeft(void);
	void setLeft(bool isLeft);

	void setPlayingField(PlayingField* playingField);
	void increaseScore(void);
	int getScore(void);
	void reset();

private:
	float m_yPosition;
	float m_yRenderPosition;
	bool m_isLeft;
	int m_score;
	Marker* m_marker;
	PlayingField* m_playingField;
	void arrayToCvMat(float* transform, CvMat* mat);
};
