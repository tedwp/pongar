#pragma once
#include <string>
std::string toString(int a);
std::string toString(long a);
std::string toString(double a);

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
const int CAM_ANGLE = 60;

const int CAM_INDEX = 0;

const bool FLIP_IMAGE = false;

const float BALL_SPEED_INIT = 3.14159265358979f*0.01f;
const float BALL_SPEED_INCREASE_FACTOR = 1.1f;

const float BALL_RADIUS = 0.15f;

const float PLAYINGFIELD_WIDTH = 6.0f;
const float PLAYINGFIELD_HEIGHT = 4.0f;

const float PADDLE_LENGTH = 1.0f;
const float PADDLE_WIDTH = 0.2f;

//
const bool HIDE_CONSOLE = true;

//Game
const bool FULLSCREEN_ENABLED = true;
const bool FULLSCREEN_ONSTARTUP = false;

//Gamemode only works if FULLSCREEN_ONSTARTUP is enabled
const bool FULLSCREEN_USEGAMEMODE = false;

const bool SHOW_CONVERTED = true;

//only applicable if gamemode is used
const int FULLSCREEN_BITRATE = 32;
const int FULLSCREEN_REFRESHRATE = 60;
const bool FULLSCREEN_DISABLECHECK = true; //Turn on if glut says that the mode is not available despite it's working

const bool SHOW_CAMERA_IMAGE = true;
const int MAX_POINTS_PER_ROUND = 3;

//How long after the last update should a marker still be considered visible?
const int MARKER_VISIBILITYTIMEOUT = 100; //millisec

// How long should the playing field marker have been continously visible before the game accepts it?
const int STARTUP_DURATION = 3000; //millisec
//How long should the two markers be in the game at a time before the game actually starts?
const int INITIALIZATION_DURATION = 3000; //millisec

//Colors
//#include "Color.h"
//const Color BallColor = {red:10,green:10,blue:10};


//Actions

const float ACTION_INCREASESPEED_BALL_FACTOR = 1.1f;
const float ACTION_DECREASESPEED_BALL_FACTOR = 0.9f;