#pragma once
#include "Marker.h"
class Capture
{
public:
	Capture(ArrayList<Marker> markers);
	~Capture(void);
	void init(void);
	void updateMarkerPositions(void);

private:
	ArrayList<Marker>& m_markers;
	Marker getMarkerForId(int id);
	void initVideoStream(void);
	int subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p );
	
	//trackbar
	void trackbarHandler(int pos);
	void bw_trackbarHandler(int pos);
};


