#include <iostream>


#include "temp.h"

#include "Capture.h"
#include "Marker.h"

#include <cv.h>
#include <highgui.h>

#include "PoseEstimation.h"



using namespace std;



Capture::~Capture(void)
{
}

Capture& Capture::getInstance(void)
{
	static Capture m_instance;
	return m_instance;

}
void Capture::setMarkers(std::vector<Marker*> markers)
{
	m_markers = markers;
}
void Capture::init(void)
{
	cvNamedWindow ("Original Image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow ("Converted", CV_WINDOW_AUTOSIZE);
	cvNamedWindow ("Stripe", CV_WINDOW_AUTOSIZE);
	cvNamedWindow ("Marker", 0 );
	cvResizeWindow("Marker", 120, 120 );
	initVideoStream();

	int value = thresh;
	int max = 255;
	cvCreateTrackbar( "Threshold", "Converted", &value, max, trackbarHandler);
	
	int bw_value = bw_thresh;
	cvCreateTrackbar( "BW Threshold", "Converted", &bw_value, max, bw_trackbarHandler);

	memStorage = cvCreateMemStorage();
}

void Capture::updateMarkerPositions(void)
{
}

Marker Capture::getMarkerForId(int id)
{
	return NULL;
}

void Capture::initVideoStream(void)
{
	cap = cvCaptureFromCAM (0);
	if (!cap) {
		cout << "No webcam found\n";
		exit(0);
	}
}

int Capture::subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p )
{

	int x = int( floorf ( p.x ) );
	int y = int( floorf ( p.y ) );

	if ( x < 0 || x >= pSrc->width  - 1 ||
		 y < 0 || y >= pSrc->height - 1 )
		return 127;

	int dx = int ( 256 * ( p.x - floorf ( p.x ) ) );
	int dy = int ( 256 * ( p.y - floorf ( p.y ) ) );

	unsigned char* i = ( unsigned char* ) ( ( pSrc->imageData + y * pSrc->widthStep ) + x );
	int a = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
	i += pSrc->widthStep;
	int b = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
	return a + ( ( dy * ( b - a) ) >> 8 );
}



//trackbar
void Capture::trackbarHandler(int pos)
{
	thresh = pos;
}

void Capture::bw_trackbarHandler(int pos)
{
	bw_thresh = pos;
}