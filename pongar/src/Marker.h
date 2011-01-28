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
	int getPurpose(void);
	void setPurpose(int p);

	void setOffset(float offset);
	float getOffset();

	void render(void);

	bool hasPositionChanged(void);

private:
		int m_id;
		float* m_position;
		bool m_positionChanged;
		int m_purpose;
		float m_offset;
    void drawCircle(float r);
};
