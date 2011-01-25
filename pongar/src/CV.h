#pragma once
class CV
{
public:
	CV(ArrayList<Marker> markers);
	~CV(void);
	void init(void);
	void updateMarkerPositions(void);

private:
	ArrayList<Marker>& m_markers;
	Marker getMarkerForId(int id);
	void initVideoStream(void);
	int subpixSampleSafe ( const IplImage* pSrc, CvPoint2D32f p );
};


