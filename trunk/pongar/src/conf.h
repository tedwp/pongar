#pragma once

#include <cv.h>
#include <highgui.h>

//camera
const int CAM_WIDTH = 640; 
const int CAM_HEIGHT = 480;

//macbook pro camera: CAM_ANGLE = 60
//cheap logilink camera: CAM_ANGLE = 75
const int CAM_ANGLE = 60;

const int CAM_INDEX = 0;

const float BALL_SPEED_INIT = 3.14159265358979*0.003;
const float BALL_SPEED_INCREASE_FACTOR = 1.05;

const float BALL_RADIUS = 0.05;

const float PLAYINGFIELD_WIDTH = 1.6;
const float PLAYINGFIELD_HEIGHT = 1.0;

const float PADDLE_LENGTH = 0.2;