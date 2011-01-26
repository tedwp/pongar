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
	void cleanup();

	void updateMarkerPositions(void);

private:
	static Capture& m_instance;

	Capture(void);
	Capture(const Capture&);
	~Capture(void);

	int subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p );

	Marker getMarkerForId(int id);
	void initVideoStream(void);

	//thresholding
	int threshold;
	int bw_threshold;

	//trackbar for thresholding
	static void trackbarHandler(int pos);
	static void bw_trackbarHandler(int pos);

	//capture variables
	CvCapture* m_cap;
	CvMemStorage* m_memStorage;
	
	//transformation matrices
	float m_resultMatrix_005A[16];
	float m_resultMatrix_0272[16];

};


