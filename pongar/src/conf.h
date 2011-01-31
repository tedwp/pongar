#pragma once

#include <cv.h>
#include <highgui.h>

//capture
const int THRESHOLD = 100;
const int THRESHOLD_BW = 100;

//camera
const int CAM_WIDTH = 640; 
const int CAM_HEIGHT = 480;

//macbook pro camera: CAM_ANGLE = 60
//cheap logilink camera: CAM_ANGLE = 75
const int CAM_ANGLE = 60;

const int CAM_INDEX = 0;

const float BALL_SPEED_INIT = 3.14159265358979f*0.005f;
const float BALL_SPEED_INCREASE_FACTOR = 1.2f;

const float BALL_RADIUS = 0.05f;

const float PLAYINGFIELD_WIDTH = 1.6f;
const float PLAYINGFIELD_HEIGHT = 1.0f;

const float PADDLE_LENGTH = 0.2f;
const float PADDLE_WIDTH = 0.03f;


//Game
const bool FULLSCREEN_ENABLED = true;
const bool SHOW_CAMERA_IMAGE = true;