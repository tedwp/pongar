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
	//float getYRenderPosition(void);
	void updatePositionFromMarker(void);
	void setMarker(Marker* marker);
	void render(void);
	bool isLeft(void);
	void setLeft(bool isLeft);

	void setPlayingField(PlayingField* playingField);
	void increaseScore(void);
	int getScore(void);
	void reset(void);
	float getSize(void);

	
	void enableActionSizeIncrease(void);
	void disableActionSizeIncrease(void);
	void enableActionSizeDecrease(void);
	void disableActionSizeDecrease(void);
	void disableAllActions(void);

	float m_zoomFactor;
	float m_zoomOffset;

private:
	float m_yPosition;
	//float m_yRenderPosition;
	bool m_isLeft;
	float m_size;
	int m_score;
	Marker* m_marker;
	PlayingField* m_playingField;
	void arrayToCvMat(float* transform, CvMat* mat);
	bool m_actionSizeIncreaseEnabled;
	bool m_actionSizeDecreaseEnabled;
};

