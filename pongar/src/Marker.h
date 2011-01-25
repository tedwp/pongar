#pragma once
#include <cv.h>

class Marker
{

public:
	Marker(int id);
	~Marker(void);
	void setPosition(CvMat newPosition);
private:
		int m_id;

};
