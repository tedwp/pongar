#pragma once

#include <cv.h>
#include <highgui.h>
#include "GL\glut.h"

#define getTimeSinceStart() (glutGet(GLUT_ELAPSED_TIME))

//capture
const int THRESHOLD = 160;
const int THRESHOLD_BW = 130;

//camera
const int CAM_WIDTH = 640; 
const int CAM_HEIGHT = 480;

//macbook pro camera: CAM_ANGLE = 60
//cheap logilink camera: CAM_ANGLE = 75
const int CAM_ANGLE = 62;

const int CAM_INDEX = 0;

const float BALL_SPEED_INIT = 3.14159265358979f*0.005f;
const float BALL_SPEED_INCREASE_FACTOR = 1.2f;

const float BALL_RADIUS = 0.05f;

const float PLAYINGFIELD_WIDTH = 1.6f;
const float PLAYINGFIELD_HEIGHT = 0.0f;

const float PADDLE_LENGTH = 0.2f;
const float PADDLE_WIDTH = 0.03f;


//Game
const bool FULLSCREEN_ENABLED = true;
const bool SHOW_CAMERA_IMAGE = true;

//How long after the last update should a marker still be considered visible?
const int MARKER_VISIBILITYTIMEOUT = 100; //millisec

// How long should the playing field marker have been continously visible before the game accepts it?
const int STARTUP_DURATION = 3000; //millisec
//How long should the two markers be in the game at a time before the game actually starts?
const int INITIALIZATION_DURATION = 3000; //millisec

//Colors
//#include "Color.h"
//const Color BallColor = {red:10,green:10,blue:10};

