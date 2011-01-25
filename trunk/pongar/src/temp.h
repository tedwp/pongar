#pragma once

#include <cv.h>
#include <highgui.h>

////thresholding
//int thresh = 100;
//int bw_thresh = 100;
//
////camera
//CvCapture* cap;
//const int width = 640; 
//const int height = 480;
////macbook pro camera: camangle = 60
//const int camangle = 60;
//
//float resultMatrix_005A[16];
//float resultMatrix_0272[16];
//
//CvMemStorage* memStorage;
//unsigned char bkgnd[width*height*3];

//thresholding
int thresh = 100;
int bw_thresh = 100;

//camera
CvCapture* cap;
const int width = 640; 
const int height = 480;
//macbook pro camera: camangle = 60
const int camangle = 60;

float resultMatrix_005A[16];
float resultMatrix_0272[16];

CvMemStorage* memStorage;
unsigned char bkgnd[width*height*3];

//UltraTEMP

void idle(void);