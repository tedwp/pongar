#pragma once
#include "conf.h"
#include <cv.h>
#include "GL\glut.h"
#include "Marker.h"
#include "Game.h"
#include "Paddle.h"
#include "Color.h"
#include "ColoredObject.h"

class Paddle;
class Ball;

class PlayingField : public ColoredObject
{
public:
	PlayingField(void);
	~PlayingField(void);
	static PlayingField& getInstance();
	void render();
	void computeBallPosition(float paddle1Start, float paddle1End, float paddle2Start, float paddle2End);
	void setCorrespondingMarker(Marker* marker);
	Marker* getCorrespondingMarker(void);
	void setPaddle(Paddle* paddle, bool isLeft);
	void updatePaddlePositions(void);
	Paddle* spawnPaddle(bool isLeft);
	Ball* spawnBall(void);
	void setBall(Ball* ball);
	Paddle* getLeftPaddle(void);
	Paddle* getRightPaddle(void);
	Ball* getBall(void);
private:
	
	Marker* m_correspondingMarker;
	Paddle* m_leftPaddle;
	Paddle* m_rightPaddle;
	Ball* m_ball;
};

