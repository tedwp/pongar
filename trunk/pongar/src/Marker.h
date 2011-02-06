#pragma once

#include <cv.h>
#include "GL\glut.h"

class Marker
{

public:
	Marker(int id);
	Marker(int id, float size);
	~Marker(void);
	void updatePosition(float position[16]);
	float* getPosition(void);
	
	int getId(void);
	int getPurpose(void);
	void setPurpose(int p);
	
	void clearHasPositionChanged(void);
	bool hasPositionChanged(void);

	int getLastUpdateTime();
	bool isVisible(void);
	void lock(void);
	void unLock(void);
	float getSize();

private:
	int m_id;
	float m_size;
	float* m_position;
	bool m_positionChanged;
	int m_purpose;
	long int m_lastUpdateTime;
	bool m_locked;
};
