#pragma once
#include "conf.h"
#include <cv.h>
#include "GL\glut.h"
#include "Marker.h"
#include "Game.h"
#include "Paddle.h"

class PlayingField
{
public:
	PlayingField(void);
	~PlayingField(void);
	static PlayingField& getInstance();
	void render();
	void drawCircle(float r);
	void computeBallPosition(float paddle1Start, float paddle1End, float paddle2Start, float paddle2End);
	void setCorrespondingMarker(Marker* marker);
	Marker* getCorrespondingMarker(void);
	//void setPaddle(Paddle* paddle, bool isLeft);
	void updatePaddlePositions(void);
private:
	float ballSpeed;
	float ballAngle;
	std::pair<float, float> ballVector;
	std::pair<float, float> ballPosition;
	void transposeMatrix(float* src, float* dst);
	Marker* m_correspondingMarker;
	Paddle* m_leftPaddle;
	//Paddle* m_rightPaddle;
};

