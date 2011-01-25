#pragma once
#include <cv.h>

class Marker
{

public:
	Marker(int id);
	~Marker(void);
	void updatePosition(float position[16]);
	float* getPosition(void);
	int getId(void);

	void render(void);

	bool hasPositionChanged(void);

private:
		int m_id;
		float* m_position;
		bool m_positionChanged;
};
