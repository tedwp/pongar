#pragma once
#include <cv.h>
#include "GL\glut.h"
#include "Marker.h"
#include "Game.h"

class PlayingField
{
public:
	PlayingField(void);
	~PlayingField(void);
	static PlayingField& getInstance();
	void render();
	void drawCircle(float r);
	void computeBallPosition(float paddle1Start, float paddle1End, float paddle2Start, float paddle2End);
private:
	float ballSpeed;
	float ballAngle;
	std::pair<float, float> ballVector;
	std::pair<float, float> ballPosition;
	void transposeMatrix(float* src, float* dst);

};
