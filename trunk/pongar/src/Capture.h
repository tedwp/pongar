#pragma once
#include <vector>

#include "conf.h"

#include "PoseEstimation.h"

#include <cv.h>
#include "Marker.h"
#include <highgui.h>
class Capture
{
public:
	static Capture& getInstance(void);
	
	void init();

	void updateMarkerPositions(void);
	 //TODO make the following private!
	int subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p );
	int threshold;
	int bw_threshold;
	CvCapture* m_cap;
	CvMemStorage* m_memStorage;
	
	float m_resultMatrix_005A[16];
	float m_resultMatrix_0272[16];
	unsigned char m_bkgnd[width*height*3];

private:
	static Capture& m_instance;


	Capture(void);
	Capture(const Capture&);
	~Capture(void);

	Marker getMarkerForId(int id);
	void initVideoStream(void);
	
	//trackbar
	static void trackbarHandler(int pos);
	static void bw_trackbarHandler(int pos);
};


