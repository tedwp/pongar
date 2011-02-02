#pragma once

#include <cv.h>
#include "GL\glut.h"

class Marker
{

public:
	Marker(int id);
	~Marker(void);
	void updatePosition(float position[16]);
	float* getPosition(void);
	
	int getId(void);
	int getPurpose(void);
	void setPurpose(int p);

	void setOffset(float offset);
	float getOffset();
	void clearHasPositionChanged(void);
	bool hasPositionChanged(void);

	int getLastUpdateTime();
	bool isVisible(void);
	void lock(void);
	void unLock(void);

private:
	int m_id;
	float* m_position;
	bool m_positionChanged;
	int m_purpose;
	long int m_lastUpdateTime;
	bool m_locked;
};
