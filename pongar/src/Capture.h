#pragma once
#include <vector>
#include "Marker.h"

class Capture
{
public:
	static Capture& getInstance(void);
	
	void init(void);
	void updateMarkerPositions(void);
	void setMarkers(std::vector<Marker*> markers);
	int subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p ); //TODO make private!

private:
	static Capture& m_instance;
	Capture(void);
	Capture(const Capture&);
	~Capture(void);

	std::vector<Marker*>& m_markers;
	Marker getMarkerForId(int id);
	void initVideoStream(void);
	
	//trackbar
	static void trackbarHandler(int pos);
	static void bw_trackbarHandler(int pos);
};


